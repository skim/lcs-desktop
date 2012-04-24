#include <glib/gprintf.h>
#include <X11/Xlib.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>
#include <gtk/gtk.h>
#include "lcs-wm.h"

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
} MotifWMHints;

char *lcs_wm_xidstring_new (WnckWindow *window)
{
    return g_strdup_printf ("xid-%ld", wnck_window_get_xid (window));
}

char *lcs_wm_wrap_xidstring_new (WnckWindow *window)
{
    return g_strdup_printf ("wrap-xid-%ld", wnck_window_get_xid (window));
}


void lcs_wm_gobject_set_boolean_property (gpointer object, 
                                          const char *name,
                                          int value)
{
    GValue gvalue = G_VALUE_INIT;    
    g_value_init (&gvalue, G_TYPE_BOOLEAN);
    g_value_set_boolean (&gvalue, value);
    g_object_set_property (G_OBJECT (object), "visible", &gvalue);

}

char *lcs_wm_string_new (const char *string)
{
    return g_strdup_printf ("lcs-wm-%s", string);
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

void lcs_wm_xwindow_set_dock (long xid)
{
	lcs_wm_xwindow_change_property(xid, 
	                               "_NET_WM_WINDOW_TYPE",
	                               "_NET_WM_WINDOW_TYPE_UTILITY");
}

