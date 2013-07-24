<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\BookNavigation.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the title of a node
 *
 * @DsField(
 *   id = "book_navigation",
 *   title = @Translation("Book navigation"),
 *   entity_type = "node",
 *   module = "ds"
 * )
 */
class BookNavigation extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function displays() {

    // Get all the allowed types
    $types = config('book.settings')->get('allowed_types');

    $displays = array();
    if (!empty($types)) {
      foreach ($types as $type) {
         $displays[] = $type . '|full';
      }
    }

    // When there are no displays, never render this field.
    if (empty($displays)) {
      return FALSE;
    }

    return $displays;
  }

}
