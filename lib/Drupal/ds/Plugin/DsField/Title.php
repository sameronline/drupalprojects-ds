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
  public function settings() {

    $settings = array();
    $settings['link'] = array(
      'type' => 'select',
      'options' => array('no', 'yes')
    );
    $settings['wrapper'] = array(
      'type' => 'textfield',
      'description' => t('Eg: h1, h2, p')
    );
    $settings['class'] = array(
      'type' => 'textfield',
      'description' => t('Put a class on the wrapper. Eg: block-title')
    );

    return $settings;
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
