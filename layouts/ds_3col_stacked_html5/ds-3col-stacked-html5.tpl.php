<?php
// $Id$

/**
 * @file
 * Display Suite 3 column stacked template HTML 5 version.
 */

// Get the region content, move this to the preprocess.
$header = ds_render_region($content, 'header', $ds_layout);
$middle = ds_render_region($content, 'middle', $ds_layout);
$left = ds_render_region($content, 'left', $ds_layout);
$right = ds_render_region($content, 'right', $ds_layout);
$footer = ds_render_region($content, 'footer', $ds_layout);

?>
<div class="<?php print $classes;?> clearfix">
  <header class="group-header">
    <?php print $header; ?>
  </header>
  
  <aside class="group-left">
    <?php print $left; ?>
  </aside> 
  
  <section class="group-middle">
    <?php print $middle; ?>
  </section>

  <aside class="group-right">
    <?php print $right; ?>
  </aside>

  <footer class="group-footer">
    <?php print $footer; ?>
  </footer>
</div>