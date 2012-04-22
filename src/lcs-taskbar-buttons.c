#include "lcs-taskbar.h"
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <gtk/gtk.h>
#include "lcs-wm.h"

static void on_taskbar_back_clicked (ClutterActor *actor, 
                                     ClutterEvent *event,
                                     ClutterActor *taskbar)
{
    ClutterActor *stage = clutter_actor_get_stage (taskbar);
    clutter_actor_save_easing_state (taskbar);
    clutter_actor_save_easing_state (stage);
    clutter_actor_set_easing_duration (taskbar, 500);
    clutter_actor_set_easing_duration (stage, 500);
    clutter_actor_set_easing_mode (taskbar, CLUTTER_EASE_OUT_QUAD);
    clutter_actor_set_easing_mode (stage, CLUTTER_EASE_IN_QUAD);
    clutter_actor_set_opacity (stage, 0);    
    clutter_actor_set_opacity (taskbar, 0);
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

    button = 
    lcs_taskbar_button_new ("go-down", 
                            16, 
                            G_CALLBACK (on_taskbar_back_clicked), 
                            taskbar);
    clutter_actor_add_child (buttons, button);
    clutter_box_layout_set_alignment (CLUTTER_BOX_LAYOUT (layout),
                                      button,
                                      CLUTTER_BIN_ALIGNMENT_CENTER,
                                      CLUTTER_BIN_ALIGNMENT_CENTER);
    lcs_wm_gobject_set_boolean_property (button, "visible", FALSE);

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
