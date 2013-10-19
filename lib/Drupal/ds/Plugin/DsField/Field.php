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
  public function render() {
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
      $path = $uri_info['path'];
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
