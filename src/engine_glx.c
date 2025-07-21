#include "engine.h"

#ifdef ENGINE_GLX

struct engine_window {
  char *window_title;
  bool engine_is_running;
  int window_width, window_height;
  Display *display;
  int screen;
  GLXContext context;
  Window window;
  Colormap colormap;
};

float engine_glx_get_aspect_ratio(void) {
  return (float)engine_window_instance.window_width /
         engine_window_instance.window_height;
}

bool engine_glx_is_running(void) {
  return engine_window_instance.engine_is_running;
}

float (*engine_get_aspect_ratio)(void) = engine_glx_get_aspect_ratio;
bool (*engine_is_running)(void) = engine_glx_is_running;

struct engine_window engine_window_instance = {
    .window_title = "Game Window",
    .engine_is_running = true,
    .window_width = 640,
    .window_height = 480,
};

bool engine_glx_start(void) {
  engine_window_instance.display = XOpenDisplay(NULL);
  if (engine_window_instance.display == NULL) {
    engine_error("cannot connect to X server");
    return false;
  }

  engine_window_instance.screen = DefaultScreen(engine_window_instance.display);

  int glx_version = gladLoaderLoadGLX(engine_window_instance.display,
                                      engine_window_instance.screen);
  if (!glx_version) {
    engine_error("Unable to load GLX.");
    return false;
  }
  engine_log("Loaded GLX %d.%d", GLAD_VERSION_MAJOR(glx_version),
             GLAD_VERSION_MINOR(glx_version));

  Window root =
      RootWindow(engine_window_instance.display, engine_window_instance.screen);

  GLint visual_attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER,
                               None};
  XVisualInfo *visual_info =
      glXChooseVisual(engine_window_instance.display,
                      engine_window_instance.screen, visual_attributes);

  engine_window_instance.colormap = XCreateColormap(
      engine_window_instance.display, root, visual_info->visual, AllocNone);

  XSetWindowAttributes attributes;
  attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
  attributes.colormap = engine_window_instance.colormap;

  engine_window_instance.window = XCreateWindow(
      engine_window_instance.display, root, 0, 0,
      engine_window_instance.window_width, engine_window_instance.window_height,
      0, visual_info->depth, InputOutput, visual_info->visual,
      CWColormap | CWEventMask, &attributes);

  XMapWindow(engine_window_instance.display, engine_window_instance.window);
  XStoreName(engine_window_instance.display, engine_window_instance.window,
             engine_window_instance.window_title);

  if (!engine_window_instance.window) {
    engine_error("Unable to create window.");
    return false;
  }

  engine_window_instance.context = glXCreateContext(
      engine_window_instance.display, visual_info, NULL, GL_TRUE);
  glXMakeCurrent(engine_window_instance.display, engine_window_instance.window,
                 engine_window_instance.context);

  int gl_version = gladLoaderLoadGL();
  if (!gl_version) {
    engine_error("Unable to load GL.");
    return false;
  }
  engine_log("Loaded GL %d.%d", GLAD_VERSION_MAJOR(gl_version),
             GLAD_VERSION_MINOR(gl_version));

  XWindowAttributes gwa;
  XGetWindowAttributes(engine_window_instance.display,
                       engine_window_instance.window, &gwa);
  glViewport(0, 0, gwa.width, gwa.height);
  return true;
}

void engine_glx_stop(void) {
  glXMakeCurrent(engine_window_instance.display, 0, 0);
  glXDestroyContext(engine_window_instance.display,
                    engine_window_instance.context);

  XDestroyWindow(engine_window_instance.display, engine_window_instance.window);
  XFreeColormap(engine_window_instance.display,
                engine_window_instance.colormap);
  XCloseDisplay(engine_window_instance.display);

  gladLoaderUnloadGLX();
}

void engine_glx_update(void) {
  glXSwapBuffers(engine_window_instance.display, engine_window_instance.window);
  while (XPending(engine_window_instance.display)) {
    XEvent xev;
    XNextEvent(engine_window_instance.display, &xev);

    switch (xev.type) {
    case KeyPress: {
      engine_window_instance.engine_is_running = false;
    } break;
    case Expose: {
      engine_log("%d %d", engine_window_instance.window_width,
                 engine_window_instance.window_height);
      if (xev.xconfigure.width != engine_window_instance.window_width ||
          xev.xconfigure.height != engine_window_instance.window_height) {
        XWindowAttributes attribs;
        XGetWindowAttributes(engine_window_instance.display,
                             engine_window_instance.window, &attribs);
        glViewport(0, 0, attribs.width, attribs.height);
        engine_window_instance.window_width = attribs.width;
        engine_window_instance.window_height = attribs.height;
      }
    } break;
    default: {
      engine_log("unhandled XEvent of type: %d", xev.type);
    } break;
    }
  }
}

int main() {
  engine_glx_start();
  engine_scene_load();

  while (engine_is_running()) {
    engine_scene_update();
    engine_scene_draw();
    engine_glx_update();
  }

  engine_scene_unload();
  engine_glx_stop();
}

#endif // ENGINE_GLX
