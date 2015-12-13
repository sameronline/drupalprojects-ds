<?php

/**
 * @file
 * Definition of Drupal\ds\Tests\ManageDisplayTabTest.
 */

namespace Drupal\ds\Tests;
use Drupal\comment\Tests\CommentTestBase;

/**
 * Tests for the manage display tab in Display Suite.
 *
 * @group ds
 */
class CommentTest extends CommentTestBase {

  use DsTestTrait;

  /**
   * Modules to install.
   *
   * @var array
   */
  public static $modules = array('node', 'user', 'comment', 'field_ui', 'block', 'ds', 'layout_plugin');

  /**
   * The created user
   *
   * @var User
   */
  protected $adminUser;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    // Create a test user.
    $this->adminUser = $this->drupalCreateUser(array(
      'access content',
      'admin classes',
      'admin display suite',
      'admin fields',
      'administer nodes',
      'view all revisions',
      'administer content types',
      'administer node fields',
      'administer node form display',
      'administer node display',
      'administer users',
      'administer permissions',
      'administer account settings',
      'administer user display',
      'administer software updates',
      'access site in maintenance mode',
      'administer site configuration',
      'bypass node access',
      'administer comments',
      'administer comment types',
      'administer comment fields',
      'administer comment display',
      'skip comment approval',
      'post comments',
      'access comments',
      // Usernames aren't shown in comment edit form autocomplete unless this
      // permission is granted.
      'access user profiles',
    ));
    $this->drupalLogin($this->adminUser);
  }

  /**
   * Test adding comments to a node
   */
  public function testComments() {
    // Create a node.
    $settings = array('type' => 'article', 'promote' => 1);
    $node = $this->drupalCreateNode($settings);

    $this->dsSelectLayout(array(), array(), 'admin/structure/comment/manage/comment/display');

    $fields = array(
      'fields[comment_title][region]' => 'left',
      'fields[comment_body][region]' => 'left',
    );
    $this->dsConfigureUI($fields, 'admin/structure/comment/manage/comment/display');

    // Post comment
    $comment1 = $this->postComment($node, $this->randomMachineName(), $this->randomMachineName());
    $this->assertRaw($comment1->comment_body->value, 'Comment1 found.');

    // Post comment
    $comment2 = $this->postComment($node, $this->randomMachineName(), $this->randomMachineName());
    $this->assertRaw($comment2->comment_body->value, 'Comment2 found.');
  }

}
