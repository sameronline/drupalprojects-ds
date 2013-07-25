<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\Menu\LocalTask\SettingsTask.
 */

namespace Drupal\ds\Plugin\Menu\LocalTask;

use Drupal\Core\Annotation\Menu\LocalTask;
use Drupal\Core\Menu\LocalTaskBase;
use Drupal\Core\Annotation\Translation;

/**
 * @LocalTask(
 *   id = "ds_settings_task",
 *   route_name = "ds_admin_settings",
 *   title = @Translation("Settings"),
 *   tab_root_id = "ds_display_task",
 *   tab_parent_id = "ds_display_task",
 *   weight = "10"
 * )
 */
class SettingsTask extends LocalTaskBase {

}
