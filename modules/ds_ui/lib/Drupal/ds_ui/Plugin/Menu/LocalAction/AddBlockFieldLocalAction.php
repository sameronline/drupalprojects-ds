<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Plugin\Menu\LocalAction\AddBlockFieldLocalAction.
 */

namespace Drupal\ds_ui\Plugin\Menu\LocalAction;

use Drupal\Core\Annotation\Translation;
use Drupal\Core\Menu\LocalActionBase;
use Drupal\Core\Annotation\Menu\LocalAction;

/**
 * @LocalAction(
 *   id = "ds_block_field_add_local_action",
 *   route_name = "add_block_field",
 *   title = @Translation("Add a block field"),
 *   appears_on = {"fields_list"}
 * )
 */
class AddBlockFieldLocalAction extends LocalActionBase {

}
