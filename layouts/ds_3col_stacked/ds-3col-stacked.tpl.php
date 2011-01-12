<?php
// $Id$

/**
 * @file
 * Display Suite 3 column 25/50/25 stacked template.
 */

// Get the region content, move this to the preprocess.
$header = ds_render_region($content, 'header', $ds_layout);
$middle = ds_render_region($content, 'middle', $ds_layout);
$left = ds_render_region($content, 'left', $ds_layout);
$right = ds_render_region($content, 'right', $ds_layout);
$footer = ds_render_region($content, 'footer', $ds_layout);

?>
<div class="<?php print $classes;?> clearfix">
  <div class="group-header">
    <?php print $header; ?>
  </div>

  <div class="group-left">
    <?php print $left; ?>
  </div>

  <div class="group-middle">
    <?php print $middle; ?>
  </div>
  
  <div class="group-right">
    <?php print $right; ?>
  </div>
  
  <div class="group-footer">
    <?php print $footer; ?>
  </div>
</div>