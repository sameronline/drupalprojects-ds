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
 * @param string $field_key The name of the field key.
 * @param array $field The current field.
 */
function theme_ds_field($content, $field_key, $field) {

  $output = '';
  if (!empty($content)) {
    if ($field['type'] == 'nd') {
      $output .= '<div class="field field-'. $field_key .'">';
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
 * Template helper for theme_views_view_row_object
 *
 * @todo support for users & comments.
 */
function template_preprocess_ds_row_object(&$vars) {

  $options = $vars['options'];
  $vars['object'] = ''; // make sure var is defined.
  $nid = $vars['row']->nid;
  if (!is_numeric($nid)) {
    return;
  }

  $node = node_load($nid);
  $node->build_mode = $options['build_mode'];

  if (empty($node)) {
    return;
  }

  $vars['object'] = node_view($node, FALSE, FALSE, FALSE);
}

/**
 * Theme preprocess function for ds-display-overview-form.tpl.php.
 */
function template_preprocess_ds_display_overview_form(&$vars) {
  $form = &$vars['form'];

  $build_mode = $form['#build_mode'];

  // Order the fields.
  $order = array();
  foreach ($form['#fields'] as $key => $field) {
    $order[$field] = $form[$field]['ds_weight']['#default_value'];
  }
  asort($order);

  // Render exclude build mode checkbox.
  $vars['exclude_build_mode'] = drupal_render($form['exclude_build_mode']);

  $rows = array();
  foreach ($order as $key => $field) {
    $element = &$form[$key];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {
        $row->{$child}->label = drupal_render($element[$child]['label']);
        $row->{$child}->format = drupal_render($element[$child]['format']);
        $row->{$child}->region = drupal_render($element[$child]['region']);
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
    $rows[$region][] = $row;
  }

  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds.js');
  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');

  $regions = ds_regions(variable_get('ds_build_mode_'. $build_mode, 'all'));
  foreach ($regions as $region => $title) {
    drupal_add_tabledrag('fields', 'match', 'sibling', 'field-region-select', 'field-region-'. $region, NULL, FALSE);
    // We need to have subgroups to make the dragging available within parents
    drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', 'field-weight-'. $region);
  }

  // Plugins available.
  if ($form['#plugins'] == TRUE) {
    $vars['plugins'] = TRUE;
    $vars['plugins_content'] = '';
    foreach ($form['#plugin_keys'] as $key) {
      $vars['plugins_content'] .= drupal_render($form[$key]);
    }
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $regions;
  $vars['build_mode'] = $build_mode;
  $vars['extra_style'] = ($form['exclude_build_mode']['#default_value'] == TRUE) ? 'style="display: none;"' : '';
}
