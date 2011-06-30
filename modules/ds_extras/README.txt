
The extras module contains functionality that is not often used. It holds
following functionality:

 - switch view modes: switch view mode on a per node basis.
 - block region: add regions which will be exposed as blocks.
 - extra fields: expose extra fields defined by other modules.
 - field templates: overwrite any field with custom markup.
 - panel view modes: use the panels layout editor instead of field ui.
 - switch view mode field: switch from one view mode to another inline.
 - hide page title: hide the page title.
 - contextual links: add the 'manage display' link to contextual links 
   and on the full page view of nodes, users and terms.
 - fivestar: add fivestar support.
 - Views displays: render views into a different layout.
     Important: If you are creating new ds fields for vd,
     check ds_vd_render_title_field() how to return the content.
 
Any other functionality will be included in this module.