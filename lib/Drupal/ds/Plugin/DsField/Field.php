<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Field.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Component\Utility\String;

/**
 * The base plugin to create DS fields.
 */
abstract class Field extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $settings = $this->getChosenSettings();

    // Initialize output
    $output = '';

    // Basic string.
    $entity_render_key = $this->entityRenderKey();

    if (isset($settings['link text'])) {
      $output = t($settings['link text']);
    }
    elseif (!empty($entity_render_key) && isset($this->entity->{$entity_render_key})) {
      if ($this->entityType() == 'user' && $entity_render_key == 'name') {
        $output = $this->entity->getUsername();
      }
      else {
        $output = $this->entity->{$entity_render_key}->value;
      }
    }

    if (empty($output)) {
      return array();
    }

    // Link.
    if (!empty($settings['link'])) {
      $uri_info = $this->entity->uri();
      $output = l($output, $uri_info['path'], $uri_info['options']);
    }
    else {
      $output = String::checkPlain($output);
    }

    // Wrapper and class.
    if (!empty($settings['wrapper'])) {
      $wrapper = String::checkPlain($settings['wrapper']);
      $class = (!empty($settings['class'])) ? ' class="' . String::checkPlain($settings['class']) . '"' : '';
      $output = '<' . $wrapper . $class . '>' . $output . '</' . $wrapper . '>';
    }

    return array(
      '#markup' => $output,
    );
  }

  /**
   * Returns the entity render key for this field.
   */
  protected function entityRenderKey() {
    return '';
  }

}
