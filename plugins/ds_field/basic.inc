<?php
/**
 * The default field type for Display Suite
 *
 * This will almost always be overridden by another type.
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_BASIC,
  'name' => t('Basic field'),
  'description' => t('Basic field handler on which all others are based.'),
  'class' => 'dsField',
);

class dsField {
  /**
   * Field key
   */
  public $key;

  /**
   * Content for the field
   */
  public $content;

  /**
   * Field settings
   */
  public $settings = array();

  /**
   * Field attributes
   */
  protected $attributes = array();

  /**
   * Wrapper to set a value on a field
   */
  public function setting($key, $value) {
    $this->settings[$key] = $value;
  }

  /**
   * Return default values for a field
   */
  protected function defaults() {
    return array();
  }

  /**
   * Initialise field defaults
   *
   * This overly complicated function merges defaults and settings in the
   * following order:
   *  - default field settings across all fields, from this function
   *  - type defaults loaded from the field types
   *  - field settings from the database
   * Settings from latter groups override previous groups.
   */
  public function initialise($settings) {
    $defaults = array(
      'labelformat' => DS_DEFAULT_LABEL_FORMAT,
      'label' => '',
      'theme' => DS_DEFAULT_THEME_FIELD,
      'weight' => DS_DEFAULT_WEIGHT,
      'content' => NULL,
    );
    $type_defaults = array_merge($defaults, $this->defaults());
    $this->settings = array_merge($type_defaults, $settings);
  }

  /**
   * Build an individual field value
   *
   * Prepares a field for ds to pass to ds_render_content. This does not resolve
   * parent relationships.
   *
   * @param $field_key
   *  the field to build
   * @param $field_settings
   *  an array of field settings
   *
   * @return
   *  a field settings array ready to pass to ds_render_content
   */
  function build(){

    // Default class and extra class from the UI
    $classes = array();
    $classes[] = 'field-'. strtr($field_key, '_', '-');
    if (isset($field_settings['properties']['css-class']) && !empty($field_settings['properties']['css-class'])) {
      $classes[] = $field_settings['properties']['css-class'];
      unset($field_settings['properties']['css-class']); // dont set these on the field
    }
    if (isset($field_settings['css-class']) && !empty($field_settings['css-class'])) {
      $classes[] = $field_settings['css-class'];
      unset($field_settings['css-class']); // dont set these on the field
    }

    // Field defaults - all fields get these
    // @todo 
    // Abstract field types into config functions which returns defaults for 
    // that type
    $field_defaults = array(
      'labelformat' => DS_DEFAULT_LABEL_FORMAT,
      'label' => '',
      'theme' => DS_DEFAULT_THEME_FIELD,
      'weight' => DS_DEFAULT_WEIGHT,
      'content' => NULL,
    );

    // Merge defaults and settings to produce the field array
    $field = array_merge($field_defaults, $field_settings);
    $field['key'] = $field_key;
    $field['type'] = empty($field_settings) ? 'other' : 'ds';

    // Check for weight in region and parent (if any). If a parent key is found, 
    // we'll unset the original field from the region it might be set in and 
    // we'll add that field to the group array.
    if (isset($field_settings['weight'])) {
      $field['weight'] = $field_settings['weight'];
    }
    $field['parent'] = (isset($field_settings['parent'])) ? $field_settings['parent'] : NULL;

    // Process groups (fieldsets)
    if ($field['field_type'] == DS_FIELD_TYPE_GROUP || $field['field_type'] == DS_FIELD_TYPE_MULTIGROUP) {
      if (isset($field_settings['format'])) {
        $field['theme'] = $field_settings['format'];
      }
      else {
        $field['theme'] = DS_DEFAULT_THEME_FIELDSET;
      }

      // Additional formatting settings for fieldsets
      if ($field_settings['field_type'] == DS_FIELD_TYPE_GROUP) {
        $classes[] = 'field-group';
      }

      // Additional formatting settings for CCK multigroups
      if ($field_settings['field_type'] == DS_FIELD_TYPE_MULTIGROUP) {
        $field['subgroup_theme'] = isset($field_settings['subgroup_format']) ? $field_settings['subgroup_format'] : DS_DEFAULT_THEME_FIELDSET;
        $classes[] = 'field-multigroup';
      }
    }

    $field['class'] = implode(' ', $classes);

    // Change the title if this is configured and label is not hidden.
    if (isset($field_settings['label_value']) && $field['labelformat'] != DS_DEFAULT_LABEL_FORMAT) {
      $field['title'] = t(check_plain($field_settings['label_value']));
    }

    // Add extra properties to be used in themeing
    $field['key'] = $field_key;

    // Theming can either be done in preprocess, with a custom function or an
    // existing formatter theming function. Always pass the $field_settings as parameter.
    // @todo: some of these should break earlier as no processing is required
    switch ($field['field_type']) {
      case DS_FIELD_TYPE_PREPROCESS:
      case DS_FIELD_TYPE_IGNORE:
        if (isset($field_settings['properties']['key']) && !empty($field_settings['properties']['key'])) {
          $field['preprocess_settings'] = array('type' => $field['type'], 'key' => $field['properties']['key']);
        }
        else {
          $field['preprocess_settings'] = array('type' => $field['type']);
        }
        break;
      case DS_FIELD_TYPE_CODE:
        $field['formatter'] = isset($field['format']) ? $field['format'] : 'ds_eval_code';
        break;
      case DS_FIELD_TYPE_BLOCK:
        $field['formatter'] = 'ds_eval_block';
        break;
      case DS_FIELD_TYPE_FUNCTION:
        $field['function'] = (isset($field_settings['format'])) ? $field_settings['format'] : key($field_settings['properties']['formatters']);
        break;
      case DS_FIELD_TYPE_THEME:
        $field['formatter'] = (isset($field_settings['format'])) ? $field_settings['format'] : key($field_settings['properties']['formatters']);
        break;
    }

    //  Format content for the field
    $field['content'] = ds_field_format_content($field);

    return $field;
  }

  /**
   * Get content for use in a field.
   *
   * This will check an array of content values for a matching key and use its
   * value as the field's content.
   *
   * @param stdClass $item
   *  the item to set content for
   * @param array $content_vars (optional)
   *  the $vars array, or an array containing possible values
   * @param string $vars_key 
   *  the key within $content_vars to retrieve
   *
   * @return 
   *  the content value as a string, or FALSE if none was found
   */
  function getContent($content_vars = array(), $vars_key = NULL) {
    // If content has been prerendered by ds_build_fields_and_objects use that
    if (!empty($this->content)) {
      return $this->content;
    }
    if (empty($vars_key)) {
      $vars_key = $this->key;
    }
    // If content has been rendered by another source (e.g. CCK) use that
    if (!empty($content_vars)) {
      if (isset($content_vars[$vars_key .'_rendered'])) {
        $this->content = $content_vars[$vars_key .'_rendered'];
      }
      elseif (isset($content_vars[$vars_key])) {
        $this->content = $content_vars[$vars_key];
      }
    }

    if (!empty($this->content)){
      return $this->content;
    }

    return FALSE;
  }

  /**
   * Format content for use in an item.
   *
   * Most field types will override this function, as dsField::render simply
   * wraps the content in default field HTML.
   */
  public function formatContent() {
    return filter_xss($this->content);
  }

  /**
   * Return a value for a field or group
   *
   * @param array $item
   *  The item to render. The item requires a 'content' key.
   *
   * @return
   *  A rendered item, or FALSE if no content was found
   */
  public function render(){
    if (empty($this->settings['theme'])) {
      $this->settings['theme'] = DS_DEFAULT_THEME_FIELD;
    }

    $this->content = theme($this->settings['theme'], $this);

    // theme() can return anything, but we only want to return content where
    // there is 1 or more characters
    if (isset($this->content) && strlen($this->content) > 0) {
      return $this->content;
    }

    return FALSE;
  }
}