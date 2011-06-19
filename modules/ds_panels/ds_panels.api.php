<?php

/**
 * @file
 * Hooks provided by Display Suite module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Return fields to be added when creating a new display.
 */
function hook_ds_panels_default_fields($entity_type, $bundle, $view_mode) {
  // Get the fields from Field API.
  $fields = field_info_instances($entity_type, $bundle);
  return $fields;
}

/**
 * @} End of "addtogroup hooks".
 */