#include "lcs-taskbar.h"
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <gtk/gtk.h>
#include "lcs-wm.h"


static void on_taskbar_show_clicked (ClutterActor *actor,
                                     ClutterEvent *event,
                                     ClutterActor *taskbar)
{
    ClutterActor *stage = clutter_actor_get_stage (taskbar);
	GList *child = clutter_actor_get_children(taskbar);
	for (; child; child = child->next)
	{
		lcs_wm_clutter_actor_set_visible (child->data, TRUE);
	}
    g_list_free (child);
	ClutterActor *minibuttons = 
		clutter_container_find_child_by_name (CLUTTER_CONTAINER (taskbar),
		                                      "mini-buttons");
	lcs_wm_clutter_actor_set_visible (minibuttons, FALSE);
        clutter_actor_set_size (stage,
                                clutter_actor_get_width (taskbar),
                                clutter_actor_get_height (taskbar));
}

static void on_taskbar_back_clicked (ClutterActor *actor, 
                                     ClutterEvent *event,
                                     ClutterActor *taskbar)
{
    ClutterActor *stage = clutter_actor_get_stage (taskbar);
	GList *child = clutter_actor_get_children(taskbar);
	for (; child; child = child->next)
	{
		lcs_wm_clutter_actor_set_visible (child->data, FALSE);
	}
    g_list_free (child);
	ClutterActor *minibuttons = 
		clutter_container_find_child_by_name (CLUTTER_CONTAINER (taskbar),
		                                      "mini-buttons");
	lcs_wm_clutter_actor_set_visible (minibuttons, TRUE);
	float w, h;
	clutter_actor_get_size (minibuttons, &w, &h);
	ClutterMargin margin;
	clutter_actor_get_margin (taskbar, &margin);
	w += margin.left + margin.right;
	h += margin.top + margin.bottom;
	clutter_actor_set_size (stage, w, h); 
	                          
	                        
}

static void on_taskbar_quit_clicked (ClutterActor *actor,
                                     ClutterEvent *event,
                                     ClutterActor *taskbar)
{
    ClutterActor *stage = clutter_actor_get_stage (taskbar);
    g_signal_emit_by_name (stage, "destroy"); 
}

static ClutterActor *lcs_taskbar_button_new (const char *icon_name, 
                                             int size,
                                             GCallback handler,
                                             gpointer data)
{
    ClutterActor *icon = lcs_wm_clutter_texture_new_from_icon (icon_name, 16);
    clutter_actor_set_reactive (icon, handler != NULL);
    clutter_actor_set_name (icon, "back");    
    if (handler)
    {
        g_signal_connect (icon, 
                          "button-press-event", 
                          G_CALLBACK (handler), 
                          data);
    }
    return icon;
}

ClutterActor *lcs_taskbar_buttons_new (int connect, ClutterActor *taskbar)
{
    ClutterActor *buttons = clutter_actor_new ();
	clutter_actor_set_name (buttons, "buttons");
    ClutterLayoutManager *layout = clutter_box_layout_new ();    
    clutter_actor_set_layout_manager (buttons, layout);

    ClutterActor *button = 
        lcs_taskbar_button_new ("go-up", 
                                16, 
                                G_CALLBACK (on_taskbar_back_clicked), 
                                taskbar);
    clutter_actor_add_child (buttons, button);
    clutter_box_layout_set_alignment (CLUTTER_BOX_LAYOUT (layout),
                                      button,
                                      CLUTTER_BIN_ALIGNMENT_CENTER,
                                      CLUTTER_BIN_ALIGNMENT_CENTER);

	ClutterActor *text = 
	clutter_text_new_full ("Ubuntu 9", 
                           "Taskbar", 
                           clutter_color_new (0, 0, 0, 200));
	clutter_actor_set_margin (text, lcs_wm_clutter_margin_new_full (4, 4, 0,0));	
	clutter_actor_add_child (buttons, text);

	
    button = lcs_taskbar_button_new ("window-close",
                                     16,
                                     G_CALLBACK (on_taskbar_quit_clicked),
                                     taskbar);
    ClutterActor *wrap = lcs_wm_clutter_wrap_new (button,
                                                  CLUTTER_BIN_ALIGNMENT_END,
                                                  CLUTTER_BIN_ALIGNMENT_CENTER);  

    clutter_actor_add_child (buttons, wrap);
    clutter_box_layout_set_expand (CLUTTER_BOX_LAYOUT (layout), wrap, TRUE);    
    clutter_box_layout_set_fill (CLUTTER_BOX_LAYOUT (layout), wrap, TRUE,
                                                                    FALSE);    
    
    clutter_actor_set_margin (buttons, 
                              lcs_wm_clutter_margin_new_full (0, 0, 0, 2));
    g_signal_connect (taskbar, 
                      "button-press-event", 
                      G_CALLBACK (on_taskbar_back_clicked), 
                      taskbar);
    return buttons;
}

ClutterActor *lcs_taskbar_mini_buttons_new (int connect, ClutterActor *taskbar)
{
    ClutterActor *buttons = clutter_actor_new ();
	clutter_actor_set_name (buttons, "mini-buttons");
    ClutterLayoutManager *layout = clutter_box_layout_new ();
	clutter_actor_set_layout_manager (buttons, layout);

	
	ClutterActor *button = 
		lcs_taskbar_button_new ("go-down",
                                16,
                                G_CALLBACK (on_taskbar_show_clicked),
                                taskbar);
	clutter_actor_add_child (buttons, button);

	ClutterActor *text = 
		clutter_text_new_full ("Ubuntu 9", 
                               "Taskbar", 
                               clutter_color_new (0, 0, 0, 200));
	clutter_actor_set_margin (text, lcs_wm_clutter_margin_new_full (4, 4, 0,0));
	clutter_actor_add_child (buttons, text);
	
	button = lcs_taskbar_button_new ("window-close",
         		                     16,
                 	                 G_CALLBACK (on_taskbar_quit_clicked),
                     	             taskbar);
	clutter_actor_add_child (buttons, button);

    return buttons;
}
