<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\BlockTitleContent.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Field that renders the titel and the content of a block.
 */
class BlockTitleContent extends BlockPluginBase {

  /**
   * {@inheritdoc}
   */
  protected function renderBlock($block) {
    return '<h2 class="block-title">' . $block->subject . '</h2>' . $block->content;
  }

}
