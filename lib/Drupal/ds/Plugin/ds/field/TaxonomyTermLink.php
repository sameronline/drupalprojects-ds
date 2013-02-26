<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\TaxonomyTermLink.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a node
 *
 * @Plugin(
 *   id = "taxonomy_term_link",
 *   title = @Translation("Read more"),
 *   entity_type = "taxonomy_term",
 *   module = "ds"
 * )
 */
class TaxonomyTermLink extends Link {

}
