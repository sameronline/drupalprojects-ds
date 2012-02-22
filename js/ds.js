
(function($) {

Drupal.DisplaySuite = Drupal.DisplaySuite || {};
Drupal.DisplaySuite.fieldopened = '';
Drupal.DisplaySuite.layout_original = '';

/**
 * Ctools selection content.
 */
Drupal.behaviors.CToolsSelection = {
  attach: function (context) {
    if ($('#ctools-content-selection').length > 0) {
      $('#ctools-content-selection .section-link').click(function() {
        $('#ctools-content-selection .content').hide();
        container = $(this).attr('id') + '-container';
        $('#' + container).show();
        return false;
      });
    }
  }
};

/**
 * Save the Dynamic field content configuration.
 */
$.fn.dsCtoolsContentConfiguration = function (configuration) {
  $(this[0]).val(configuration);
}

/**
 * Update the select content text.
 */
$.fn.dsCtoolsContentUpdate = function () {
  $(this[0]).html(Drupal.t('Click update to save the configuration'));
}

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
        console.log('kak');
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

/**
 * Save the page after saving a new field.
 */
$.fn.dsRefreshDisplayTable = function () {
  $('#edit-submit').click();
}

/**
 * Row handlers for the 'Manage display' screen.
 */
Drupal.fieldUIDisplayOverview = Drupal.fieldUIDisplayOverview || {};

Drupal.fieldUIDisplayOverview.ds = function (row, data) {

  this.row = row;
  this.name = data.name;
  this.region = data.region;
  this.tableDrag = data.tableDrag;

  this.$regionSelect = $('select.ds-field-region', row);
  this.$regionSelect.change(Drupal.fieldUIOverview.onChange);

  return this;
};

Drupal.fieldUIDisplayOverview.ds.prototype = {

  /**
   * Returns the region corresponding to the current form values of the row.
   */
  getRegion: function () {
    return this.$regionSelect.val();
  },

  /**
   * Reacts to a row being changed regions.
   *
   * This function is called when the row is moved to a different region, as a
   * result of either :
   * - a drag-and-drop action
   * - user input in one of the form elements watched by the
   *   Drupal.fieldUIOverview.onChange change listener.
   *
   * @param region
   *   The name of the new region for the row.
   * @return
   *   A hash object indicating which rows should be AJAX-updated as a result
   *   of the change, in the format expected by
   *   Drupal.displayOverview.AJAXRefreshRows().
   */
  regionChange: function (region) {

     // Replace dashes with underscores.
     region = region.replace('-', '_');

     // Set the region of the select list.
     this.$regionSelect.val(region);

     // Prepare rows to be refreshed in the form.
     var refreshRows = {};
     refreshRows[this.name] = this.$regionSelect.get(0);

     // If a row is handled by field_group module, loop through the children.
     if ($(this.row).hasClass('field-group') && $.isFunction(Drupal.fieldUIDisplayOverview.group.prototype.regionChangeFields)) {
       Drupal.fieldUIDisplayOverview.group.prototype.regionChangeFields(region, this, refreshRows);
     }

     return refreshRows;
  }
};

})(jQuery);
