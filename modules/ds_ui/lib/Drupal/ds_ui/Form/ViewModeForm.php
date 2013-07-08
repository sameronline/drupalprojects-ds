<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\ViewModeForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\system\SystemConfigFormBase;
use Drupal\Core\ControllerInterface;
use Drupal\Core\Entity\EntityManager;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Config\Context\ContextInterface;

use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Manages view modes used by Display Suite.
 */
class ViewModeForm extends SystemConfigFormBase implements ControllerInterface {


  /**
   * Holds the entity manager
   *
   * @var \Drupal\Core\Entity\EntityManager
   */
  protected $entityManager;

  /**
   * Constructs a \Drupal\system\ViewModeForm object.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Config\Context\ContextInterface $context
   *   The configuration context to use.
   * @param \Drupal\Core\Entity\EntityManager
   *   The enitty manager.
   */
  public function __construct(ConfigFactory $config_factory, ContextInterface $context, EntityManager $entity_manager) {
    parent::__construct($config_factory, $context);
    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static($container->get('config.factory'), $container->get('config.context.free'), $container->get('plugin.manager.entity'));
  }

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'ds_view_modes_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state, $view_mode = '') {

    $config = $this->configFactory->get('ds.view_modes.' . $view_mode);

    if (!empty($view_mode)) {
      $view_mode = $config->get();
    }

    if (!$view_mode) {
      $view_mode = array();
      $view_mode['label'] = '';
      $view_mode['view_mode'] = '';
      $view_mode['entities'] = array();
    }

    $form['name'] = array(
      '#title' => t('Label'),
      '#type' => 'textfield',
      '#default_value' => $view_mode['label'],
      '#description' => t('The human-readable label of the view mode. This name must be unique.'),
      '#required' => TRUE,
      '#maxlength' => 32,
      '#size' => 30,
    );

    $form['view_mode'] = array(
      '#title' => t('Machine name'),
      '#type' => 'machine_name',
      '#default_value' => $view_mode['view_mode'],
      '#maxlength' => 32,
      '#description' => t('The machine-readable name of this view mode. This name must contain only lowercase letters and underscores. This name must be unique.'),
      '#disabled' => !empty($view_mode['view_mode']),
      '#machine_name' => array(
        'exists' => array($this, 'ds_view_mode_unique'),
        'source' => array('name'),
      ),
    );

    $entity_options = array();
    $entities = $this->entityManager->getDefinitions();
    foreach ($entities as $entity_type => $entity_info) {
      if (isset($entity_info['fieldable']) && $entity_info['fieldable']) {
        $entity_options[$entity_type] = drupal_ucfirst(str_replace('_', ' ', $entity_type));
      }
    }
    $form['entities'] = array(
      '#title' => t('Entities'),
      '#description' => t('Select the entities for which this view mode will be made available.'),
      '#type' => 'checkboxes',
      '#required' => TRUE,
      '#options' => $entity_options,
      '#default_value' => $view_mode['entities'],
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );

    $form['existing'] = array('#type' => 'value', '#value' => !empty($view_mode['view_mode']));

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {
    $view_mode = array();
    $view_mode['view_mode'] = $form_state['values']['view_mode'];
    $view_mode['label'] = $form_state['values']['name'];

    $reserved = array();
    $entities = $form_state['values']['entities'];
    foreach ($entities as $key => $value) {
      if ($key !== $value) {
        unset($entities[$key]);
      }
      else {
        $reserved += entity_get_view_modes($key);
      }
    }

    if (array_key_exists($view_mode['view_mode'], $reserved) && !isset($form_state['values']['existing'])) {
      form_set_error('type', t('The machine-readable name %view_mode is reserved.', array('%view_mode' => $view_mode['view_mode'])));
    }
    else {
      $view_mode['entities'] = $entities;
      $form_state['view_mode'] = $view_mode;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $view_mode = $form_state['view_mode'];

    $config = $this->configFactory->get('ds.view_modes.' . $view_mode['view_mode']);

    // Save view mode.
    $config->setData($view_mode)->save();

    // Clear entity info cache and trigger menu build on next request.
    entity_info_cache_clear();
    menu_router_rebuild();

    // Redirect.
    $form_state['redirect'] = 'admin/structure/ds/view_modes';
    drupal_set_message(t('The view mode %view_mode has been saved.', array('%view_mode' => $view_mode['label'])));
  }

  /**
   * Returns whether a view mode machine name is unique.
   */
  public function ds_view_mode_unique($name) {
    $value = strtr($name, array('-' => '_'));
    if (config('ds.view_modes.' . $value)->get()) {
      return TRUE;
    }
    return FALSE;
  }

}
