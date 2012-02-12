<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_GROUP,
  'name' => t('Fieldgroup'),
  'class' => 'dsFieldGroup',
);

/**
 * The group field type
 */
class dsFieldGroup extends dsField {
  
  /**
   * Implements dsField::render()
   * @todo
   */
  function render(&$object, $group_key, $vars = array()){
    $object->ds_fields[$group_key]['content'] = '';

    if (array_key_exists($group_key, $object->ds_fields)) {

      // Sort group items by weight, and pass them to the render function
      asort($object->ds_groups[$group_key]);
      foreach ($object->ds_groups[$group_key] as $field_key => $field_weight) {
        // Items inside groups wont have a content value set, so assign it now
        // @todo find a way to move ds_set_content into a consistent place for 
        // all fields, e.g. ds_build_fields_and_objects
        $object->ds_fields[$group_key]['fields'][$field_key]['content'] = ds_get_content($object->ds_fields[$group_key]['fields'][$field_key], $vars, $field_key);
        $object->ds_fields[$group_key]['fields'][$field_key]['rendered'] = ds_render_item($object->ds_fields[$group_key]['fields'][$field_key]);

        $object->ds_fields[$group_key]['content'] .= $object->ds_fields[$group_key]['fields'][$field_key]['rendered'];
      }

      $object->ds_fields[$group_key]['count'] = count($object->ds_fields[$group_key]['fields']); // @todo - is this still required?

      // Render the complete group
      if (!empty($object->ds_fields[$group_key]['content'])) {
        $object->ds_fields[$group_key]['rendered'] = ds_render_item($object->ds_fields[$group_key]);
      }
    }

    return $object->ds_fields[$group_key]['rendered'];
  }
}