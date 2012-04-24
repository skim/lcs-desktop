#ifndef LCS_WM_H
#define LCS_WM_H

#include <clutter/clutter.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

void lcs_wm_clutter_stage_move_by (ClutterStage *stage, float x, float y);

void lcs_wm_clutter_actor_set_visible (ClutterActor *actor, int visible);

char *lcs_wm_xidstring_new (WnckWindow *window);

char *lcs_wm_wrap_xidstring_new (WnckWindow *window);

void lcs_wm_gobject_set_boolean_property (gpointer object, 
                                          const char *name,
                                          int value);
                                  

ClutterColor *lcs_wm_clutter_color_static_new (ClutterStaticColor color);

ClutterActor *lcs_wm_clutter_wrap_new (ClutterActor *actor, 
                                       ClutterBinAlignment xalign, 
                                       ClutterBinAlignment yalign);

ClutterMargin *lcs_wm_clutter_margin_new_full (float left, 
                                              float right, 
                                              float top, 
                                              float bottom);

void lcs_wm_clutter_actor_set_pink (ClutterActor *actor, 
                                    int highlight);

void lcs_wm_clutter_texture_set_from_pixbuf (ClutterTexture *texture, 
                                             GdkPixbuf *pixbuf);

ClutterActor *lcs_wm_clutter_texture_new_from_pixbuf (GdkPixbuf *pixbuf);

ClutterActor *lcs_wm_clutter_texture_new_from_pixbuf_full (GdkPixbuf *pixbuf,
                                                           int width,
                                                           int height);

ClutterActor *lcs_wm_clutter_texture_new_from_icon (const char *icon_name,
                                                    int size);

void lcs_wm_clutter_enable_transparency (int enabled);

void lcs_wm_xwindow_set_decorated (long xid, int decorated);

long lcs_wm_get_stage_xid (ClutterStage *stage);

void lcs_wm_xwindow_set_above (long xid);

void lcs_wm_xwindow_change_property (long xid,
                                    const char *property,
                                    const char *value);

#endif /*MW_SCL _H */
