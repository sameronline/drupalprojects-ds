<?php
/**
 * @file
 * The Display Suite field type block plugin
 */

$plugin = array(
  'type' => DS_FIELD_TYPE_BLOCK,
  'name' => t('Block field'),
  'description' => t('Provides a field type for rendering content as blocks'),
  'class' => 'dsFieldBlock',
);