<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsField\UserSignature.
 */

namespace Drupal\ds\Plugin\DsField;

/**
 * Plugin that renders the user signature.
 *
 * @DsField(
 *   id = "user_signature",
 *   title = @Translation("User signature"),
 *   entity_type = "user",
 *   provider = "user"
 * )
 */
class UserSignature extends Markup {

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
  public function isAllowed($bunlde, $view_mode) {
    // Checks if user signatures are enabled
    $user_signatures = \Drupal::config('user.settings')->get('signatures');

    // We use this function to decide if we should show this field.
    // When user signatures are disabled we should ignore this.
    if (!empty($user_signatures)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

}
