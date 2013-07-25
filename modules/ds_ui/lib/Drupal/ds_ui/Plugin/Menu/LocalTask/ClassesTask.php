<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Plugin\Menu\LocalTask\ClassesTask.
 */

namespace Drupal\ds_ui\Plugin\Menu\LocalTask;

use Drupal\Core\Annotation\Menu\LocalTask;
use Drupal\Core\Menu\LocalTaskBase;
use Drupal\Core\Annotation\Translation;

/**
 * @LocalTask(
 *   id = "ds_classes_task",
 *   route_name = "ds_classes",
 *   title = @Translation("Classes"),
 *   tab_root_id = "ds_display_task"
 * )
 */
class ClassesTask extends LocalTaskBase {

}
