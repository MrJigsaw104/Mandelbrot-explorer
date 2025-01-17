#include <complex.h>
#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include "mouse_handler.h"
#include "ui.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_ITERATIONS 150

int mandelbrot(Complex c) {
    Complex z = {0.0, 0.0};
    int i;
    
    for (i = 0; i < MAX_ITERATIONS; i++) {
        // z = z * z + c
        double temp_real = z.real * z.real - z.imag * z.imag + c.real;
        double temp_imag = 2 * z.real * z.imag + c.imag;
        z.real = temp_real;
        z.imag = temp_imag;
        
        double abs_z = hypot(z.real, z.imag);
        if (abs_z > 2)
            return i;
    }
    return MAX_ITERATIONS;
}

int julia(Complex z, Complex c) {
    int i;
    for (i = 0; i < MAX_ITERATIONS; i++) {
        // z = z * z + c
        double temp_real = z.real * z.real - z.imag * z.imag + c.real;
        double temp_imag = 2 * z.real * z.imag + c.imag;
        z.real = temp_real;
        z.imag = temp_imag;
        
        double abs_z = hypot(z.real, z.imag);
        if (abs_z > 2)
            return i;
    }
    return MAX_ITERATIONS;
}

void render(SDL_Renderer* renderer, ViewPort view, int is_julia, Complex julia_c) {
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
    SDL_RenderClear(renderer);
    
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            double real = view.x_min + (x * (view.x_max - view.x_min)) / WINDOW_WIDTH;
            double imag = view.y_min + (y * (view.y_max - view.y_min)) / WINDOW_HEIGHT;
            Complex c = {real, imag};
            
            int iterations;
            if (is_julia)
                iterations = julia(c, julia_c);
            else
                iterations = mandelbrot(c);
            
            if (iterations == MAX_ITERATIONS) {
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else {
                
                double t = (double)iterations / MAX_ITERATIONS;
                t = 0.5 + 0.5 * cos(log(t + 0.0001) * 3.0);
                
                
                int r = (int)(255 * t * 0.2);  
                int g = (int)(255 * t * 0.4);  
                int b = (int)(255 * t);        
                
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Mandelbrot/Julia Explorer", 
                            SDL_WINDOWPOS_UNDEFINED, 
                            SDL_WINDOWPOS_UNDEFINED, 
                            WINDOW_WIDTH, WINDOW_HEIGHT, 
                            SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    ViewPort view = {
        .x_min = -2.0,
        .x_max = 1.0,
        .y_min = -1.5,
        .y_max = 1.5,
        .zoom = 1.0
    };
    
    MouseState mouse = {
        .is_dragging = 0,
        .start_x = 0,
        .start_y = 0
    };
    
    int quit = 0;
    int is_julia = 0;
    Complex julia_c = {0, 0};
    
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
    Uint32 frame_start;
    int frame_time;
    
    UI ui;
    init_ui(&ui, renderer);
    
    while (!quit) {
        frame_start = SDL_GetTicks();
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            if (handle_ui_event(&ui, event, &view)) {
                continue;
            }
            
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE)
                        is_julia = !is_julia;
                    break;
                default:
                    handle_mouse(event, &mouse, &view);
                    break;
            }
            
            if (!mouse.is_dragging) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                julia_c = get_complex_from_mouse(x, y, view, WINDOW_WIDTH, WINDOW_HEIGHT);
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);  
        SDL_RenderClear(renderer);
        
        render(renderer, view, is_julia, julia_c);
        render_ui(&ui, renderer, view, julia_c, is_julia);
        SDL_RenderPresent(renderer);  
        
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
    
    cleanup_ui(&ui);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
} 
