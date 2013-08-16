<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\DynamicBlockField.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\ds\Annotation\DsField;
use Drupal\Core\Annotation\Translation;

/**
 * Defines a generic dynamic block field.
 *
 * @DsField(
 *   id = "dynamic_block_field",
 *   derivative = "Drupal\ds\Plugin\Derivative\DynamicBlockField",
 *   module = "ds"
 * )
 */
class DynamicBlockField extends BlockBase {

  /**
   * {@inheritdoc}
   */
  protected function blockPluginId() {
    $definition = $this->getPluginDefinition();
    return $definition['properties']['block'];
  }

  /**
   * {@inheritdoc}
   */
  public function isAllowed($bundle, $view_mode) {
    $definition = $this->getPluginDefinition();

    if (!isset($definition['ui_limit'])) {
      return TRUE;
    }

    $limits = $definition['ui_limit'];
    dpm($limits);
    foreach ($limits as $limit) {
      dpm($limit);
      dpm('test');
      list($bundle_limit, $view_mode_limit) = explode('|', $limit);

      if (($bundle_limit == $bundle || $bundle_limit == '*') && $view_mode_limit == $view_mode) {
        return FALSE;
      }
    }

    return TRUE;
  }

}
