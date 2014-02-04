<?php
/**
 * @files
 * Holds all the the field layout templates.
 */

/**
 * Reset all HTML for the field.
 */
function theme_ds_field_reset($variables) {
  $output = '';

  // Render the label.
  if (!$variables['label_hidden']) {
    $output .= '<div class="label-' . $variables['element']['#label_display'] . '">' . $variables['label'];
    if (!\Drupal::config('ds.settings')->get('ft-kill-colon')) {
      $output .= ':&nbsp;';
    }
    $output .= '</div>';
  }

  // Render the items.
  foreach ($variables['items'] as $item) {
    $output .= drupal_render($item);
  }

  return $output;
}

/**
 * Provide minimal HTML for the field.
 */
function theme_ds_field_minimal($variables) {
  $output = '';
  $config = $variables['ds-config']['layout'];
  $classes = isset($config['classes']) ? ' ' . $config['classes'] : '';

  // Add a simple wrapper.
  $output .= '<div class="field field-name-' . strtr($variables['element']['#field_name'], '_', '-') . $classes . '">';

  // Render the label.
  if (!$variables['label_hidden']) {
    $output .= '<div class="label-' . $variables['element']['#label_display'] . '">' . $variables['label'];
    if (!isset($config['lb-col'])) {
      $output .= ':&nbsp;';
    }
    $output .= '</div>';
  }

  // Render the items.
  foreach ($variables['items'] as $item) {
    $output .= drupal_render($item);
  }
  $output .="</div>";

  return $output;
}

/**
 * Custom output all HTML for the field.
 */
function theme_ds_field_expert($variables) {
  $output = '';

  $config = $variables['ds-config']['layout'];

  // Render the label if it's not hidden.
  if (!$variables['label_hidden']) {
    $label_wrapper = isset($config['lb-el']) ? $config['lb-el'] : 'div';
    $class = array('label-' . $variables['element']['#label_display']);
    if (!empty($config['lb-cl'])) $class[] = $config['lb-cl'];
    $class = !empty($class) ? ' class="' . implode(' ', $class) . '"' : '';
    $attributes = array();
    if (!empty($config['lb-at'])) $attributes[] = $config['lb-at'];
    if (!empty($config['lb-def-at'])) $attributes[] = $variables['title_attributes'];
    $attributes = (!empty($attributes)) ? ' ' . implode(' ', $attributes) : '';
    $output .= '<' . $label_wrapper . $class . $attributes . '>' . $variables['label'];
    if (empty($config['lb-col'])) $output .= ':&nbsp;';
    $output .= '</' . $label_wrapper . '>';
  }

  // Field items wrapper
  if (!empty($config['fis'])) {
    $class = (!empty($config['fis-cl'])) ? ' class="' . $config['fis-cl'] . '"' : '';
    $attributes = array();
    if (!empty($config['fis-at'])) $attributes[] = $config['fis-at'];
    if (!empty($config['fis-def-at'])) $attributes[] = $variables['content_attributes'];
    $attributes = (!empty($attributes)) ? ' ' . implode(' ', $attributes) : '';
    $output .= '<' . $config['fis-el'] . $class . $attributes . '>';
  }

  // Render items.
  foreach ($variables['items'] as $delta => $item) {
    // Field item wrapper.
    if (!empty($config['fi'])) {
      $class = array();
      if (!empty($config['fi-cl'])) {
        $class[] = $config['fi-cl'];
      }
      $class = !empty($class) ? ' class="'. implode(' ', $class)  .'"' : '';
      $attributes = array();
      if (!empty($config['fi-at'])) {
        $attributes[] = \Drupal::service('token')->replace($config['fi-at'], array($variables['element']['#entity_type'] => $variables['element']['#object']), array('clear' => TRUE));
      }
      if (!empty($config['fi-def-at'])) {
        $attributes[] = $variables['item_attributes'][$delta];
      }
      $attributes = (!empty($attributes)) ? ' ' . implode(' ', $attributes) : '';
      $output .= '<' . $config['fi-el'] . $class . $attributes .'>';
    }

    // Render field content.
    $output .= drupal_render($item);

    // Closing field item wrapper.
    if (!empty($config['fi'])) {
      $output .= '</' . $config['fi-el'] . '>';
    }
  }

  // Closing field items wrapper.
  if (!empty($config['fis'])) {
    $output .= '</' . $config['fis-el'] . '>';
  }

  // Outer wrapper.
  if (isset($config['ow'])) {
    $class = array();
    if (!empty($config['ow-cl'])) $class[] = $config['ow-cl'];
    if (!empty($config['ow-def-cl'])) $class[] = $variables['attributes']['class'];
    $class = (!empty($class)) ? ' class="' . implode(' ', $class) . '"' : '';
    $attributes = array();
    if (!empty($config['ow-at'])) $attributes[] = $config['ow-at'];
    if (!empty($config['ow-def-at'])) $attributes[] = $variables['attributes'];
    $attributes = (!empty($attributes)) ? ' ' . implode(' ', $attributes) : '';
    $output = '<' . $config['ow-el'] . $class . $attributes . '>' . $output . '</' . $config['ow-el'] . '>';
  }

  return $output;
}
