#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double zoom;
} ViewPort;

typedef struct {
    int is_dragging;
    int start_x;
    int start_y;
    ViewPort original_view;
} MouseState;

typedef struct {
    double real;
    double imag;
} Complex;

void handle_mouse(SDL_Event event, MouseState* mouse, ViewPort* view);
Complex get_complex_from_mouse(int x, int y, ViewPort view, int width, int height);

#endif 