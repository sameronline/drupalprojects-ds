<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Plugin\DsField\SwitchField.
 */

namespace Drupal\ds_extras\Plugin\DsField;

use Drupal\Core\Annotation\Translation;

/**
 * Plugin that generates a link to switch view mode with via ajax.
 *
 * @DsField(
 *   id = "switch_field",
 *   title = @Translation("Switch field"),
 *   entity_type = "node"
 * )
 */
class SwitchField extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $output = '';
    static $added = FALSE;

    $settings = $this->getChosenSettings($field);
    if (!empty($settings)) {

      $entity = $field['entity'];
      $id = $entity->id();
      $url = $field['entity_type'] . '-' . $field['view_mode'] . '-' . $id . '-';
      $switch = array();

      foreach ($settings['vms'] as $key => $value) {
        if (!empty($value)) {
          $class = 'switch-' . $key;
          if ($key == $field['view_mode']) {
            $switch[] = '<span class="' . $class . '">' . check_plain(t($value)) . '</span>';
          }
          else {
            $switch[] = '<span class="' . $class . '"><a href="" class="' . $url . $key . '">' . check_plain(t($value)) . '</a></span>';
          }
        }
      }

      if (!empty($switch)) {
        if (!$added) {
          $added = TRUE;
          drupal_add_js(drupal_get_path('module', 'ds_extras') . '/js/ds_extras.js');
        }
        $output = '<div class="switch-view-mode-field">' . implode(' ', $switch) . '</div>';
      }
    }

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm($field, $settings) {
    $entity_type = $field['entity_type'];
    $bundle = $field['bundle'];
    $view_mode = $field['view_mode'];
    $settings = isset($settings['vms']) ? $settings['vms'] : array();
    $view_modes = entity_get_view_modes($entity_type);

    $form['info'] = array(
      '#markup' => t('Enter a label for the link for the view modes you want to switch to.<br />Leave empty to hide link. They will be localized.'),
    );

    $view_mode_settings = field_view_mode_settings($entity_type, $bundle);

    foreach ($view_modes as $key => $value) {
      $visible = $view_mode_settings[$key]['status'];

      if ($visible) {
        $form['vms'][$key] = array(
          '#type' => 'textfield',
          '#default_value' => isset($settings[$key]) ? $settings[$key] : '',
          '#size' => 20,
          '#title' => check_plain($value['label']),
        );
      }
    }

    return $form;
  }

  /**
   * {@inhertidoc}
   */
  public function settingsSummary($field, $settings) {
    $entity_type = $field['entity_type'];
    $bundle = $field['bundle'];
    $view_mode = $field['view_mode'];
    $settings = isset($settings['vms']) ? $settings['vms'] : array();
    $view_modes = entity_get_view_modes($entity_type);

    $summary[] = 'View mode labels';

    foreach ($view_modes as $key => $value) {
      $view_mode_settings = field_view_mode_settings($entity_type, $bundle);
      $visible = !empty($view_mode_settings[$key]['status']);

      if ($visible) {
        $label = isset($settings[$key]) ? $settings[$key] : $key;
        $summary[] = $key . ' : ' . $label;
      }
    }

    return $summary;
  }

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    if (\Drupal::config('ds.extras')->get('switch_field')) {
      return TRUE;
    }

    return FALSE;
  }

}
