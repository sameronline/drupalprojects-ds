<?php

/**
 * @file
 * Contains \Drupal\ds_search\Plugin\Search\NodeSearch.
 */

namespace Drupal\ds_search\Plugin\Search;

use Drupal\Core\Form\FormStateInterface;
use Drupal\ds_search\DsSearch;
use Drupal\node\Plugin\Search\NodeSearch;
use Drupal\search\SearchQuery;

/**
 * Handles searching for node entities using the Search module index.
 *
 * @SearchPlugin(
 *   id = "ds_node_search",
 *   title = @Translation("Content (Display Suite)")
 * )
 */
class DsNodeSearch extends NodeSearch {

  use DsSearch;

  /**
   * {@inheritdoc}
   */
  public function execute() {
    $results = array();
    if (!$this->isSearchExecutable()) {
      return $results;
    }
    $keys = $this->keywords;

    // Build matching conditions.
    $query = $this->database
      ->select('search_index', 'i', array('target' => 'replica'))
      ->extend('Drupal\search\SearchQuery')
      ->extend('Drupal\Core\Database\Query\PagerSelectExtender');
    $query->join('node_field_data', 'n', 'n.nid = i.sid');
    $query->condition('n.status', 1)
      ->addTag('node_access')
      ->searchExpression($keys, $this->getPluginId());

    // Handle advanced search filters in the f query string.
    // \Drupal::request()->query->get('f') is an array that looks like this in
    // the URL: ?f[]=type:page&f[]=term:27&f[]=term:13&f[]=langcode:en
    // So $parameters['f'] looks like:
    // array('type:page', 'term:27', 'term:13', 'langcode:en');
    // We need to parse this out into query conditions, some of which go into
    // the keywords string, and some of which are separate conditions.
    $parameters = $this->getParameters();
    if (!empty($parameters['f']) && is_array($parameters['f'])) {
      $filters = array();
      // Match any query value that is an expected option and a value
      // separated by ':' like 'term:27'.
      $pattern = '/^(' . implode('|', array_keys($this->advanced)) . '):([^ ]*)/i';
      foreach ($parameters['f'] as $item) {
        if (preg_match($pattern, $item, $m)) {
          // Use the matched value as the array key to eliminate duplicates.
          $filters[$m[1]][$m[2]] = $m[2];
        }
      }

      // Now turn these into query conditions. This assumes that everything in
      // $filters is a known type of advanced search.
      foreach ($filters as $option => $matched) {
        $info = $this->advanced[$option];
        // Insert additional conditions. By default, all use the OR operator.
        $operator = empty($info['operator']) ? 'OR' : $info['operator'];
        $where = new Condition($operator);
        foreach ($matched as $value) {
          $where->condition($info['column'], $value);
        }
        $query->condition($where);
        if (!empty($info['join'])) {
          $query->join($info['join']['table'], $info['join']['alias'], $info['join']['condition']);
        }
      }
    }

    // Add the ranking expressions.
    $this->addNodeRankings($query);

    // Run the query and load results.
    $query
      // Add the language code of the indexed item to the result of the query,
      // since the node will be rendered using the respective language.
      ->fields('i', array('langcode'))
      // And since SearchQuery makes these into GROUP BY queries, if we add
      // a field, for PostgreSQL we also need to make it an aggregate or a
      // GROUP BY. In this case, we want GROUP BY.
      ->groupBy('i.langcode');

    // Add limit
    if (!empty($this->configuration['limit'])) {
      $query->limit($this->configuration['limit']);
    }

    $find = $query->execute();

    // Check query status and set messages if needed.
    $status = $query->getStatus();

    if ($status & SearchQuery::EXPRESSIONS_IGNORED) {
      drupal_set_message($this->t('Your search used too many AND/OR expressions. Only the first @count terms were included in this search.', array('@count' => $this->searchSettings->get('and_or_limit'))), 'warning');
    }

    if ($status & SearchQuery::LOWER_CASE_OR) {
      drupal_set_message($this->t('Search for either of the two terms with uppercase <strong>OR</strong>. For example, <strong>cats OR dogs</strong>.'), 'warning');
    }

    if ($status & SearchQuery::NO_POSITIVE_KEYWORDS) {
      drupal_set_message(\Drupal::translation()->formatPlural($this->searchSettings->get('index.minimum_word_size'), 'You must include at least one positive keyword with 1 character or more.', 'You must include at least one positive keyword with @count characters or more.'), 'warning');
    }

    $node_storage = $this->entityManager->getStorage('node');
    $node_render = $this->entityManager->getViewBuilder('node');

    foreach ($find as $item) {
      // Render the node.
      $node = $node_storage->load($item->sid)->getTranslation($item->langcode);
      $build = $node_render->view($node, $this->configuration['view_mode'], $item->langcode);
      unset($build['#theme']);
      $node->rendered = drupal_render($build);

      // Fetch comment count for snippet.
      $node->rendered .= ' ' . $this->moduleHandler->invoke('comment', 'node_update_index', array($node, $item->langcode));

      $node->search_extra = $this->moduleHandler->invokeAll('node_search_result', array($node, $item->langcode));
      $node->snippet = search_excerpt($keys, $node->rendered, $item->langcode);

      $results[] = $node;
    }

    return $results;
  }

  /**
   * {@inheritdoc}
   */
  public function buildResults() {
    $results = $this->execute();

    // Build shared variables.
    $build = array();
    $this->buildSharedPageVariables($build, $this->configuration);

    $i = 0;
    foreach ($results as $result) {
      $data = entity_view($result, $this->configuration['view_mode']);
      $build['search_results'][$i] = $data;
      $i++;
    }

    return array(
      '#theme' => 'ds_search_page',
      '#build' => $build,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    // Fetch default for nodes.
    $configuration = parent::defaultConfiguration();

    // Set general defaults.
    $this->generalDefaultSettings($configuration);

    // Add node specific Display Suite settings.
    $configuration['advanced_search'] = FALSE;

    return $configuration;
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    // Fetch form from node search.
    $form = parent::buildConfigurationForm($form, $form_state);

    // Add general settings
    $form = $this->generalConfigurationForm($form, $form_state, $this->configuration, 'node');

    // Add node specific settings
    $form['node'] = array(
      '#type' => 'details',
      '#title' => t('Node'),
    );
    $form['node']['advanced_search'] = array(
      '#type' => 'checkbox',
      '#title' => t('Advanced'),
      '#description' => t('Enable the advanced search form.'),
      '#default_value' => $this->configuration['advanced_search'],
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);

    // Submits general settings.
    $this->generalSubmitConfigurationForm($this->configuration, $form_state, TRUE);

    // Submits node specific settings.
    $this->configuration['advanced_search'] = $form_state->getValue('advanced_search');
  }

  /**
   * {@inheritdoc}
   */
  public function searchFormAlter(array &$form, FormStateInterface $form_state) {
    if ($this->configuration['advanced_search']) {
      parent::searchFormAlter($form, $form_state);
    }
  }
}
