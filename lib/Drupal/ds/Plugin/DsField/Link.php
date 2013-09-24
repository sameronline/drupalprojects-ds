<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Link.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders a link.
 */
abstract class Link extends Field {

  /**
   * {@inheritdoc}
   */
  public function settingsForm($field, $settings) {
    $default_settings = $this->defaultSettings();

    $form['link text'] = array(
      '#type' => 'textfield',
      '#title' => 'Link text',
      '#default_value' => isset($settings['link text']) ? $settings['link text'] : $default_settings['link text'],
    );
    $form['wrapper'] = array(
      '#type' => 'textfield',
      '#title' => 'Wrapper',
      '#default_value' => isset($settings['wrapper']) ? $settings['wrapper'] : $default_settings['wrapper'],
      '#description' => t('Eg: h1, h2, p')
    );
    $form['class'] = array(
      '#type' => 'textfield',
      '#title' => 'Class',
      '#default_value' => isset($settings['class']) ? $settings['class'] : $default_settings['class'],
      '#description' => t('Put a class on the wrapper. Eg: block-title')
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary($field, $settings) {
    $default_settings = $this->defaultSettings();

    $summary = array();
    if (isset($settings['link text'])) {
      $summary[] = 'Link text: ' . $settings['link text'];
    }
    else {
      $summary[] = 'Link text: ' . $default_settings['link text'];
    }
    if (isset($settings['wrapper']) && !empty($settings['wrapper'])) {
      $summary[] = 'Wrapper: ' . $settings['wrapper'];
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
      'link text' => 'Read more',
      'wrapper' => '',
      'class' => '',
      'link' => 1,
    );

    return $settings;
  }

}
