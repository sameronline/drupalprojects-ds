<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Field.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * The base plugin to create DS fields.
 */
abstract class Field extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function render($field) {
    $settings = $this->getChosenSettings($field);

    // Initialize output
    $output = '';

    // Basic string.
    $entity_render_key = $this->entityRenderKey();

    if (isset($settings['link text'])) {
      $output = t($settings['link text']);
    }
    elseif (!empty($entity_render_key) && isset($field['entity']->{$entity_render_key})) {
      if ($field['entity_type'] == 'user' && $entity_render_key == 'name') {
        $output = user_format_name($field['entity']);
      }
      else {
        $output = $field['entity']->{$entity_render_key}->value;
      }
    }

    if (empty($output)) {
      return;
    }

    // Link.
    if (!empty($settings['link'])) {
      if (isset($field['entity']->uri['path'])) {
        $path = $field['entity']->uri['path'];
      }
      else {
        $uri_info = $field['entity']->uri();
        $path = $uri_info['path'];
      }
      $output = l($output, $path);
    }
    else {
      $output = check_plain($output);
    }

    // Wrapper and class.
    if (!empty($settings['wrapper'])) {
      $wrapper = check_plain($settings['wrapper']);
      $class = (!empty($settings['class'])) ? ' class="' . check_plain($settings['class']) . '"' : '';
      $output = '<' . $wrapper . $class . '>' . $output . '</' . $wrapper . '>';
    }

    return $output;
  }

  /**
   * Returns the entity render key for this field.
   */
  protected function entityRenderKey() {
    return '';
  }

}
