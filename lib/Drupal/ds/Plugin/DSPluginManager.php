<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPluginManager.
 */

namespace Drupal\ds\Plugin;

use Drupal\Component\Plugin\PluginManagerBase;
use Drupal\Component\Plugin\Factory\DefaultFactory;
use Drupal\Core\Plugin\Discovery\AlterDecorator;
use Drupal\Core\Plugin\Discovery\AnnotatedClassDiscovery;
use Drupal\Core\Plugin\Discovery\CacheDecorator;

/**
 * Plugin type manager for all ds plugins.
 */
class DSPluginManager extends PluginManagerBase {

  /**
   * Constructs a DSPluginManager object.
   */
  public function __construct() {
    $this->discovery = new AnnotatedClassDiscovery('ds', 'field');
    $this->discovery = new AlterDecorator($this->discovery, 'ds_plugins');
    $this->discovery = new CacheDecorator($this->discovery, 'ds');
    $this->factory = new DefaultFactory($this->discovery);

    // We need a processdecorator if we want to merge defaults.
  }

}
