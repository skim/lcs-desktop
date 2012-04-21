#ifndef LCS_WM_H
#define LCS_WM_H

#include <clutter/clutter.h>

ClutterActor *lcs_wm_clutter_wrap_new (ClutterActor *actor, 
                                       ClutterBinAlignment xalign, 
                                       ClutterBinAlignment yalign);
ClutterMargin *lcs_wm_clutter_margin_new_full (float left, 
                                              float right, 
                                              float top, 
                                              float bottom);
void lcs_wm_clutter_enable_transparency (int enabled);
void lcs_wm_xwindow_set_decorated (long xid, int decorated);
long lcs_wm_get_stage_xid (ClutterStage *stage);
void lcs_wm_xwindow_set_above (long xid);
void lcs_wm_xwindow_change_property (long xid,
                                    const char *property,
                                    const char *value);

#endif /*MW_SCL _H */
