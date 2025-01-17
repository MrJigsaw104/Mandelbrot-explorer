#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "mouse_handler.h"

#define MAX_ITERATIONS 150

typedef struct {
    SDL_Rect reset_button;
    SDL_Rect julia_preview_button;
    SDL_Rect julia_preview_window;
    SDL_Rect zoom_display;
    int show_julia_preview;
    SDL_Texture* preview_texture;
    TTF_Font* font;
} UI;

void init_ui(UI* ui, SDL_Renderer* renderer);
void render_ui(UI* ui, SDL_Renderer* renderer, ViewPort view, Complex julia_c, int is_julia);
int handle_ui_event(UI* ui, SDL_Event event, ViewPort* view);
void cleanup_ui(UI* ui);

#endif 