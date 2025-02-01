#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "colors.h"

void start_display();
void start_http_server(void);
bool verify_connection_wifi();
void create_http_response(void);
void turn_on_led();
void send_toggle(bool*);
void send_colors_toggle(int* count, Color* current_color);
bool watch_buttons(int BUTTON_PIN);
void send_turn(int* count);
#endif // FUNCTIONS_H