<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\Username.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Plugin that renders the username.
 *
 * @DsField(
 *   id = "username",
 *   title = @Translation("Username"),
 *   entity_type = "user",
 *   module = "ds"
 * )
 */
class Username extends Title {

  /**
   * {@inheritdoc}
   */
  public function entityRenderKey() {
    return 'name';
  }

}
