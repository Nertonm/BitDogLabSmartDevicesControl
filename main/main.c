#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>
#include "pico_http_client/pico_http_client.h"
#include "config.h"
#include "functions.h"
#include "include/ssd1306.h"


int main() {
    stdio_init_all();  // Inicializa a saída padrão

    i2c_init(i2c1, ssd1306_i2c_clock * 10000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();
  // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
  struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
  };

  calculate_render_area_buffer_length(&frame_area);

  // zera o display inteiro
  uint8_t ssd[ssd1306_buffer_length];
  memset(ssd, 0, ssd1306_buffer_length);
  render_on_display(ssd, &frame_area);
//    char *text[] = {
//     "  Bem-vindos!   ",
//     "  Embarcatech   "};

//   int y = 0;
//   for (uint i = 0; i < count_of(text); i++) {
//     ssd1306_draw_string(ssd, 5, y, text[i]);
//     y += 8;
//   }
  render_on_display(ssd, &frame_area);
// */

    const uint8_t bitmap_128x64[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x7f, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xf0, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x7f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f,
   0xf8, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff,
   0x3f, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00,
   0xf0, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff,
   0x3f, 0x00, 0xff, 0x1f, 0xf0, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0x3f,
   0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x3f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f,
   0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf8, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0x3f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xfc,
   0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff, 0xff,
   0x81, 0xff, 0x03, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0x01, 0xf8,
   0xff, 0xff, 0xff, 0xff, 0x01, 0xfe, 0x01, 0xf0, 0xff, 0xff, 0xff, 0xff,
   0x01, 0xf8, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07, 0xf0, 0x60, 0xe0,
   0xff, 0xff, 0xff, 0xff, 0x0f, 0xc0, 0xe0, 0xc0, 0xff, 0xff, 0xff, 0xff,
   0x3f, 0x00, 0xf0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xf0, 0x81,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xf0, 0x83, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x03, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xf0, 0x07,
   0xfe, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf0, 0x07, 0xfe, 0xff, 0xff, 0xff,
   0xff, 0x7f, 0xf0, 0x0f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
   0xfc, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff,
   0x1f, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00,
   0xfc, 0xff, 0xff, 0xff, 0x1f, 0x00, 0xc0, 0x3f, 0xfc, 0xff, 0xff, 0xff,
   0x1f, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x1f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfe, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x3f, 0x18, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x10, 0xfe, 0xff,
   0xff, 0xe1, 0xc3, 0xff, 0x3f, 0x00, 0xfe, 0xff, 0xff, 0xe0, 0xc1, 0xff,
   0x3f, 0x00, 0xfc, 0xff, 0x7f, 0xe0, 0xc1, 0xff, 0x7f, 0x00, 0xfc, 0xff,
   0x7f, 0xe0, 0xc0, 0xff, 0xff, 0x00, 0x00, 0xff, 0x3f, 0x60, 0xc0, 0xff,
   0xff, 0x03, 0x00, 0xf0, 0x3f, 0x30, 0xe0, 0xff, 0xff, 0x0f, 0x00, 0x80,
   0x3f, 0x38, 0xe0, 0x07, 0xff, 0x3f, 0x00, 0x00, 0x3e, 0x38, 0xf0, 0x03,
   0xff, 0x3f, 0x1c, 0x00, 0x3e, 0x3c, 0xf8, 0x01, 0xff, 0x3f, 0x3c, 0x00,
   0x3e, 0x3c, 0xfc, 0x00, 0xff, 0x3f, 0x0c, 0x00, 0xfe, 0xff, 0xff, 0x80,
   0xff, 0x3f, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xc0, 0xff, 0x3f, 0x00, 0xe0,
   0xff, 0x07, 0xe0, 0xe0, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x03, 0x80, 0xf0,
   0xff, 0x07, 0x80, 0xff, 0xff, 0x00, 0x00, 0xf0, 0xff, 0x03, 0xe0, 0xff,
   0x7f, 0x00, 0x00, 0xfc, 0xff, 0x03, 0xf8, 0x0f, 0x00, 0x80, 0x01, 0x1c,
   0xff, 0x03, 0xf8, 0x0f, 0x00, 0xc0, 0x0f, 0x18, 0xff, 0x03, 0xf8, 0x0f,
   0x00, 0xf0, 0x1f, 0x18, 0xff, 0xc3, 0xff, 0x0f, 0x00, 0xf8, 0x3f, 0x10,
   0xff, 0xff, 0xff, 0x0f, 0x00, 0xfc, 0x3f, 0xf0, 0xff, 0xff, 0xff, 0x0f,
   0x06, 0xff, 0x7f, 0xf0, 0xf0, 0x7f, 0x00, 0x08, 0x06, 0xfe, 0x7f, 0xe0,
   0x00, 0x00, 0x00, 0x08, 0x06, 0x7e, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x08,
   0x0e, 0x06, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x18, 0x0c, 0x02, 0xf0, 0xc0,
   0x00, 0x00, 0x00, 0x18, 0x1c, 0x00, 0xf0, 0xc1, 0xff, 0x1f, 0x00, 0x1c,
   0x1c, 0x00, 0xf8, 0x01, 0xff, 0x07, 0xc0, 0x1f, 0x18, 0x80, 0xff, 0x01,
   0xff, 0x01, 0xf0, 0x3f, 0x10, 0xf8, 0xff, 0x01, 0x7f, 0x00, 0xfc, 0x3f,
   0x30, 0xf8, 0xff, 0x00, 0x7f, 0x00, 0xfc, 0x3f, 0x60, 0xf0, 0x1f, 0x40,
   0x7f, 0x00, 0x80, 0x7f, 0x60, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0xfc,
   0x40, 0x00, 0x00, 0xe0, 0xff, 0x03, 0x00, 0xe0, 0x40, 0x00, 0x00, 0xf8,
   0xff, 0x3f, 0x00, 0xc0, 0x41, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0f, 0xc0,
   0x01, 0xe0, 0xff, 0x01, 0xff, 0xff, 0x0f, 0xc0, 0x03, 0x60, 0x80, 0x01,
   0xff, 0xff, 0x01, 0xc0, 0x03, 0x60, 0x00, 0x00, 0xff, 0x7f, 0x00, 0xe0,
   0x03, 0x78, 0x00, 0x18, 0xff, 0x0f, 0x00, 0xfc, 0x07, 0x7e, 0x00, 0xf0,
   0xff, 0x01, 0x80, 0xff, 0x87, 0xff, 0x07, 0xf0, 0xff, 0x00, 0xf0, 0xff,
   0xff, 0xff, 0x3f, 0xf0, 0xff, 0x00, 0xfc, 0xff, 0xff, 0xff, 0x7f, 0xf0,
   0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x03, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xc0, 0xe1,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x80, 0xc1, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x00, 0x80, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x80, 0x81,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x83, 0x81, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xf0, 0x07, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0x80,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xc0, 0x07, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xf0,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x0e, 0xf8, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x01, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xff, 0xff, 0xff, 0xff 
};



  ssd1306_t ssd_bm;
  ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
  ssd1306_config(&ssd_bm);

  ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);

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
        send_http_put_request();

    // Loop principal
    while (true) {
        while(is_conected == false){
            bool connection = verify_connection_wifi();
            if (connection){
                printf("Wi-Fi conectado!\n");
                is_conected = true;
            }
        }

        cyw43_arch_poll();  // Necessário para manter o Wi-Fi ativo
        sleep_ms(5000);
    }

    cyw43_arch_deinit();  // Desliga o Wi-Fi (não será chamado, pois o loop é infinito)
    return 0;
}
