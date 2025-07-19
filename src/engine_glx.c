#include "engine.h"

#ifdef ENGINE_GLX

float engine_glx_get_aspect_ratio(void) {
  return (float)engine_glx_instance.window_width / engine_glx_instance.window_height;
}

float (*engine_get_aspect_ratio)(void) = engine_glx_get_aspect_ratio;

struct engine_glx engine_glx_instance = {
  .window_title = "Game Window",
  .engine_running = true,
  .window_width = 640,
  .window_height = 480,
};

bool engine_glx_start(void) {
  engine_glx_instance.display = XOpenDisplay(NULL);
  if (engine_glx_instance.display == NULL) {
    engine_error("cannot connect to X server");
    return false;
  }

  engine_glx_instance.screen = DefaultScreen(engine_glx_instance.display);

  int glx_version = gladLoaderLoadGLX(engine_glx_instance.display, engine_glx_instance.screen);
  if (!glx_version) {
    engine_error("Unable to load GLX.");
    return false;
  }
  engine_log("Loaded GLX %d.%d", GLAD_VERSION_MAJOR(glx_version),
             GLAD_VERSION_MINOR(glx_version));

  Window root = RootWindow(engine_glx_instance.display, engine_glx_instance.screen);

  GLint visual_attributes[] = {GLX_RGBA, GLX_DOUBLEBUFFER, None};
  XVisualInfo *visual_info =
      glXChooseVisual(engine_glx_instance.display, engine_glx_instance.screen, visual_attributes);

  engine_glx_instance.colormap = XCreateColormap(engine_glx_instance.display, root, visual_info->visual, AllocNone);

  XSetWindowAttributes attributes;
  attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
  attributes.colormap = engine_glx_instance.colormap;

  engine_glx_instance.window = XCreateWindow(engine_glx_instance.display, root, 0, 0, engine_glx_instance.window_width, engine_glx_instance.window_height, 0,
                         visual_info->depth, InputOutput, visual_info->visual,
                         CWColormap | CWEventMask, &attributes);

  XMapWindow(engine_glx_instance.display, engine_glx_instance.window);
  XStoreName(engine_glx_instance.display, engine_glx_instance.window, engine_glx_instance.window_title);

  if (!engine_glx_instance.window) {
    engine_error("Unable to create window.");
    return false;
  }

  engine_glx_instance.context = glXCreateContext(engine_glx_instance.display, visual_info, NULL, GL_TRUE);
  glXMakeCurrent(engine_glx_instance.display, engine_glx_instance.window, engine_glx_instance.context);

  int gl_version = gladLoaderLoadGL();
  if (!gl_version) {
    engine_error("Unable to load GL.");
    return false;
  }
  engine_log("Loaded GL %d.%d", GLAD_VERSION_MAJOR(gl_version),
             GLAD_VERSION_MINOR(gl_version));

  XWindowAttributes gwa;
  XGetWindowAttributes(engine_glx_instance.display, engine_glx_instance.window, &gwa);
  glViewport(0, 0, gwa.width, gwa.height);
  glClearColor(0.2,0.3,0.4,1);
  return true;
}

void engine_glx_stop(void) {
  glXMakeCurrent(engine_glx_instance.display, 0, 0);
  glXDestroyContext(engine_glx_instance.display, engine_glx_instance.context);

  XDestroyWindow(engine_glx_instance.display, engine_glx_instance.window);
  XFreeColormap(engine_glx_instance.display, engine_glx_instance.colormap);
  XCloseDisplay(engine_glx_instance.display);

  gladLoaderUnloadGLX();
}

void engine_glx_update(void) {
  glXSwapBuffers(engine_glx_instance.display, engine_glx_instance.window);
  while (XPending(engine_glx_instance.display)) {
    XEvent xev;
    XNextEvent(engine_glx_instance.display, &xev);

    if (xev.type == KeyPress) {
      engine_glx_instance.engine_running = false;
    }
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif // ENGINE_GLX
