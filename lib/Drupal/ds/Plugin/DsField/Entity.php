<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Entity.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Renders an entity by a given view mode.
 */
abstract class Entity extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function settings() {

    $entity = $this->entity();
    $view_modes = entity_get_view_modes($entity);

    $options = array();
    foreach ($view_modes as $id => $view_mode) {
      $options[$id] = $view_mode['label'];
    }

    $settings = array();
    $settings['View mode'] = array(
      'type' => 'select',
      'options' => $options
    );

    return $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {

    $settings = array(
      'view_mode' => 0,
    );

    return $settings;
  }

  /**
   * Gets the wanted entity
   */
  public function entity() {
    return '';
  }

  /**
   * Gets the view mode
   */
  public function getViewMode($field) {
    $settings = $this->getChosenSettings($field);
    return $settings['View mode'];
  }

}

