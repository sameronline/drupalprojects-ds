<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\Title.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders a title.
 */
abstract class Title extends Field {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::settingsForm().
   */
  public function settingsForm() {

    $form = array();
    $form['link'] = array(
      'type' => 'select',
      'options' => array('no', 'yes')
    );
    $form['wrapper'] = array(
      'type' => 'textfield',
      'description' => t('Eg: h1, h2, p')
    );
    $form['class'] = array(
      'type' => 'textfield',
      'description' => t('Put a class on the wrapper. Eg: block-title')
    );

    return $form;
  }

  /**
   * Overrides \Drupal\ds\Plugin\ds\FieldPluginBase::defaultSettings().
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
   * Overrides \Drupal\ds\Plugin\ds\FieldPluginBase::entityRenderKey().
   */
  public function entityRenderKey() {
    return 'title';
  }

}
