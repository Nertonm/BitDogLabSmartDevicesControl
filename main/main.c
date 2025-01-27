#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>
#include "pico_http_client/pico_http_client.h"
#include "config.h"
#include "functions.h"

int main() {
    stdio_init_all();  // Inicializa a saída padrão
    sleep_ms(10000);
    printf("Iniciando servidor HTTP\n");

    // Inicializa o Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();
    printf("Conectando ao Wi-Fi...\n");

    // Configura o LED e os botões
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON1_PIN);
    gpio_set_dir(BUTTON1_PIN, GPIO_IN);
    gpio_pull_up(BUTTON1_PIN);

    gpio_init(BUTTON2_PIN);
    gpio_set_dir(BUTTON2_PIN, GPIO_IN);
    gpio_pull_up(BUTTON2_PIN);

    printf("Botões configurados com pull-up nos pinos %d e %d\n", BUTTON1_PIN, BUTTON2_PIN);

    bool is_conected = false;
    
    // Inicia o servidor HTTP
    //start_http_server();

    // Loop principal
    while (true) {
        while(is_conected == false){
            bool connection = verify_connection_wifi();
            if (connection){
                printf("Wi-Fi conectado!\n");
                is_conected = true;
            }
        }
        send_http_put_request();

        cyw43_arch_poll();  // Necessário para manter o Wi-Fi ativo
        sleep_ms(5000);
    }

    cyw43_arch_deinit();  // Desliga o Wi-Fi (não será chamado, pois o loop é infinito)
    return 0;
}
