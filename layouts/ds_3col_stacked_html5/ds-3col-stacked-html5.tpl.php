<?php
// $Id$

/**
 * @file
 * Display Suite 3 column stacked template HTML 5 version.
 */
?>
<div class="<?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <header class="group-header <?php print $header_classes; ?>">
    <?php print $header; ?>
  </header>

  <aside class="group-left <?php print $left_classes; ?>">
    <?php print $left; ?>
  </aside>

  <section class="group-middle <?php print $middle_classes; ?>">
    <?php print $middle; ?>
  </section>

  <aside class="group-right <?php print $right_classes; ?>">
    <?php print $right; ?>
  </aside>

  <footer class="group-footer <?php print $footer_classes; ?>">
    <?php print $footer; ?>
  </footer>
</div>