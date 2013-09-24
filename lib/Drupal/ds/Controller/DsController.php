<?php

/**
 * @file
 * Contains \Drupal\ds\Controller\DsController.
 */

namespace Drupal\ds\Controller;

use Drupal\Core\Controller\ControllerBase;
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
    $rows = array();
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
          $row[] = check_plain($bundle['label']);

          if ($field_ui_enabled) {
            $path = $this->entityManager()->getAdminPath($entity_type, $bundle_type);
            $operations['manage_display'] = array(
              'title' => t('Manage display'),
              'href' => $path . '/display',
            );

            // Add Mangage Form link if Display Suite Forms is enabled.
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

          $info_rows = array();
          $rows = array();
        }
      }
    }

    $build['#attached']['library'][] = array('ds', 'ds.admin.css');

    return $build;
  }

  /**
   * Adds a contextual tab to nodes, users and taxonomy terms.
   */
  public function contextualTab($entity_id, $entity_type) {
    $object = entity_load($entity_type, $entity_id);

    switch ($entity_type) {
      case 'node':
        $bundle = $object->bundle();
        $view_mode = (!empty($object->ds_switch->value)) ? $object->ds_switch->value : 'full';

        // Let's always go back to the node page.
        $destination = 'node/' . $object->id();
        break;
      case 'user':
        $bundle = 'user';
        $view_mode = 'full';
        $destination = 'user/' . $object->id();
        break;
      case 'taxonomy_term':
        $bundle = $object->bundle();
        $view_mode = 'full';
        $destination = 'taxonomy/term/' . $object->id();
        break;
    }

    // Check if we have a configured layout. Do not fallback to default.
    $layout = ds_get_layout($entity_type, $bundle, $view_mode, FALSE);

    // Get the manage display URI.
    $admin_path = $this->entityManager()->getAdminPath($entity_type, $bundle);

    // Check view mode settings.
    $view_mode_settings = field_view_mode_settings($entity_type, $bundle);
    $overriden = (!empty($view_mode_settings[$view_mode]['custom_settings']) ? TRUE : FALSE);

    if (empty($layout) && !$overriden) {
      $admin_path .= '/display';
    }
    else {
      $admin_path .= '/display/' . $view_mode;
    }

    return new RedirectResponse(url($admin_path, array('query' => array('destination' => $destination))));
  }

}
