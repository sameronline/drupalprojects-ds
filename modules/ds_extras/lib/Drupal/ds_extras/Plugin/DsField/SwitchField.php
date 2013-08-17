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
 * Plugin that generates a link to switch view mode with via ajax.
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
  public function isAllowed($bundle, $view_mode) {
    if (\Drupal::config('ds.extras')->get('switch_field')) {
      return TRUE;
    }

    return FALSE;
  }

}
