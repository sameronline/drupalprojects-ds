
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\TaxonomyTermTitle.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a node
 *
 * @Plugin(
 *   id = "taxonomy_term_title",
 *   title = @Translation("Name"),
 *   entity_type = "taxonomy_term",
 *   module = "taxonomy",
 *   field_type = "function"
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
