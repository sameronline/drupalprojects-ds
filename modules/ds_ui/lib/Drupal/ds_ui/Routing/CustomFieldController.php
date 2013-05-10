<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Routing\CustomFieldController.
 */

namespace Drupal\ds_ui\Routing;

use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Config\StorageInterface;
use Drupal\Core\ControllerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Route controller for view modes.
 */
class CustomFieldController implements ControllerInterface {

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
   * Constructs a CustomFieldController object.
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
   * Builds a list of custom fields
   */
  public function customFieldList() {
    $output = '';

    $custom_fields = $this->storage->listAll('ds.field');
    if (!empty($custom_fields)) {

      $rows = array();
      foreach ($custom_fields as $config) {
        $field_value = $this->configFactory->get($config)->get();
        $row = array();
        $row[] = check_plain($field_value['label']);
        $row[] = $this->getHumanNameFieldFromType($field_value['field_type']);
        $row[] = $field_value['field'];
        $row[] = ucwords(str_replace('_', ' ', implode(', ', $field_value['entities'])));

        $operations = array();
        $operations['edit'] = array(
          'title' => t('Edit'),
          'href' => 'admin/structure/ds/fields/manage/' . $field_value['field'],
        );
        $operations['delete'] = array(
          'title' => t('Delete'),
          'href' => 'admin/structure/ds/fields/delete/' . $field_value['field'],
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
          'Type',
          'Machine name',
          'Entities',
          'Operations',
        ),
        'rows' => $rows,
      );

      $output = theme('table', $table);
    }
    else {
      $output = t('No custom fields have been defined.');
    }

    return $output;
  }

  /**
   * Return the human name of a field.
   *
   * @return $human_name
   *   The human name of a field.
   */
  protected function getHumanNameFieldFromType($type) {

    switch ($type) {
      case DS_FIELD_TYPE_CODE:
        return t('Code field');
      case DS_FIELD_TYPE_BLOCK:
        return t('Block field');
      case DS_FIELD_TYPE_PREPROCESS:
        return t('Preprocess field');
    }

    // Fallback
    return t('Unknown');
  }

  /**
   * Redirect to the correct manage callback.
   */
  public function manageRedirect($field_key) {
    $redirect = '';
    if ($field = $this->configFactory->get('ds.field.' . $field_key)->get()) {
      switch ($field['field_type']) {
        case DS_FIELD_TYPE_CODE:
          $redirect = 'admin/structure/ds/fields/manage_custom/' . $field_key;
          break;

        case DS_FIELD_TYPE_BLOCK:
          $redirect = 'admin/structure/ds/fields/manage_block/' . $field_key;
          break;

        case DS_FIELD_TYPE_PREPROCESS:
          $redirect = 'admin/structure/ds/fields/manage_preprocess/' . $field_key;
          break;
      }
    }

    if (!$redirect) {
      drupal_set_message(t('Field not found'));
      $redirect = 'admin/structure/ds/fields';
    }

    drupal_goto($redirect);
  }

}
