<?php
/**
 * @file
 * Display Suite helper class
 */

namespace Drupal\ds;

use Drupal\Component\Utility\String;
use Drupal\Core\Cache\Cache;
use Drupal\Core\Entity\Display\EntityDisplayInterface;

/**
 * Helper class that holds all the main Display Suite helper functions.
 */
class Ds {

  /**
   * Gets all Display Suite fields.
   *
   * @param $entity_type
   *   The name of the entity.
   *
   * @return array
   *   Collection of fields.
   */
  public static function getFields($entity_type) {
    static $static_fields;

    if (!isset($static_fields[$entity_type])) {
      foreach (\Drupal::service('plugin.manager.ds')->getDefinitions() as $plugin_id => $plugin) {
        // Needed to get derivatives working
        $plugin['plugin_id'] = $plugin_id;
        $static_fields[$plugin['entity_type']][$plugin_id] = $plugin;
      }
    }

    return isset($static_fields[$entity_type]) ? $static_fields[$entity_type] : array();
  }

  /**
   * Gets the value for a Display Suite field.
   *
   * @param $key
   *   The key of the field.
   * @param $field
   *   The configuration of a DS field.
   * @param $entity
   *   The current entity.
   * @param $view_mode
   *   The name of the view mode.
   * @param $build
   *   The current built of the entity.
   * @return $markup
   *   The markup of the field used for output.
   */
  public static function getFieldValue($key, $field, $entity, $view_mode, $build = array()) {
    $configuration = array(
      'field' => $field,
      'field_name' => $key,
      'entity' => $entity,
      'build' => $build,
      'view_mode' => $view_mode,
    );

    // Load the plugin.
    $field_instance = \Drupal::service('plugin.manager.ds')->createInstance($field['plugin_id'], $configuration);

    // Render the field.
    return $field_instance->build();
  }

  /**
   * Gets the field settings.
   *
   * @param $entity_type
   *   The name of the entity.
   * @param $bundle
   *   The name of bundle (ie, page or story for node types, profile for users)
   * @param $view_mode
   *   The name of view mode.
   * @param bool $default
   *
   * @return array
   */
  public static function getFieldSettings($entity_type, $bundle, $view_mode, $default = TRUE) {
    static $field_settings = NULL;

    if (!isset($field_settings)) {
      if ($cache = \Drupal::cache()->get('ds_field_settings')) {
        $field_settings = $cache->data;
      }
      else {
        $ds_field_settings = \Drupal::configFactory()->listAll('ds.field_settings');
        foreach ($ds_field_settings as $config) {
          $field_setting = \Drupal::config($config)->get();
          if (!isset($field_setting['settings'])) {
            continue;
          }
          foreach ($field_setting['settings'] as $field => $settings) {
            $field_settings[$field_setting['entity_type']][$field_setting['bundle']][$field_setting['view_mode']][$field] = $settings;
          }
        }
        \Drupal::cache()->set('ds_field_settings', $field_settings, Cache::PERMANENT, array('ds_fields_info'));
      }
    }

    return (isset($field_settings[$entity_type][$bundle][$view_mode])) ? $field_settings[$entity_type][$bundle][$view_mode] : (isset($field_settings[$entity_type][$bundle]['default']) && $default ? $field_settings[$entity_type][$bundle]['default'] : array());
  }

  /**
   * Gets Display Suite layouts.
   */
  public static function getLayouts() {
    static $layouts = FALSE;

    if (!$layouts) {
      $errors = array();

      $layouts = \Drupal::moduleHandler()->invokeAll('ds_layout_info');

      // Give modules a chance to alter the layouts information.
      \Drupal::moduleHandler()->alter('ds_layout_info', $layouts);

      // Check that there is no 'content' region, but ignore panel layouts.
      // Because when entities are rendered, the field items are stored into a
      // 'content' key so fields would be overwritten before they're all moved.
      foreach ($layouts as $key => $info) {
        if (isset($info['regions']['content'])) {
          $errors[] = $key;
        }
      }
      if (!empty($errors)) {
        drupal_set_message(t('Following layouts have a "content" region key which is invalid: %layouts.', array('%layouts' => implode(', ', $errors))), 'error');
      }
    }

    return $layouts;
  }

