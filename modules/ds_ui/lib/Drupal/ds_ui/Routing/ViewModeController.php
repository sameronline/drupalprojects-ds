<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Routing\ViewModeController.
 */

namespace Drupal\ds_ui\Routing;

use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Config\StorageInterface;
use Drupal\Core\ControllerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Route controller for view modes.
 */
class ViewModeController implements ControllerInterface {

  /**
   * Stores the configuration factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $configFactory;

  /**
   * The config storage.
   *
   * @var \Drupal\Core\Config\StorageInterface;
   */
  protected $storage;

  /**
   * Constructs a ViewModeController object.
   *
   * @param \Drupal\Core\Config\StorageInterface $storage
   *   The configuration storage.
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The factory for configuration objects.
   */
  public function __construct(StorageInterface $storage, ConfigFactory $config_factory) {
    $this->storage = $storage;
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('config.storage'), $container->get('config.factory'));
  }

  /**
   * Builds a view mode list
   */
  public function viewModeList() {
    $output = '';
    $view_modes = $this->storage->listAll('ds.view_modes');

    if (!empty($view_modes)) {

      $rows = array();
      foreach ($view_modes as $config) {
        $view_mode_value = $this->configFactory->get($config)->get();

        $row = array();
        $row[] = check_plain($view_mode_value['label']);
        $row[] = $view_mode_value['view_mode'];
        $row[] = ucwords(str_replace('_', ' ', implode(', ', $view_mode_value['entities'])));
        $operations = array();
        $operations['edit'] = array(
          'title' => t('Edit'),
          'href' => 'admin/structure/ds/view_modes/manage/' . $view_mode_value['view_mode'],
        );
        $operations['delete'] = array(
          'title' => t('Delete'),
          'href' => 'admin/structure/ds/view_modes/delete/' . $view_mode_value['view_mode'],
        );
        $row[] = array(
          'data' => array(
            '#type' => 'operations',
            '#subtype' => 'ds',
            '#links' => $operations,
          ),
        );

        $rows[] = $row;
      }

      $table = array(
        'header' => array(
          'Label',
          'Machine name',
          'Entities',
          'Operations',
        ),
        'rows' => $rows,
      );

      $output = theme('table', $table);
    }
    else {
      $output = t('No custom view modes have been defined.');
    }

    return $output;
  }

}
