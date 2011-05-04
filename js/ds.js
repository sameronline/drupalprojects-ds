
(function($) {

Drupal.DisplaySuite = Drupal.DisplaySuite || {};
Drupal.DisplaySuite.fieldopened = '';
  
/**
 * Field settings.
 */
Drupal.behaviors.settingsToggle = {
  attach: function (context) {
  
    // remove click from link
    $('.ft-link').click(function(e) {
      e.preventDefault();
    });

    // Bind update button.
    $('#field-display-overview .ft-update').click(function() {
      var settings = $(this).parents('.field-template');
      settings.hide();
      $(this).parents('tr').removeClass('field-formatter-settings-editing');
      return false;
    });   
    
    // Bind on field template select button.
    /*$('.ds-extras-field-template').change(function() {
      ds_show_custom_settings(this);
    });*/
    
    // Add click event to field settings link.
    $('.ft-link').click(function() {
      
      $(this).parents('tr').siblings().removeClass('field-formatter-settings-editing');
      $(this).parents('tr').addClass('field-formatter-settings-editing');
      
      var settings = $(this).siblings('.field-template');
      if (Drupal.DisplaySuite.fieldopened != '' && Drupal.DisplaySuite.fieldopened != settings.attr('id')) {
        $('#' + Drupal.DisplaySuite.fieldopened).hide();
      }

      if (settings.is(':visible')) {
        $(this).parents('tr').removeClass('field-formatter-settings-editing');
        settings.hide();
      }
      else {
        // Slide down.
        settings.slideDown('normal');
      }
      // Store the opened setting.
      Drupal.DisplaySuite.fieldopened = settings.attr('id');
    });
    
    /*function ds_show_custom_settings(element) {
      ft = $('.ds-extras-field-template').val();
      var field = $(element).parents('.field-template');
      console.log(field);
      if (ft == 'theme_ds_field_custom') {
        $(field).siblings('.ft-group .ow').show();        
      }
      else {
        $(field).siblings('ft-group .ow').hide();
      }      
    }*/
  }
};
  
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

    this.$regionSelect.val(region);    

    var refreshRows = {};
    refreshRows[this.name] = this.$regionSelect.get(0);
    
    return refreshRows;
  },
};

})(jQuery);
