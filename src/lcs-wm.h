#ifndef LCS_WM_H
#define LCS_WM_H

#include <clutter/clutter.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#include "lcs-clutter.h"


char *lcs_wm_string_new (const char *string);

char *lcs_wm_xidstring_new (WnckWindow *window);

char *lcs_wm_wrap_xidstring_new (WnckWindow *window);

void lcs_wm_gobject_set_boolean_property (gpointer object, 
                                          const char *name,
                                          int value);
                                  
void lcs_wm_xwindow_set_decorated (long xid, int decorated);

long lcs_wm_get_stage_xid (ClutterStage *stage);

void lcs_wm_xwindow_set_above (long xid);

void lcs_wm_xwindow_change_property (long xid,
                                    const char *property,
                                    const char *value);

void lcs_wm_xwindow_set_dock (long xid);

#endif /*MW_SCL _H */
