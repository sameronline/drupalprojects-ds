<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\ds\field\Username.
 */

namespace Drupal\ds\Plugin\ds\field;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Annotation\Plugin;

/**
 * Function field that renders the title of a node
 *
 * @Plugin(
 *   id = "username",
 *   title = @Translation("Username"),
 *   entity_type = "user",
 *   module = "ds"
 * )
 */
class Username extends Title {

  /**
   * Overrides \Drupal\ds\Plugin\ds\field\Title::entityRenderKey().
   */
  public function entityRenderKey() {
    return 'name';
  }

}
