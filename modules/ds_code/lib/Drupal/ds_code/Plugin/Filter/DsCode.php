<?php

/**
 * @file
 * Contains \Drupal\ds_code\Plugin\Filter\DsCode.
 */

namespace Drupal\ds_code\Plugin\Filter;

use Drupal\filter\Annotation\Filter;
use Drupal\Core\Annotation\Translation;
use Drupal\filter\Plugin\FilterBase;

/**
 * Display Suite evaluator.
 *
 * @Filter(
 *   id = "ds_code",
 *   title = @Translation("This filter will only work in the Display Suite text format, machine name is <em>ds_code</em>. No other filters can be enabled either."),
 *   cache = FALSE,
 *   module = "filter",
 *   type = FILTER_TYPE_HTML_RESTRICTOR
 * )
 */
class DsCode extends FilterBase {

  /**
   * {@inheritdoc}
   *
   * Wrapper function around PHP eval(). We don't use php_eval from the PHP
   * module because custom fields might need properties from the current entity.
   */
  public function process($text, $langcode, $cache, $cache_id) {
    global $theme_path, $theme_info, $conf;

    // Store current theme path.
    $old_theme_path = $theme_path;

    // Restore theme_path to the theme, as long as ds_php_eval() executes,
    // so code evaluted will not see the caller module as the current theme.
    // If theme info is not initialized get the path from theme_default.
    if (!isset($theme_info)) {
      $theme_path = drupal_get_path('theme', $conf['theme_default']);
    }
    else {
      $theme_path = dirname($theme_info->filename);
    }

    ob_start();
    print eval('?>' . $code);
    $output = ob_get_contents();
    ob_end_clean();

    // Recover original theme path.
    $theme_path = $old_theme_path;

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function tips($long = FALSE) {
    if ($long) {
      $output = '<h4>' . t('Using custom code with Display Suite') . '</h4>';
      $output .= t('Include &lt;?php ?&gt; tags when using PHP. The $entity object is available.');
      return $output;
    }
    else {
      return t('You may post Display Suite code. You should include &lt;?php ?&gt; tags when using PHP. The $entity object is available.');
    }
  }

}
