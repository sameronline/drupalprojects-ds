<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Form\ViewModeDeleteForm.
 */

namespace Drupal\ds_ui\Form;

use Drupal\Core\Form\ConfirmFormBase;
use Drupal\Core\ControllerInterface;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Config\StorageInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a form to delete a user's Open ID identity.
 */
class ViewModeDeleteForm extends ConfirmFormBase implements ControllerInterface {

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
   * Constructs a ViewModeDeleteForm object.
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
   * {@inheritdoc}
   */
  protected function getQuestion() {
    return t('Are you sure you want to delete %view_mode ?', array('%view_mode' => $this->viewMode['label']));
  }

  /**
   * {@inheritdoc}
   */
  protected function getCancelPath() {
    return 'admin/structure/ds/view_modes';
  }

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'view_mode_delete_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state, $view_mode = '') {
    $config = $this->configFactory->get('ds.view_modes.' . $view_mode);
    $this->viewMode = $config->get();

    if (empty($this->viewMode)) {
      drupal_set_message(t('View mode not found.'));
      drupal_goto('admin/structure/ds/view_modes');
    }

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $view_mode = $this->viewMode;

    // Remove view mode from database.
    $this->configFactory->get('ds.view_modes.' . $view_mode['view_mode'])->delete();

    // Remove layout and field settings for this view mode.
    $field_settings = $this->storage->listAll('ds.field_settings');
    foreach ($field_settings as $config) {
      $config = $this->configFactory->get($config);
      if ($config->get('view_mode') == $view_mode['view_mode']) {
        $config->delete();
      }
    }

    $layout_settings = $this->storage->listAll('ds.layout_settings');
    foreach ($layout_settings as $config) {
      $config = $this->configFactory->get($config);
      if ($config->get('view_mode') == $view_mode['view_mode']) {
        $config->delete();
      }
    }

    // Clear entity info cache and trigger menu build on next request.
    entity_info_cache_clear();
    menu_router_rebuild();

    // Redirect.
    $form_state['redirect'] = 'admin/structure/ds/view_modes';
    drupal_set_message(t('The view mode %view_mode has been deleted.', array('%view_mode' => $view_mode['label'])));

  }

}
