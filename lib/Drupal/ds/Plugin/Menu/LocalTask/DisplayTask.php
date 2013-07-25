<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Menu\LocalTask\DisplayTask.
 */

namespace Drupal\ds\Plugin\Menu\LocalTask;

use Drupal\Core\Annotation\Menu\LocalTask;
use Drupal\Core\Menu\LocalTaskBase;
use Drupal\Core\Annotation\Translation;

/**
 * @LocalTask(
 *   id = "ds_display_task",
 *   route_name = "ds_structure",
 *   title = @Translation("Displays"),
 *   tab_root_id = "ds_display_task"
 * )
 */
class DisplayTask extends LocalTaskBase {

}
