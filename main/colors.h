#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

// Definição da estrutura de cor
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

extern const Color COLORS[];

// // Definição das cores
// const Color COLORS[] = {
//     {255, 255, 255}, // WHITE
//     {255, 0, 0},     // RED
//     {0, 255, 0},     // GREEN
//     {0, 0, 255},     // BLUE
//     {255, 0, 128},   // ROSE
//     {255, 0, 255},   // MAGENTA
//     {128, 0, 255},   // VIOLET
//     {0, 128, 255},   // AZURE
//     {0, 255, 255},   // CYAN
//     {0, 255, 128},   // SPRING_GREEN
//     {128, 255, 0},   // CHARTREUSE
//     {255, 255, 0},   // YELLOW
//     {255, 128, 0}    // ORANGE
// };

typedef enum {
    WHITE,
    RED,
    GREEN,
    BLUE,
    ROSE,
    MAGENTA,
    VIOLET,
    AZURE,
    CYAN,
    SPRING_GREEN,
    CHARTREUSE,
    YELLOW,
    ORANGE,
    COLOR_COUNT 
} ColorName;

#endif // COLORS_H