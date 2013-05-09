<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DSPlugin\Username.
 */

namespace Drupal\ds\Plugin\DSPlugin;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DSPlugin;

/**
 * Function field that renders the title of a node
 *
 * @DSPlugin(
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
