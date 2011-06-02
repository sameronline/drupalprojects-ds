
(function ($) {

Drupal.behaviors.DSExtrasSwitchViewmode = {
  attach: function (context) {

    if ($('.switch-view-mode-field').length > 0) {
      $('.switch-view-mode-field a').click(function() {

        // Create an object.
        var $link = $(this);

        // Get params from the class.
        var params = $(this).attr('class').split('-');

        $.ajax({
          type: 'GET',
          url: Drupal.settings.basePath + 'ds-switch-view-mode',
          data: {entity_type: params[0], view_mode: params[3], id: params[2]},
          dataType: 'json',
          success: function (data) {
            if (data.status) {
              old_view_mode = params[1];
              wrapper = $link.parents('.view-mode-' + old_view_mode);
              wrapper.replaceWith(data.content);
              Drupal.attachBehaviors();
            }
            else {
              alert(data.errorMessage);
            }
          },
          error: function (xmlhttp) {
            alert('An HTTP error '+ xmlhttp.status +' occurred.');            
          }
        });
        return false;
      });
    }
  }
};

})(jQuery);