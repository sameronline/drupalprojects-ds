<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Derivative\DynamicCopyField.
 */

namespace Drupal\ds\Plugin\Derivative;

use Drupal\ds_ui\Form\CopyFieldForm;

/**
 * Retrieves dynamic ds field plugin definitions.
 */
class DynamicCopyField extends DynamicField {

  /**
   * {@inheritdoc}
   */
  protected function getType() {
    return CopyFieldForm::TYPE;
  }

}
