<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\CodePluginBase.
 */

namespace Drupal\ds\Plugin\ds\field;

/**
 * The base plugin to create DS block fields.
 */
abstract class CodePluginBase extends PluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::renderField().
   */
  public function renderField($field) {
    if (isset($field['properties']['code'])) {
      $format = (isset($field['properties']['code']['format'])) ? $field['properties']['code']['format'] : 'plain_text';
      if ($format == 'ds_code' && module_exists('ds_code')) {
        $value = ds_code_php_eval($field['properties']['code']['value'], $field['entity'], isset($field['build']) ? $field['build'] : array());
      }
      else {
        $value = check_markup($field['properties']['code']['value'], $format);
      }
      // Token support - check on token property so we don't run every single field through token.
      if (isset($field['properties']['use_token']) && $field['properties']['use_token'] == TRUE) {
        $value = token_replace($value, array($field['entity_type'] => $field['entity']), array('clear' => TRUE));
      }
      return $value;
    }
  }

  /**
   * Returns the format of the code field.
   */
  public function format() {
    return 'ds_code';
  }

  /**
   * Returns the value of the code field.
   */
  public function code() {
    return '';
  }

  /**
   * Returns if the code makes use of tokens.
   */
  public function usesTokens() {
    return FALSE;
  }

}
