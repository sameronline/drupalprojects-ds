<?php

/**
 * @file
 * Display Suite 3 column equal width template.
 */
?>
<<?php print $layout_wrapper ?> class="ds-3col-equal <?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <?php if ($left): ?>
    <<?php print $left_wrapper ?> class="group-left<?php print $left_classes; ?>">
      <?php print $left; ?>
    </<?php print $left_wrapper ?>>
  <?php endif; ?>

  <?php if ($middle): ?>
    <<?php print $middle_wrapper ?> class="group-middle<?php print $middle_classes; ?>">
      <?php print $middle; ?>
    </<?php print $middle_wrapper ?>>
  <?php endif; ?>

  <?php if ($right): ?>
    <<?php print $right_wrapper ?> class="group-right<?php print $right_classes; ?>">
      <?php print $right; ?>
    </<?php print $right_wrapper ?>>
  <?php endif; ?>
</<?php print $layout_wrapper ?>>
