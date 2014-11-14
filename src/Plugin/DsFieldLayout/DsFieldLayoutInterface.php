<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsFieldLayout\DsFieldLayoutInterface.
 */

namespace Drupal\ds\Plugin\DsFieldLayout;

/**
 * Base class for all the ds plugins.
 */
interface DsFieldLayoutInterface {

  /**
   * TODO
   */
  public function alterForm(&$form);

  /**
   * TODO
   */
  public function massageRenderValues(&$field_settings, $values);
  /**
   * TODO
   */
  public function getThemeFunction();
  /**
   *
   * TODO
   */
  public function defaultConfiguration();

  /**
   * TODO
   */
  public function getConfiguration();

  /**
   * TODO
   */
  public function setConfiguration(array $configuration);

}