  /**
   * Gets a layout for a given entity.
   *
   * @param $entity_type
   *   The name of the entity.
   * @param $bundle
   *   The name of the bundle.
   * @param $view_mode
   *   The name of the view mode.
   * @param $fallback
   *   Whether to fallback to default or not.
   *
   * @return $layout
   *   Array of layout variables for the regions.
   */
  public static function getLayout($entity_type, $bundle, $view_mode, $fallback = TRUE) {
    $overridden = TRUE;
    /** @var $entity_display EntityDisplayInterface */
    if ($view_mode != 'form') {
      $entity_display = entity_load('entity_view_display', $entity_type . '.' . $bundle . '.' . $view_mode);
      if ($entity_display) {
        $overridden = $entity_display->status();
      }
      else {
        $overridden = FALSE;
      }
    }
    else {
      $entity_display = entity_load('entity_form_display', $entity_type . '.' . $bundle . '.' . $view_mode);
    }

    if ($entity_display) {
      $layout = array(
        'layout' => $entity_display->getThirdPartySetting('ds', 'layout'),
        'settings' => $entity_display->getThirdPartySetting('ds', 'settings'),
      );
      if (!empty($layout) && ($overridden || $view_mode == 'default')) {
        $layout['view_mode'] = $view_mode;
        return $layout;
      }
    }

    // In case $view_mode is not found, check if we have a default layout,
    // but only if the view mode settings aren't overridden for this view mode.
    if ($view_mode != 'default' && !$overridden && $fallback) {
      /** @var $entity_default_display EntityDisplayInterface */
      $entity_default_display = entity_load('entity_view_display', $entity_type . '.' . $bundle . '.default');
      if ($entity_default_display) {
        $default_layout = array(
          'layout' => $entity_default_display->getThirdPartySetting('ds', 'layout'),
          'settings' => $entity_default_display->getThirdPartySetting('ds', 'settings'),
        );
        $default_layout['view_mode'] = 'default';
        return $default_layout;
      }
    }

    return FALSE;
  }

  /**
   * Checks if we can go on with Display Suite.
   *
   * In some edge cases, a view might be inserted into the view of an entity, in
   * which the same entity is available as well. This is simply not possible and
   * will lead to infinite loops, so you can temporarily disable DS completely
   * by setting this variable, either from code or visit the UI through
   * * admin/structure/ds/emergency
   */
  public static function isDisabled() {
    if (\Drupal::state()->get('ds.disabled', FALSE)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets all Display Suite field layouts options.
   *
   * Mainly used by select fields.
   *
   * @return array
   *   List of field layouts.
   */
  public static function getFieldLayoutOptions() {
    $options = array();
    foreach (\Drupal::service('plugin.manager.ds.field.layout')->getDefinitions() as $plugin_id => $plugin) {
      $options[$plugin_id] = $plugin['title'];
    }
    return $options;
  }

  /**
   * Utility function to return CSS classes.
   */
  public static function getClasses($name = 'region') {
    static $classes = array();

    if (!isset($classes[$name])) {
      $classes[$name] = array();
      $custom_classes = \Drupal::config('ds.settings')->get('classes.' . $name);
      if (!empty($custom_classes)) {
        $classes[$name][''] = t('None');
        foreach ($custom_classes as $value) {
          $classes_splitted = explode("|", $value);
          $key = trim($classes_splitted[0]);
          $friendly_name = isset($classes_splitted[1]) ? trim($classes_splitted[1]) : $key;
          $classes[$name][String::checkPlain($key)] = $friendly_name;
        }
      }
      $name_clone = $name; // Prevent the name from being changed.
      \Drupal::moduleHandler()->alter('ds_classes', $classes[$name], $name_clone);
    }

    return $classes[$name];
  }

}
