<?php
/**
 * @file
 * Display plugin for Node Displays
 */

$plugin = array(
  'type' => 'node',
  'name' => t('Node displays'),
  'description' => t('Enables display suite for node content'),
  'module' => 'nd',
  'object' => 'node',
  'views_base' => array('node', 'apachesolr', 'apachesolr_node'),
  'types_callback' => 'node_get_types',
  'extra' => array('has_body'),
  'plugins_exclude' => array(NODE_BUILD_RSS),
  'regions_single' => array(NODE_BUILD_RSS),
  'help' =>
    '<p>This module comes with a nd.tpl.php, available in the templates folder, which is optimized for use with Node Displays. You can copy the file to your themes folder to use your own version if you want. Copying the file to your theme folder will only work if node.tpl.php is in the theme folder.</p>
    <p>You can create a file called <strong>nd-<em>node_type</em>.tpl.php</strong>, <strong>nd-<em>node_type</em>-<em>build_mode</em>.tpl.php</strong> or <strong>nd-<em>node_type</em>-<em>build_mode</em>-<em>nid</em>.tpl.php</strong> for even more fine-grained theming.</p>
    <p>If you exclude the RSS build mode, the default node feed is generated, but might render strange content. Make sure you do not choose the "Titles only" as your feed content setting, otherwhise, this screen will not have any effect. Choosing either "Full text" or "Titles + teaser" does not matter, this screen will have effect on both settings, apart from the read more link and the format of the body (full/teaser). Remember that some fields like upload and terms are added automatically to the feed and that this build mode manipulates the $description variable, it does not add extra keys to the feed.</p>
    <p>If you have a problem with the content not being rendered through DS, please read <a href="http://drupal.org/node/572614">http://drupal.org/node/572614</a>.',
);