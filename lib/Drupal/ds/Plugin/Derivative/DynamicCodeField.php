<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DynamicCodeField.
 */

namespace Drupal\ds\Plugin\Derivative;

use Drupal\Component\Plugin\Derivative\DerivativeInterface;

/**
 * Retrieves block plugin definitions for all custom blocks.
 */
class DynamicCodeField implements DerivativeInterface {

  /**
   * List of derivative definitions.
   *
   * @var array
   */
  protected $derivatives = array();

  /**
   * Implements \Drupal\Component\Plugin\Derivative\DerivativeInterface::getDerivativeDefinition().
   *
   * Retrieves a specific custom block definition from storage.
   */
  public function getDerivativeDefinition($derivative_id, array $base_plugin_definition) {
    if (!empty($this->derivatives) && !empty($this->derivatives[$derivative_id])) {
      return $this->derivatives[$derivative_id];
    }
    $this->getDerivativeDefinitions($base_plugin_definition);
    return $this->derivatives[$derivative_id];
  }

  /**
   * Implements \Drupal\Component\Plugin\Derivative\DerivativeInterface::getDerivativeDefinitions().
   *
   * Retrieves dynamic code fields from storage.
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
