<?php
// $Id$

/**
 * @file
 * Theming functions for Display Suite.
 */

 /**
 * Template preprocess function for theme_views_view_row_entity().
 * We delegate the manipulation of the entity to a function
 * which every entity should implement.
 */
function template_preprocess_ds_row_entity(&$vars) {

  $options = $vars['options'];
  $vars['object'] = '';
  $vars['data_object'] = '';

  // The advanced selector searches for a function called
  // ds_views_row_adv_VIEWSNAME.
  if ($options['advanced']) {
    $view_mode = $options['view_mode'];
    $row_function = 'ds_views_row_adv_'. $vars['view']->name;
    $row_function($vars, $view_mode);
  }
  // The default selector searches for a function called
  // ds_views_row_VIEWSBASETABLE.
  // This function also takes care for changing the build
  // mode per row and pagination. Simple grouping is
  // also supported if there is a sort handler.
  else {

    // @todo see if we can make this a bit more sane

    static $nrs, $grouping = array();
    $group_value_content = '';
    $view_name = $vars['view']->name .'_'. $vars['view']->current_display;

    // Keep a static number for this view.
    if (!isset($nrs[$view_name])) {
      $nrs[$view_name] = 0;
    }

    // Change the build mode per view.
    if (isset($options['changing']) && $options['changing']) {
      // Check for paging to determine the build mode.
      if (isset($_GET['page']) && isset($options['changing_fieldset']['allpages']) && !$options['changing_fieldset']['allpages']) {
        $view_mode = $options['view_mode'];
      }
      else {
        $view_mode = $options['changing_fieldset']['item_'. $nrs[$view_name]];
      }
    }
    else {
      $view_mode = $options['view_mode'];
    }

    // Call the function.
    $row_function = 'ds_views_row_'. $vars['view']->base_table;
    $row_function($vars, $view_mode);

    // Keep a static grouping for this view.
    if (isset($options['grouping']) && $options['grouping']) {
      $group_value = $vars['view']->result[$nrs[$view_name]]->$options['grouping_fieldset']['group_field'];
      if (!isset($grouping[$view_name][$group_value])) {
        $group_value_content = '<h2 class="grouping-title">'. $group_value .'</h2>';
        $grouping[$view_name][$group_value] = $group_value;
      }
    }

    // Grouping or not ?
    if (!empty($grouping[$view_name])) {
      if (!empty($group_value_content)) {
        $vars['data_object'] = $group_value_content;
      }
      $vars['data_object'] .= '<div class="grouping-content">'. $vars['object'] .'</div>';
    }
    else {
      $vars['data_object'] = $vars['object'];
    }

    // Increment the number for this view.
    $nrs[$view_name]++;
  }
}

/**
 * Theme the node through the views entity plugin.
 */
function ds_views_row_node(&$vars, $view_mode) {
  $nid = $vars['row']->{$vars['field_alias']};
  //$nid = $vars['row']->nid;
  if (!is_numeric($nid)) {
    return;
  }

  $row_plugin = $vars['view']->style_plugin->row_plugin;
  $node = $row_plugin->nodes[$nid];

  if (empty($node)) {
    return;
  }

  // @todo render this through our template rendering system.
  $vars['object'] = drupal_render(node_view($node, $view_mode));
}
