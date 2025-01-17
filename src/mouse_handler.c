#include "mouse_handler.h"

void handle_mouse(SDL_Event event, MouseState* mouse, ViewPort* view) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse->is_dragging = 1;
                mouse->start_x = event.button.x;
                mouse->start_y = event.button.y;
                mouse->original_view = *view;
            }
            break;
            
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse->is_dragging = 0;
            }
            break;
            
        case SDL_MOUSEWHEEL:
            {
                // Mouse pozisyonunu al
                int x, y;
                SDL_GetMouseState(&x, &y);
                
                // Zoom öncesi mouse'un kompleks düzlemdeki konumu
                double mouse_real = view->x_min + (x * (view->x_max - view->x_min)) / WINDOW_WIDTH;
                double mouse_imag = view->y_min + (y * (view->y_max - view->y_min)) / WINDOW_HEIGHT;
                
                // Zoom faktörü (yukarı = yakınlaştır, aşağı = uzaklaştır)
                double zoom_factor = event.wheel.y > 0 ? 0.9 : 1.1;
                
                // Yeni görünüm aralıklarını hesapla
                double new_width = (view->x_max - view->x_min) * zoom_factor;
                double new_height = (view->y_max - view->y_min) * zoom_factor;
                
                // Mouse pozisyonunu merkez alarak zoom yap
                view->x_min = mouse_real - (mouse_real - view->x_min) * zoom_factor;
                view->x_max = mouse_real + (view->x_max - mouse_real) * zoom_factor;
                view->y_min = mouse_imag - (mouse_imag - view->y_min) * zoom_factor;
                view->y_max = mouse_imag + (view->y_max - mouse_imag) * zoom_factor;
                
                // Zoom seviyesini güncelle
                view->zoom *= (1.0 / zoom_factor);
            }
            break;
            
        case SDL_MOUSEMOTION:
            if (mouse->is_dragging) {
                int dx = event.motion.x - mouse->start_x;
                int dy = event.motion.y - mouse->start_y;
                
                double scale_x = (mouse->original_view.x_max - mouse->original_view.x_min) / WINDOW_WIDTH;
                double scale_y = (mouse->original_view.y_max - mouse->original_view.y_min) / WINDOW_HEIGHT;
                
                view->x_min = mouse->original_view.x_min - dx * scale_x;
                view->x_max = mouse->original_view.x_max - dx * scale_x;
                view->y_min = mouse->original_view.y_min - dy * scale_y;
                view->y_max = mouse->original_view.y_max - dy * scale_y;
            }
            break;
    }
}

Complex get_complex_from_mouse(int x, int y, ViewPort view, int width, int height) {
    Complex c;
    c.real = view.x_min + (x * (view.x_max - view.x_min)) / width;
    c.imag = view.y_min + (y * (view.y_max - view.y_min)) / height;
    return c;
} 