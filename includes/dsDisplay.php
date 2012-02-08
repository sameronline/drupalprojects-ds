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
   * Display settings
   */
  public $settings = array();

  /**
   * Region styles
   */
  public $region_styles = array();

  /**
   * The content to return
   */
  public $content;

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
      $this->regions[$name]['#hidden'] = TRUE; 
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
        $this->regions[$key]['#hidden'] = FALSE;
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
   * Render content
   */
  public function renderRegionContent() {
    $this->content = '';
    foreach ($this->regions as $key => $region) {
      if ($region['#hidden'] == FALSE) {
        $fields = ds_element_children($region);
        foreach ($fields as $field) {
          if (!empty($region[$field])) {
            $this->regions[$key]['#content'] .= $region[$field];
          }
        }
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
   * Complete any last minute items before rendering
   */
  public function displayFinalise() {

    foreach ($this->regions as $key => $region) {
      if ($this->regionIsActive($key)) {

        // Default region classes
        $this->regionAttr($key, 'class', 'ds-region');
        $this->regionAttr($key, 'class', $this->api_info['module'] .'-region-'. $key);
        if (isset($display->region_styles[$key]) && !empty($display->region_styles[$key])) {
          $this->regionAttr($key, 'class', $display->region_styles[$key]);
        }

        // Clean region attributes
        $this->regions[$key]['attributes'] = ds_clean_attributes($this->regions[$key]['#attributes']);
      }
    }

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
    if (isset($this->regions['#wrapper'])) {
      $vars['wrapper'] = $this->regions['#wrapper'];
    }

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
}