/* SPDX-License-Identifier: Apache-2.0
 * Copyright 2011-2022 Blender Foundation */

#ifndef __UTIL_VIEW_H__
#define __UTIL_VIEW_H__

/* Functions to display a simple OpenGL window using GLUT, simplified to the
 * bare minimum we need to reduce boilerplate code in tests apps. */

CCL_NAMESPACE_BEGIN

typedef void (*ViewInitFunc)();
typedef void (*ViewExitFunc)();
typedef void (*ViewResizeFunc)(int width, int height);
typedef void (*ViewDisplayFunc)();
typedef void (*ViewKeyboardFunc)(unsigned char key);
typedef void (*ViewMotionFunc)(int x, int y, int button);

void view_main_loop(const char *title,
                    int width,
                    int height,
                    ViewInitFunc initf,
                    ViewExitFunc exitf,
                    ViewResizeFunc resize,
                    ViewDisplayFunc display,
                    ViewKeyboardFunc keyboard,
                    ViewMotionFunc motion);

void view_display_info(const char *info);
void view_display_help();
void view_redraw();

CCL_NAMESPACE_END

#endif /*__UTIL_VIEW_H__*/
