#ifndef LCS_TASKBAR_H
#define LCS_TASKBAR_H

#include <clutter/clutter.h>
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>


ClutterActor *lcs_task_new (WnckWindow *window, int connect);
ClutterActor *lcs_taskbar_new ();
ClutterActor *lcs_taskbar_buttons_new (int connect, ClutterActor *taskbar);

#endif /*RABKSAT_SCL _H */
