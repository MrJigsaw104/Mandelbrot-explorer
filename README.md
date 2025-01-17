# Mandelbrot-explorer
Mandelbrot set explorer in C using SDL2 library

# Features

- Real time julia set preview
- Mandelbrot explorer
- A tool that shows how much you have zoomed in the mandelbrot set

# Dependencies

- gcc
- SDL2 library
- SDL2_ttf library (for ui texts)

# Installation

- For Linux: `gcc src/*.c -o mandelbrot -lSDL2 -lSDL2_ttf -lm`

- For Windows: `gcc src/*.c -o mandelbrot.exe -I./include -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lm` (dont forget to install gcc for windows)
