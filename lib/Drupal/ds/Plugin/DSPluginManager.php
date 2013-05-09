<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPluginManager.
 */

namespace Drupal\ds\Plugin;

use Drupal\Component\Plugin\Discovery\DerivativeDiscoveryDecorator;
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
   * Overrides \Drupal\Component\Plugin\PluginManagerBase::__construct().
   *
   * @param \Traversable $namespaces
   *   An object that implements \Traversable which contains the root paths
   *   keyed by the corresponding namespace to look for plugin implementations,
   */
  public function __construct(\Traversable $namespaces) {
    $annotation_namespaces = array('Drupal\ds\Annotation' => $namespaces['Drupal\ds']);
    $this->discovery = new AnnotatedClassDiscovery('DSPlugin', $namespaces, $annotation_namespaces, 'Drupal\ds\Annotation\DSPlugin');
    $this->discovery = new DerivativeDiscoveryDecorator($this->discovery);
    $this->discovery = new AlterDecorator($this->discovery, 'ds_plugins');
    $this->discovery = new CacheDecorator($this->discovery, 'ds');
    $this->factory = new DefaultFactory($this->discovery);
  }

}
