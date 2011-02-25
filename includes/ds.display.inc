<?php

/**
 * @file
 * Display overview form.
 */

/**
 * Menu callback; presents a listing of fields display settings for a content type.
 *
 * Form includes form widgets to select which fields appear for teaser, full node
 * etc and how the field labels should be rendered.
 *
 * This function is based on content_display_overview_form from
 * content.admin.inc, but altered to have weights and regions.
 */
function ds_display_overview_form(&$form_state, $node_type, $build_mode = 'full', $module = 'nd') {
  // Gather information.
  $type = node_get_types('type', $node_type);
  $nd_display_settings = variable_get($module .'_display_settings_'. $node_type, array());

  $form = array(
    '#tree' => TRUE,
    '#node_type' => $node_type,
    '#has_body' => $type->has_body,
    '#fields' => array(),
    '#build_mode' => $build_mode,
  );

  // Exclude build mode.
  $exclude_build_modes = variable_get('nd_buildmodes_exclude', array());
  $excluded = ((isset($exclude_build_modes[$node_type][$build_mode])
    && $exclude_build_modes[$node_type][$build_mode] == TRUE) ||
    variable_get('nd_contenttype_'. $node_type, FALSE) == TRUE) ? TRUE : FALSE;
  $form['exclude_build_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude this build mode from rendering.'),
    '#description' => t('The node object will still have this build mode property, so you can make exceptions in your template file. You can also change this setting at <a href="@url">the exclude matrix</a>.<br />You can also create a file called <strong>@template.tpl.php</strong> or <strong>@template-nid.tpl.php</strong> to use for templating.', array('@url' => url('admin/content/types/nd'), '@template' => 'node-'. $type->type .'-'. $build_mode)),
    '#default_value' => $excluded,
    '#attributes' => array('onClick' => 'toggleFieldOverviewForm()'),
  );
  // RSS support.
  if ($build_mode == NODE_BUILD_RSS) {
    $form['exclude_build_mode']['#description'] = t('If you exclude the RSS build mode, the default node feed is generated, but might render strange content. Make sure you do not choose the "Titles only" as your feed content setting, otherwhise, this screen will not have any effect. Choosing either "Full text" or "Titles + teaser" does not matter, this screen will have effect on both settings, apart from the read more link. Remember that some fields like upload and terms are added automatically to the feed and that this manipulates the $description variable, it does not add extra keys to the feed.');
  }

  $global_exclude = variable_get('nd_contenttype_'. $node_type, FALSE);
  $form['exclude_build_mode']['#disabled'] = $global_exclude;
  if ($global_exclude == TRUE) {
    $description = t('The checkbox has been disabled because the complete content type has been excluded from rendering.') .'<br />';
    $form['exclude_build_mode']['#description'] = $description .' '. $form['exclude_build_mode']['#description'];
  }

  // If global exclude is true, no point in rendering.
  if ($global_exclude == FALSE) {
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
    $form['#submit'] = array('nd_display_submit');

    // Add the fields.
    ds_add_fields($form, $nd_display_settings, $module);

    // Plugins.
    if ($build_mode != NODE_BUILD_RSS)
    ds_plugins_display_form($form, $nd_display_settings);
  }


  return $form;
}

/**
 * Add fields to display overview form.
 */
function ds_add_fields(&$form, $nd_display_settings, $ds_type) {

  $build_mode = $form['#build_mode'];
  $node_type = $form['#node_type'];
  $has_body = $form['#has_body'];

  // Get extra fields.
  $fields = ds_get_fields($node_type, $has_body, $build_mode, $ds_type);
  if (!empty($fields)) {
    foreach ($fields as $field => $value) {
      $form['#fields'][] = $field;
      $form[$field] = (!empty($form[$form['#fields'][0]])) ? $form[$form['#fields'][0]] : ds_field_default_form_properties($build_mode);
      $form[$field]['human_name']['#value'] = $value['title'];
      $form[$field]['weight']['#default_value'] = ds_default_value($nd_display_settings, $build_mode, 'fields', $field, 'weight', DS_DEFAULT_WEIGHT);
      $form[$field]['ds_weight']['#default_value'] = ds_default_value($nd_display_settings, $build_mode, 'fields', $field, 'weight', DS_DEFAULT_WEIGHT);
      $form[$field][$build_mode]['label']['format']['#default_value'] = isset($value['display_settings'][$build_mode]['label']['format']) ? $value['display_settings'][$build_mode]['label']['format'] : ds_default_value($nd_display_settings, $build_mode, 'fields', $field, 'labelformat', DS_DEFAULT_LABEL_FORMAT);
      $form[$field][$build_mode]['format']['#options'] = isset($value['formatters']) ? $value['formatters'] : array();
      $format = ds_default_value($nd_display_settings, $build_mode, 'fields', $field, 'format', '');
      if (empty($format) && isset($value['formatters'])) {
        $format = key($value['formatters']);
      }
      $form[$field][$build_mode]['format']['#default_value'] = $format;
      $form[$field][$build_mode]['format']['#access'] = (count($form[$field][$build_mode]['format']['#options']) == 1 || count($form[$field][$build_mode]['format']['#options']) == 0) ? FALSE : TRUE;
      $form[$field][$build_mode]['region']['#default_value'] = ds_default_value($nd_display_settings, $build_mode, 'fields', $field, 'region', ND_DEFAULT_REGION);
    }
  }
}

