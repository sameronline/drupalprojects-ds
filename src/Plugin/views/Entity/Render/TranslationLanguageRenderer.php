<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\views\Entity\Render\TranslationLanguageRenderer.
 */

namespace Drupal\ds\Plugin\views\Entity\Render;

use Drupal\views\Plugin\views\query\QueryPluginBase;
use Drupal\views\ResultRow;

/**
 * Renders entities in the current language.
 */
class TranslationLanguageRenderer extends RendererBase {

  /**
   * Stores the field alias of the langcode column.
   *
   * @var string
   */
  protected $langcodeAlias;

  /**
   * {@inheritdoc}
   */
  public function query(QueryPluginBase $query) {
    // If the data table is defined, we use the translation language as render
    // language, otherwise we fall back to the default entity language, which is
    // stored in the revision table for revisionable entity types.
    $entity_info = $this->view->rowPlugin->entityManager->getDefinition($this->entityType->id());
    foreach (array('data_table', 'revision_table', 'base_table') as $key) {
      if ($table = $entity_info->get($key)) {
        $table_alias = $query->ensureTable($table);
        $this->langcodeAlias = $query->addField($table_alias, 'langcode');
        break;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function preRender(array $result) {
    parent::dsPreRender($result, TRUE);
  }

  /**
   * {@inheritdoc}
   */
  public function render(ResultRow $row) {
    $entity_id = $row->_entity->id();
    $langcode = $this->getLangcode($row);
    return $this->build[$entity_id][$langcode];
  }

  /**
   * {@inheritdoc}
   */
  protected function getLangcode(ResultRow $row) {
    return $row->{$this->langcodeAlias};
  }

}
