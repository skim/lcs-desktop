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

void lcs_wm_clutter_texture_set_from_pixbuf_full (ClutterTexture *texture,
                                                  GdkPixbuf *pixbuf,
                                                  int width,
                                                  int height)  
{
    int status = 
        clutter_texture_set_from_rgb_data (CLUTTER_TEXTURE (texture),
                                           gdk_pixbuf_get_pixels (pixbuf),
                                           TRUE,
                                           width,
                                           height,
                                           gdk_pixbuf_get_rowstride (pixbuf),
                                           4,
                                           CLUTTER_TEXTURE_NONE,
                                           NULL);
    if (!status)
        fprintf (stderr, "error setting texture data from pixbuf");
    clutter_texture_set_sync_size (texture, TRUE);
}

void lcs_wm_clutter_texture_set_from_pixbuf (ClutterTexture *texture,
                                             GdkPixbuf *pixbuf)  
{
    lcs_wm_clutter_texture_set_from_pixbuf_full (
                                                texture,
                                                pixbuf,
                                                gdk_pixbuf_get_width (pixbuf),
                                                gdk_pixbuf_get_height (pixbuf));
}
 

ClutterActor *lcs_wm_clutter_texture_new_from_pixbuf (GdkPixbuf *pixbuf)
{
    ClutterActor *texture = clutter_texture_new ();
    lcs_wm_clutter_texture_set_from_pixbuf (CLUTTER_TEXTURE (texture), pixbuf);
    return texture;
}

ClutterActor *lcs_wm_clutter_texture_new_from_pixbuf_full (GdkPixbuf *pixbuf,
                                                           int width,
                                                           int height)
{
    ClutterActor *texture = clutter_texture_new ();
    lcs_wm_clutter_texture_set_from_pixbuf_full (CLUTTER_TEXTURE (texture), 
                                                 pixbuf,
                                                 width,
                                                 height);
    return texture;
}

ClutterActor *lcs_wm_clutter_texture_new_from_icon (const char *icon_name, 
                                                    int size)
{
    GtkIconTheme *icons = gtk_icon_theme_get_default ();
    GtkIconInfo *iconinfo = 
        gtk_icon_theme_lookup_icon (icons, 
                                    icon_name, 
                                    size, 
                                    GTK_ICON_LOOKUP_GENERIC_FALLBACK);
    GdkPixbuf *buf = gtk_icon_info_load_icon (iconinfo, NULL);
    
    return lcs_wm_clutter_texture_new_from_pixbuf (buf);
    
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

