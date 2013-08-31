<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Access\SwitchFieldAccessCheck.
 */

namespace Drupal\ds_extras\Access;

use Drupal\Core\Access\StaticAccessCheckInterface;
use Drupal\Core\Database\Connection;
use Drupal\Core\Config\ConfigFactory;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\Routing\Route;

/**
 * Provides an access check for ds extras switch field routes.
 */
class SwitchFieldAccessCheck implements StaticAccessCheckInterface {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $configFactory;

  /**
   * Constructs a HTTP basic authentication provider object.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The config factory.
   */
  public function __construct(ConfigFactory $config_factory) {
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function appliesTo() {
    return array('_access_switch_field');
  }

  /**
   * {@inheritdoc}
   */
  public function access(Route $route, Request $request) {
    return \Drupal::currentUser()->hasPermission('access content') && $this->configFactory->get('ds.extras')->get('switch_field') ? static::ALLOW : static::DENY;
  }

}
