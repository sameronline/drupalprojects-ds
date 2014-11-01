<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Controller\FieldController.
 */

namespace Drupal\ds_ui\Controller;

use Drupal\Component\Utility\String;
use Drupal\Core\Config\StorageInterface;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Drupal\Core\Url;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Route controller fields.
 */
class FieldController extends ControllerBase implements ContainerInjectionInterface {

  /**
   * The config storage.
   *
   * @var \Drupal\Core\Config\StorageInterface;
   */
  protected $storage;

  /**
   * Constructs a \Drupal\ds_ui\Routing\FieldController object.
   *
   * @param \Drupal\Core\Config\StorageInterface $storage
   *   The configuration storage.
   */
  public function __construct(StorageInterface $storage) {
    $this->storage = $storage;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('config.storage'));
  }

  /**
   * Builds a list of fields
   */
  public function fieldList() {
    $custom_fields = $this->storage->listAll('ds.field.');
    if (!empty($custom_fields)) {

      $rows = array();
      foreach ($custom_fields as $config) {
        $field_value = $this->config($config)->get();
        $row = array();
        $row[] = String::checkPlain($field_value['label']);
        $row[] = isset($field_value['type_label']) ? $field_value['type_label'] : $this->t('Unknown');
        $row[] = $field_value['id'];
        $row[] = ucwords(str_replace('_', ' ', implode(', ', $field_value['entities'])));

        $operations = array();
        $operations['edit'] = array(
          'title' => $this->t('Edit'),
          'url' => new url('ds_ui.manage_field', array('field_key' => $field_value['id'])),
        );
        $operations['delete'] = array(
          'title' => $this->t('Delete'),
          'url' => new url('ds_ui.delete_field', array('field' => $field_value['id'])),
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

      $build = array(
        '#theme' => 'table',
        '#header' => array(
          'Label',
          'Type',
          'Machine name',
          'Entities',
          'Operations',
        ),
        '#rows' => $rows,
      );

    }
    else {
      $build = array(
        '#markup' => $this->t('No custom fields have been defined.'),
      );
    }

    return $build;
  }

  /**
   * Redirect to the correct manage callback.
   */
  public function manageRedirect($field_key) {
    $config = $this->config('ds.field.' . $field_key);
    if ($field = $config->get()) {
      $url = new url('ds_ui.manage_' . $field['type'] . '_field', array('field_key' => $field_key));
      if ($url->toString()) {
        return new RedirectResponse($url->toString());
      }
    }

    drupal_set_message($this->t('Field not found'));
    $url = new Url('ds_ui.fields_list');
    return new RedirectResponse($url->toString());
  }

}
