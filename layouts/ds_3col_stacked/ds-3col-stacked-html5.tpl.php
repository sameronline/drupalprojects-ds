<?php
// $Id$

/**
 * @file
 * Display Suite 3 column stacked template HTML 5 version.
 */
?>

<header>
  <?php print render($content['group_header']); ?>
</header>

<aside>
  <?php print render($content['group_left']); ?>
</aside>

<aside>
  <?php print render($content['group_middle']); ?>
</aside>

<aside>
  <?php print render($content['group_right']); ?>
</aside>

<footer>
  <?php print render($content['group_left']); ?>
</footer>