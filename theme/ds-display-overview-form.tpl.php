<?php
// $Id$

/**
 * @file
 *   Template file for the display settings overview form
 *
 * @var
 * $build_mode String
 *   Current selected display mode
 * $rows Array of field objects
 *   Fields declared in drupal core and custom fields
 *   Properties (human_name, weight, stickyorder, build_mode, class, label_class)
 */

print $exclude_build_mode;

if ($rows): ?>

<div id="ds-display-content" <?php print $extra_style; ?>>
  <?php if (!empty($plugins_tabs)): ?>
    <div id="ds-tabs">
      <div id="field-tab" class="tab selected"><a href="javascript:;" onClick="javascript:toggleFieldPluginsLink('field-tab', 'plugin-tab', 'fields-content', 'plugins-content');"><?php print t('Fields'); ?></a></div>
      <?php foreach ($plugins_tabs as $key => $title): ?>
      <div id="<?php print $key; ?>-tab" class="tab<?php ?>"><a href="javascript:;" onClick="javascript:toggleFieldPluginsLink('plugin-tab', 'field-tab', 'plugins-content', 'fields-content');"><?php print $title; ?></a></div>
      <?php endforeach; ?>
    </div>
    <div style="clear: both"></div>
  <?php endif; ?>

  <div id="fields-content">

    <table id="fields" class="sticky-enabled">
      <thead>
        <tr>
          <th><?php print t('Field'); ?></th>
          <th><?php print t('Label'); ?></th>
          <th><?php print t('Format'); ?></th>
          <th><?php print t('Region'); ?></th>
          <th><?php print t('Weight'); ?></th>
        </tr>
      </thead>
      <tbody>

      <!-- Node regions -->
      <?php foreach ($regions as $region => $title): ?>
        <tr class="region region-<?php print $region?>">
          <td colspan="6" class="region"><?php print $title; ?></td>
        </tr>
        <tr class="region-message region-<?php print $region?>-message <?php print empty($rows[$region]) ? 'region-empty' : 'region-populated'; ?>">
          <td colspan="6"><em><?php print t('No fields in this region'); ?></em></td>
        </tr>

        <!-- fields -->
        <?php
        if (!empty($rows[$region])):
          $count = 0;
          foreach ($rows[$region] as $row): ?>
            <tr class="<?php print $count % 2 == 0 ? 'odd' : 'even'; ?> <?php print $row->class ?>">
              <td class="ds-label"><span class="<?php print $row->label_class; ?>"><?php print $row->human_name; ?></span><span class="label-edit"><?php print $row->{$build_mode}->label_edit; ?></span><?php print $row->{$build_mode}->label_value; ?></td>
              <td><?php print $row->{$build_mode}->label; ?></td>
              <td><?php print $row->{$build_mode}->format; ?></td>
              <td><?php print $row->{$build_mode}->region; ?></td>
              <td><?php print $row->ds_weight; ?></td>
            </tr>
            <?php
            $count++;
          endforeach;
        endif;
      endforeach;
      ?>
      </tbody>
    </table>
  </div>
  <?php if (!empty($plugins_tabs)): ?>
  <div id="plugins-content">
    <?php print $plugins_content; ?>
  </div>
  <?php endif; ?>
</div>
<?php
print $submit;
endif;
