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

// Buffer para resposta HTTP
char http_response[1024];

// Estado dos botões (inicialmente sem mensagens)
char button1_message[50] = "Nenhum evento no botão 1";
char button2_message[50] = "Nenhum evento no botão 2";

// Função para monitorar o estado dos botões
void monitor_buttons() {
    static bool button1_last_state = false;
    static bool button2_last_state = false;

    bool button1_state = !gpio_get(BUTTON1_PIN); // Botão pressionado = LOW
    bool button2_state = !gpio_get(BUTTON2_PIN);

    if (button1_state != button1_last_state) {
        button1_last_state = button1_state;
        if (button1_state) {
            snprintf(button1_message, sizeof(button1_message), "Botão 1 foi pressionado!");
            printf("Botão 1 pressionado\n");
        } else {
            snprintf(button1_message, sizeof(button1_message), "Botão 1 foi solto!");
            printf("Botão 1 solto\n");
        }
    }

    if (button2_state != button2_last_state) {
        button2_last_state = button2_state;
        if (button2_state) {
            snprintf(button2_message, sizeof(button2_message), "Botão 2 foi pressionado!");
            printf("Botão 2 pressionado\n");
        } else {
            snprintf(button2_message, sizeof(button2_message), "Botão 2 foi solto!");
            printf("Botão 2 solto\n");
        }
    }
}


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

// Função para enviar uma requisição HTTP PUT
void send_toggle(bool* is_on) {
    // Tamanho para a URL
    char url[256];  
    snprintf(url, sizeof(url), "http://%s:%s%s", HTTP_SERVER_IP, HTTP_SERVER_PORT, HTTP_POWER);
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
        *is_on ? "true" : "false");
    add_header(client, "Content-Type", "application/json");
    printf("Corpo da requisição: %s\n", json_body);
    add_post(client, json_body);
    http_response_t response = http_request(3, client);

    // Verifica a resposta do servidor
    if (response.code >= 200 && response.code < 300) 
        printf("Requisição PUT bem-sucedida! Código de status: %d\n", response.code);
    else 
        printf("Erro na requisição PUT. Código de status: %d\n", response.code);
        
    // Libera os recursos alocados para o cliente HTTP
    free_http_client(client);
    *is_on = !(*is_on);
    return;
}

bool watch_buttons(int BUTTON_PIN){
    if (gpio_get(BUTTON_PIN) == 0) {
        // send_http_toggle(is_bulb_on);
        // is_bulb_on = !is_bulb_on;
        printf("Botão %i pressionado. Estado da lâmpada: \n", BUTTON_PIN);
        while (gpio_get(BUTTON_PIN) == 0)
        {
            sleep_ms(50);
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




