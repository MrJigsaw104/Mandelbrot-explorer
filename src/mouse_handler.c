#include "ui.h"
#include <stdio.h>
#include <math.h>
#include "mandelbrot.h"

#define BUTTON_WIDTH 120
#define BUTTON_HEIGHT 30
#define PREVIEW_SIZE 200
#define UI_PADDING 10
#define UI_ALPHA 200
#define FONT_SIZE 16

void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect* rect) {
    SDL_Color color = {200, 200, 200, UI_ALPHA};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_Rect text_rect = {
        rect->x + (rect->w - surface->w) / 2,
        rect->y + (rect->h - surface->h) / 2,
        surface->w,
        surface->h
    };
    
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_julia_preview(UI* ui, SDL_Renderer* renderer, ViewPort view, Complex julia_c) {

    ViewPort preview_view = {
        .x_min = -1.5,
        .x_max = 1.5,
        .y_min = -1.5,
        .y_max = 1.5,
        .zoom = 1.0
    };

    SDL_SetRenderTarget(renderer, ui->preview_texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    for (int x = 0; x < PREVIEW_SIZE; x++) {
        for (int y = 0; y < PREVIEW_SIZE; y++) {
            double real = preview_view.x_min + (x * (preview_view.x_max - preview_view.x_min)) / PREVIEW_SIZE;
            double imag = preview_view.y_min + (y * (preview_view.y_max - preview_view.y_min)) / PREVIEW_SIZE;
            Complex z = {real, imag};
            
            int iterations = julia(z, julia_c);
            
            if (iterations == MAX_ITERATIONS) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                double t = (double)iterations / MAX_ITERATIONS;
                t = 0.5 + 0.5 * cos(log(t + 0.0001) * 3.0);
                
                int r = (int)(255 * t);
                int g = (int)(255 * t);
                int b = (int)(128 + (1.0 - t) * 127);
                
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    SDL_SetRenderTarget(renderer, NULL);
}

void init_ui(UI* ui, SDL_Renderer* renderer) {
    ui->reset_button = (SDL_Rect){UI_PADDING, UI_PADDING, BUTTON_WIDTH, BUTTON_HEIGHT};
    
    ui->julia_preview_button = (SDL_Rect){UI_PADDING, UI_PADDING * 2 + BUTTON_HEIGHT, 
                                        BUTTON_WIDTH, BUTTON_HEIGHT};
    
    ui->julia_preview_window = (SDL_Rect){WINDOW_WIDTH - PREVIEW_SIZE - UI_PADDING,
                                        UI_PADDING, PREVIEW_SIZE, PREVIEW_SIZE};
    
    ui->zoom_display = (SDL_Rect){UI_PADDING, UI_PADDING * 3 + BUTTON_HEIGHT * 2,
                                 BUTTON_WIDTH, BUTTON_HEIGHT};
    
    ui->show_julia_preview = 0;
    
    ui->preview_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                          SDL_TEXTUREACCESS_TARGET, PREVIEW_SIZE, PREVIEW_SIZE);
    
    TTF_Init();
    ui->font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", FONT_SIZE);
    if (!ui->font) {
        printf("Font yüklenemedi: %s\n", TTF_GetError());
    }
}

void render_ui(UI* ui, SDL_Renderer* renderer, ViewPort view, Complex julia_c, int is_julia) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, UI_ALPHA);
    SDL_RenderFillRect(renderer, &ui->reset_button);
    SDL_RenderFillRect(renderer, &ui->julia_preview_button);
    SDL_RenderFillRect(renderer, &ui->zoom_display);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, UI_ALPHA);
    SDL_RenderDrawRect(renderer, &ui->reset_button);
    SDL_RenderDrawRect(renderer, &ui->julia_preview_button);
    SDL_RenderDrawRect(renderer, &ui->zoom_display);
    
    if (ui->font) {
        render_text(renderer, ui->font, "Reset", &ui->reset_button);
        render_text(renderer, ui->font, "Julia Preview", &ui->julia_preview_button);
        
        char zoom_text[32];
        snprintf(zoom_text, sizeof(zoom_text), "Zoom: %.2fx", view.zoom);
        render_text(renderer, ui->font, zoom_text, &ui->zoom_display);
    }
    
    if (ui->show_julia_preview && !is_julia) {
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, UI_ALPHA);
        SDL_RenderFillRect(renderer, &ui->julia_preview_window);
        SDL_RenderDrawRect(renderer, &ui->julia_preview_window);
        
        render_julia_preview(ui, renderer, view, julia_c);
        SDL_RenderCopy(renderer, ui->preview_texture, NULL, &ui->julia_preview_window);
    }
}

int handle_ui_event(UI* ui, SDL_Event event, ViewPort* view) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x;
        int y = event.button.y;
        
        if (SDL_PointInRect(&(SDL_Point){x, y}, &ui->reset_button)) {
            view->x_min = -2.0;
            view->x_max = 1.0;
            view->y_min = -1.5;
            view->y_max = 1.5;
            view->zoom = 1.0;
            return 1;
        }
        
        if (SDL_PointInRect(&(SDL_Point){x, y}, &ui->julia_preview_button)) {
            ui->show_julia_preview = !ui->show_julia_preview;
            return 1;
        }
    }
    return 0;
}

void cleanup_ui(UI* ui) {
    if (ui->preview_texture) {
        SDL_DestroyTexture(ui->preview_texture);
    }
    if (ui->font) {
        TTF_CloseFont(ui->font);
    }
    TTF_Quit();
} 
