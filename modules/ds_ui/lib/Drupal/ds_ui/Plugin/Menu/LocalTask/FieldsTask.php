<?php

/**
 * @file
 * Contains \Drupal\ds_ui\Plugin\Menu\LocalTask\FieldsTask.
 */

namespace Drupal\ds_ui\Plugin\Menu\LocalTask;

use Drupal\Core\Annotation\Menu\LocalTask;
use Drupal\Core\Menu\LocalTaskBase;
use Drupal\Core\Annotation\Translation;

/**
 * @LocalTask(
 *   id = "ds_fields_task",
 *   route_name = "fields_list",
 *   title = @Translation("Fields"),
 *   tab_root_id = "ds_display_task"
 * )
 */
class FieldsTask extends LocalTaskBase {

}
