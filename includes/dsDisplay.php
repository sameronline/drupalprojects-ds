<?php
/**
 * @file
 * Class definition for a Display Suite Display object
 */

/**
 * The Display Suite Display object
 */
class dsDisplay {
  /**
   * Basic settings
   */
  public $dsid;
  public $name;
  public $module;
  public $type;
  public $build_mode;

  /**
   * Display settings
   */
  public $settings = array();

  /**
   * Fields set on this display
   */
  public $fields;

  /**
   * Layout settings from hook_ds_layouts
   */
  public $layout = array();

  /**
   * Regions configured for this object
   */
  public $regions = array();

  /**
   * API information
   */
  public $api_info = array();

  /**
   * Region styles
   */
  public $region_styles = array();

  /**
   * The content to return
   */
  public $content;

  /**
   * __constructor
   */
  public function __construct() {
    // Setup the object
    $this->regions = array();
    $this->fields = array();
  }

  /**
   * Initialise a display object
   */
  public function initialise($object) {
    // API info for this module and type.
    $this->api_info = ds_get_display_hander($object->module);

    // Get settings for this display/build mode combination
    $this->settings = ds_get_settings($object->module, $object->type, $object->build_mode);
  }

  /**
   * Load a display
   *
   * @param $name (optional)
   *  The machine name which identifies the display. If not provided, the
   *  existing name will be used, if set.
   *
   * @return
   *  either a complete display object, or FALSE
   */
  public function load($name = '') {
    if (!empty($name)) {
      $this->name = $name;
    }

    if (isset($this->name)){
      $stored_display = db_fetch_array(db_query("SELECT * FROM {ds_settings} WHERE name = '%s'", $this->name));
      if (!empty($stored_display)) {
        $this->dsid = $stored_display['dsid'];
        $this->module = $stored_display['module'];
        $this->type = $stored_display['type'];
        $this->build_mode = $stored_display['build_mode'];
        $this->settings = unserialize($stored_display['settings']);
        $this->fields = unserialize($stored_display['fields']);
      }
    }
  }

