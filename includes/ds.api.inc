<?php
/**
 * @file
 * API functions for building and rendering displays
 */

/**
 * API function to build and render a build mode for a given object
 *
 * This is not used internally by ds, but can be called by other modules
 * wishing to call a build mode programatically. 
 *
 * Note that ds normally steps in during the normal build process and expects 
 * certain values to be available in $vars as a result, so you will need to 
 * determine what those properties are and provide them, or ensure that all 
 * your fields do not require values from that array. In a lot of cases, calling
 * this function outside of a page load workflow will fail as a result.
 *
 * @param $key
 *  Key of the object, e.g. NID
 * @param $object
 *  the object to be manipulated
 * @param $module
 *  the ds module providing the display
 * @param $vars
 *  an array of values to use in the render
 * @param $build_mode (optional)
 *  the build mode to use
 *
 * @return 
 *  a string containing the fully rendered display
 */
function ds_make($key, $module, &$object, &$vars, $build_mode = NULL) {
  if (isset($build_mode) && !is_null($build_mode)){
    $object->build_mode = $build_mode;
  }

  $data = array(
    'object' => $object,
    'module' => $module,
    'vars' => $vars,
  );
  drupal_alter('ds_render', $data);

  $pre_render = ds_build_fields_and_regions($data['object'], $module);
  $render = ds_render($data['object'], $module, $data['vars']);

  return $render;
}

/**
 * API function to return rendered content for an item
 *
 * @param string $key
 *  A key for the object, e.g. an NID
 * @param stdClass $object 
 *  The object to manipulate, e.g. a $node object
 * @param string $module 
 *  The name of the module requesting the render
 * @param array $vars 
 *  The variables required for rendering
 *
 * @return 
 *  a string containing the fully rendered display
 */
function ds_render($key, $object, $module, $vars) {
  static $renders = array();

  if (isset($renders[$module][$object->build_mode][$key])) {
    return $renders[$module][$object->build_mode][$key];
  }
  $renders[$module][$object->build_mode][$key] = ds_render_content($object, $module, $vars);
  return $renders[$module][$object->build_mode][$key];
}

/**
 * Get fields and regions for an object.
 *
 * @param stdClass $object 
 *  The object to manipulate.
 * @param string $module 
 *  The module that is requesting.
 */
function ds_build_fields_and_regions(&$object, $module) {

  // API info for this module and type.
  $api_info = ds_api_info($module, $object->type);

  // See if rendering is needed later on.
  // There are two ways of excluding: global exclude on object type
  // or per build mode per object type.
  // We don't return here, because we want to add all fields on the object
  // so themers can use it in their template.
  $exclude_build_modes = variable_get($module .'_buildmodes_exclude', array());
  $object->render_by_ds = ((isset($exclude_build_modes[$object->type][$object->build_mode])
    && $exclude_build_modes[$object->type][$object->build_mode] == TRUE) ||
    variable_get($module .'_type_'. $object->type, FALSE) == TRUE) ? FALSE : TRUE;

  // Setup the object
  $object->regions = array();
  $object->ds_fields = array();
  $object->preprocess_fields = array();

  // Get settings for this display/build mode combination
  $display_settings = ds_get_settings($module, $object->type, $object->build_mode);

  if (!empty($display_settings['fields'])) {

    // Get all fields and settings for this build mode (this doesnt load CCK fields, only groups)
    $available_fields = ds_get_fields($module, $object->type, $object->build_mode, $api_info['extra']);

    // The node Body can sometimes be populated with other content when empty
    // @todo - we do this here so the fields populate correctly, but find 
    // somewhere logical for this
    if (isset($object->has_empty_body) && $object->has_empty_body == 1) {
      $object->body = '';
    }

    // Iterate over fields listed on the display
    foreach ($display_settings['fields'] as $field_key => $field_defaults) {

      // Dont render fields which are set to disabled
      $region = (isset($field_defaults['region'])) ? $field_defaults['region'] : DS_DEFAULT_REGION;
      if ($region != DS_DISABLED_REGION) {

        // Create a dummy region in $temp_regions for later nesting
        if (!isset($temp_regions[$region])) {
          $temp_regions[$region] = array();
        }

        // @todo 
        // Settings per field should be retrieved from a single cached function
        // call to ds_get_settings (or similar)
        if (isset($available_fields[$field_key])) {
          $field_settings = $available_fields[$field_key];
        }
        else {
          $field_settings = array();
        }

        $object->ds_fields[$field_key] = array();
        $object->ds_fields[$field_key] = array_merge($field_defaults, $field_settings);

        // Additional field properties (which should be included a normalised
        // settings function)
        $object->ds_fields[$field_key]['region'] = $region;
        $object->ds_fields[$field_key]['field_type'] = isset($object->ds_fields[$field_key]['type']) ? $object->ds_fields[$field_key]['type'] : DS_FIELD_TYPE_NON_DS; // fix key collision
        unset($object->ds_fields[$field_key]['type']);
        $object->ds_fields[$field_key]['module'] = $module;
        $object->ds_fields[$field_key]['object'] =& $object;
        $object->ds_fields[$field_key]['object_type'] = $api_info['object'];
        // end @todo
      }
    }

    // Build the individual fields using settings created previously
    $temp_regions = array();
    foreach ($object->ds_fields as $key => $setting) {
      // Build the field
      ds_build_field($key, $object->ds_fields[$key]);

      // Create a temp reference only if it doesnt already exists
      // This ensures that the weight and region always comes from the first
      // fieldgroup positioned.
      $region_key = $object->ds_fields[$key]['region'];
      $parent = $object->ds_fields[$key]['parent'];

      if ($object->ds_fields[$key]['format'] == 'ds_tabs') {
        if (!isset($temp_regions[$region_key][$parent]) && $parent != '#root') {
          $temp_regions[$region_key][$parent] = array(
            '#weight' => $object->ds_fields[$key]['weight'],
            '#parent' => '#root',
          ); 
        }
      }
      $temp_regions[$region_key][$key] = array(
        '#weight' => $object->ds_fields[$key]['weight'],
        '#parent' => $parent,
      );

      if ($parent != '#root') {
        $object->ds_fields[$parent]['fields'][] = $key;
      }
    }

    // Nest and order fields in regions
    if (!empty($temp_regions)) {
      // Nest groups
      foreach ($temp_regions as $key => $region){
        $nested_fields = array();
        ds_nest_fields($region, $nested_fields);

        $object->regions[$key] = $nested_fields;
      }
      // Sort groups
      foreach ($object->regions as $key => $region) {
        ds_order_fields($object->regions[$key]);
      }
    }
  }
  // Reset render_by_ds property if needed.
  if (empty($object->regions)) {
    $object->render_by_ds = FALSE;
  }
}

