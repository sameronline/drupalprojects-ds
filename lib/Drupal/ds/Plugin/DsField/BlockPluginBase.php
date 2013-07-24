<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\BlockPluginBase.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * The base plugin to create DS block fields.
 */
abstract class BlockPluginBase extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $contextual = module_exists('contextual') && user_access('access contextual links');
    $module = $this->blockModule();
    $delta = $this->blockDelta();
    $block = module_invoke($module, 'block_view', $delta);
    $contextual_links = array();
    // Get contextual links.
    if ($contextual) {
      if (is_array($block['content']) && isset($block['content']['#contextual_links'])) {
        $contextual_links = $block['content']['#contextual_links'];
      }
    }
    if (isset($block['content']) && is_array($block['content'])) {
      $block['content'] = drupal_render($block['content']);
    }
    if (!empty($block['content'])) {

      global $theme_key;
      $block_title = db_query("SELECT title FROM {block} WHERE module = :module AND delta = :delta AND theme = :theme", array(':module' => $module, ':delta' => $delta, ':theme' => $theme_key))->fetchField();
      if (!empty($block_title)) {
        $block['subject'] = $block_title == '<none>' ? '' : check_plain($block_title);
      }

      $block = (object) $block;
      $this->renderBlock($block);
    }
  }

  /**
   * Returns the module defining the block.
   */
  protected function blockModule() {
    return '';
  }

  /**
   * Returns the delte of the block.
   */
  protected function blockDelta() {
    return '';
  }

  /**
   * Renders the block.
   */
  protected function renderBlock($block) {
    return '';
  }

}
