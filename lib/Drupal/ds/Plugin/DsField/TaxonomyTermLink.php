<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\TaxonomyTermLink.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the title of a node
 *
 * @DsField(
 *   id = "taxonomy_term_link",
 *   title = @Translation("Read more"),
 *   entity_type = "taxonomy_term",
 *   module = "ds"
 * )
 */
class TaxonomyTermLink extends Link {

}
