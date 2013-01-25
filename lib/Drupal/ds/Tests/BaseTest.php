<?php

/**
 * @file
 * Definition of Drupal\ds\Tests\BaseTest.
 */

namespace Drupal\ds\Tests;

use Drupal\simpletest\WebTestBase;

class BaseTest extends WebTestBase {


  /**
   * Modules to enable.
   *
   * @var array
   */
  public static $modules = array('ds', 'ds_extras', 'ds_code', 'search', 'ds_search', 'ds_forms', 'ds_ui', 'ds_test');

  protected $profile = 'standard';

  /**
   * Implementation of setUp().
   */
  function setUp() {
    parent::setUp();

    config('search.settings')->set('active_modules', array('node' => '', 'user' => 'user', 'ds_search' => 'ds_search'))->save();
    menu_router_rebuild();

    $this->admin_user = $this->drupalCreateUser(array('admin_classes', 'admin_view_modes', 'admin_fields', 'admin_display_suite', 'ds_switch article', 'use text format ds_code', 'access administration pages', 'administer content types', 'administer users', 'administer comments', 'administer nodes', 'bypass node access', 'administer blocks', 'search content', 'use advanced search', 'administer search', 'access user profiles', 'administer permissions', 'administer node fields', 'administer node display'));
    $this->drupalLogin($this->admin_user);
  }

  /**
   * Select a layout.
   */
  function dsSelectLayout($edit = array(), $assert = array(), $url = 'admin/structure/types/manage/article/display', $options = array()) {

    $edit += array(
      'layout' => 'ds_2col_stacked',
    );

    $this->drupalPost($url, $edit, t('Save'), $options);

    $assert += array(
      'regions' => array(
        'header' => '<td colspan="8">' . t('Header') . '</td>',
        'left' => '<td colspan="8">' . t('Left') . '</td>',
        'right' => '<td colspan="8">' . t('Right') . '</td>',
        'footer' => '<td colspan="8">' . t('Footer') . '</td>',
      ),
    );

    foreach ($assert['regions'] as $region => $raw) {
      $this->assertRaw($region, t('Region !region found', array('!region' => $region)));
    }
  }

  /**
   * Configure classes
   */
  function dsConfigureClasses($edit = array()) {

    $edit += array(
      'regions' => "class_name_1\nclass_name_2|Friendly name"
    );

    $this->drupalPost('admin/structure/ds/classes', $edit, t('Save configuration'));
    $this->assertText(t('The configuration options have been saved.'), t('CSS classes configuration saved'));
    $this->assertRaw('class_name_1', 'Class name 1 found');
    $this->assertRaw('class_name_2', 'Class name 1 found');
  }

  /**
   * Configure classes on a layout.
   */
  function dsSelectClasses($edit = array(), $url = 'admin/structure/types/manage/article/display') {

    $edit += array(
      "header[]" => 'class_name_1',
      "footer[]" => 'class_name_2',
    );

    $this->drupalPost($url, $edit, t('Save'));
  }

  /**
   * Configure Field UI.
   */
  function dsConfigureUI($edit, $url = 'admin/structure/types/manage/article/display') {
    $this->drupalPost($url, $edit, t('Save'));
  }

  /**
   * Edit field formatter settings
   */
  function dsEditFormatterSettings($edit, $url = 'admin/structure/types/manage/article/display', $element_value = 'edit body') {
    $this->drupalPost($url, array(), $element_value);
    $this->drupalPost(NULL, $edit, t('Update'));
    $this->drupalPost(NULL, array(), t('Save'));
  }

  /**
   * Create a view mode.
   *
   * @param $edit
   *   An optional array of view mode properties.
   */
  function dsCreateViewMode($edit = array()) {

    $edit += array(
      'name' => 'Testing',
      'view_mode' => 'testing',
      'entities[node]' => '1'
    );

    $this->drupalPost('admin/structure/ds/view_modes/manage', $edit, t('Save'));
    $this->assertText(t('The view mode ' . $edit['name'] . ' has been saved'), t('!name view mode has been saved', array('!name' => $edit['name'])));
  }

  /**
   * Create a code field.
   *
   * @param $edit
   *   An optional array of field properties.
   */
  function dsCreateCodeField($edit = array(), $url = 'admin/structure/ds/fields/manage_custom') {

    $edit += array(
      'name' => 'Test field',
      'field' => 'test_field',
      'entities[node]' => '1',
      'code[value]' => 'Test field',
      'use_token' => '0',
    );

    $this->drupalPost($url, $edit, t('Save'));
    $this->assertText(t('The field ' . $edit['name'] . ' has been saved'), t('!name field has been saved', array('!name' => $edit['name'])));
  }

  /**
   * Create a block field.
   *
   * @param $edit
   *   An optional array of field properties.
   */
  function dsCreateBlockField($edit = array(), $url = 'admin/structure/ds/fields/manage_block', $first = TRUE) {

    $edit += array(
      'name' => 'Test block field',
      'entities[node]' => '1',
      'block' => 'node|recent',
      'block_render' => DS_BLOCK_TEMPLATE,
    );

    if ($first) {
      $edit += array('field' => 'test_block_field');
    }

    $this->drupalPost($url, $edit, t('Save'));
    $this->assertText(t('The field ' . $edit['name'] . ' has been saved'), t('!name field has been saved', array('!name' => $edit['name'])));
  }

  /**
   * Create a block field.
   *
   * @param $edit
   *   An optional array of field properties.
   */
  function dsCreatePreprocessField($edit = array(), $url = 'admin/structure/ds/fields/manage_preprocess', $first = TRUE) {

    $edit += array(
      'name' => 'Submitted',
      'entities[node]' => '1',
    );

    if ($first) {
      $edit += array('field' => 'submitted');
    }

    $this->drupalPost($url, $edit, t('Save'));
    $this->assertText(t('The field ' . $edit['name'] . ' has been saved'), t('!name field has been saved', array('!name' => $edit['name'])));
  }
}
