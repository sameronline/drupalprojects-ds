<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\FieldFormBase.
 */

namespace Drupal\ds_ui\Form;

use Drupal\system\SystemConfigFormBase;
use Drupal\Core\ControllerInterface;
use Drupal\Core\Entity\EntityManager;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Config\Context\ContextInterface;
use Drupal\Core\Cache\CacheBackendInterface;

use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Manages view modes used by Display Suite.
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
   * @var Drupal\Core\Cache\CacheBackendInterface
   */
  protected $cacheBackend;

  /**
   * The field array
   *
   * @var array
   */
  protected $field;

  /**
   * Constructs a \Drupal\system\CustomFieldFormBase object.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Config\Context\ContextInterface $context
   *   The configuration context to use.
   * @param \Drupal\Core\Entity\EntityManager
   *   The enitity manager.
   * @param \Drupal\Core\Cache\DatabaseBackend
   *   The cache backend.
   */
  public function __construct(ConfigFactory $config_factory, ContextInterface $context, EntityManager $entity_manager, CacheBackendInterface $cache_backend) {
    parent::__construct($config_factory, $context);
    $this->entityManager = $entity_manager;
    $this->cacheBackend = $cache_backend;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('config.factory'), $container->get('config.context.free'), $container->get('plugin.manager.entity'), $container->get('cache.cache'));
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
        'exists' => 'ds_field_unique',
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
      '#description' => t('Limit this field on field UI per bundles and/or view modes. The values are in the form of $bundle|$view_mode. You may use * to select all. Enter multiple values per line.'),
      '#type' => 'textarea',
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
    $this->cacheBackend->invalidateTags(array('ds_fields' => TRUE));

    // Redirect.
    $form_state['redirect'] = 'admin/structure/ds/fields';
    drupal_set_message(t('The field %field has been saved.', array('%field' => $field['label'])));
  }

}
