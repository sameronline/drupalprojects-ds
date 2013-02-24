
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\function_field\Link.
 */

namespace Drupal\ds\Plugin\ds\function_field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders a link.
 */
abstract class Link extends Field {

  /**
   * Implements \Drupal\ds\Plugin\ds\FieldPluginBase::settingsForm().
   */
  public function settingsForm() {

    $form = array();
    $form['link_text'] => array(
      'type' => 'textfield'
    ),
    $form['wrapper'] => array(
      'type' => 'textfield',
      'description' => t('Eg: h1, h2, p')
    ),
    $form['class'] => array(
      'type' => 'textfield',
      'description' => t('Put a class on the wrapper. Eg: block-title')
    ),

    return $form;
  }

  /**
   * Implements \Drupal\ds\Plugin\ds\FieldPluginBase::defaultSettings().
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
