<?php

/**
 * @file
 * Definition of Drupal\ds\Tests\SearchTest.
 */

namespace Drupal\ds\Tests;

/**
 * Tests for display of search results for nodes and users.
 *
 * @group ds
 */
class SearchTest extends BaseTest {

  function testDSSearch() {

    // Create nodes.
    $i = 15;
    while ($i > 0) {
      $settings = array(
        'title' => 'title' . $i,
        'type' => 'article',
        'promote' => 1,
      );
      $this->drupalCreateNode($settings);
      $i--;
    }

    // Set default search.
    $this->drupalGet('admin/config/search/pages/manage/ds_node_search/set-default');
    $this->assertText("The default search page is now Content (Display Suite)");

    // Run cron.
    $this->cronRun();
    $this->drupalGet('admin/config/search/pages');
    $this->assertText(t('100% of the site has been indexed. There are 0 items left to index.'), 'Site has been indexed');

    // Configure search result view mode.
    $svm = array('display_modes_custom[search_result]' => 'search_result');
    $this->dsConfigureUI($svm);
    $layout = array(
      'layout' => 'ds_2col_stacked',
    );
    $assert = array(
      'regions' => array(
        'header' => '<td colspan="8">' . t('Header') . '</td>',
        'left' => '<td colspan="8">' . t('Left') . '</td>',
        'right' => '<td colspan="8">' . t('Right') . '</td>',
        'footer' => '<td colspan="8">' . t('Footer') . '</td>',
      ),
    );
    $this->dsSelectLayout($layout, $assert, 'admin/structure/types/manage/article/display/search_result');
    $fields = array(
      'fields[node_title][region]' => 'header',
      'fields[node_post_date][region]' => 'header',
      'fields[node_author][region]' => 'left',
      'fields[body][region]' => 'right',
      'fields[node_link][region]' => 'footer',
    );
    $this->dsConfigureUI($fields, 'admin/structure/types/manage/article/display/search_result');

    // Let's search.
    $this->drupalGet('search/content', array('query' => array('keys' => 'title1')));
    $this->assertRaw('view-mode-search-result', 'Search view mode found');
    $this->assertRaw('group-left', 'Search template found');
    $this->assertRaw('group-right', 'Search template found');
    $this->assertNoText(t('Advanced search'), 'No advanced search found');


    $edit = array('advanced_search' => '1');
    $this->drupalPostForm('admin/config/search/pages/manage/ds_node_search', $edit, t('Save search page'));
    $this->drupalGet('search/content', array('query' => array('keys' => 'title1')));
    $this->assertText(t('Advanced search'), 'Advanced search found');

    // Search on user.

    // Configure user. We'll just do default.
    $layout = array(
      'layout' => 'ds_2col_stacked',
    );
    $assert = array(
      'regions' => array(
        'header' => '<td colspan="8">' . t('Header') . '</td>',
        'left' => '<td colspan="8">' . t('Left') . '</td>',
        'right' => '<td colspan="8">' . t('Right') . '</td>',
        'footer' => '<td colspan="8">' . t('Footer') . '</td>',
      ),
    );
    $this->dsSelectLayout($layout, $assert, 'admin/config/people/accounts/display');
    $fields = array(
      'fields[username][region]' => 'left',
      'fields[member_for][region]' => 'right',
    );
    $this->dsConfigureUI($fields, 'admin/config/people/accounts/display');

    $this->drupalGet('search/users', array('query' => array('keys' => $this->admin_user->getUsername())));
    $this->assertRaw('view-mode-search-result', 'Search view mode found');
    $this->assertRaw('group-left', 'Search template found');
    $this->assertRaw('group-right', 'Search template found');
  }
}
