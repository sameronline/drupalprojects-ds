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
  public function __construct($type) {
    $this->discovery = new AnnotatedClassDiscovery('ds', $type);
    $this->discovery = new AlterDecorator($this->discovery, 'ds_field_plugins_' . $type);
    $this->discovery = new CacheDecorator($this->discovery, 'ds:' . $type);

    $this->factory = new DefaultFactory($this->discovery);

    // Find out why we would need a derivative decorator... If we need any...

    // Does ds use a custom cache bin? I guess not.

    // We need a processdecorator if we want to merge defaults.

    // Do we want an alter for each field type or do we prefer different alters
    // for each field plugin type?
  }

}
