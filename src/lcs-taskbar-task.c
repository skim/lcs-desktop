#include "lcs-taskbar.h"
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include "lcs-wm.h"

static void on_window_state_changed (WnckScreen *screen, 
                                     WnckWindow *window, 
                                     ClutterActor *task)
{

	ClutterActor *stage = clutter_actor_get_stage (task);
	ClutterActor *taskbar = 
		clutter_container_find_child_by_name (
		                                      CLUTTER_CONTAINER (stage), 
		                                      "taskbar");
	WnckWindow *wactive = wnck_screen_get_active_window (screen);
	WnckWindow *wprevious = wnck_screen_get_previously_active_window (screen);
	if (wactive) 
	{
		ClutterActor *active = 
			clutter_container_find_child_by_name (CLUTTER_CONTAINER (taskbar),
			                                      lcs_wm_wrap_xidstring_new (
	                                                                 wactive));
		if (active)
			clutter_actor_set_background_color (active, 
			                                    clutter_color_new (173, 255, 47,
			                                                       96));	
	}
	if (wprevious)
	{
		ClutterActor *previous = 
			clutter_container_find_child_by_name (CLUTTER_CONTAINER (taskbar),
			                                      lcs_wm_wrap_xidstring_new (
                                                                 wprevious));
		if (previous)
			clutter_actor_set_background_color (previous,
			                                    clutter_color_new (255, 255, 
			                                                       255, 127));
	}	
}


static void on_task_clicked (ClutterActor *task, 
                             ClutterEvent *event, 
                             WnckWindow *window)
{
	//wnck_window_activate (window, time(NULL));
	long xid = wnck_window_get_xid (window);
	Display *display = XOpenDisplay (NULL); 	
	XSetInputFocus (display, xid, RevertToNone, CurrentTime);
	XRaiseWindow (display, xid);
    XCloseDisplay (display);	             
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
	lcs_clutter_texture_set_from_pixbuf (CLUTTER_TEXTURE (icon), buf);
}    

static void lcs_task_connect (ClutterActor *task, WnckWindow *window)
{
	clutter_actor_set_reactive (task, TRUE);
	WnckScreen *screen = wnck_window_get_screen (window);
	g_signal_connect (screen, 
	                  "active-window-changed", 
	                  G_CALLBACK (on_window_state_changed),
	                  task);
	g_signal_connect (window, "name-changed", G_CALLBACK (on_window_changed),
	                  task);
	g_signal_connect (window, "icon-changed", G_CALLBACK (on_window_changed),
	                  task);

	g_signal_connect (task, "button-press-event", G_CALLBACK (on_task_clicked),
	                  window);
}

ClutterActor *lcs_task_new (WnckWindow *window, int connect)
{
	ClutterActor *task = clutter_actor_new ();
	clutter_actor_set_name (task, lcs_wm_xidstring_new (window));
	ClutterLayoutManager *layout = clutter_box_layout_new ();
	clutter_box_layout_set_spacing (CLUTTER_BOX_LAYOUT (layout), 4);
	clutter_actor_set_layout_manager (task, layout);

	ClutterActor *icon = clutter_texture_new ();
	clutter_actor_set_name (icon, "icon");
	clutter_actor_add_child (task, icon);

	ClutterActor *name = 
		clutter_text_new_full ("Ubuntu 9", 
		                       "", 
		                       clutter_color_new (0, 0, 0, 200));
	clutter_actor_set_name (name, "name");
	clutter_actor_set_width (name, 320);
	clutter_text_set_single_line_mode (CLUTTER_TEXT (name), TRUE);
	clutter_text_set_ellipsize (CLUTTER_TEXT (name),
	                            PANGO_ELLIPSIZE_END);
	ClutterActor *wrap = lcs_clutter_wrap_new (name,
	                                              CLUTTER_BIN_ALIGNMENT_START,
	                                              CLUTTER_BIN_ALIGNMENT_CENTER);
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
	clutter_actor_set_margin (task, 
	                          lcs_clutter_margin_new_full (4, 4, 4, 4));
	wrap = lcs_clutter_wrap_new (task, CLUTTER_BIN_ALIGNMENT_FILL,
	                                CLUTTER_BIN_ALIGNMENT_CENTER);
	char *wrapstring = lcs_wm_wrap_xidstring_new (window);
	clutter_actor_set_name (wrap, wrapstring);
	clutter_actor_set_background_color (wrap, clutter_color_new (255, 255, 255, 
	                                                             160));
	return wrap;
}

