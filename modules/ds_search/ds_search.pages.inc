<?php

/**
 * @file
 */

/**
 * Prepares variables for the ds search page.
 *
 * Default template: ds-search-page.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - content: An array of content types.
 */
function template_preprocess_ds_search_page(&$variables) {
  $build = $variables['build'];

  // This is probably because we don't declare anything in ds_search_theme().
  unset($build['theme_hook_original']);

  // Check on empty search results.
  if (empty($build['build']['search_results'])) {
    // Alter the title and extra variables.
    if (!empty($build['build']['search_title'])) {
      $build['build']['search_title']['#markup'] = '<h2>' . t('Your search yielded no results') . '</h2>';
      unset($build['build']['search_extra']);
    }
  }

  $variables['builded_content'] = drupal_render($build);
}
