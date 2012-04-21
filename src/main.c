#include <stdio.h>
#include <stdlib.h>
#include <gdk/gdk.h>
#include <clutter/clutter.h>
#include "lcs-wm.h"
#include "lcs-taskbar.h"

static void on_stage_destroy (ClutterActor *stage)
{
    clutter_main_quit ();
}

int main(int argc, char *argv[])
{
    gdk_init (&argc, &argv);
    lcs_wm_clutter_enable_transparency (TRUE);
    if (!clutter_init (&argc, &argv))
    {
        fprintf (stderr, "error initializing clutter");
        exit (1);
    }
    ClutterActor *stage = clutter_stage_new ();
    clutter_stage_set_use_alpha (CLUTTER_STAGE (stage), TRUE);
    clutter_actor_set_background_color (stage, 
                                        clutter_color_new (255, 255, 255, 0));
    ClutterActor *taskbar = lcs_taskbar_new ();
    clutter_actor_add_child (stage, taskbar);
    g_signal_connect (stage, "destroy", G_CALLBACK (on_stage_destroy), NULL);
    clutter_actor_show (stage);
    long stagexid = lcs_wm_get_stage_xid (CLUTTER_STAGE(stage));
    lcs_wm_xwindow_set_decorated (stagexid, FALSE);
    lcs_wm_xwindow_set_above (stagexid);
        
    clutter_main ();
	return (0);
}
