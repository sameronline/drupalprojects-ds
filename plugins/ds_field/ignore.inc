<?php
/**
 * @file
 * The Display Suite field type ignore plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_IGNORE,
  'name' => t('Ignore'),
  'description' => t('Special field type to ignore field settings'),
  'class' => 'dsFieldIgnore',
);