
<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\function_field\TaxonomyTermLink.
 */

namespace Drupal\ds\Plugin\ds\function_field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a node
 *
 * @Plugin(
 *   id = "taxonomy_term_link",
 *   title = @Translation("Read more"),
 *   entity_type = "taxonomy_term",
 *   module = "taxonomy"
 * )
 */
class TaxonomyTermLink extends Link {

}
