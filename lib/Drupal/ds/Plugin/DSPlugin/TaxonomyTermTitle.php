<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\TaxonomyTermTitle.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the title of a node
 *
 * @DSPlugin(
 *   id = "taxonomy_term_title",
 *   title = @Translation("Name"),
 *   entity_type = "taxonomy_term",
 *   module = "ds"
 * )
 */
class TaxonomyTermTitle extends Title {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\Title::entityRenderKey().
   */
  public function entityRenderKey() {
    return 'name';
  }

}
