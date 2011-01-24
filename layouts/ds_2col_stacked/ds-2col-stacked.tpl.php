<?php
// $Id$

/**
 * @file
 * Display Suite 2 column stacked template.
 */
?>
<div class="<?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <div class="group-header">
    <?php print $header; ?>
  </div>

  <div class="group-left">
    <?php print $left; ?>
  </div>

  <div class="group-right">
    <?php print $right; ?>
  </div>

  <div class="group-footer">
    <?php print $footer; ?>
  </div>
</div>