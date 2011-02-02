<?php
// $Id$

/**
 * @file
 * Display Suite comment entity file.
 */

/**
 * Implements hook_ds_fields().
 */
function comment_ds_fields($entity_type, $bundle, $view_mode) {
  $fields = array();

  return array('comment' => $fields);
}