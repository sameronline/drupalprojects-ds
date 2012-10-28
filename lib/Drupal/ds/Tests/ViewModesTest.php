<?php

/**
 * @file
 * Definition of Drupal\ds\Tests\ViewModesTest.
 */

namespace Drupal\ds\Tests;

/**
 * Test managing of view modes.
 */
class ViewModesTest extends BaseTest {

  /**
   * Implements getInfo().
   */
  public static function getInfo() {
    return array(
      'name' => t('View modes'),
      'description' => t('Tests for managing custom view modes.'),
      'group' => t('Display Suite'),
    );
  }

  /**
   * Test managing view modes.
   */
  function testDSManageViewModes() {

    $edit = array(
      'name' => 'Testing',
      'view_mode' => 'testing',
      'entities[node]' => '1'
    );

    $this->dsCreateViewMode($edit);

    // Create the same and assert it already exists.
    $this->drupalPost('admin/structure/ds/view_modes/manage', $edit, t('Save'));
    $this->assertText(t('The machine-readable name is already in use. It must be unique.'), t('View mode testing already exists.'));

    // Assert it's found on the Field UI for article.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertRaw('additional_settinmodes[view_modes_custom][testing]', t('Testing view mode found on node article.'));

    // Assert it's not found on the Field UI for article.
    $this->drupalGet('admin/config/people/accounts/display');
    $this->assertNoRaw('additional_settinmodes[view_modes_custom][testing]', t('Testing view mode not found on user.'));

    // Update testing label
    $edit = array(
      'name' => 'Testing 2',
    );
    $this->drupalPost('admin/structure/ds/view_modes/manage/testing', $edit, t('Save'));
    $this->assertText(t('The view mode Testing 2 has been saved'), t('Testing label updated'));

    // Remove a view mode.
    $this->drupalPost('admin/structure/ds/view_modes/delete/testing', array(), t('Delete'));
    $this->assertText(t('The view mode Testing 2 has been deleted'), t('Testing view mode removed'));

    // Assert the view mode is gone at the manage display screen.
    $this->drupalGet('admin/structure/types/manage/article/display');
    $this->assertNoRaw('additional_settinmodes[view_modes_custom][testing]', t('Testing view mode found on node article.'));
  }
}
