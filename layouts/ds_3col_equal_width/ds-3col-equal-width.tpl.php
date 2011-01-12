<?php
// $Id$

/**
 * @file
 * Display Suite 3 column equal width template.
 */

// Get the region content, move this to the preprocess.
$middle = ds_render_region($content, 'middle', $ds_layout);
$left = ds_render_region($content, 'left', $ds_layout);
$right = ds_render_region($content, 'right', $ds_layout);

?>
<div class="<?php print $classes;?> clearfix">
  <div class="group-left">
    <?php print $left; ?>
  </div>
  
  <div class="group-middle">
    <?php print $middle; ?>
  </div>

  <div class="group-right">
    <?php print $right; ?>
  </div>
</div>