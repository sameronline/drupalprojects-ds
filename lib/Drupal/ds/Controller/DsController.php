<?php

/**
 * @file
 * Contains \Drupal\ds\Controller\DsController.
 */

namespace Drupal\ds\Controller;

use Drupal\Component\Utility\String;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Entity\EntityInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Returns responses for Display Suite UI routes.
 */
class DsController extends ControllerBase {

  /**
   * Lists all bundles per entity type.
   *
   * @return array
   *   The Views fields report page.
   */
  public function listDisplays() {
    $build = array();

    // All entities.
    $entity_info = $this->entityManager()->getDefinitions();;

    // Move node to the top.
    if (isset($entity_info['node'])) {
      $node_entity = $entity_info['node'];
      unset($entity_info['node']);
      $entity_info = array_merge(array('node' => $node_entity), $entity_info);
    }

    $field_ui_enabled = $this->moduleHandler()->moduleExists('field_ui');
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
        $bundles = $this->entityManager()->getBundleInfo($entity_type);
        foreach ($bundles as $bundle_type => $bundle) {
          $row = array();
          $operations = array();
          $row[] = String::checkPlain($bundle['label']);

          if ($field_ui_enabled) {
            $path = $this->entityManager()->getAdminPath($entity_type, $bundle_type);
            $operations['manage_display'] = array(
              'title' => t('Manage display'),
              'href' => $path . '/display',
            );

            // Add Manage Form link if Display Suite Forms is enabled.
            if ($this->moduleHandler()->moduleExists('ds_forms')) {
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
        }
      }
    }

    $build['#attached']['library'][] = array('ds', 'ds.admin.css');

    return $build;
  }

  /**
   * Adds a contextual tabs to users
   */
  public function contextualUserTab(EntityInterface $user) {
    return $this->contextualTab($user->entityType(), $user->id());
  }

  /**
   * Adds a contextual tabs to taxonomy terms
   */
  public function contextualTaxonomyTermTab(EntityInterface $taxonomy_term) {
    return $this->contextualTab($taxonomy_term->entityType(), $taxonomy_term->id());
  }

  /**
   * Adds a contextual tabs to nodes
   */
  public function contextualNodeTab(EntityInterface $node) {
    return $this->contextualTab($node->entityType(), $node->id());
  }

  /**
   * Adds a contextual tab to entities.
   */
  public function contextualTab($entity_type, $entity_id) {
    $entity = entity_load($entity_type, $entity_id);

    $destination = $entity->uri();

    if (!empty($entity->ds_switch->value)) {
      $view_mode = $entity->ds_switch->value;
    }
    else {
      $view_mode = 'full';
    }

    // Check if we have a configured layout. Do not fallback to default.
    $layout = ds_get_layout($entity_type, $entity->bundle(), $view_mode, FALSE);

    // Get the manage display URI.
    $admin_path = $this->entityManager()->getAdminPath($entity_type, $entity->bundle());

    // Check view mode settings.
    $overridden = FALSE;
    $entity_display = entity_load('entity_display', $entity_type . '.' . $entity->bundle() . '.' . $view_mode);
    if ($entity_display) {
      $overridden = $entity_display->status();
    }

    if (empty($layout) && !$overridden) {
      $admin_path .= '/display';
    }
    else {
      $admin_path .= '/display/' . $view_mode;
    }

    return new RedirectResponse(url($admin_path, array('query' => array('destination' => $destination['path']))));
  }

}
