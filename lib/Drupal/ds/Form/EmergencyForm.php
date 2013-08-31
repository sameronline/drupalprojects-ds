<?php

/**
 * @file
 * Contains \Drupal\ds\Form\EmergencyForm.
 */

namespace Drupal\ds\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Emergency form for DS.
 */
class EmergencyForm extends FormBase {

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Extension\ConfigFactory
   */
  protected $configFactory;

  /**
   * The module handler.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * Constructs a \Drupal\ds\Form\EmergencyForm object.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The config factory.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface
   *   The module handler.
   */
  public function __construct(ConfigFactory $config_factory, ModuleHandlerInterface $module_handler) {
    $this->configFactory = $config_factory;
    $this->moduleHandler = $module_handler;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('config.factory'),
      $container->get('module_handler')
    );
 }

  /**
   * {@inheritdoc}
   */
  public function getFormID() {
    return 'ds_emergy_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {

    $form['#title'] = 'Emergency settings';

    $form['ds_fields_error'] = array(
      '#type' => 'fieldset',
      '#title' => $this->t('Fields error'),
    );

    $form['ds_fields_error']['disable'] = array(
      '#type' => 'checkbox',
      '#title' => $this->t('Disable attaching fields via Display Suite'),
      '#description' => $this->t('In case you get an error after configuring a layout printing a message like "Fatal error: Unsupported operand types", you can temporarily disable adding fields from DS by toggling this checkbox. You probably are trying to render an node inside a node, for instance through a view, which is simply not possible. See <a href="http://drupal.org/node/1264386">http://drupal.org/node/1264386</a>.'),
      '#default_value' => $this->configFactory->get('ds.settings')->get('disable', FALSE),
      '#weight' => 0,
    );

    $form['ds_fields_error']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Disable/enable field attach'),
      '#submit' => array(array($this, 'submitFieldAttach')),
      '#weight' => 1,
    );

    if ($this->moduleHandler->moduleExists('ds_extras')) {
      $region_blocks = $this->configFactory->get('ds.extras')->get('region_blocks');
      if (!empty($region_blocks)) {

        $region_blocks_options = array();
        foreach ($region_blocks as $key => $info) {
          $region_blocks_options[$key] = $info['title'];
        }

        $form['region_to_block'] = array(
          '#type' => 'fieldset',
          '#title' => $this->t('Block regions'),
        );

        $form['region_to_block']['remove_block_region'] = array(
          '#type' => 'checkboxes',
          '#options' => $region_blocks_options,
          '#description' => $this->t('In case you renamed a content type, you will not see the configured block regions anymore, however the block on the block settings page is still available. On this screen you can remove orphaned block regions.'),
        );

        $form['region_to_block']['submit'] = array(
          '#type' => 'submit',
          '#value' => $this->t('Remove block regions'),
          '#submit' => array(array($this, 'submitRegionToBlock')),
          '#weight' => 1,
        );
      }
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    // empty
  }

  /**
   * Submit callback for the fields error form.
   */
  public function submitFieldAttach(array &$form, array &$form_state) {
    $this->configFactory->get('ds.settings')->set('disable', $form_state['values']['disable'])->save();
    drupal_set_message(t('The configuration options have been saved.'));
  }

  /**
   * Submit callback for the region to block form
   */
  public function submitRegionToBlock(array &$form, array &$form_state) {
    if (isset($form_state['values']['remove_block_region'])) {
      $save = FALSE;
      $region_blocks = $this->configFactory->get('ds.extras')->get('region_blocks');
      $remove = $form_state['values']['remove_block_region'];
      foreach ($remove as $key => $value) {
        if ($key === $value) {
          $save = TRUE;

          // Make sure there is no active block instance for this ds block region.
          if (\Drupal::moduleHandler()->moduleExists('block')) {
            $ids = \Drupal::entityQuery('block')
              ->condition('plugin', 'ds_region_block:' . $key)
              ->execute();
            $block_storage = \Drupal::entityManager()->getStorageController('block');
            foreach ($block_storage->loadMultiple($ids) as $block) {
              $block->delete();
            }
          }

          unset($region_blocks[$key]);
        }
      }

      if ($save) {
        drupal_set_message(t('Block regions were removed.'));

        // Clear cached block and ds plugin defintions
        \Drupal::service('plugin.manager.block')->clearCachedDefinitions();
        \Drupal::service('plugin.manager.ds')->clearCachedDefinitions();

        $this->configFactory->get('ds.extras')->set('region_blocks', $region_blocks)->save();
      }
    }
    else {
      drupal_set_message($this->t('No block regions were removed.'));
    }
  }

}
