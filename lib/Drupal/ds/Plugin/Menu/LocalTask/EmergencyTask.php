<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Menu\LocalTask\EmergencyTask.
 */

namespace Drupal\ds\Plugin\Menu\LocalTask;

use Drupal\Core\Annotation\Menu\LocalTask;
use Drupal\Core\Menu\LocalTaskBase;
use Drupal\Core\Annotation\Translation;

/**
 * @LocalTask(
 *   id = "ds_emergency_task",
 *   route_name = "ds_admin_emergency",
 *   title = @Translation("Emergency"),
 *   tab_root_id = "ds_display_task",
 *   tab_parent_id = "ds_display_task",
 *   weight = "100"
 * )
 */
class EmergencyTask extends LocalTaskBase {

}
