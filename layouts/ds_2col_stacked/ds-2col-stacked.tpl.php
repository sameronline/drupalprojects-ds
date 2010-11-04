<?php
// $Id$

/**
 * @file
 * Display Suite 2 column stacked template.
 */
?>

<div class="group-header">
  <?php print ds_render_region($content, 'header', $ds_layout); ?>
</div>

<div class="group-left">
  <?php print ds_render_region($content, 'left', $ds_layout); ?>
</div>

<div class="group-right">
  <?php print ds_render_region($content, 'right', $ds_layout); ?>
</div>

<div class="group-footer">
  <?php print ds_render_region($content, 'footer', $ds_layout); ?>
</div>