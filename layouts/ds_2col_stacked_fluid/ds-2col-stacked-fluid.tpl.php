<?php

/**
 * @file
 * Display Suite fluid 2 column stacked template.
 */
?>
<?php
  // Add sidebar classes so that we can apply the correct width in css.
  if (($left && !$right) || ($right && !$left)) {
    $classes .= ' group-one-column';
  }
?>
<<?php print $layout_wrapper ?> class="ds-2col-stacked-fluid <?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <?php if ($header): ?>
    <<?php print $header_wrapper ?> class="group-header<?php print $header_classes; ?>">
      <?php print $header; ?>
    </<?php print $header_wrapper ?>>
  <?php endif; ?>

  <?php if ($left): ?>
    <<?php print $left_wrapper ?> class="group-left<?php print $left_classes; ?>">
      <?php print $left; ?>
    </<?php print $left_wrapper ?>>
  <?php endif; ?>

  <?php if ($right): ?>
    <<?php print $right_wrapper ?> class="group-right<?php print $right_classes; ?>">
      <?php print $right; ?>
    </<?php print $right_wrapper ?>>
  <?php endif; ?>

  <?php if ($footer): ?>
    <<?php print $footer_wrapper ?> class="group-footer<?php print $footer_classes; ?>">
      <?php print $footer; ?>
    </<?php print $footer_wrapper ?>>
  <?php endif; ?>
</<?php print $layout_wrapper ?>>
