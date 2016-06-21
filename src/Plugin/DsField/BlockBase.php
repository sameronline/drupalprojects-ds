<?php

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Render\Element;
use Drupal\Core\Block\BlockManagerInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * The base plugin to create DS block fields.
 */
abstract class BlockBase extends DsFieldBase {

  /**
   * The block.
   *
   * @var \Drupal\Core\Block\BlockPluginInterface
   */
  protected $block;

  /**
   * The BlockManager service
   *
   * @var \Drupal\Core\Block\BlockManagerInterface
   */
  protected $blockManager;

  /**
   * Constructs a Display Suite field plugin.
   */
  public function __construct($configuration, $plugin_id, $plugin_definition, BlockManagerInterface $block_manager) {
    $this->blockManager = $block_manager;

    parent::__construct($configuration, $plugin_id, $plugin_definition);
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('plugin.manager.block')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    // Get block
    $block = $this->getBlock();

    // Apply block config.
    $block_config = $this->blockConfig();
    $block->setConfiguration($block_config);

    // Get render array.
    $block_elements = $block->build();

    // Return an empty array if there is nothing to render.
    return Element::isEmpty($block_elements) ? [] : $block_elements;
  }

  /**
   * Returns the plugin ID of the block.
   */
  protected function blockPluginId() {
    return '';
  }

  /**
   * Returns the config of the block.
   */
  protected function blockConfig() {
    return array();
  }

  /**
   * Return the block entity.
   */
  protected function getBlock() {
    if (!$this->block) {
      // Create an instance of the block.
      /** @var $block BlockPluginInterface */
      $block_id = $this->blockPluginId();
      $block = $this->blockManager->createInstance($block_id);

      $this->block = $block;
    }

    return $this->block;
  }
}
