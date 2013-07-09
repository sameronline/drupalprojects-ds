<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\TaxonomyTermTitle.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the title of a node
 *
 * @DsField(
 *   id = "taxonomy_term_title",
 *   title = @Translation("Name"),
 *   entity_type = "taxonomy_term",
 *   module = "ds"
 * )
 */
class TaxonomyTermTitle extends Title {

  /**
   * {@inheritdoc}
   */
  public function entityRenderKey() {
    return 'name';
  }

}
