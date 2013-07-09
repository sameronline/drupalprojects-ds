<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\BlockTemplate.
 */

namespace Drupal\ds\Plugin\DSPlugin;

/**
 * Field that renders the block in a template.
 */
class BlockTemplate extends BlockPluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\BlockPluginBase::renderBlock().
   */
  protected function renderBlock($block) {
    if (!isset($block->subject)) {
      $block->subject = NULL;
    }
    $block->region = NULL;
    $block->module = $module;
    $block->delta = $delta;
    $elements = array('elements' => array('#block' => $block, '#children' => $block->content));
    // Add contextual links
    if ($contextual) {
      $elements['elements'] += array('#contextual_links' => array_merge($contextual_links, array('block' => array('admin/structure/block/manage', array($block->module, $block->delta)))));
    }
    return theme('block', $elements);
  }

}



