<?php

/**
 * @file
 * Contains Drupal\ds\Annotation\DSPlugin.
 */

namespace Drupal\ds\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines a DSPlugin annotation object.
 *
 * @Annotation
 */
class DSPlugin extends Plugin {

  /**
   * The plugin ID.
   *
   * @var string
   */
  public $id;

  /**
   * The human-readable name of the DS plugin.
   *
   * @ingroup plugin_translatable
   *
   * @var \Drupal\Core\Annotation\Translation
   */
  public $title;

  /**
   * The name of the module providing the DS plugin.
   *
   * @var string
   */
  public $module;

  /**
   * The entity type this plugin should work on.
   *
   * @var string
   */
  public $entity_type;

}
