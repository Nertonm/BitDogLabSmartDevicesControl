#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include "functions.h"
#include "config.h"
#include "pico_http_client/pico_http_client.h"
#include "pico_http_client/socket.c"

#define LED_PIN 12

// Estado dos botões (inicialmente sem mensagens)
char button1_message[50] = "Nenhum evento no botão 1";
char button2_message[50] = "Nenhum evento no botão 2";

// Buffer para resposta HTTP
char http_response[1024];

// Função para criar a resposta HTTP
void create_http_response() {
    snprintf(http_response, sizeof(http_response),
             "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
             "<!DOCTYPE html>"
             "<html>"
             "<head>"
             "  <meta charset=\"UTF-8\">"
             "  <title>Controle do LED e Botões</title>"
             "</head>"
             "<body>"
             "  <h1>Controle do LED e Botões</h1>"
             "  <p><a href=\"/led/on\">Ligar LED</a></p>"
             "  <p><a href=\"/led/off\">Desligar LED</a></p>"
             "  <p><a href=\"/update\">Atualizar Estado</a></p>"
             "  <h2>Estado dos Botões:</h2>"
             "  <p>Botão 1: %s</p>"
             "  <p>Botão 2: %s</p>"
             "</body>"
             "</html>\r\n",
             button1_message, button2_message);
}

// Função de callback para processar requisições HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {
        // Cliente fechou a conexão
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Processa a requisição HTTP
    char *request = (char *)p->payload;

    if (strstr(request, "GET /led/on")) {
        gpio_put(LED_PIN, 1);  // Liga o LED
    } else if (strstr(request, "GET /led/off")) {
        gpio_put(LED_PIN, 0);  // Desliga o LED
    }

    // Atualiza o conteúdo da página com base no estado dos botões
    create_http_response();

    // Envia a resposta HTTP
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

// Callback de conexão: associa o http_callback à conexão
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
    return ERR_OK;
}

// Função de setup do servidor TCP
void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    // Liga o servidor na porta 80
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao ligar o servidor na porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);  // Coloca o PCB em modo de escuta
    tcp_accept(pcb, connection_callback);  // Associa o callback de conexão

    printf("Servidor HTTP rodando na porta 80...\n");
}

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
void send_http_toggle(bool is_on) {
    char url[256];  // Tamanho para a URL
    snprintf(url, sizeof(url), "http://%s:%s%s", HTTP_SERVER_IP, HTTP_SERVER_PORT, HTTP_POWER);
    printf("URL: %s\n", url);
    http_client_t *client = new_http_client(url);
    if (!client) {
        printf("Erro ao criar o cliente HTTP\n");
        return;
    }
    const char *json_body = 
        "{"
        "  \"power\": true,"
        "  \"toggles\": ["
        "    { \"name\": \"White Lamp\", \"toggle\": true },"
        "    { \"name\": \"Black Lamp\", \"toggle\": false }"
        "  ]"
        "}";
    if (is_on == true){
        json_body = 
        "{"
        "  \"power\": false,"
        "  \"toggles\": ["
        "    { \"name\": \"White Lamp\", \"toggle\": true },"
        "    { \"name\": \"Black Lamp\", \"toggle\": false }"
        "  ]"
        "}";
    }
    add_header(client, "Content-Type", "application/json");
    printf("Corpo da requisição: %s\n", json_body);
    add_post(client, json_body);
    http_response_t response = http_request(3, client);

    // Verifica a resposta do servidor
    if (response.code >= 200 && response.code < 300) {
        printf("Requisição PUT bem-sucedida! Código de status: %d\n", response.code);
    } else {
        printf("Erro na requisição PUT. Código de status: %d\n", response.code);
    }

    // Libera os recursos alocados para o cliente HTTP
    free_http_client(client);
    return;
}

void turn_on_led(){
    // Criar uma string que contém a URL completa
    char url[256];  // Tamanho adequado para a URL
    snprintf(url, sizeof(url), "%s:%s%s", HTTP_SERVER_IP, HTTP_SERVER_PORT, HTTP_POWER);
    printf("URL: %s\n", url);
    http_client_t *client = new_http_client(url);
     const char *json_body = 
    "{"
    "  \"power\": true,"
    "  \"toggles\": ["
    "    { \"name\": \"White Lamp\", \"toggle\": true },"
    "    { \"name\": \"Black Lamp\", \"toggle\": false }"
    "  ]"
    "}";
  // Adiciona o cabeçalho Content-Type
    add_header(client, "Content-Type", "application/json");
    printf(json_body);
    // Adiciona o corpo da requisição
    add_post(client, json_body);

    // Realiza a requisição PUT
    http_response_t response = http_request(3, client);

    // Verifica a resposta do servidor
    if (response.code == 200) {
        printf("Requisição PUT bem-sucedida!\n");
    } else {
        printf("Erro na requisição PUT. Código de status: %d\n", response.code);
    }

    // Libera os recursos alocados para o cliente HTTP
    free_http_client(client);
}