/**
 * Render content for an object.
 *
 * @deprecated This function is a builder, use the statically cached ds_render
 *
 * @param string $key
 *  A key for the object 
 * @param stdClass $object 
 *  The object to manipulate.
 * @param string $module 
 *  The module that is requesting.
 * @param array $vars 
 *  The variables required for rendering.
 * @param string $theme_function 
 *  The theming function for a field.
 */
function ds_render_content(&$object, $module, $vars, $theme_function = DS_DEFAULT_THEME_FIELD) {

  // Initialise the display
  $display = ds_create_display();
  if (!isset($object->layout)) {
    $object->layout = DS_DEFAULT_LAYOUT;
  }
  $display->getLayout($object->layout);

  // Sort regions to get them in the right order for rendering
  $display->assignActiveRegions($vars['regions']);

  // Build mode for this object
  $display->build_mode = $object->build_mode;

  // API info for this module and type. This doesn't cost a lot
  // since it will be cached already in ds_build_fields_and_regions().
  $display->api_info = ds_api_info($module, $object->type);

  // Display settings for this module and object.
  $display->display_settings = ds_get_settings($module, $object->type, $display->build_mode);
  $display->region_styles = ds_default_value($display->display_settings, 'region_styles');

  // Iterate over the active regions to build the display object
  $count = 0;
  foreach ($display->regions as $region_name => $region) {
    if ($region['#hidden'] == FALSE) {

      // Initialise the region with default values
      $display->regionSetup($region_name);

      // Order region fields by weight
      $display->regionOrderFields($region_name);

      // Loop through fields and extract them from the passed-in object
      foreach ($display->regions[$region_name]['#field_weights'] as $key => $weight) {

        /**
         * Preprocess some field content
         */
        switch ($object->ds_fields[$key]['field_type']) {
          case DS_FIELD_TYPE_PREPROCESS:
            if (!empty($object->ds_fields[$key]['preprocess_settings']['key'])) {
              $object->ds_fields[$key]['content'] = $vars[$key][$object->preprocess_fields[$key]['key']];
            }
            else {
              $object->ds_fields[$key]['content'] = $vars[$key];
            }
            break;
            
          case DS_FIELD_TYPE_IGNORE:
            $object->ds_fields[$key]['content'] = isset($object->content[$key]['#value']) ? $object->content[$key]['#value'] : '';
            break;
        }

        /**
         * Choose a field rendering pipeline based on the type
         * 
         * Groups get content and render within wrapper functions.
         * Fields get content then call the renderer directly.
         */
        switch ($object->ds_fields[$key]['field_type']) {
          case DS_FIELD_TYPE_GROUP:
            $display->regions[$region_name][$key] = ds_render_group($object, $key, $vars);
            break;

          case DS_FIELD_TYPE_MULTIGROUP:
            $display->regions[$region_name][$key] = ds_render_multigroup($object, $key, $vars);
            break;

          default:
            // Set content for this item
            $object->ds_fields[$key]['content'] = ds_get_content($object->ds_fields[$key], $vars, $key);
            $display->regions[$region_name][$key] = ds_render_item($object->ds_fields[$key]);
            break;
        }

        // Tabs are unsupported for the time being. Warn.
        if (!empty($object->ds_fields[$key]['theme'])) {
          if ($object->ds_fields[$key]['theme'] == 'ds_tabs') {
            drupal_set_message('This build mode use Tabs, which are currently unsupported. Please select a different formatter.');
          }
        }

      }
    }
  }

  // Add field content to all regions
  $display->regionsAddContent();

  // Add classes based on regions.
  if ($display->regionIsActive('middle')) {
    $display->regions['middle']['#field_content'] = '<div class="'. $display->api_info['module'] .'-region-middle">'. $display->regions['middle']['#field_content'] .'</div>';
    $middle_class = $module .'-no-sidebars';
    if ($display->regionIsActive('left') && $display->regionIsActive('right')) {
      $middle_class = $module .'-two-sidebars';
    }
    elseif ($display->regionIsActive('left')) {
      $middle_class = $module .'-one-sidebar '. $module .'-sidebar-left';
    }
    elseif ($display->regionIsActive('right')) {
      $middle_class = $module .'-one-sidebar '. $module .'-sidebar-right';
    }
    $display->regionAttr('middle', 'class', $middle_class);
    $display->regionAttr('middle', 'class', $display->api_info['module'] .'-region-middle-wrapper');
    $display->regionRemoveAttr('middle', 'class', $display->api_info['module'] .'-region-middle');
  }

  // Clean up regions ready for rendering
  $display->regionsRenderAll();

  // Plugins.
  ds_plugins_process($display, $object, $vars);

  // Theme the regions with their content.
  $display->displayFinalise();
  $display->displayRender();

  return $display->content();
}

