<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\BlockBase.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * The base plugin to create DS block fields.
 */
abstract class BlockBase extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $manager = \Drupal::service('plugin.manager.block');

    // Create the wanted block class
    $id = $this->blockPluginId();
    $block = $manager->createInstance($id);

    // Get render array
    $block_elements = $block->build();

    // Build the output by looping through the elements
    $output = '';
    foreach ($block_elements as $block_element) {
      $output .= drupal_render($block_element);
    }

    // Output block
    return $output;
  }

  /**
   * Returns the delte of the block.
   */
  protected function blockPluginId() {
    return '';
  }

}
