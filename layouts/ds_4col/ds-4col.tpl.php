<?php
// $Id$

/**
 * @file
 * Display Suite 4 column template.
 */

// Get the region content, move this to the preprocess.
$first = ds_render_region($content, 'first', $ds_layout);
$second = ds_render_region($content, 'second', $ds_layout);
$third = ds_render_region($content, 'third', $ds_layout);
$fourth = ds_render_region($content, 'fourth', $ds_layout);

?>
<div class="<?php print $classes;?> clearfix">
  <div class="group-first">
    <?php print $first; ?>
  </div>

  <div class="group-second">
    <?php print $second; ?>
  </div>

  <div class="group-third">
    <?php print $third; ?>
  </div>
  
  <div class="group-fourth">
    <?php print $fourth; ?>
  </div>
</div>