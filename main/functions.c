#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>
#include "functions.h"
#include "config.h"
#include "include/ssd1306.h"
#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"
#include "pico_http_client/pico_http_client.h"
#include "pico_http_client/pico_http_client.c"
#include "pico_http_client/socket.c"
#include "colors.h"
#include <math.h> 

// Buffer para resposta HTTP
char http_response[1024];

// Estado dos botões (inicialmente sem mensagens)
char button1_message[50] = "Nenhum evento no botão 1";
char button2_message[50] = "Nenhum evento no botão 2";


bool verify_connection_wifi(){
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar ao Wi-Fi\n");
        return 1;
    }else {
        printf("Connected.\n");
        // Read the ip address in a human readable way
        uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
        printf("Endereço IP %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
    }
} 

bool watch_buttons(int BUTTON_PIN){
    if (gpio_get(BUTTON_PIN) == 0) {
        // send_http_toggle(is_bulb_on);
        // is_bulb_on = !is_bulb_on;
        printf("Botão %i pressionado.\n", BUTTON_PIN);
        while (gpio_get(BUTTON_PIN) == 0)
        {
            sleep_ms(20);
        }
        return true;

    }
    return false;
}

void send_colors_toggle(int* count, Color* current_color) {
    // Tamanho para a URL
    char url[256];  
    snprintf(url, sizeof(url), "http://%s:%s%s", HTTP_SERVER_IP, HTTP_SERVER_PORT, "/set_colour");
    printf("URL: %s\n", url);
    http_client_t *client = new_http_client(url);
    if (!client) {
        printf("Erro ao criar o cliente HTTP\n");
        return;
    }
    char json_body[512];
    if(count == -1){
        snprintf(json_body, sizeof(json_body),
        "{"
        "  \"red\": %d,"
        "  \"green\": %d,"
        "  \"blue\": %d,"
        "  \"toggles\": ["
        "    { \"name\": \"White Lamp\", \"toggle\": true, \"bright_mul\": 1.0 },"
        "    { \"name\": \"bulb2\", \"toggle\": false, \"bright_mul\": 1.0 }"
        "  ]"
        "}", (*current_color).red, (*current_color).green, (*current_color).blue
        );
    } else {
        snprintf(json_body, sizeof(json_body),
            "{"
            "  \"red\": %d,"
            "  \"green\": %d,"
            "  \"blue\": %d,"
            "  \"toggles\": ["
            "    { \"name\": \"White Lamp\", \"toggle\": true, \"bright_mul\": 1.0 },"
            "    { \"name\": \"bulb2\", \"toggle\": false, \"bright_mul\": 1.0 }"
            "  ]"
            "}", COLORS[*count].red, COLORS[*count].green, COLORS[*count].blue
            );
    }    
    add_header(client, "Content-Type", "application/json");
    printf("Corpo da requisição: %s\n", json_body);
    add_post(client, json_body);
    http_response_t response = http_request(3, client);
    // Verifica a resposta do servidor
    if (response.code >= 200 && response.code < 300) 
        printf("Requisição PUT bem-sucedida! Código de status: %d\n", response.code);
    else {
        printf("Erro na requisição PUT. Código de status: %d\n", response.code);
        *count--;
        if (*count == 0)
            *count++;
    }
    // Libera os recursos alocados para o cliente HTTP
    free_http_client(client);
    *current_color = COLORS[*count];
    if ((*count) > 13 )
        *count = -1;
    (*count)++;
    return;
}

void send_turn(bool state) {
    // Tamanho para a URL    
    char url[256];  
    snprintf(url, sizeof(url), "http://%s:%s%s", HTTP_SERVER_IP, HTTP_SERVER_PORT, "/set_power");
    printf("URL: %s\n", url);
    http_client_t *client = new_http_client(url);
    if (!client) {
        printf("Erro ao criar o cliente HTTP\n");
        return;
    }
    char json_body[512];
    snprintf(json_body, sizeof(json_body),
        "{"
        "  \"power\": %s,"
        "  \"toggles\": ["
        "    { \"name\": \"White Lamp\", \"toggle\": true },"
        "    { \"name\": \"Black Lamp\", \"toggle\": false }"
        "  ]"
        "}",
        (state) ? "true" : "false");
    add_header(client, "Content-Type", "application/json");
    printf("Corpo da requisição: %s\n", json_body);
    add_post(client, json_body);
    http_response_t response = http_request(3, client);
    // Verifica a resposta do servidor
    if (response.code >= 200 && response.code < 300) 
        printf("Requisição PUT bem-sucedida! Código de status: %d\n", response.code);
    else {
        printf("Erro na requisição PUT. Código de status: %d\n", response.code);
    }
    // Libera os recursos alocados para o cliente HTTP
    free_http_client(client);
    return;
}

int adc_to_color(int x) {
    return (int)round((255.0 * (x - 25)) / 4055);
    }
