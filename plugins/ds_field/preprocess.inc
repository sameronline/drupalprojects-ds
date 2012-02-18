<?php
/**
 * @file
 * The Display Suite field type theme plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_PREPROCESS,
  'name' => t('Preprocess field'),
  'description' => t('Renders content available through preprocess sources'),
  'class' => 'dsFieldPreprocess',
);