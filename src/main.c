#include <stdio.h>
#include <stdlib.h>
#include <gdk/gdk.h>
#include <clutter/clutter.h>
#include <gtk/gtk.h>
#include "lcs-wm.h"
#include "lcs-taskbar.h"

static void on_stage_destroy (ClutterActor *stage)
{
    clutter_main_quit ();
}

static void on_stage_drag_motion (ClutterDragAction *action,
                                  ClutterActor *actor,
                                  float dx,
                                  float dy)
{    
	lcs_clutter_stage_move_by (CLUTTER_STAGE (actor), dx, dy);
}

static int lcs_taskbar (int *argc, char ***argv)
{
	gdk_init (argc, argv);
    lcs_clutter_enable_transparency (TRUE);
    gtk_init (argc, argv);
    if (!clutter_init (argc, argv))
    {
        fprintf (stderr, "error initializing clutter");
        exit (1);
    }
    
    ClutterActor *stage = clutter_stage_new ();
    clutter_actor_set_layout_manager (
                         stage, 
                         clutter_bin_layout_new (CLUTTER_BIN_ALIGNMENT_START,
                                                 CLUTTER_BIN_ALIGNMENT_START));
    clutter_stage_set_use_alpha (CLUTTER_STAGE (stage), TRUE);
    clutter_actor_set_background_color (stage, 
                                        clutter_color_new (255, 255, 255, 96));
    ClutterActor *taskbar = lcs_taskbar_new ();    
    clutter_actor_set_margin (taskbar, 
                              lcs_clutter_margin_new_full (4, 4, 4, 4));
    
    clutter_actor_add_child (stage, taskbar);
    g_signal_connect (stage, "destroy", G_CALLBACK (on_stage_destroy), NULL);

	ClutterAction *drag = clutter_drag_action_new ();
	g_signal_connect (drag, 
	                  "drag-motion", 
	                  G_CALLBACK (on_stage_drag_motion), 
	                  NULL);
	clutter_actor_add_action_with_name (stage, "drag", drag);
	
    clutter_actor_show (stage);
    
    long stagexid = lcs_wm_get_stage_xid (CLUTTER_STAGE(stage));
	g_object_set_data (G_OBJECT (stage), "xid", GINT_TO_POINTER (stagexid));
    lcs_wm_xwindow_set_decorated (stagexid, FALSE);
    lcs_wm_xwindow_set_above (stagexid);
	lcs_wm_xwindow_set_dock (stagexid);

        
    clutter_main ();
	return (0);

}

int main(int argc, char **argv)
{
	return lcs_taskbar (&argc, &argv);
}
