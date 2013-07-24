<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\UserSignature.
 */

namespace Drupal\ds\Plugin\DsField;

use Drupal\Core\Annotation\Translation;
use Drupal\ds\Annotation\DsField;

/**
 * Function field that renders the user signature
 *
 * @DsField(
 *   id = "user_signature",
 *   title = @Translation("User signature"),
 *   entity_type = "user",
 *   module = "ds"
 * )
 */
class UserSignature extends DsFieldBase {

  /**
   * {@inheritdoc}
   */
  public function key() {
    return 'signature';
  }

  /**
   * {@inheritdoc}
   */
  public function format() {
    return 'signature_format';
  }

  /**
   * {@inheritdoc}
   */
  public function displays() {

    // Checks if user signatures are enabled
    $user_signatures = config('user.settings')->get('signatures');

    // We use this function to decide if we should show this field.
    // When user signatures are disabled we should ignore this.
    if ($user_signatures) {
      // An empty array means we show this field on each display of user.
      return array();
    }
    else {
      return FALSE;
    }
  }

}
