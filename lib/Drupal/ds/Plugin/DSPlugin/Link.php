<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\Link.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders a link.
 */
abstract class Link extends Field {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::settings().
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
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::defaultSettings().
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
