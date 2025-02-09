#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "colors.h"

bool verify_connection_wifi();
void send_colors_toggle(int* count, Color* current_color);
bool watch_buttons(int BUTTON_PIN);
void send_turn(bool state);
int adc_to_color(int valor);
#endif // FUNCTIONS_H