<?php

/**
 * @file
 * Contains \Drupal\ds\Plugin\DsFieldLayout\Expert.
 */

namespace Drupal\ds\Plugin\DsFieldLayout;

/**
 * Plugin for the expert field template.
 *
 * @DsFieldLayout(
 *   id = "expert",
 *   title = @Translation("Expert"),
 *   theme = "theme_ds_field_expert"
 * )
 */
class Expert extends DsFieldLayoutBase {

  /**
   * {@inheritdoc}
   */
  public function alterForm(&$form) {
    parent::alterForm($form);

    $config = $this->getConfiguration();

    $wrappers = array(
      'lb' => array('title' => t('Label')),
      'ow' => array('title' => t('Outer wrapper')),
      'fis' => array('title' => t('Field items')),
      'fi' => array('title' => t('Field item')),
    );

    foreach ($wrappers as $wrapper_key => $value) {
      $form[$wrapper_key] = array(
        '#type' => 'checkbox',
        '#title' => $value['title'],
        '#prefix' => '<div class="ft-group ' . $wrapper_key . '">',
        '#default_value' => $config[$wrapper_key],
      );
      $form[$wrapper_key . '-el'] = array(
        '#type' => 'textfield',
        '#title' => t('Element'),
        '#size' => '10',
        '#description' => t('E.g. div, span, h2 etc.'),
        '#default_value' => $config[$wrapper_key . '-el'],
      );
      $form[$wrapper_key . '-cl'] = array(
        '#type' => 'textfield',
        '#title' => t('Classes'),
        '#size' => '10',
        '#default_value' => $config[$wrapper_key . '-cl'],
        '#description' => t('E.g.') .' field-expert',
      );
      $form[$wrapper_key . '-at'] = array(
        '#type' => 'textfield',
        '#title' => t('Attributes'),
        '#size' => '20',
        '#default_value' => $config[$wrapper_key . '-at'],
        '#description' => t('E.g. name="anchor"'),
      );

      // Hide colon.
      if ($wrapper_key == 'lb') {
        $form['lb-col'] = array(
          '#type' => 'checkbox',
          '#title' => t('Hide label colon'),
          '#default_value' => $config['lb-col'],
          '#attributes' => array(
            'class' => array('colon-checkbox'),
          ),
        );
      }
      if ($wrapper_key == 'fi') {
        $form['fi-odd-even'] = array(
          '#type' => 'checkbox',
          '#title' => t('Add odd/even classes'),
          '#default_value' => $config['fi-odd-even'],
        );
      }
      $form[$wrapper_key . '-def-at'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add default attributes'),
        '#default_value' => $config[$wrapper_key . '-def-at'],
        '#suffix' => ($wrapper_key == 'ow') ? '' : '</div><div class="clearfix"></div>',
      );

      // Default classes for outer wrapper.
      if ($wrapper_key == 'ow') {
        $form[$wrapper_key . '-def-cl'] = array(
          '#type' => 'checkbox',
          '#title' => t('Add default classes'),
          '#default_value' => $config[$wrapper_key . '-def-cl'],
          '#suffix' => '</div><div class="clearfix"></div>',
        );
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $config = parent::defaultConfiguration();
    $config['lb'] = '';
    $config['lb-col'] = \Drupal::config('ds.settings')->get('ft-kill-colon');
    $config['fi-odd-even'] = FALSE;

    $wrappers = array(
      'lb' => array('title' => t('Label')),
      'ow' => array('title' => t('Outer wrapper')),
      'fis' => array('title' => t('Field items')),
      'fi' => array('title' => t('Field item')),
    );
    foreach ($wrappers as $wrapper_key => $value) {
      $config[$wrapper_key] = FALSE;
      $config[$wrapper_key . '-el'] = '';
      $config[$wrapper_key . '-at'] = '';
      $config[$wrapper_key . '-cl'] = '';

      $config[$wrapper_key . '-def-at'] = FALSE;
      $config[$wrapper_key . '-def-cl'] = FALSE;
    }

    return $config;
  }

}
