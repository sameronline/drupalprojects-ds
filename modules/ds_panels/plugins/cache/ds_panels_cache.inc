<?php

/**
 * @file
 * Provides a event based caching option for panel view modes.
 */

$plugin = array(
  'title' => t("Entity cache"),
  'description' => t('Entity cache.'),
  'cache get' => 'ds_panels_cache_get_cache',
  'cache set' => 'ds_panels_cache_set_cache',
  'cache clear' => 'ds_panels_cache_clear_cache',
  'settings form' => 'ds_panels_cache_settings_form',
  'settings form submit' => 'ds_panels_cache_settings_form_submit',
  'defaults' => array(
    'lifetime' => 'entity_updated',
    'granularity' => 'none', // TODO needed ?
  ),
);

/**
 * Get cached content.
 */
function ds_panels_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  $cid = ds_panels_cache_get_id($conf, $display, $args, $contexts, $pane);
  $cache = cache_get($cid, 'cache_ds_panels');
  if (!$cache) {
    return FALSE;
  }

  return $cache->data;
}

/**
 * Set cached content.
 */
function ds_panels_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {
  $cid = ds_panels_cache_get_id($conf, $display, $args, $contexts, $pane);
  cache_set($cid, $content, 'cache_ds_panels');
}

/**
 * Clear cached content.
 *
 * Cache clears are always for an entire display, regardless of arguments.
 */
function ds_panels_cache_clear_cache($display) {
  $cid = 'panels_simple_cache';

  // This is used in case this is an in-code display, which means did will be something like 'new-1'.
  if (isset($display->owner) && isset($display->owner->id)) {
    $cid .= ':' . $display->owner->id;
  }
  $cid .= ':' . $display->did;

  cache_clear_all($cid, 'cache_ds_panels', TRUE);
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function ds_panels_cache_get_id($conf, $display, $args, $contexts, $pane) {

  $id = $display->entity_type . ':' . $display->bundle . ':' . $display->entity_id;

  // Revise and test with all this other stuff.

  // This is used in case this is an in-code display, which means did will be something like 'new-1'.
  if (isset($display->owner) && isset($display->owner->id)) {
    $id .= ':' . $display->owner->id;
  }
  $id .= ':' . $display->did;

  if ($pane) {
    $id .= ':' . $pane->pid;
  }

  if (user_access('view pane admin links')) {
    $id .= ':admin';
  }

  switch ($conf['granularity']) {
    case 'args':
      foreach ($args as $arg) {
        $id .= ':' . $arg;
      }
      break;

    case 'context':
      if (!is_array($contexts)) {
        $contexts = array($contexts);
      }
      foreach ($contexts as $context) {
        if (isset($context->argument)) {
          $id .= ':' . $context->argument;
        }
      }
  }
  if (module_exists('locale')) {
    global $language;
    $id .= ':' . $language->language;
  }

  if(!empty($pane->configuration['use_pager'])) {
    $id .= ':p' . check_plain($_GET['page']);
  }

  return $id;
}

function ds_panels_cache_settings_form($conf, $display, $pid) {

  // @todo add more options.
  $options = array('entity_updated' => t('When the individual node, user etc. is updated'));
  $form['lifetime'] = array(
    '#title' => t('Lifetime'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['lifetime'],
    '#description' => t('Clear the cache on a certain event, e.g when a single node is updated, the display cache for that node will be cleared.'),
  );

  $form['granularity'] = array(
    '#title' => t('Granularity'),
    '#type' => 'select',
    '#options' => array(
      'args' => t('Arguments'),
      'context' => t('Context'),
      'none' => t('None'),
    ),
    '#description' => t('If "arguments" are selected, this content will be cached per individual argument to the entire display; if "contexts" are selected, this content will be cached per unique context in the pane or display; if "neither" there will be only one cache for this display or pane.'),
    '#default_value' => $conf['granularity'],
  );

  return $form;
}

