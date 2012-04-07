/**
 * @file
 * Javascript functionality for the Display Suite Extras administration UI.
 */

(function ($) {

Drupal.behaviors.DSExtrasSummaries = {
  attach: function (context) {

    $('#edit-additional-settings-fs1', context).drupalSetSummary(function (context) {
      var fieldtemplates = $('#edit-additional-settings-fs1-ds-extras-field-template', context);

      if (fieldtemplates.is(':checked')) {
        var fieldtemplate = $('#edit-additional-settings-fs1-ft-default option:selected').text();
        return Drupal.t('Enabled') + ': ' + Drupal.t(fieldtemplate);
      }

      return Drupal.t('Disabled');
    });

    $('#edit-additional-settings-fs2', context).drupalSetSummary(function (context) {
      var extra_fields = $('#edit-additional-settings-fs2-ds-extras-fields-extra', context);

      if (extra_fields.is(':checked')) {
        return Drupal.t('Enabled');
      }

      return Drupal.t('Disabled');
    });
    
    $('#edit-additional-settings-fs3', context).drupalSetSummary(function (context) {
      var panel_view_modes = $('#edit-additional-settings-fs3-ds-extras-panel-view-modes', context);

      if (panel_view_modes.is(':checked')) {
        return Drupal.t('Enabled');
      }

      return Drupal.t('Disabled');
    });


    $('#edit-additional-settings-fs4', context).drupalSetSummary(function (context) {
      var vals = [];

      $('input:checked', context).parent().each(function () {
        vals.push(Drupal.checkPlain($.trim($('.option', this).text())));
      });

      if (vals.length > 0) {
        return vals.join(', ');
      }
      return Drupal.t('Disabled');
    });
  }
};

/**
 * Field template.
 */
Drupal.behaviors.settingsToggle = {
  attach: function (context) {

    // Bind on click.
    $('.field-formatter-settings-edit-form', context).once('ds-ft', function() {

      var fieldTemplate = $(this);

      // Bind update button.
      fieldTemplate.find('input[value="Update"]').click(function() {
        // Check the label.
        var row = $(this).parents('tr');
        var label = $('.label-change', settings).val();
        var original = $('.original-label', row).val();
        if (label != '') {
          new_label = label + ' (Original: ' + original + ')<input type="hidden" class="original-label" value="' + original + '">';
          $('.field-label-row', row).html(new_label);
        }
        else {
          new_label = original + '<input type="hidden" class="original-label" value="' + original + '">';
          $('.field-label-row', row).html(new_label);
        }
        return false;
      });

      // Bind on field template select button.
      fieldTemplate.find('.ds-extras-field-template').change(function() {
        ds_show_expert_settings(fieldTemplate);
      });

      ds_show_expert_settings(fieldTemplate);

    });

    // Show / hide settings on field template form.
    function ds_show_expert_settings(element, open) {
      field = element;
      ft = $('.ds-extras-field-template', field).val();
      console.log(ft);
      if (ft == 'theme_ds_field_expert') {
        // Show second and third label.
        if ($('.lb .form-item:nth-child(1)', field).is(':visible')) {
          $('.lb .form-item:nth-child(2), .lb .form-item:nth-child(3)', field).show();
        }
        // Remove margin from update button.
        $('.ft-update', field).css({'margin-top': '-10px'});
        // Show wrappers.
        $('.ow, .fis, .fi', field).show();
      }
      else {
        // Hide second and third label.
        $('.lb .form-item:nth-child(2), .lb .form-item:nth-child(3)', field).hide();
        // Add margin on update button.
        $('.ft-update', field).css({'margin-top': '10px'});
        // Hide wrappers.
        $('.ow, .fis, .fi', field).hide();
      }

      // Colon.
      if (ft == 'theme_field' || ft == 'theme_ds_field_reset') {
        $('.colon-checkbox', field).parent().hide();
      }
      else if ($('.lb .form-item:nth-child(1)', field).is(':visible')) {
        $('.colon-checkbox', field).parent().show();
      }

      // CSS classes.
      if (ft != 'theme_ds_field_expert' && ft != 'theme_ds_field_reset') {
        $('.field-classes', field).show();
      }
      else {
        $('.field-classes', field).hide();
      }
    }

    $('.label-change').change(function() {
      var field = $(this).parents('tr');
      if ($('.field-template', field).length > 0) {
        ft = $('.ds-extras-field-template', field).val();
        if (ft == 'theme_field' || ft == 'theme_ds_field_reset') {
          $('.colon-checkbox', field).parent().hide();
        }
      }
    });
  }
};

})(jQuery);
