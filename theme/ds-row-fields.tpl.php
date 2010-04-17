<?php
// $Id$

/**
 * @file
 * DS display views field mode row.
 *
 * @ingroup views_templates
 */

  $region_classes = array();
  $all_regions = ds_regions('all', TRUE);
  $module = $view->style_plugin->row_plugin->options['base_table'];
  $regions = $view->style_plugin->row_plugin->options['regions'];

$object_display = new stdClass();

foreach ($all_regions as $region_name => $region_title) {

   if (isset($regions[$region_name])) {
      $region_content = '';
      $region = $regions[$region_name];

      // Loop through all fields after ordering on weight.
      asort($region);

      if ($region_name == 'left' || $region_name == 'right') {
        $region_classes[$region_name] = $region_name;
      }

      $object_display->themed_regions[$region_name]['content'] = '';

      foreach ($region as $field_key => $weight) {

        if (isset($fields[$field_key])) {
          $field = $fields[$field_key];
          $class = $view->style_plugin->row_plugin->options[$field_key]['css-class'];
          $content = '<div class="field field-image '. $class .'">';
          if ($field->label) {
            $content .= '<label class="views-label-'. $field->class .'">'. $field->label .'</label>';
          }

          $content .= '<div class="field-content">'. $field->content .'</div></div>';


          $object_display->themed_regions[$region_name]['content'] .= $content;
        }
      }
   }
}

  $object_display->ds_middle_classes = $module .'-no-sidebars';
  if (isset($region_classes['left']) && isset($region_classes['right'])) {
    $object_display->ds_middle_classes = $module .'-two-sidebars';
  }
  elseif (isset($region_classes['left'])) {
    $object_display->ds_middle_classes = $module .'-one-sidebar '. $module .'-sidebar-left';
  }
  elseif (isset($region_classes['right'])) {
    $object_display->ds_middle_classes = $module .'-one-sidebar '. $module .'-sidebar-right';
  }

print theme('ds_regions', $object_display, $module);
