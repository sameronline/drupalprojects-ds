<?php
/**
 * @file
 * The Display Suite field type multigroup plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_MULTIGROUP,
  'name' => t('CCK 6.x-3.x Multigroup'),
  'description' => t('Special field type to render CCK multigroups'),
  'class' => 'dsFieldMultigroup',
);

/**
 * The multigroup field type
 */
class dsFieldMultigroup extends dsField {

  /**
   * Return a rendered multigroup group
   * @todo
   *
   * Multigroups are not fieldsets but multi-value compound fields. The
   * individual field values must be rebuilt across fields to produce the 
   * correct structure..
   *
   * @param stdClass $object
   *  the object (e.g. a node) containing the group to be rendered
   * @param string $group_key
   *  the key of the group to render
   * @param array $vars (optional)
   *  the $vars array
   *
   * @return 
   *  a string containing the fully-rendered group, or FALSE if one is not 
   *  rendered
   */
  public function render(&$object, $group_key, $vars = array()){
    // Add CSS for multigroups
    // It would be nice to avoid this, however when returning individual CCK
    // rendered rows from a multigroup, every label after the first is hidden.
    drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds-multigroup.css');

    // Initialise storage for fieldset subgroups
    $object->ds_fields[$group_key]['subgroups'] = array();

    // Iterate over the number of properties ($fields) in the multigroup, and
    // construct a single numerically keyed, compound value array representing
    // the field. This is complicated because we are building individually 
    // rendered fields for fieldset implementations, as well as array keys for 
    // things like tables.
    $field_keys = array_keys($object->ds_fields[$group_key]['fields']);
    $subgroup_count = count($vars[$field_keys[0]]);

    for ($i = 0; $i < $subgroup_count; $i++) {
      $object->ds_fields[$group_key]['subgroups']['rows'][$i] = array(
        'index' => $group_key . '_delta_' . $i,
        'class' => 'content-multigroup-wrapper field-multigroup-row ds-multigroup-'. $group_key .'-delta-'. $i . ($i % 2 ? ' odd' : ' even'),
        'weight' => $i - 100,
      );

      // Iterate over fields in the group and move their values into subgroups
      foreach ($object->ds_fields[$group_key]['fields'] as $field_key => $field_weight) {
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key] = $object->ds_fields[$group_key]['fields'][$field_key];
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['view'] = ds_get_content($object->ds_fields[$group_key]['fields'][$field_key], $vars[$field_key][$i], 'view');

        // The field title may not be stored on the field, so retrieve it from
        // CCK via nd_cck
        if (empty($object->ds_fields[$group_key]['fields'][$field_key]['title'])) {
          $cck_field = content_fields($field_key, $object->type);
          $object->ds_fields[$group_key]['fields'][$field_key]['title'] = nd_cck_get_label_value($cck_field, $object->build_mode);
        }
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['title'] = $object->ds_fields[$group_key]['fields'][$field_key]['title'];

        // CCK returns different content depending on its default display
        // settings, so we fallback on the default rendered view and pass it
        // through the ds rendering pipeline
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['content'] = $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['view'];
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['type'] = 'ds';
        // Because CCK fields normally ignore ds formatters, we need to explicitly
        // set the pipeline for multigroups to use DS, or they dont get formatted
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['rendered'] = ds_render_item($object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]);

        // Add subgroup row information
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['title'] = $object->content[$group_key]['group'][$i]['#title'];

        // Add the rendered value to its parent subgroup content
        $object->ds_fields[$group_key]['subgroups']['rows'][$i]['content'] .= $object->ds_fields[$group_key]['subgroups']['rows'][$i]['fields'][$field_key]['rendered'];
      }

      // Build the rendered subgroup row for use in fieldsets etc.
      $object->ds_fields[$group_key]['subgroups']['rows'][$i]['rendered'] = ds_render_item($object->ds_fields[$group_key]['subgroups']['rows'][$i]);
    }

    // Add fields for reference
    $object->ds_fields[$group_key]['subgroups']['fields'] = $object->ds_fields[$group_key]['fields'];

    // Subgroups may be rendered as if they are a single field, so we must construct
    // a wrapping field object to pass to ds_render_item()
    $object->ds_fields[$group_key]['subgroups']['key'] = $group_key .'_subgroup';
    $object->ds_fields[$group_key]['subgroups']['theme'] = isset($object->ds_fields[$group_key]['subgroup_theme']) ? $object->ds_fields[$group_key]['subgroup_theme'] : 'ds_multigroup_fieldset';
    // To be passed to a rendering function, the 'content' key must exist,
    // however the subgroup has not rendered content at this point
    $object->ds_fields[$group_key]['subgroups']['content'] = TRUE;

    // Render the subgroups
    $object->ds_fields[$group_key]['content'] = ds_render_item($object->ds_fields[$group_key]['subgroups']);

    // Render the group wrapper
    $object->ds_fields[$group_key]['rendered'] = ds_render_item($object->ds_fields[$group_key]);

    return $object->ds_fields[$group_key]['rendered'];
  }
}