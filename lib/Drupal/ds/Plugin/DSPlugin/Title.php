<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\Title.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders a title.
 */
abstract class Title extends Field {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::settings().
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
   * Overrides \Drupal\ds\Plugin\ds\PluginBase::defaultSettings().
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
   * Overrides \Drupal\ds\Plugin\ds\Field::entityRenderKey().
   */
  protected function entityRenderKey() {
    return 'title';
  }

}
