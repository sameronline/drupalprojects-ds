<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Title.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders a title.
 */
abstract class Title extends Field {

  /**
   * {@inheritdoc}
   */
  public function settingsForm($field, $settings) {

    $default_settings = $this->defaultSettings();

    $settings['link'] = array(
      '#type' => 'checkbox',
      '#title' => 'Link',
      '#default_value' => isset($settings['link']) ? $settings['link'] : $default_settings['link'],
    );
    $settings['wrapper'] = array(
      '#type' => 'textfield',
      '#title' => 'Wrapper',
      '#default_value' => isset($settings['wrapper']) ? $settings['wrapper'] : $default_settings['wrapper'],
      '#description' => t('Eg: h1, h2, p')
    );
    $settings['class'] = array(
      '#type' => 'textfield',
      '#title' => 'Class',
      '#default_value' => isset($settings['class']) ? $settings['class'] : $default_settings['class'],
      '#description' => t('Put a class on the wrapper. Eg: block-title')
    );

    return $settings;
  }

  /**
   * {@inhertidoc}
   */
  public function settingsSummary($field, $settings) {
    $default_settings = $this->defaultSettings();

    $summary = array();
    if (isset($settings['link']) && !empty($settings['link'])) {
      $summary[] = 'Link: yes';
    }
    else {
      $summary[] = 'Link: no';
    }
    if (isset($settings['wrapper']) && !empty($settings['wrapper'])) {
      $summary[] = 'Wrapper: ' . $settings['wrapper'];
    }
    else {
      $summary[] = 'Wrapper: ' . $default_settings['wrapper'];
    }
    if (isset($settings['class']) && !empty($settings['class'])) {
      $summary[] = 'Class: ' . $settings['class'];
    }

    return $summary;
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {

    $settings = array(
      'link' => 0,
      'wrapper' => 'h2',
      'class' => ''
    );

    return $settings;
  }

  /**
   * {@inheritdoc}
   */
  protected function entityRenderKey() {
    return 'title';
  }

}
