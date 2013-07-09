<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\CodePluginBase.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * The base plugin to create DS block fields.
 */
abstract class CodePluginBase extends PluginBase {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::renderField().
   */
  public function renderField($field) {
    $code = $this->code();
    if ($code) {
      $format = $this->format();
      if ($format == 'ds_code' && module_exists('ds_code')) {
        $value = ds_code_php_eval($code, $field['entity'], isset($field['build']) ? $field['build'] : array());
      }
      else {
        $value = check_markup($code, $format);
      }
      // Token support - check on token property so we don't run every single field through token.
      $uses_tokens = $this->usesTokens();
      if ($uses_tokens == TRUE) {
        $value = token_replace($value, array($field['entity_type'] => $field['entity']), array('clear' => TRUE));
      }
      return $value;
    }
  }

  /**
   * Returns the format of the code field.
   */
  protected function format() {
    return 'plain_text';
  }

  /**
   * Returns the value of the code field.
   */
  protected function code() {
    return '';
  }

  /**
   * Returns if the code makes use of tokens.
   */
  protected function usesTokens() {
    return FALSE;
  }

}
