<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\FieldFormBase.
 */

namespace Drupal\ds_ui\Form;

use Drupal\system\SystemConfigFormBase;
use Drupal\Core\Controller\ControllerInterface;
use Drupal\Core\Entity\EntityManager;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Config\Context\ContextInterface;
use Drupal\Core\Cache\CacheBackendInterface;
use Drupal\Core\Extension\ModuleHandler;

use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Base form for fields.
 */
class FieldFormBase extends SystemConfigFormBase implements ControllerInterface {

  /**
   * Holds the entity manager
   *
   * @var \Drupal\Core\Entity\EntityManager
   */
  protected $entityManager;

  /**
   * Holds the cache backend
   *
   * @var \Drupal\Core\Cache\CacheBackendInterface
   */
  protected $cacheBackend;

  /**
   * The field array
   *
   * @var array
   */
  protected $field;

  /**
   * Drupal module handler
   *
   * @var \Drupal\Core\Extension\ModuleHandler
   */
  protected $moduleHandler;

  /**
   * Constructs a \Drupal\system\CustomFieldFormBase object.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Config\Context\ContextInterface $context
   *   The configuration context to use.
   * @param \Drupal\Core\Entity\EntityManager
   *   The enitity manager.
   * @param \Drupal\Core\Cache\CacheBackendInterface
   *   The cache backend.
   * @param \Drupal\Core\Extension\ModuleHandler
   *   The module handler.
   */
  public function __construct(ConfigFactory $config_factory, ContextInterface $context, EntityManager $entity_manager, CacheBackendInterface $cache_backend, ModuleHandler $module_handler) {
    parent::__construct($config_factory, $context);
    $this->entityManager = $entity_manager;
    $this->cacheBackend = $cache_backend;
    $this->moduleHandler = $module_handler;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('config.factory'),
      $container->get('config.context.free'),
      $container->get('plugin.manager.entity'),
      $container->get('cache.cache'),
      $container->get('module_handler')
    );
  }


  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'ds_custom_field_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state, $field_key = '') {

    if (!empty($field_key)) {
      $field = $this->configFactory->get('ds.field.' . $field_key)->get();
    }

    if (!isset($field)) {
      $field = array();
      $field['label'] = '';
      $field['field'] = '';
      $field['ui_limit'] = '';
      $field['entities'] = array();
      $field['properties'] = array();
    }

    $this->field = $field;

    $form['name'] = array(
      '#title' => t('Label'),
      '#type' => 'textfield',
      '#default_value' => $field['label'],
      '#description' => t('The human-readable label of the field.'),
      '#maxlength' => 32,
      '#required' => TRUE,
      '#size' => 30,
    );

    $form['field'] = array(
      '#type' => 'machine_name',
      '#default_value' => $field['field'],
      '#maxlength' => 32,
      '#description' => t('The machine-readable name of this field. This name must contain only lowercase letters and underscores. This name must be unique.'),
      '#disabled' => !empty($field['field']),
      '#machine_name' => array(
        'exists' => array($this, 'ds_field_unique'),
        'source' => array('name'),
      ),
    );

    $entity_options = array();
    $entities = $this->entityManager->getDefinitions();
    foreach ($entities as $entity_type => $entity_info) {
      if ((isset($entity_info['fieldable']) && $entity_info['fieldable']) || $entity_type == 'ds_views') {
        $entity_options[$entity_type] = drupal_ucfirst(str_replace('_', ' ', $entity_type));
      }
    }
    $form['entities'] = array(
      '#title' => t('Entities'),
      '#description' => t('Select the entities for which this field will be made available.'),
      '#type' => 'checkboxes',
      '#required' => TRUE,
      '#options' => $entity_options,
      '#default_value' => $field['entities'],
    );

    $form['ui_limit'] = array(
      '#title' => t('Limit field'),
      '#description' => t('Limit this field on field UI per bundles and/or view modes. The values are in the form of $bundle|$view_mode, where $view_mode may be either a view mode set to use custom settings, or \'default\'. You may use * to select all, e.g article|*, *|full or *|*. Enter one value per line.'),      '#type' => 'textarea',
      '#default_value' => $field['ui_limit'],
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 100,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {
    $field = array();
    $field['properties'] = array();
    $field['field'] = $form_state['values']['field'];
    $field['label'] = $form_state['values']['name'];
    $field['ui_limit'] = $form_state['values']['ui_limit'];

    $entities = $form_state['values']['entities'];
    foreach ($entities as $key => $value) {
      if ($key !== $value) {
        unset($entities[$key]);
      }
    }
    $field['entities'] = $entities;

    $this->field = $field;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $field = $this->field;

    // Save field and clear ds_fields.
    $this->configFactory->get('ds.field.' . $field['field'])->setData($field)->save();
    $this->cacheBackend->deleteTags(array('ds_fields_info' => TRUE));

    // @todo find out how we can clear derivatives without clearing everything.
    drupal_flush_all_caches();

    // Redirect.
    $form_state['redirect'] = 'admin/structure/ds/fields';
    drupal_set_message(t('The field %field has been saved.', array('%field' => $field['label'])));
  }

  /**
   * Returns whether a field machine name is unique.
   */
  public function ds_field_unique($name) {
    $value = strtr($name, array('-' => '_'));
    if (\Drupal::config('ds.field.' . $value)->get()) {
      return TRUE;
    }
    return FALSE;
  }

}
