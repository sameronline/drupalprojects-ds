/**
 * @file
 * Javascript functionality for the Display Suite Extras administration UI.
 */

(function ($, Drupal) {

Drupal.behaviors.DSExtrasSummaries = {
  attach: function (context) {

    $('#edit-fs2', context).drupalSetSummary(function (context) {
      var extra_fields = $('#edit-fs2-fields-extra', context);

      if (extra_fields.is(':checked')) {
        return Drupal.t('Enabled');
      }

      return Drupal.t('Disabled');
    });

    $('#edit-fs3', context).drupalSetSummary(function (context) {
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

})(jQuery, Drupal);
