<?php
// $Id$

/**
 * @file
 * Contains the display suite fieldgroup handler.
 */

/**
 * Field handler to create a fieldgroup.
 */
class ds_handler_field_fieldgroup extends views_handler_field {

  function query() {
    $this->field_alias = 'ds_fieldgroup_'. $this->position;

    $value = db_escape_string($this->options['value']);
    $this->query->add_field('', "'$value'", $this->field_alias);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'ds_field');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array(
      'ds_field' => 'A simple div',
      'ds_group_fieldset_open' => 'Fieldset open',
      'ds_group_fieldset_collapsible' => 'Fieldset collapsible',
      'ds_group_fieldset_collapsed' => 'Fieldset collapsed',
    );

    if (module_exists('tabs')) {
      $options['tabs'] = 'Tabs';
    }

    $form['format'] = array(
      '#type' => 'select',
      '#title' => t('Format'),
      '#default_value' => $this->options['format'],
      '#options' => $options,
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    return check_markup($value, $this->options['format'], FALSE);
  }
}
