#include "lcs-taskbar.h"
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <gtk/gtk.h>
#include "lcs-wm.h"

 
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
                                     TRUE,
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
    char *wrapxid = lcs_wm_wrap_xidstring_new (window);
    ClutterActor *taskwrap = 
        clutter_container_find_child_by_name (CLUTTER_CONTAINER (taskbar), 
                                              wrapxid);
    if (taskwrap)
    {
        clutter_actor_remove_child (taskbar, taskwrap);
        clutter_actor_set_size (clutter_actor_get_stage (taskbar),
                                clutter_actor_get_width (taskbar),
                                clutter_actor_get_height (taskbar));
    } else {
        fprintf (stderr, "no actor for name: %s\n", wrapxid);
    }
    g_free (wrapxid);
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
	clutter_actor_set_name (taskbar, "taskbar");
    ClutterLayoutManager *layout = clutter_box_layout_new ();    
    clutter_box_layout_set_vertical (CLUTTER_BOX_LAYOUT (layout), TRUE);
    clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (layout), 2);
    clutter_actor_set_layout_manager (taskbar, layout);
    ClutterActor *buttons = lcs_taskbar_buttons_new (connect, taskbar);
    clutter_box_layout_pack (CLUTTER_BOX_LAYOUT (layout),
                             buttons,
                             TRUE,
                             TRUE,
                             FALSE,
                             CLUTTER_BOX_ALIGNMENT_START,
                             CLUTTER_BOX_ALIGNMENT_START);

    ClutterActor *minibuttons = lcs_taskbar_mini_buttons_new (TRUE, taskbar);
    clutter_actor_add_child (taskbar, minibuttons);
	lcs_wm_clutter_actor_set_visible (minibuttons, FALSE);
    
    if (connect)
        lcs_taskbar_connect (taskbar);
        clutter_actor_set_reactive (taskbar, TRUE);
    return taskbar;
}