<?php
// $Id$

/**
 * @file
 * Display Suite 1 column template.
 */

// Get the region content, move this to the preprocess.
$ds_content = ds_render_region($content, 'ds_content', $ds_layout);

?>
<div class="<?php print $classes;?> clearfix">
  <?php print $ds_content; ?>
</div>