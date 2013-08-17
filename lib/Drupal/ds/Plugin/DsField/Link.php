<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Link.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders a link.
 */
abstract class Link extends Field {

  /**
   * {@inheritdoc}
   */
  public function settings() {

    $settings = array();
    $settings['link text'] = array(
      'type' => 'textfield'
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
      'link text' => 'Read more',
      'wrapper' => '',
      'class' => '',
      'link' => 1 // TODO verify why we have this link here
    );

    return $settings;
  }

}
