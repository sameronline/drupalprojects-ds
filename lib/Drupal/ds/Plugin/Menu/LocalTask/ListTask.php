<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Menu\LocalTask\ListTask.
 */

namespace Drupal\ds\Plugin\Menu\LocalTask;

use Drupal\Core\Annotation\Menu\LocalTask;
use Drupal\Core\Menu\LocalTaskBase;
use Drupal\Core\Annotation\Translation;

/**
 * @LocalTask(
 *   id = "ds_list_task",
 *   route_name = "ds_list",
 *   title = @Translation("List"),
 *   tab_root_id = "ds_display_task",
 *   tab_parent_id = "ds_display_task"
 * )
 */
class ListTask extends LocalTaskBase {

}
