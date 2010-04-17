<?php
// $Id$

/**
 * @file
 * Theming functions for ds.
 */

/**
 * Theme function to render the field content.
 *
 * @param string $content The content to render.
 * @param string $field The field to theme.
 */
function theme_ds_field($content, $field) {
  $output = '';

  if (!empty($content)) {
    if ($field['type'] == 'ds') {
      $output .= '<div class="field '. $field['class'] .'">';
      // Above label.
      if ($field['labelformat'] == 'above') {
        $output .= '<div class="field-label">'. $field['title'] .': </div>';
      }
      // Inline label
      if ($field['labelformat'] == 'inline') {
        $output .= '<div class="field-label-inline-first">'. $field['title'] .': </div>';
      }
      $output .= $content;
      $output .= '</div>';
    }
    else {
      $output = $content;
    }
  }

  return $output;
}

/**
 * Theme function to render all regions with their content.
 *
 * @param stdClass $object_display The complete object and display properties.
 * @param string $module The module name.
 */
function theme_ds_regions($object_display, $module) {
  $output = '';

  foreach ($object_display->themed_regions as $region_name => $region_data) {
    $output .= '<div class="'. $module .'-region-'. $region_name;
    if ($region_name == 'middle') {
      $output .= ' '. $object_display->ds_middle_classes;
    }
    if ($region_name == 'header' || $region_name == 'footer') {
      $output .= ' clear-block';
    }
    if (isset($region_data['extra_class'])) {
      $output .= $region_data['extra_class'];
    }
    $output .= '" ';
    if (isset($region_data['inline_css'])) {
      $output .= $region_data['inline_css'];
    }
    $output .= '>'. $region_data['content'] .'</div>';
  }

  return $output;
}

/**
 * Evaluate custom code.
 *
 * @param array $field The field with all its properties.
 */
function theme_ds_eval_code($field) {
  if (isset($field['properties']['code'])) {
    $value = ds_eval($field['properties']['code'], $field['object']);
    // Token support - check on token property so we don't run every single field through token.
    if (module_exists('token') && isset($field['properties']['token']) && $field['properties']['token'] == TRUE) {
      $value = token_replace($value, $field['object_type'], $field['object']);
    }
    return $value;
  }
}

/**
 * Evaluate block field.
 *
 * @param array $field The field array.
 */
