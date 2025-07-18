#include "engine.h"

Display *display;
int screen;
GLXContext context;
Window window;
Colormap colormap;

bool engine_start_glx(void) {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        engine_error("cannot connect to X server");
        return false;
    }

    screen = DefaultScreen(display);

    int glx_version = gladLoaderLoadGLX(display, screen);
    if (!glx_version) {
        engine_error("Unable to load GLX.");
        return false;
    }
    engine_log("Loaded GLX %d.%d", GLAD_VERSION_MAJOR(glx_version), GLAD_VERSION_MINOR(glx_version));

    Window root = RootWindow(display, screen);

    GLint visual_attributes[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
    XVisualInfo *visual_info = glXChooseVisual(display, screen, visual_attributes);

    colormap = XCreateColormap(display, root, visual_info->visual, AllocNone);

    XSetWindowAttributes attributes;
    attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
    attributes.colormap = colormap;

    window =
        XCreateWindow(display, root, 0, 0, window_width, window_height, 0,
                      visual_info->depth, InputOutput, visual_info->visual,
                      CWColormap | CWEventMask, &attributes);

    XMapWindow(display, window);
    XStoreName(display, window, window_title);

    if (!window) {
        engine_error("Unable to create window.");
        return false;
    }

    context = glXCreateContext(display, visual_info, NULL, GL_TRUE);
    glXMakeCurrent(display, window, context);

    int gl_version = gladLoaderLoadGL();
    if (!gl_version) {
        engine_error("Unable to load GL.");
        return false;
    }
    engine_log("Loaded GL %d.%d", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));

    XWindowAttributes gwa;
    XGetWindowAttributes(display, window, &gwa);
    glViewport(0, 0, gwa.width, gwa.height);
    return true;
}

void engine_stop_glx(void) {
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, context);

    XDestroyWindow(display, window);
    XFreeColormap(display, colormap);
    XCloseDisplay(display);

    gladLoaderUnloadGLX();
}

void engine_update_glx(void) {
    glXSwapBuffers(display, window);
    while (XPending(display)) {
      XEvent xev;
      XNextEvent(display, &xev);

      if (xev.type == KeyPress) {
        engine_running = true;
      }
    }
}
