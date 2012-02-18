<?php

/**
 * @file
 * General file for administrative functions.
 */

/**
 * General page.
 */
function ds_general_page() {
  $i = 0;
  $output = '';
  $items = array();

  // Child pages.
  $item = menu_get_item(DS_PATH_BASE);
  if ($content = system_admin_menu_block($item)) {
    foreach ($content as $item) {
      $link = l($item['title'], $item['link_path']);
      if (!empty($item['description'])) {
        $link .= ' : '. $item['description'];
      }
      $items[$i] = $link;
      $i++;
    }
    $childs = theme('item_list', $items);
  }
  else {
    $childs = t('You do not have any administrative items.');
  }

  $output = t('Click on one of the following items to get started.');
  $output .= $childs;

  return $output;
}

/**
 * Page callback to list installed plugins
 */
function ds_plugins_page() {
  $output = 'This page shows all available plugin types used by Display Suite.';
  $plugins = array();
  $plugin_types = array(
    'ds_display' => 'Display types',
    'ds_field' => 'Field types',
    'ds_extension' => 'Extensions',
  );
  foreach($plugin_types as $type => $name) {
    $output .= ds_plugins_list_view($type, $name);
  }

  return $output;
}

/**
 * Callback to load an interface to view a list of plugins
 *
 * @param $type
 *  The DS plugin type to load
 * @param $heading (optional)
 *  A heading to use for the table
 *
 * @return
 *  A list of plugins, formatted as a table.
 */
function ds_plugins_list_view($type, $heading = NULL){
  static $module_info = array();

  $plugins = ds_get_plugins($type);
  if(isset($plugins) && !empty($plugins)){
    // build a table
    $output = isset($heading) ? '<h3>'. check_plain($heading) .'</h3>' : '';
    $header = array(t('Name'),t('Description'), t('Providing module'), t('Status'));
    $rows = array();

    foreach($plugins as $plugin){
      if (!isset($module_info[$plugin['module']])) {
        $path = drupal_get_path('module', $plugin['module']) . '/' . $plugin['module'] . '.info';
        $module_info[$plugin['module']] = drupal_parse_info_file($path);
      }
      $row = array();
      $row[] = array('data' => $plugin['name'], 'class' => 'title', 'sort' => 'asc');
      $row[] = array('data' => $plugin['description'], 'class' => 'description');
      $row[] = array('data' => $module_info[$plugin['module']]['name'], 'class' => 'provider');
      $row[] = array('data' => 'Installed', 'class' => 'status');

      $rows[] = $row;
    }

    $output .= theme('table', $header, $rows);
  }
  return $output;
}