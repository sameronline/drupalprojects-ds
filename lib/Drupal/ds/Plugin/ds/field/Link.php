
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\Link.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders a link.
 */
abstract class Link extends Field {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\PluginBase::settingsForm().
   */
  public function settingsForm() {

    $form = array();
    $form['link_text'] => array(
      'type' => 'textfield'
    );
    $form['wrapper'] => array(
      'type' => 'textfield',
      'description' => t('Eg: h1, h2, p')
    );
    $form['class'] => array(
      'type' => 'textfield',
      'description' => t('Put a class on the wrapper. Eg: block-title')
    );

    return $form;
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
