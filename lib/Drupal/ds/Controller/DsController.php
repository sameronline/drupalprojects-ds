<?php

/**
 * @file
 * Contains \Drupal\ds\Controller\DsController.
 */

namespace Drupal\ds\Controller;

use Drupal\Core\Controller\ControllerInterface;
use Drupal\Core\Entity\EntityManager;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Returns responses for Views UI routes.
 */
class DsController implements ControllerInterface {

  /**
   * Stores the Entity manager.
   *
   * @var \Drupal\Core\Entity\EntityManager
   */
  protected $entityManager;

  /**
   * Constructs a new \Drupal\views_ui\Controller\ViewsUIController object.
   *
   * @param \Drupal\Core\Entity\EntityManager $entity_manager
   *   The Entity manager.
   */
  public function __construct(EntityManager $entity_manager) {
    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('plugin.manager.entity')
    );
  }

  /**
   * Lists all instances of fields on any views.
   *
   * @return array
   *   The Views fields report page.
   */
  public function listDisplays() {
    $build = array();

    // All entities.
    $rows = array();
    $entity_info = entity_get_info();

    // Move node to the top.
    if (isset($entity_info['node'])) {
      $node_entity = $entity_info['node'];
      unset($entity_info['node']);
      $entity_info = array_merge(array('node' => $node_entity), $entity_info);
    }

    $field_ui_enabled = module_exists('field_ui');
    if (!$field_ui_enabled) {
      $build['no_field_ui'] = array(
        '#markup' => '<p>' . t('You need to enable Field UI to manage the displays of entities.') . '</p>',
        '#weight' => -10,
      );
    }

    if (isset($entity_info['comment'])) {
      $comment_entity = $entity_info['comment'];
      unset($entity_info['comment']);
      $entity_info['comment'] = $comment_entity;
    }

    foreach ($entity_info as $entity_type => $info) {
      if (!empty($info['fieldable']) && !empty($info['base_table'])) {
        $rows = array();
        $bundles = entity_get_bundles($entity_type);
        foreach ($bundles as $bundle_type => $bundle) {
          $row = array();
          $operations = array();
          $row[] = check_plain($bundle['label']);

          if ($field_ui_enabled) {
            $path = $this->entityManager->getAdminPath($entity_type, $bundle_type);
            $operations['manage_display'] = array(
              'title' => t('Manage display'),
              'href' => $path . '/display',
            );

            // Add Mangage Form link if Display Suite Forms is enabled.
            if (module_exists('ds_forms')) {
              $operations['manage_form'] = array(
                'title' => t('Manage form'),
                'href' => $path . '/fields',
              );
            }
          }

          // Add operation links.
          if (!empty($operations)) {
            $row[] = array(
              'data' => array(
                '#type' => 'operations',
                '#subtype' => 'ds',
                '#links' => $operations,
              ),
            );
          }
          else {
            $row[] = array('data' => '');
          }

          $rows[] = $row;
        }

        if (!empty($rows)) {
          $variables = array(
            'header' => array(
              array('data' => $info['label']),
              array(
                'data' => $field_ui_enabled ? t('operations') : '',
                'class' => 'ds-display-list-options')
              ),
            'rows' => $rows,
          );
          $build['list_' . $entity_type] = array(
            '#markup' => theme('table', $variables)
          );

          $info_rows = array();
          $rows = array();
        }
      }
    }

    $build['#attached']['css'] = array(drupal_get_path('module', 'ds') . '/css/ds.admin.css');

    return $build;
  }

}