function theme_ds_eval_block($field) {
  if (isset($field['properties'])) {
    list($module, $delta) = explode('|', $field['properties']['block']);
    $block = module_invoke($module, 'block', 'view', $delta);
    if (!empty($block)) {
      switch ($field['properties']['render']) {
        case DS_BLOCK_TEMPLATE:
          global $theme_key;
          $block = (object) $block;
          $block->region = NULL;
          $block->module = $module;
          $block->delta = $delta;
          $block_title = db_result(db_query("SELECT title FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme_key));
          if (!empty($block_title)) {
            $block->subject = $block_title == '<none>' ? '' : check_plain($block_title);
          }
          $content = theme('block', $block);
          break;
        case DS_BLOCK_TITLE_CONTENT:
          $content = '<h2 class="block-title">'. $block['subject'] .'</h2>';
          $content .= $block['content'];
          break;
        case DS_BLOCK_CONTENT:
          $content = $block['content'];
          break;
      }
      return $content;
    }
  }
}

/**
 * Theme a group (fieldset).
 *
 * @param string $content The content to render.
 * @param string $field The field to theme.
 */
function ds_group_fieldset($content, $field) {
  $add_js = FALSE;
  $class = $field['class'];
  $variation = array_pop(explode('_', $field['theme']));

  switch ($variation) {
    case 'collapsible':
      $add_js = TRUE;
      $class .= ' collapsible';
      break;
    case 'collapsed':
      $add_js = TRUE;
      $class .= ' collapsible collapsed';
      break;
    default:
      break;
  }

  // Add js.
  if ($add_js == TRUE) {
    drupal_add_js('misc/collapse.js');
  }

  return '<fieldset class="group '. $class .'"><legend>'. $field['title'] .'</legend>'. $content .'</fieldset>';
}

/**
 * Theme tabs. This is based on the theme_tabset() function
 * from the tabs module. We use our own theming function because
 * otherwhise we need to loop through our fields to build the
 * right array before we an send it to theme('tabset');
 */
function theme_ds_tabs($field) {
  tabs_load();

  static $i = 1;
  $fields = $field['fields'];
  ksort($fields);

  $tab_navigation = (variable_get('tabs_navigation', 0)) ? ' tabs-navigation' : '';

  $tabs_content = '';
  $output = '<div id="tabs-'. $field['name'] .'" class="drupal-tabs js-hide'. $tab_navigation .'" style="display:block;">';
  $tabs = '<ul class="clear-block ui-tabs-nav tabs">';
  foreach ($fields as $weight => $data) {
    $tabs .= '<li class="tabset-tab-'. $i .'">
    <a href="#tabset-tab-'. $i .'"><span class="tab">'. $data['title'] .'</span></a></li>';
    $tabs_content .= '<div id="tabset-tab-'. $i .'" class="tabs-'. $field['name'] .' ui-tabs-panel '. $data['class'] .'">'. $data['content'] .'</div>';
    $i++;
  }
  $tabs .= '</ul>';
  $output .= $tabs;
  $output .= $tabs_content;
  $output .= '</div>';
  return $output;
}

/**
 * Theme author name.
 */
function theme_ds_author_nolink($field) {
  return $field['object']->name;
}

/**
 * Theme author name linked.
 */
function theme_ds_author_link($field) {
  return theme('username', $field['object']);
}

/**
 * Theme the user picture.
 */
function theme_ds_picture($field) {
  return theme('user_picture', $field['object']);
}

/**
 * Template preprocess function for theme_views_view_row_object().
 */
function template_preprocess_ds_row_object(&$vars) {
  $options = $vars['options'];
  $vars['object'] = ''; // make sure var is defined.

  // We delegate the manipulation of the object to a function
  // which every module implementing the ds_api should define.
  // This can either be the simple selector or the advanced selector.
  // The advanced selector searches for a function build like this:
  // ds_views_row_adv_VIEWSNAME.
  if ($options['advanced']) {
    $build_mode = $options['build_mode'];
    $row_function = 'ds_views_row_adv_'. $vars['view']->name;
    $row_function($vars, $build_mode);
  }
  // Simple selector.
  else {
    $row_function = 'ds_views_row_'. $vars['view']->base_table;
    $build_mode = $options['build_mode'];
    $row_function($vars, $build_mode);
  }
}

/**
 * Preprocess theme function to print a single record from a row, with fields
 */
function template_preprocess_ds_row_fields(&$vars) {
  template_preprocess_views_view_fields($vars);
}

/**
 * Theme preprocess function for ds-display-overview-form.tpl.php.
 */
function template_preprocess_ds_display_overview_form(&$vars) {
  $form = &$vars['form'];
  $build_mode = $form['#build_mode'];

  // Sort fields.
  $order = array();
  $root_fields = array();
  $leaf_fields = array();
  foreach ($form['#fields'] as $key => $field) {
    if (!empty($form[$field][$build_mode]['parent_id']['#default_value'])) {
      $leaf_fields[$form[$field][$build_mode]['parent_id']['#default_value']][$field] = $form[$field]['ds_weight']['#default_value'];
    }
    else {
      $root_fields[$field] = $form[$field]['ds_weight']['#default_value'];
    }
  }

  asort($root_fields);
  foreach ($root_fields as $root_field => $root_weight) {
    $order[$root_field] = $root_weight;
    if (isset($leaf_fields[$root_field])) {
      $fields_in_leaf = $leaf_fields[$root_field];
      asort($fields_in_leaf);
      foreach ($fields_in_leaf as $leaf_field => $leaf_weight) {
        $order[$leaf_field] = $leaf_weight;
      }
    }
  }

  $rows = array();
  foreach ($order as $field => $field_weight) {
    $element = &$form[$field];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {
        $row->{$child}->indentation = theme('indentation', $element[$child]['#depth']);
        $row->{$child}->class = drupal_render($element[$child]['css-class']);

        // Parent & Field id.
        $row->{$child}->parent_id = drupal_render($element[$child]['parent_id']);
        $row->{$child}->field_id = drupal_render($element[$child]['field_id']);

        // Extra classes when fieldgroup.
        if ($element[$child]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
          $element[$child]['format']['#attributes']['class'] = 'fieldgroup-format';

          if (substr($element[$child]['format']['#default_value'], 0, 17) == 'ds_group_fieldset'
            || substr($element[$child]['format']['#default_value'], 0, 7) == 'ds_tabs'
            ) {
            $element[$child]['label']['format']['#attributes']['class'] = 'ds-hidden';
          }
        }
        if (!empty($element[$child]['parent_id']['#default_value'])) {
          $element[$child]['region']['#attributes']['class'] .= ' ds-hidden';
        }

        $row->{$child}->region = drupal_render($element[$child]['region']);
        $row->{$child}->format = drupal_render($element[$child]['format']);
        $row->{$child}->label = drupal_render($element[$child]['label']);
        $row->{$child}->label_edit = drupal_render($element[$child]['label_edit']);
        $row->{$child}->label_value = drupal_render($element[$child]['label_value']);
      }
      // Render the rest of the fields
      else {
        // Process weight.
        if ($child == 'ds_weight') {
          $element['ds_weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['ds_weight'] = process_weight($element['ds_weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }
    }

    // Add draggable.
    $row->class = 'draggable';
    if ($region == 'disabled') {
      $row->class .= ' region-css-'. $region;
    }

    $row->label_class = 'label-field';
    if ($element[$build_mode]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
      $row->label_class .= ' field-group';
      $row->class .= ' tabledrag-root';
    }
    else {
      $row->class .= ' tabledrag-leaf';
    }

    // Add to rows.
    $rows[$region][] = $row;
  }

  // CSS and JS.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds.js');
  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');

  // Parent & field order.
  drupal_add_tabledrag('fields', 'match', 'parent', 'ds-parent-id', 'ds-parent-id', 'ds-field-id', FALSE, 1);
  drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', NULL, NULL, TRUE);

  // Sync / copy tab.
  $vars['sync_copy_tab'] = '';
  if (isset($form['sync_copy'])) {
    $vars['sync_copy_tab'] = drupal_render($form['sync_copy']);
  }

  // Plugins available.
  $vars['plugins_tabs'] = array();
  $vars['plugins_content'] = '';
  if ($form['#plugins'] == TRUE) {
    foreach ($form['#plugin_keys'] as $key => $title) {
      $vars['plugins_tabs'][$key] = $title;
      $vars['plugins_content'][$key] = drupal_render($form[$key]);
    }
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $form['#regions'];
  $vars['build_mode'] = $build_mode;
}

/**
 * Theme preprocess function for ds-display-overview-views-form.tpl.php.
 */
function template_preprocess_ds_display_overview_views_form(&$vars) {

  $form = &$vars['form'];
  $build_mode = $form['#build_mode'];

  // Sort fields.
  $order = array();
  $root_fields = array();
  $leaf_fields = array();
  foreach ($form['#fields'] as $key => $field) {
    if (!empty($form[$field][$build_mode]['parent_id']['#default_value'])) {
      $leaf_fields[$form[$field][$build_mode]['parent_id']['#default_value']][$field] = $form[$field]['ds_weight']['#default_value'];
    }
    else {
      $root_fields[$field] = $form[$field]['ds_weight']['#default_value'];
    }
  }

  asort($root_fields);
  foreach ($root_fields as $root_field => $root_weight) {
    $order[$root_field] = $root_weight;
    if (isset($leaf_fields[$root_field])) {
      $fields_in_leaf = $leaf_fields[$root_field];
      asort($fields_in_leaf);
      foreach ($fields_in_leaf as $leaf_field => $leaf_weight) {
        $order[$leaf_field] = $leaf_weight;
      }
    }
  }

  $rows = array();
  foreach ($order as $field => $field_weight) {
    $element = &$form[$field];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {
        $row->{$child}->indentation = theme('indentation', $element[$child]['#depth']);
        $row->{$child}->class = drupal_render($element[$child]['css-class']);

        // Parent & Field id.
        $row->{$child}->parent_id = drupal_render($element[$child]['parent_id']);
        $row->{$child}->field_id = drupal_render($element[$child]['field_id']);

        // Extra classes when fieldgroup.
        if ($element[$child]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
          $element[$child]['format']['#attributes']['class'] = 'fieldgroup-format';

          if (substr($element[$child]['format']['#default_value'], 0, 17) == 'ds_group_fieldset'
            || substr($element[$child]['format']['#default_value'], 0, 7) == 'ds_tabs'
            ) {
            $element[$child]['label']['format']['#attributes']['class'] = 'ds-hidden';
          }
        }
        if (!empty($element[$child]['parent_id']['#default_value'])) {
          $element[$child]['region']['#attributes']['class'] .= ' ds-hidden';
        }

        $row->{$child}->region = drupal_render($element[$child]['region']);
        $row->{$child}->label = drupal_render($element[$child]['label']);
      }
      // Render the rest of the fields
      else {
        // Process weight.
        if ($child == 'ds_weight') {
          $element['ds_weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['ds_weight'] = process_weight($element['ds_weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }
    }

    // Add draggable.
    $row->class = 'draggable';
    if ($region == 'disabled') {
      $row->class .= ' region-css-'. $region;
    }

    $row->label_class = 'label-field';
    if ($element[$build_mode]['type']['#value'] == DS_FIELD_TYPE_GROUP) {
      $row->label_class .= ' field-group';
      $row->class .= ' tabledrag-root';
    }
    else {
      $row->class .= ' tabledrag-leaf';
    }

    // Add to rows.
    $rows[$region][] = $row;
  }

  // CSS and JS.
  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds.js');
  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');

  // Parent & field order.
  drupal_add_tabledrag('fields', 'match', 'parent', 'ds-parent-id', 'ds-parent-id', 'ds-field-id', FALSE, 1);
  drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', NULL, NULL, TRUE);

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $form['#regions'];
  $vars['build_mode'] = $build_mode;
}