/**
 * Add plugins to display overview form.
 *
 * @param array $form The display form.
 * @param array $nd_display_settings Current display settings.
 */
function ds_plugins_display_form(&$form, $nd_display_settings) {

  $plugins = variable_get('ds_plugin_settings', array());
  if (!empty($plugins)) {
    $form['#plugins'] = TRUE;
    $form['#plugin_keys'] = array();
  }

  foreach ($plugins as $key => $plugin) {
    if (isset($plugin['filename'])) {
      require_once($plugin['filename']);
    }
    $function = $plugin['form_callback'];
    $plugin_form = $function($form, $nd_display_settings);
    $form['#plugin_keys'][] = $key;
  }
}

/**
 * Save fields per context.
 */
function ds_display_submit($form, &$form_state) {

  $node_type = $form['#node_type'];
  $build_mode = $form['#build_mode'];

  // Exclude build mode checkbox.
  $exclude_build_mode = $form_state['values']['exclude_build_mode'];
  $exclude_build_modes = variable_get('nd_buildmodes_exclude', array());
  $exclude_build_modes[$node_type][$build_mode] = $exclude_build_mode;
  variable_set('nd_buildmodes_exclude', $exclude_build_modes);

  $nd_display_settings = variable_get('nd_display_settings_'. $node_type, array());
  unset($nd_display_settings[$build_mode]);

  // Fields.
  $fields = nd_get_fields($form['#node_type'], $form['#has_body'], $build_mode);
  if (!empty($fields)) {
    foreach ($fields as $key => $field) {
      $nd_display_settings[$build_mode]['fields'][$key]['weight'] = $form_state['values'][$key]['nd_weight'];
      $nd_display_settings[$build_mode]['fields'][$key]['format'] = $form_state['values'][$key][$build_mode]['format'];
      $nd_display_settings[$build_mode]['fields'][$key]['region'] = $form_state['values'][$key][$build_mode]['region'];
      $nd_display_settings[$build_mode]['fields'][$key]['labelformat'] = $form_state['values'][$key][$build_mode]['label']['format'];

      // Ditch this field, unless it has its storage in another module (ie CCK)
      if (!isset($field['storage']))
      unset($form_state['values'][$key]);
    }
  }

  // Plugins
  if ($build_mode != NODE_BUILD_RSS)
  nd_plugins_display_submit($form, $form_state, $nd_display_settings);

  // Save all these settings.
  variable_set('nd_display_settings_'. $node_type, $nd_display_settings);

  drupal_set_message(t('Your settings have been saved.'));
}

/**
 * Save plugins settings.
 *
 * @param array $form The display form.
 * @param array $form_state The submitted values of the form.
 * @param array $nd_display_settings Current display settings being saved.
 */
function ds_plugins_display_submit($form, $form_state, &$nd_display_settings) {
  $plugins = variable_get('nd_plugin_settings', array());

  foreach ($plugins as $key => $plugin) {
    if (isset($plugin['filename'])) {
      require_once($plugin['filename']);
    }
    $function = $plugin['submit_callback'];
    $function($form, $form_state, $nd_display_settings);
  }
}

/**
 * Return array of available label options.
 *
 * @param string $build_mode The build mode. Needed for a check on RSS build mode.
 */
function ds_label_options($build_mode = 'anything') {
  if ($build_mode != NODE_BUILD_RSS) {
    return array(
      'above' => t('Above'),
      'inline' => t('Inline'),
      'hidden' => t('<Hidden>'),
    );
  }
  else {
    return array(
      'inline' => t('Show'),
      'hidden' => t('<Hidden>'),
    );
  }
}

/**
 * Function to load default form properties for a field in a context
 */
function ds_field_default_form_properties($build_mode = NULL) {

  $field = array(
    'label' => array(
      'format' => array(
        '#type' => 'select',
        '#options' => ds_label_options($build_mode),
        '#default_value' => DS_DEFAULT_LABEL_FORMAT,
      ),
    ),
    'format' => array(
      '#type' => 'select',
      '#options' => array(),
      '#default_value' => DS_DEFAULT_FORMAT,
    ),
    'region' => array(
      '#type' => 'select',
      '#options' => ds_regions($build_mode),
      '#default_value' => DS_DEFAULT_REGION,
      '#attributes' => array(
        'class' => 'field-region-select field-region-'. DS_DEFAULT_REGION,
      ),
    ),
  );

  // If build mode is not empty, this means we're looking at
  // a content type which has no cck fields defined. Add
  // nd_weight and build mode key to the default field array
  // so the draghandling & formatters start working.
  if (!empty($build_mode)) {
    $field['ds_weight'] = array(
      '#type' => 'weight',
      '#default_value' => -19,
      '#delta' => 20,
    );
    $field[$build_mode] = array(
      'label' => array(
        'format' => array(
          '#type' => 'select',
          '#options' => ds_label_options($build_mode),
          '#default_value' => 'hidden',
        ),
      ),
      'format' => array(
        '#type' => 'select',
        '#options' => array(),
        '#default_value' => 'default',
      ),
      'region' => array(
        '#type' => 'select',
        '#options' => ds_regions($build_mode),
        '#default_value' => '',
        '#attributes' => array(
          'class' => 'field-region-select field-region-disabled',
        ),
      ),
    );
  }

  return $field;
}
