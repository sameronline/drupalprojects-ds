<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_FUNCTION,
  'name' => t('Custom function'),
  'function' => 'dsFieldFunction'
);

/**
 * The function field type
 */
class dsFieldFunction extends dsField {

  /**
   * Format content for use in an item
   */
  public function formatContent() {
    $content = NULL;

    if (isset($this->settings['function'])) {
      // Load includes
      if (isset($this->settings['file'])) {
        include_once($this->settings['file']);
      }
      // If its a function, call it
      if (isset($this->settings['function']) && !empty($this->settings['function'])) {
        if (function_exists($this->settings['function'])) {
          $this->content = call_user_func($this->settings['function'], $this->settings);
          return $this->content;
        }
      }
    }
    return FALSE;
  }
}