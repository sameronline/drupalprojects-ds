<?php
// $Id$

/**
 * @file
 * Display Suite 2 column template.
 */

// Get the region content, move this to the preprocess.
$left = ds_render_region($content, 'left', $ds_layout);
$right = ds_render_region($content, 'right', $ds_layout);
?>
<div class="<?php print $classes;?> clearfix">
  <div class="group-left">
    <?php print $left; ?>
  </div>

  <div class="group-right">
    <?php print $right; ?>
  </div>
</div>