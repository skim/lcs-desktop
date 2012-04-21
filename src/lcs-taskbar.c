#include "lcs-taskbar.h"
#include <glib/gprintf.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include "lcs-wm.h"

char * xidstring_new (WnckWindow *window)
{
    return g_strdup_printf ("%ld", wnck_window_get_xid (window));
}
 
static void on_window_opened (WnckScreen *screen, 
                       WnckWindow *window,
                       ClutterActor *taskbar)
{
    long ownxid = 
        lcs_wm_get_stage_xid (CLUTTER_STAGE (clutter_actor_get_stage(taskbar))); 
    if (ownxid != wnck_window_get_xid (window))
    {
        ClutterLayoutManager *layout = 
            clutter_actor_get_layout_manager (taskbar);
            clutter_box_layout_pack (CLUTTER_BOX_LAYOUT (layout),
                                     lcs_task_new (window, TRUE),
                                     FALSE,
                                     TRUE,
                                     FALSE,
                                     CLUTTER_BOX_ALIGNMENT_START,
                                     CLUTTER_BOX_ALIGNMENT_CENTER);
    }
}

static void on_window_closed (WnckScreen *screen,
                              WnckWindow *window, 
                              ClutterActor *taskbar)
{
    ClutterActor *task = 
        clutter_container_find_child_by_name (CLUTTER_CONTAINER (taskbar), 
                                              xidstring_new (window));
    if (task)
    {
        clutter_actor_remove_child (taskbar, task);
    }
}

static void on_window_changed (WnckWindow *window, ClutterActor *task)
{
    ClutterActor *name = 
        clutter_container_find_child_by_name (CLUTTER_CONTAINER (task),
                                              "name");   
    clutter_text_set_text (CLUTTER_TEXT (name), wnck_window_get_name (window));
    ClutterActor *icon = 
        clutter_container_find_child_by_name(CLUTTER_CONTAINER (task), "icon");
    GdkPixbuf *buf = wnck_window_get_icon (window);
    int status = 
        clutter_texture_set_from_rgb_data (CLUTTER_TEXTURE (icon),
                                           gdk_pixbuf_get_pixels (buf),
                                           TRUE,
                                           32,
                                           32,
                                           gdk_pixbuf_get_rowstride (buf),
                                           4,
                                           CLUTTER_TEXTURE_NONE,
                                           NULL);
    if (!status)
        fprintf (stderr, 
                 "error loading icon: %s\n", 
                 wnck_window_get_icon_name (window));
}

static void lcs_task_connect (ClutterActor *task, WnckWindow *window)
{
    g_signal_connect (window, 
                      "name-changed", 
                      G_CALLBACK (on_window_changed), 
                      task);
    g_signal_connect (window,
                      "icon-changed",
                      G_CALLBACK (on_window_changed),
                      task);
}


ClutterActor *lcs_task_new (WnckWindow *window, int connect)
{
    ClutterMargin *margin = lcs_wm_clutter_margin_new_full (2, 2, 2, 2);
    ClutterActor *task = clutter_actor_new ();
    clutter_actor_set_name (task, xidstring_new (window));
    ClutterLayoutManager *layout = clutter_box_layout_new ();
    clutter_actor_set_layout_manager (task, layout);

    ClutterActor *icon = clutter_texture_new ();
    clutter_actor_set_name (icon, "icon");
    clutter_actor_set_margin (icon, margin);
    ClutterActor *wrap = lcs_wm_clutter_wrap_new (icon, 
                                                  CLUTTER_BIN_ALIGNMENT_CENTER, 
                                                  CLUTTER_BIN_ALIGNMENT_CENTER);
    ClutterColor *bgcolor = clutter_color_new (255, 255, 255, 200);
    clutter_actor_set_background_color (wrap, bgcolor);
    clutter_actor_add_child (task, wrap);

    ClutterActor *name = 
        clutter_text_new_full ("Ubuntu 9", 
                               "", 
                               clutter_color_new (0, 0, 0, 200));
    clutter_actor_set_name (name, "name");
    clutter_text_set_single_line_mode (CLUTTER_TEXT (name), TRUE);
    clutter_text_set_ellipsize (CLUTTER_TEXT (name),
                                PANGO_ELLIPSIZE_END);
    clutter_actor_set_margin (name, margin);
    clutter_actor_set_width (name, 300);
    wrap =lcs_wm_clutter_wrap_new (name,
                                   CLUTTER_BIN_ALIGNMENT_START,
                                   CLUTTER_BIN_ALIGNMENT_CENTER);
    clutter_actor_set_background_color (wrap, bgcolor);
    clutter_box_layout_pack (CLUTTER_BOX_LAYOUT (layout),
                             wrap,
                             FALSE,
                             FALSE,
                             TRUE,
                             CLUTTER_BOX_ALIGNMENT_START,
                             CLUTTER_BOX_ALIGNMENT_CENTER);    
    if (connect)
        lcs_task_connect (task, window);
    on_window_changed (window, task);
    return task;
}



static void lcs_taskbar_connect (ClutterActor *actor)
{
    WnckScreen *screen = wnck_screen_get_default ();
    g_signal_connect (screen, 
                      "window-opened", 
                      G_CALLBACK (on_window_opened), 
                      actor);
    g_signal_connect (screen,
                      "window-closed",
                      G_CALLBACK (on_window_closed),
                      actor);
}

ClutterActor *lcs_taskbar_new (int connect)
{
    ClutterActor *taskbar = clutter_actor_new ();
    ClutterLayoutManager *layout = clutter_box_layout_new ();
    clutter_box_layout_set_vertical (CLUTTER_BOX_LAYOUT (layout), TRUE);
    clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (layout), 2);
    clutter_actor_set_layout_manager (taskbar, layout);
    if (connect)
        lcs_taskbar_connect (taskbar);
    return taskbar;
}