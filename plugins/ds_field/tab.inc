<?php
/**
 * @file
 * The Display Suite field type tab plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_TAB,
  'name' => t('Tab field'),
  'class' => 'dsFieldTab',
);

/**
 * The tab field type
 */
class dsFieldTab extends dsField {

  /**
   * Implements dsField::render
   */
  public function render() {
    $this->content = theme('ds_tabs', $item);

    // theme() can return anything, but we only want to return content where
    // there is 1 or more characters
    if (isset($content) && strlen($content) > 0) {
      return $content;
    }

    return FALSE;
  }
}