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
  public function settingsForm($settings) {
    $entity = $this->linkedEntity();
    $view_modes = entity_get_view_modes($entity);

    $options = array();
    foreach ($view_modes as $id => $view_mode) {
      $options[$id] = $view_mode['label'];
    }

    $form['view_mode'] = array(
      '#type' => 'select',
      '#title' => 'View mode',
      '#default_value' => isset($settings['view_mode']) ? $settings['view_mode'] : '',
      '#options' => $options,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary($settings) {
    $entity = $this->linkedEntity();
    $view_modes = entity_get_view_modes($entity);

    // When no view modes are found no summary is displayed
    if (empty($view_modes)) {
      return;
    }

    // Print the chosen view mode or the default one
    if (isset($settings['view_mode'])) {
      $view_mode = $view_modes[$settings['view_mode']];
    }
    else {
      $view_mode = reset($view_modes);
    }

    $summary[] = 'View mode: ' . $view_mode['label'];

    return $summary;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {
    $entity = $this->linkedEntity();
    $view_modes = entity_get_view_modes($entity);
    reset($view_modes);
    $default_view_mode = key($view_modes);

    $settings = array(
      'view_mode' => $default_view_mode,
    );

    return $settings;
  }

  /**
   * Gets the wanted entity
   */
  public function linkedEntity() {
    return '';
  }

  /**
   * Gets the view mode
   */
  public function getViewMode() {
    $settings = $this->getChosenSettings();
    return $settings['view_mode'];
  }

}

