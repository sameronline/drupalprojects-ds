<?php

/**
 * @file
 * Contains \Drupal\ds\Access\ContextualTabAccessCheck.
 */

namespace Drupal\ds\Access;

use Drupal\Core\Access\StaticAccessCheckInterface;
use Drupal\Core\Session\AccountInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\Routing\Route;

/**
 * Provides an access check for ds extras switch field routes.
 */
class ContextualTabAccessCheck implements StaticAccessCheckInterface {

  /**
   * {@inheritdoc}
   */
  public function appliesTo() {
    return array('_access_ds_contextual_tab');
  }

  /**
   * {@inheritdoc}
   */
  public function access(Route $route, Request $request, AccountInterface $account) {
    return \Drupal::moduleHandler()->moduleExists('contextual') && \Drupal::moduleHandler()->moduleExists('field_ui') ? static::ALLOW : static::DENY;
  }

}
