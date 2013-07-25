<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Plugin\Menu\LocalAction\AddCodeFieldLocalAction.
 */

namespace Drupal\ds_ui\Plugin\Menu\LocalAction;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Menu\LocalActionBase;
use Drupal\Core\Annotation\Menu\LocalAction;

/**
 * @LocalAction(
 *   id = "ds_code_field_add_local_action",
 *   route_name = "add_code_field",
 *   title = @Translation("Add a code field"),
 *   appears_on = {"fields_list"}
 * )
 */
class AddCodeFieldLocalAction extends LocalActionBase {

}
