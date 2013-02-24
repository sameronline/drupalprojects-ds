<?php

/**
 * @file
 * Definition of Drupal\ds\DSBundle.
 */

namespace Drupal\ds;

use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\HttpKernel\Bundle\Bundle;

use Drupal\ds\DS;

/**
 * DS dependency injection container.
 */
class DSBundle extends Bundle {

  /**
   * Overrides Symfony\Component\HttpKernel\Bundle\Bundle::build().
   */
  public function build(ContainerBuilder $container) {
    $container->register("plugin.manager.ds", 'Drupal\ds\Plugin\DSPluginManager');
  }

}
