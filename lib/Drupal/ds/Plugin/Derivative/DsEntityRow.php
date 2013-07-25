<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DsEntityRow.
 */

namespace Drupal\ds\Plugin\Derivative;

use Drupal\Component\Plugin\Derivative\DerivativeInterface;
use Drupal\views\Views;

/**
 * Provides views row plugin definitions for all non-special entity types.
 *
 * @ingroup views_row_plugins
 *
 * @see \Drupal\ds\Plugin\views\row\DsEntityRow
 */
class DsEntityRow implements DerivativeInterface {

  /**
   * Stores all entity row plugin information.
   *
   * @var array
   */
  protected $derivatives = array();

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinition($derivative_id, array $base_plugin_definition) {
    if (!empty($this->derivatives) && !empty($this->derivatives[$derivative_id])) {
      return $this->derivatives[$derivative_id];
    }
    $this->getDerivativeDefinitions($base_plugin_definition);
    return $this->derivatives[$derivative_id];
  }

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions(array $base_plugin_definition) {
    $entity_types = \Drupal::entityManager()->getDefinitions();

    $views_data = Views::viewsData();
    foreach ($entity_types as $entity_type => $entity_info) {
      // Just add support for entity types which have a views integration.
      if (isset($entity_info['base_table']) && $views_data->get($entity_info['base_table']) && \Drupal::entityManager()->hasController($entity_type, 'render')) {
        $this->derivatives[$entity_type] = array(
          'id' => 'ds_entity:' . $entity_type,
          'module' => 'ds',
          'title' => 'Display Suite',
          'help' => t('Display the @label', array('@label' => $entity_info['label'])),
          'base' => array($entity_info['base_table']),
          'entity_type' => $entity_type,
          'display_types' => array('normal'),
          'class' => $base_plugin_definition['class'],
        );
      }
    }

    return $this->derivatives;
  }

}
