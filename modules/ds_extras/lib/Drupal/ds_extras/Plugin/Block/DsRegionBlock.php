<?php

/**
 * @file
 * Contains \Drupal\ds_extras\Plugin\Block\DsRegionBlock.
 */

namespace Drupal\ds_extras\Plugin\Block;

use Drupal\block\BlockBase;
use Drupal\Component\Annotation\Plugin;
use Drupal\Core\Annotation\Translation;

/**
 * Provides an 'Aggregator feed' block with the latest items from the feed.
 *
 * @Plugin(
 *   id = "ds_region_block",
 *   admin_label = @Translation("Ds region block"),
 *   module = "ds_extras",
 *   derivative = "Drupal\ds_extras\Plugin\Derivative\DsRegionBlock"
 * )
 */
class DsRegionBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    list(, $id) = explode(':', $this->getPluginId());

    $data = drupal_static('ds_block_region');
    $region_blocks = config('ds.extras')->get('region_blocks');

    if (!empty($data[$id])) {
      return array(
        $data[$id],
      );
    }
    else {
      return array();
    }
  }

}