  /**
   *  Save this display to the database
   */
  public function save() {
    $success = FALSE;

    // Iterate over fields and ditch those which are hidden.
    if (!empty($this->settings['fields'])) {
      foreach ($this->settings['fields'] as $field_key => $field_value) {
        if ($field_value['region'] == 'disabled') {
          unset($this->settings['fields'][$field_key]);
        }
      }
    }

    $this->settings = serialize($this->settings);

    if (isset($this->dsid)) {
      $op = 'update';
    }
    else {
      $op = 'create';
    }

    switch ($op) {
      case 'create':
        $result = drupal_write_record('ds_settings', $this);
        break;
      case 'update':
        $result = drupal_write_record('ds_settings', $this, 'dsid');
        break;
    }

    if ($result) {
      switch ($result) {
        case SAVED_NEW:
        case SAVED_UPDATED:
          $success = TRUE;
          $this->load($this->name); // Reload to get the dsid
          break;
      }
    }

    if (isset($result) && $result == TRUE){
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Delete
   */
  public function delete() {
    db_query("DELETE FROM {ds_settings} WHERE name = '%s'", $this->name);
  }

  /**
   * Add a field
   */
  public function addField($key, $field) {
    $this->fields[$key] = $field;
    return $this->fields[$key];
  }

  /**
   * Load a layout from hook_ds_layouts, and assign region variables
   *
   * @param string $layout
   *  (optional) A layout to load. As most of ds currently doesnt support configurable
   *  layouts, this defaults to the standard layout when none is provided.
   */
  public function getLayout($layout = DS_DEFAULT_LAYOUT) {
    $this->layout = ds_get_layout($layout);
    $this->layout['#key'] = $layout;
    foreach ($this->layout['regions'] as $name => $info) {
      $this->regions[$name] = $info;
      // All regions are disabled initially
      $this->regionHide($name); 
    }
  }

  /**
   * Given an array of region information from a node, assign active and hidden
   * regions.
   */
  public function assignActiveRegions($set_regions) {
    foreach ($this->regions as $key => $region) {
      if (array_key_exists($key, $set_regions)) {
        $this->regions[$key]['#field_weights'] = $set_regions[$key];
        $this->regionShow($key);
      }
    }
  }

  /**
   * Determine whether a region is active
   */
  public function regionIsActive($region_name) {
    if (isset($this->regions[$region_name]) && $this->regions[$region_name]['#hidden'] == FALSE) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Set up a region for use
   */
  public function regionSetup($region_name) {
    // Default region classes
    $this->regionAttr($region_name, 'class', 'ds-region');
    $this->regionAttr($region_name, 'class', $this->api_info['module'] .'-region-'. $region_name);
  }

  /**
   * Order fields for a region
   *
   * @todo this will need to be swapped out for nested ordering wttk
   */
  public function regionOrderFields($region_name) {
    if (isset($this->regions[$region_name])) {
      asort($this->regions[$region_name]['#field_weights']);
    }
  }

  /**
   * Helper to hide a region
   */
  public function regionHide($name) {
    if(isset($this->regions[$name])) {
      $this->regions[$name]['#hidden'] == TRUE;
    }
  }

  /**
   * Helper to show a region
   */
  public function regionShow($name) {
    if(isset($this->regions[$name])) {
      $this->regions[$name]['#hidden'] == FALSE;
    }
  }

  /**
   * For all regions, add content from provided fields
   */
  public function regionsAddContent() {
    $this->content = '';
    foreach ($this->regions as $key => $region) {
      if ($this->regionIsActive($key)) {
        $fields = ds_element_children($region);
        foreach ($fields as $field) {
          if (!empty($region[$field])) {
            $this->regions[$key]['#field_content'] .= $region[$field];
          }
        }
      }
    }
  }

  /**
   * Finalise a region for rendering
   */
  public function regionFinalise($key) {
    if ($this->regionIsActive($key)) {
      if (isset($display->region_styles[$key]) && !empty($display->region_styles[$key])) {
        $this->regionAttr($key, 'class', $display->region_styles[$key]);
      }
      // Clean region attributes
      $this->regions[$key]['attributes'] = ds_clean_attributes($this->regions[$key]['#attributes']);
    }
  }

  /**
   * Render a region
   */
  public function regionRender($key) {
    if ($this->regionIsActive($key)) {
      $theme = DS_DEFAULT_THEME_REGION;
      if (isset($this->regions[$key]['#theme']) && !empty($this->regions[$key]['#theme'])) {
        $theme = $this->regions[$key]['#theme'];
      }

      // Do any last minute region tasks
      $this->regionFinalise($key);

      $vars = array();
      $vars['attributes'] = $this->regions[$key]['attributes'];
      $vars['content'] = $this->regions[$key]['#field_content'];
      $vars['count'] = $this->regions[$key]['#count'];

      $this->regions[$key]['content'] = theme($theme, $vars);
    }
  }

  /**
   * Wrapper to render all region content
   */
  public function regionsRenderAll() {
    $count = 1;
    foreach ($this->regions as $key => $region) {
      if ($this->regionIsActive($key)) {
        $this->regions[$key]['#count'] = $count;
        $this->regionRender($key);
        $count++;
      }
    }
  }

  /**
   * Complete any last minute items before rendering
   */
  public function displayFinalise() {

    // Default layout classes
    $this->layout['#attributes']['class'] = 'ds-display';
    $this->layout['#attributes']['class'] = 'ds-layout-'. $this->layout['#key'];
    // Clean display attributes
    $this->layout['attributes'] = ds_clean_attributes($this->layout['#attributes']);
  }

  /**
   * Render the display
   */
  public function displayRender() {

    // Most of the time, the default theme function will be called
    $theme = DS_DEFAULT_THEME_REGIONS;
    if (isset($this->layout['theme'])) {
      $theme = $this->layout['theme'];
    }

    // Set up template variables
    $vars = array();
    foreach ($this->regions as $key => $region) {
      if ($this->regionIsActive($key)) {
        $vars['regions'][$key] = $region;
      }
    }
    $vars['attributes'] = $this->layout['attributes'];

    // Render the content and store for later
    $this->content = theme($theme, $vars);
  }

  /**
   * Return content for a display
   */
  public function content() {
    if (isset($this->content) && !empty($this->content)) {
      return $this->content;
    }
    return FALSE;
  }

  /**
   * Wrapper to add an attribute to an item
   */
  public function regionAttr($region, $type, $data) {

    // Initialise empty attribute arrays
    if (!isset($this->regions[$region]['#attributes'])) {
      $this->regions[$region]['#attributes'] = array();
    }

    // Fix attributes passed as strings
    if (!is_array($this->regions[$region]['#attributes'][$type]) && !empty($this->regions[$region]['#attributes'][$type])) {
      $tmp = $this->regions[$region]['#attributes'][$type];
      $this->regions[$region]['#attributes'][$type] = array();
      $this->regions[$region]['#attributes'][$type][] = $tmp;
    }
    // Add the attribute specified
    $this->regions[$region]['#attributes'][$type][] = $data;
  }

  /**
   * Remove an attribute from a region
   */
  public function regionRemoveAttr($region, $type, $match) {
    if (isset($this->regions[$region]['#attributes'][$type]) && is_array($this->regions[$region]['#attributes'][$type])) {
      foreach ($this->regions[$region]['#attributes'][$type] as $index => $value) {
        if ($match == $value) {
          unset($this->regions[$region]['#attributes'][$type][$index]);
        }
      }
    }
  }

  /**
   * Helper to nest fieldsets
   *
   * @param $fields
   *  a flat array of fields to nest
   * @param $nested
   *  a nested array of regions to return
   */
  function nestFields(&$fields, &$nested, $current_parent = '#root', $depth = 0) {
    foreach($fields as $key => $field) {
      if($field['#parent'] == $current_parent) {
        $children = array();
        ds_nest_fields($fields, $children, $key, $depth + 1);
        $nested[$key]['#weight'] = $field['#weight'];
        $nested[$key]['#depth'] = $depth;
        foreach($children as $child_key => $child) {
          $nested[$key]['#fields'][$child_key] = $child;
        }
      }
    }
  }

  /**
   * Helper to sort nested fields
   */
  function orderFields(&$fields) {

    // Order fields by weight
    $weight = array();
    foreach ($fields as $key => $row) {
        $weight[$key]  = $row['#weight'];
    }
    array_multisort($weight, SORT_ASC, $fields);

    foreach ($fields as &$field) {
      if (isset($field['#fields']) && !empty($field['#fields'])) {
        ds_order_fields($field['#fields']);
      }
    }
  }
}