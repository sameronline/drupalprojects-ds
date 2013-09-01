<?php

/**
 * @file
 * Definition of Drupal\ds\Tests\FieldsTest.
 */

namespace Drupal\ds\Tests;

/**
 * Test managing of custom fields.
 */
class FieldsTest extends BaseTest {

  /**
   * Implements getInfo().
   */
  public static function getInfo() {
    return array(
      'name' => t('Fields'),
      'description' => t('Tests for managing custom code, preprocess and block fields.'),
      'group' => t('Display Suite'),
    );
  }

  /**
   * Test Display fields.
   */
  function testDSFields() {

    $edit = array(
      'name' => 'Test field',
      'id' => 'test_field',
      'entities[node]' => '1',
      'code[value]' => 'Test field',
      'use_token' => '0',
    );

    $this->dsCreateCodeField($edit);

    // Create the same and assert it already exists.
    $this->drupalPost('admin/structure/ds/fields/manage_code', $edit, t('Save'));
    $this->assertText(t('The machine-readable name is already in use. It must be unique.'), t('Field testing already exists.'));

    $this->dsSelectLayout();

    // Assert it's found on the Field UI for article.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field found on node article.'));

    // Assert it's not found on the Field UI for users.
    $this->drupalGet('admin/config/people/accounts/display');
    $this->assertNoRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field not found on user.'));

    // Update testing label
    $edit = array(
      'name' => 'Test field 2',
    );
    $this->drupalPost('admin/structure/ds/fields/manage_code/test_field', $edit, t('Save'));
    $this->assertText(t('The field Test field 2 has been saved'), t('Test field label updated'));

    // Use the Field UI limit option.
    $this->dsSelectLayout(array(), array(), 'admin/structure/types/manage/page/display');
    $this->dsSelectLayout(array(), array(), 'admin/structure/types/manage/article/display/teaser');
    $edit = array(
      'ui_limit' => 'article|default',
    );
    $this->drupalPost('admin/structure/ds/fields/manage_code/test_field', $edit, t('Save'));

    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field field found on node article, default.'));

    $this->drupalGet('admin/structure/types/manage/article/display/teaser');
    $this->assertNoRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field field not found on node article, teaser.'));
    $this->drupalGet('admin/structure/types/manage/page/display');
    $this->assertNoRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field field not found on node page, default.'));
    $edit = array(
      'ui_limit' => 'article|*',
    );
    $this->drupalPost('admin/structure/ds/fields/manage_code/test_field', $edit, t('Save'));
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field field found on node article, default.'));
    $this->drupalGet('admin/structure/types/manage/article/display/teaser');
    $this->assertRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field field found on node article, teaser.'));



    // Remove the field.
    $this->drupalPost('admin/structure/ds/fields/delete/test_field', array(), t('Confirm'));
    $this->assertText(t('The field Test field 2 has been deleted'), t('Test field removed'));

    // Assert the field is gone at the manage display screen.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertNoRaw('fields[dynamic_code_field:node-test_field][weight]', t('Test field field not found on node article.'));

    // Block fields.
    $edit = array(
      'name' => 'Test block field',
      'id' => 'test_block_field',
      'entities[node]' => '1',
      'block' => 'node_recent_block',
    );

    $this->dsCreateBlockField($edit);

    // Create the same and assert it already exists.
    $this->drupalPost('admin/structure/ds/fields/manage_block', $edit, t('Save'));
    $this->assertText(t('The machine-readable name is already in use. It must be unique.'), t('Block test field already exists.'));

    $this->dsSelectLayout();

    // Assert it's found on the Field UI for article.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertRaw('fields[dynamic_block_field:node-test_block_field][weight]', t('Test block field found on node article.'));

    // Assert it's not found on the Field UI for users.
    $this->drupalGet('admin/config/people/accounts/display');
    $this->assertNoRaw('fields[dynamic_block_field:node-test_block_field][weight]', t('Test block field not found on user.'));

    // Update testing label
    $edit = array(
      'name' => 'Test block field 2',
    );
    $this->drupalPost('admin/structure/ds/fields/manage_block/test_block_field', $edit, t('Save'));
    $this->assertText(t('The field Test block field 2 has been saved'), t('Test field label updated'));

    // Remove the block field.
    $this->drupalPost('admin/structure/ds/fields/delete/test_block_field', array(), t('Confirm'));
    $this->assertText(t('The field Test block field 2 has been deleted'), t('Test field removed'));

    // Assert the block field is gone at the manage display screen.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertNoRaw('fields[dynamic_block_field:node-test_block_field][weight]', t('Test block field not found on node article.'));

    // Preprocess fields.
    $edit = array(
      'name' => 'Submitted',
      'id' => 'submitted',
      'entities[node]' => '1',
    );

    $this->dsCreatePreprocessField($edit);

    // Create the same and assert it already exists.
    $this->drupalPost('admin/structure/ds/fields/manage_code', $edit, t('Save'));
    $this->assertText(t('The machine-readable name is already in use. It must be unique.'), t('Submitted already exists.'));

    $this->dsSelectLayout();

    // Assert it's found on the Field UI for article.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertRaw('fields[dynamic_preprocess_field:node-submitted][weight]', t('Submitted found on node article.'));

    // Assert it's not found on the Field UI for users.
    $this->drupalGet('admin/config/people/accounts/display');
    $this->assertNoRaw('fields[dynamic_preprocess_field:node-submitted][weight]', t('Submitted not found on user.'));

    // Update testing label
    $edit = array(
      'name' => 'Submitted by',
    );
    $this->drupalPost('admin/structure/ds/fields/manage_preprocess/submitted', $edit, t('Save'));
    $this->assertText(t('The field Submitted by has been saved'), t('Submitted label updated'));

    // Remove a field.
    $this->drupalPost('admin/structure/ds/fields/delete/submitted', array(), t('Confirm'));
    $this->assertText(t('The field Submitted by has been deleted'), t('Submitted removed'));

    // Assert the field is gone at the manage display screen.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertNoRaw('fields[dynamic_preprocess_field:node-submitted][weight]', t('Submitted field not found on node article.'));
  }
}
