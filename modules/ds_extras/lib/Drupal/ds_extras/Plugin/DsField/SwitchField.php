<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Plugin\DsField\SwitchField.
 */

namespace Drupal\ds_extras\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;
use Drupal\ds\Plugin\DsField\DsFieldBase;

/**
 * Function field that renders the title of a node
 *
 * @DsField(
 *   id = "switch_field",
 *   title = @Translation("Switch field"),
 *   entity_type = "node",
 *   module = "ds_extras"
 * )
 */
class SwitchField extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $output = '';
    static $added = FALSE;

    if (isset($field['plugin_settings'])) {

      $entity = $field['entity'];
      $id = $entity->id();
      $url = $field['entity_type'] . '-' . $field['view_mode'] . '-' . $id . '-';
      $switch = array();

      foreach ($field['plugin_settings']['vms'] as $key => $value) {
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
          $add = TRUE;
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
  public function displays() {

    // Get all the allowed types
    if (!\Drupal::config('ds.extras')->get('switch_field')) {
      return FALSE;
    }
    else {
      return array();
    }
  }


  // TODO LOOK INTO FLAG SUPPORT

  // Flag support.
  // if (\Drupal::config('ds.extras')->get('flag') && module_exists('flag')) {
  //   if ($entity_type == 'node') {
  //     $flags = flag_get_flags('node');
  //     foreach ($flags as $name => $flag) {
  //       $ui_limit = array();
  //       if (!empty($flag->types)) {
  //         foreach ($flag->types as $type) {
  //           $ui_limit[] = $type . '|*';
  //         }
  //       }
  //       $fields['node']['ds_flag_' . $name] = array(
  //         'title' => t('Flag: ' . $flag->get_label('title')),
  //         'field_type' => DS_FIELD_TYPE_FUNCTION,
  //         'function' => 'ds_extras_flags_add_flag_link',
  //         'properties' => array(
  //           'flag' => $name,
  //         ),
  //         'ui_limit' => $ui_limit,
  //       );
  //     }
  //   }
  // }

  // if (!empty($fields)) {
  //   return $fields;
  // }

  /**
   * Output flag.
   */
  // function ds_extras_flags_add_flag_link($field) {
  //   return flag_create_link($field['properties']['flag'], $field['entity']->nid);
  // }

}
