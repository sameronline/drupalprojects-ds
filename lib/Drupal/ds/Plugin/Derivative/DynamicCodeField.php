<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DynamicCodeField.
 */

namespace Drupal\ds\Plugin\Derivative;

use Drupal\Component\Plugin\Derivative\DerivativeBase;

/**
 * Retrieves block plugin definitions for all custom blocks.
 */
class DynamicCodeField extends DerivativeBase {

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions(array $base_plugin_definition) {

    $custom_fields = config_get_storage_names_with_prefix('ds.field.');

    foreach ($custom_fields as $config) {
      $field = config($config)->get();
      foreach ($field['entities'] as $entity_type) {
        $this->derivatives[$field['field']] = $base_plugin_definition;
        $this->derivatives[$field['field']] += array(
          'title' => $field['label'],
          'properties' => $field['properties'],
          'entity_type' => $entity_type,
        );
        if (!empty($field['ui_limit'])) {
          $this->derivatives[$field['field']]['ui_limit'] = explode("\n", $field['ui_limit']);
        }
      }
    }

    return $this->derivatives;
  }
}
