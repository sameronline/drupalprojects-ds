<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\TaxonomyTermLink.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the title of a node
 *
 * @DSPlugin(
 *   id = "taxonomy_term_link",
 *   title = @Translation("Read more"),
 *   entity_type = "taxonomy_term",
 *   module = "ds"
 * )
 */
class TaxonomyTermLink extends Link {

}
