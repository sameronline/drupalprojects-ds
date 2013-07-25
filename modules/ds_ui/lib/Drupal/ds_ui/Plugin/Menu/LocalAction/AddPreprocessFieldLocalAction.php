<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Plugin\Menu\LocalAction\AddPreprocessFieldLocalAction.
 */

namespace Drupal\ds_ui\Plugin\Menu\LocalAction;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Menu\LocalActionBase;
use Drupal\Core\Annotation\Menu\LocalAction;

/**
 * @LocalAction(
 *   id = "ds_preprocess_field_add_local_action",
 *   route_name = "add_preprocess_field",
 *   title = @Translation("Add a preprocess field"),
 *   appears_on = {"fields_list"}
 * )
 */
class AddPreprocessFieldLocalAction extends LocalActionBase {

}
