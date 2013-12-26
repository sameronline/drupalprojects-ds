<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\views\row\DsEntityRow.
 */

namespace Drupal\ds\Plugin\views\row;

use Drupal\Component\Utility\String;
use Drupal\Core\Entity\EntityManager;
use Drupal\views\Plugin\views\display\DisplayPluginBase;
use Drupal\views\ViewExecutable;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\views\Plugin\views\row\RowPluginBase;

/**
 * Generic entity row plugin to provide a common base for all entity types.
 *
 * @ViewsRow(
 *   id = "ds_entity",
 *   derivative = "Drupal\ds\Plugin\Derivative\DsEntityRow"
 * )
 */
class DsEntityRow extends RowPluginBase {

  /**
   * The table the entity is using for storage.
   *
   * @var string
   */
  public $base_table;

  /**
   * The actual field which is used for the entity id.
   *
   * @var string
   */
  public $base_field;

  /**
   * Stores the entity type of the result entities.
   *
   * @var string
   */
  protected $entityType;

  /**
   * Contains the entity info of the entity type of this row plugin instance.
   *
   * @see entity_get_info
   */
  protected $entityInfo;

  /**
   * Contains an array of render arrays, one for each rendered entity.
   *
   * @var array
   */
  protected $build = array();

  /**
   * {@inheritdoc}
   *
   * @param \Drupal\Core\Entity\EntityManager $entity_manager
   *   The entity manager.
   */
  public function __construct(array $configuration, $plugin_id, array $plugin_definition, EntityManager $entity_manager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->entityManager = $entity_manager;
  }

  /**
   * {@inheritdoc}
   */
  public function init(ViewExecutable $view, DisplayPluginBase $display, array &$options = NULL) {
    parent::init($view, $display, $options);

    $this->entityType = $this->definition['entity_type'];
    $this->entityInfo = $this->entityManager->getDefinition($this->entityType);
    $this->base_table = $this->entityInfo['base_table'];
    $this->base_field = $this->entityInfo['entity_keys']['id'];
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, array $plugin_definition) {
    return new static($configuration, $plugin_id, $plugin_definition, $container->get('entity.manager'));
  }

  /**
   * {@inheritdoc}
   */
  protected function defineOptions() {
    $options = parent::defineOptions();

    $options['view_mode'] = array('default' => '');
    $options['alternating'] = array('default' => FALSE);
    $options['alternating_fieldset'] = array(
      'contains' => array(
        'alternating' => array('default' => FALSE, 'bool' => TRUE),
        'allpages' => array('default' => FALSE, 'bool' => TRUE),
        'item' => array(
          'default' => array(),
          'export' => 'ds_item_export_option',
        ),
      ),
    );
    return $options;
  }

  /**
   * Custom export function for alternating_fieldset items.
   *
   * // @todo check if this actually works.
   */
  function ds_item_export_option($indent, $prefix, $storage, $option, $definition, $parents) {
    $output = '';
    $definition = array('default' => 'teaser');
    foreach ($storage as $key => $value) {
      if (strstr($key, 'item_') !== FALSE) {
        $output .= parent::export_option($indent, $prefix, $storage, $key, $definition, $parents);
      }
    }
    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function buildOptionsForm(&$form, &$form_state) {
    parent::buildOptionsForm($form, $form_state);

    // Default view mode.
    $view_mode_options = $this->buildViewModeOptions();
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $view_mode_options,
      '#title' => t('View mode'),
      '#default_value' => $this->options['view_mode'],
    );

    // Alternating view modes.
    $form['alternating_fieldset'] = array(
      '#type' => 'details',
      '#title' => t('Alternating view mode'),
      '#collapsible' => TRUE,
      '#collapsed' => !$this->options['alternating'],
    );
    $form['alternating_fieldset']['alternating'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the changing view mode selector'),
      '#default_value' => $this->options['alternating'],
    );
    $form['alternating_fieldset']['allpages'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use this configuration on every page. Otherwhise the default view mode is used as soon you browse away from the first page of this view.'),
      '#default_value' => (isset($this->options['alternating_fieldset']['allpages'])) ? $this->options['alternating_fieldset']['allpages'] : FALSE,
    );

    $limit = $this->view->display_handler->getOption('items_per_page');
    $pager = $this->view->display_handler->getPlugin('pager');
    $limit = (isset($pager->options['items_per_page'])) ? $pager->options['items_per_page'] : 0;
    if ($limit == 0 || $limit > 20) {
      $form['alternating_fieldset']['disabled'] = array(
        '#markup' => t('This option is disabled because you have unlimited items or listing more than 20 items.'),
      );
      $form['alternating_fieldset']['alternating']['#disabled'] = TRUE;
      $form['alternating_fieldset']['allpages']['#disabled'] = TRUE;
    }
    else {
      $i = 1;
      $a = 0;
      while ($limit != 0) {
        $form['alternating_fieldset']['item_' . $a] = array(
          '#title' => t('Item @nr', array('@nr' => $i)),
          '#type' => 'select',
          '#default_value' => (isset($this->options['alternating_fieldset']['item_' . $a])) ? $this->options['alternating_fieldset']['item_' . $a] : 'teaser',
          '#options' => $view_mode_options,
        );
        $limit--;
        $a++;
        $i++;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitOptionsForm(&$form, &$form_state) {
    $form_state['values']['row_options']['alternating'] = $form_state['values']['row_options']['alternating_fieldset']['alternating'];
  }

  /**
   * Return the main options, which are shown in the summary title.
   */
  protected function buildViewModeOptions() {
    $options = array();
    $view_modes = entity_get_view_modes($this->entityType);
    foreach ($view_modes as $mode => $settings) {
      $options[$mode] = $settings['label'];
    }

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function summaryTitle() {
    $options = $this->buildViewModeOptions();
    if (isset($options[$this->options['view_mode']])) {
      return String::checkPlain($options[$this->options['view_mode']]);
    }
    else {
      return t('No view mode selected');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function preRender($result) {
    parent::preRender($result);

    if ($result) {
      // Get all entities which will be used to render in rows.
      $entities = array();
      foreach ($result as $row) {
        $entity = $row->_entity;
        $entity->view = $this->view;
        $entities[$entity->id()] = $entity;
      }

      // Change the view mode per row.
      if ($this->options['alternating']) {

        $i = 0;
        foreach ($entities as $entity_id => $entity) {

          // Check for paging to determine the view mode.
          $page = Drupal::request()->get('page');
          if (!empty($page) && isset($this->options['alternating_fieldset']['allpages']) && !$this->options['alternating_fieldset']['allpages']) {
            $view_mode = $this->options['view_mode'];
          }
          else {
            $view_mode = isset($this->options['alternating_fieldset']['item_' . $i]) ? $this->options['alternating_fieldset']['item_' . $i] : $this->options['view_mode'];
          }
          $i++;

          $this->build[$entity_id] = entity_view($entity, $view_mode);
        }
      }
      else {
        // Prepare the render arrays for all rows at once.
        $this->build = entity_view_multiple($entities, $this->options['view_mode']);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function render($row) {
    $entity_id = $row->{$this->field_alias};
    return $this->build[$entity_id];
  }
}
