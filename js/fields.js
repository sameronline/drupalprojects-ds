// $Id$

(function ($) {

/**
 * Toggle all exclude at once.
 */
Drupal.behaviors.field_excludes = {
  attach: function (context) {
    $('.select-all').bind('click', function() {
      var excluder = this;
      $(excluder).parents('div').parents('div').find('.exclude-types').not('.exclude-all').each(function() {
        if(excluder.checked) {
          $(this).attr('checked', 'checked');
        }
        else {
          $(this).attr('checked', '');
        }
      });
    });
  }
}

})(jQuery);