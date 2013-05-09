<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\BlockPluginBase.
 */

namespace Drupal\ds\Plugin\DSPlugin;

/**
 * The base plugin to create DS block fields.
 */
abstract class BlockPluginBase extends PluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::renderField().
   */
  public function renderField($field) {
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

      // i18n support.
      if (function_exists('i18n_block_block_view_alter')) {
        // Check language visibility.
        global $language;
        static $block_languages = FALSE;
        if (!$block_languages) {
          $block_languages = array();
          $result = db_query('SELECT module, delta, language FROM {i18n_block_language}');
          foreach ($result as $record) {
            $block_languages[$record->module][$record->delta][$record->language] = TRUE;
          }
        }
        if (isset($block_languages[$module][$delta]) && !isset($block_languages[$module][$delta][$language->language])) {
          return;
        }

        // Translate.
        $i18n_block = db_query("SELECT * FROM {block} WHERE module = :module AND delta = :delta", array(':module' => $module, ':delta' => $delta))->fetchObject();
        if (!empty($i18n_block->i18n_mode)) {
          i18n_block_block_view_alter($block, $i18n_block);
          // OH WTF ...
          if (!empty($i18n_block->title)) {
            $block['subject'] = $i18n_block->title;
          }
        }
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
