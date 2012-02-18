<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_THEME,
  'name' => t('Theme field'),
  'description' => t('The default field type. Renders content using a theme function'),
  'class' => 'dsFieldTheme',
);

/**
 * The theme field type
 */
class dsFieldTheme extends dsField {

  /**
   * Format content for use in an item
   */
  public function formatContent() {
    $content = NULL;

    if (isset($this->settings['formatter'])) {
      // Load includes
      if (isset($this->settings['file'])) {
        include_once($this->settings['file']);
      }
      $this->content = theme($this->settings['formatter'], $this->settings);
      return $this->content;
    }
    return FALSE;
  }
}