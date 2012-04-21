#include "lcs-taskbar.h"
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <gtk/gtk.h>
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
        ClutterActor *task = lcs_task_new (window, TRUE);
            clutter_box_layout_pack (CLUTTER_BOX_LAYOUT (layout),
                                     task,
                                     FALSE,
                                     TRUE,
                                     FALSE,
                                     CLUTTER_BOX_ALIGNMENT_START,
                                     CLUTTER_BOX_ALIGNMENT_CENTER);
        clutter_actor_set_size (clutter_actor_get_stage (taskbar),
                                clutter_actor_get_width (taskbar),
                                clutter_actor_get_height (taskbar));
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
        clutter_actor_set_size (clutter_actor_get_stage (task),
                                clutter_actor_get_width (task),
                                clutter_actor_get_height (task));
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
    lcs_wm_clutter_texture_set_from_pixbuf (CLUTTER_TEXTURE (icon), buf);
    
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
    ClutterActor *task = clutter_actor_new ();
    clutter_actor_set_name (task, xidstring_new (window));
    ClutterLayoutManager *layout = clutter_box_layout_new ();
    clutter_actor_set_layout_manager (task, layout);

    ClutterActor *icon = clutter_texture_new ();
    clutter_actor_set_name (icon, "icon");
    clutter_actor_add_child (task, icon);

    ClutterActor *name = 
        clutter_text_new_full ("Ubuntu 9", 
                               "", 
                               clutter_color_new (0, 0, 0, 200));
    clutter_actor_set_name (name, "name");
    clutter_text_set_single_line_mode (CLUTTER_TEXT (name), TRUE);
    clutter_text_set_ellipsize (CLUTTER_TEXT (name),
                                PANGO_ELLIPSIZE_END);
    clutter_box_layout_pack (CLUTTER_BOX_LAYOUT (layout),
                             name,
                             FALSE,
                             FALSE,
                             TRUE,
                             CLUTTER_BOX_ALIGNMENT_START,
                             CLUTTER_BOX_ALIGNMENT_CENTER);    
    if (connect)
        lcs_task_connect (task, window);
    on_window_changed (window, task);
    clutter_actor_set_margin (task, 
                              lcs_wm_clutter_margin_new_full (4, 4, 4, 4));
    ClutterActor *wrap = lcs_wm_clutter_wrap_new (task, 
                                                  CLUTTER_BIN_ALIGNMENT_FILL,
                                                  CLUTTER_BIN_ALIGNMENT_CENTER);
    clutter_actor_set_background_color (wrap,
                                        clutter_color_new (255, 255, 255, 200));
    return wrap;
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

static void on_taskbar_back_clicked (ClutterActor *actor, 
                                     ClutterEvent *event,
                                     ClutterActor *taskbar)
{
    ClutterActor *stage = clutter_actor_get_stage (taskbar);
    clutter_actor_save_easing_state (taskbar);
    clutter_actor_save_easing_state (stage);
    clutter_actor_set_easing_duration (taskbar, 750);
    clutter_actor_set_easing_duration (stage, 750);
    clutter_actor_set_easing_mode (taskbar, CLUTTER_EASE_IN_QUAD);
    clutter_actor_set_width (taskbar, 20);
    clutter_actor_set_opacity (stage, 0);    
    clutter_actor_set_opacity (taskbar, 0);    
}


static ClutterActor *lcs_taskbar_buttons_new (int connect, 
                                              ClutterActor *taskbar)
{
    ClutterActor *buttons = clutter_actor_new ();
    ClutterLayoutManager *layout = clutter_box_layout_new ();
    clutter_actor_set_layout_manager (buttons, layout);
    ClutterActor *icon = lcs_wm_clutter_texture_new_from_icon ("go-next", 16);
    clutter_actor_set_reactive (icon, connect);
    clutter_actor_set_name (icon, "back");
    clutter_actor_add_child (buttons, icon);
    clutter_actor_set_margin (buttons, 
                              lcs_wm_clutter_margin_new_full (0, 0, 4, 4));

    if (connect)
    {
        g_signal_connect (icon, 
                          "button-press-event", 
                          G_CALLBACK (on_taskbar_back_clicked), 
                          taskbar);
    }
    
    return buttons;
}

ClutterActor *lcs_taskbar_new (int connect)
{
    ClutterActor *taskbar = clutter_actor_new ();
    ClutterLayoutManager *layout = clutter_box_layout_new ();    
    clutter_box_layout_set_vertical (CLUTTER_BOX_LAYOUT (layout), TRUE);
    clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (layout), 2);
    clutter_actor_set_layout_manager (taskbar, layout);
    clutter_actor_set_margin (taskbar, 
                              lcs_wm_clutter_margin_new_full (4, 4, 4, 4));
    clutter_actor_add_child (taskbar, 
                             lcs_taskbar_buttons_new (connect, taskbar));
    if (connect)
        lcs_taskbar_connect (taskbar);
        clutter_actor_set_reactive (taskbar, TRUE);
    return taskbar;
}