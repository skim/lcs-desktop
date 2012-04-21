#include <X11/Xlib.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
} MotifWMHints;

ClutterActor *lcs_wm_clutter_wrap_new (ClutterActor *actor, 
                                       ClutterBinAlignment xalign, 
                                       ClutterBinAlignment yalign)
{
    ClutterLayoutManager *layout = clutter_bin_layout_new (xalign, yalign);
    ClutterActor *wrap = clutter_actor_new ();
    clutter_actor_set_layout_manager (wrap, layout);
    clutter_actor_add_child (wrap, actor);
    return wrap;
}

ClutterMargin *lcs_wm_clutter_margin_new_full (float left, 
                                              float right, 
                                              float top, 
                                              float bottom)
{
    ClutterMargin *margin = clutter_margin_new ();
    margin->left = left;
    margin->right = right;
    margin->top = top;
    margin->bottom = bottom;
    return margin;
}

void lcs_wm_clutter_enable_transparency (int enabled)
{
    clutter_x11_set_use_argb_visual (enabled);
}

long lcs_wm_get_stage_xid (ClutterStage *stage) {
    long xid = -1;	
    xid = clutter_x11_get_stage_window (stage);
    return xid;	
}

void lcs_wm_xwindow_set_decorated (long xid, int decorated)
{
    Display *display = NULL;
    MotifWMHints hints = {2, 0, 0, 0, 0};	

    display = XOpenDisplay (NULL);
    XChangeProperty (display, xid,
                     XInternAtom (display, "_MOTIF_WM_HINTS", False),
                     XInternAtom (display, "_MOTIF_WM_HINTS", False),
                     32, PropModeReplace,
                     (const unsigned char *) &hints,
                     sizeof (hints) / sizeof (long));

    XCloseDisplay (display);
}

void lcs_wm_xwindow_change_property (long xid,
                                    const char *property,
                                    const char *value)
{
    Display *display = NULL;
    Atom aprop, avalue;

    display = XOpenDisplay (NULL);
    aprop = XInternAtom (display, property, False);
    avalue = XInternAtom (display, value, False);
    XChangeProperty(display,
                    xid,
                    aprop,
                    XA_ATOM,
                    32,
                    PropModeReplace,
                    (unsigned char*) &avalue,
                    1);

    XCloseDisplay (display);
}



void lcs_wm_xwindow_set_above (long xid)
{
    lcs_wm_xwindow_change_property (xid,
                                   "_NET_WM_STATE",
                                   "_NET_WM_STATE_ABOVE");
}

