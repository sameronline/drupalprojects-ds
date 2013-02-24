<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\Field.
 */

namespace Drupal\ds\Plugin\ds;

/**
 * The base plugin to create DS field function fields.
 */
abstract class Field extends FunctionPluginBase {

  /**
   * Implements \Drupal\ds\Plugin\ds\field\PluginBase::renderField().
   */
  public function renderField($field) {
    $output = '';
    $settings = isset($field['formatter_settings']) ? $field['formatter_settings'] : array();
    $settings += $field['properties']['default'];

    // Basic string.
    if (isset($settings['link text'])) {
      $output = t($settings['link text']);
    }
    elseif (isset($field['properties']['entity_render_key']) && isset($field['entity']->{$field['properties']['entity_render_key']})) {
      if ($field['entity_type'] == 'user' && $field['properties']['entity_render_key'] == 'name') {
        $output = user_format_name($field['entity']);
      }
      else {
        $output = $field['entity']->{$field['properties']['entity_render_key']};
      }
    }

    if (empty($output)) {
      return;
    }

    // Link.
    if ($settings['link']) {
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

}
