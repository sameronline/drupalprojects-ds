<?php
// $Id$

/**
 * @file
 * General file for administrative functions.
 */

/**
 * General page.
 */
function ds_general_page() {

  $item = menu_get_item();
  if ($content = system_admin_menu_block($item)) {
    $childs = theme('admin_block_content', array('content' => $content));
  }
  else {
    $childs = t('You do not have any administrative items.');
  }

  $fieldset = array(
    '#title' => t('General'),
    '#children' => $childs,
    '#attributes' => array(),
  );

  $output = theme('fieldset', array('element' => $fieldset));

  return $output;
}