#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <string.h>
#include <stdio.h>
#include "pico_http_client/pico_http_client.h"
#include "config.h"
#include "functions.h"
#include "include/ssd1306.h"
#include "colors.c"
#include "hardware/pwm.h"
#include "hardware/adc.h"

uint slice_r, slice_g, slice_b;

void start_display(){
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

  // Desenha um bitmap no display
  ssd1306_t ssd_bm;
  ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
  ssd1306_config(&ssd_bm);
  uint8_t bitmap_128x64[] = {
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
  ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
}

void set_peripherals(){
  // Configura o display OLED
  i2c_init(i2c1, ssd1306_i2c_clock * 4000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);


  // Inicializa os LEDS
  gpio_set_function(LED_RED, GPIO_FUNC_PWM);
  gpio_set_function(LED_GREEN, GPIO_FUNC_PWM);
  gpio_set_function(LED_BLUE, GPIO_FUNC_PWM);


  // Configura os botões
  gpio_init(BUTTON1_PIN);
  gpio_set_dir(BUTTON1_PIN, GPIO_IN);
  gpio_pull_up(BUTTON1_PIN);

  gpio_init(BUTTON2_PIN);
  gpio_set_dir(BUTTON2_PIN, GPIO_IN);
  gpio_pull_up(BUTTON2_PIN);

  gpio_init(BUTTONSTICK_PIN);
  gpio_set_dir(BUTTONSTICK_PIN, GPIO_IN);
  gpio_pull_up(BUTTONSTICK_PIN);

  // Configura o JoyStick
  adc_gpio_init(26);
  adc_gpio_init(27);

  // Inicializa o Wi-Fi
  if (cyw43_arch_init()) {
    printf("Erro ao inicializar o Wi-Fi\n");
  }
  cyw43_arch_enable_sta_mode();
  printf("Conectando ao Wi-Fi...\n");
}

void setup_pwm() {
    gpio_set_function(LED_RED, GPIO_FUNC_PWM);
    gpio_set_function(LED_GREEN, GPIO_FUNC_PWM);
    gpio_set_function(LED_BLUE, GPIO_FUNC_PWM);

    slice_r = pwm_gpio_to_slice_num(LED_RED);
    slice_g = pwm_gpio_to_slice_num(LED_GREEN);
    slice_b = pwm_gpio_to_slice_num(LED_BLUE);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);

    pwm_init(slice_r, &config, true);
    pwm_init(slice_g, &config, true);
    pwm_init(slice_b, &config, true);
}

void set_led_color(Color current_color) {
    pwm_set_gpio_level(LED_RED, current_color.red * 100);
    pwm_set_gpio_level(LED_GREEN, current_color.green* 100);
    pwm_set_gpio_level(LED_BLUE, current_color.blue* 100);
}

void verify_connection(bool *is_conected, int* count_color, int is_bulb_on, bool* is_first_time){
    if(*is_conected == false){
      bool connection = verify_connection_wifi();
      if (connection){
        printf("Wi-Fi conectado!\n");
        *is_conected = true;
      }
      else{
        printf("Erro ao conectar ao Wi-Fi...\n");
        sleep_ms(1000);
      }
    }
    return;
}

int main() {
  //Setup
  adc_init();
  stdio_init_all();  
  set_peripherals();
  start_display();
  setup_pwm();

  //Initial Variables
  bool is_conected = false;
  bool is_bulb_on = true;
  bool is_first_time = true;
  int current_bright = 1;
  int count_color = 0;
  Color current_color = {255,255,255};
  Color last_color = {255,255,255};
  set_led_color(current_color);

  // Loop principal
  while (true) {
    verify_connection(&is_conected, &count_color, is_bulb_on, &is_first_time);

    // Mantem o Wi-Fi ativo
    cyw43_arch_poll();  
    
    // Controle quando a Lampada está Ligada
    if (is_bulb_on){
      // Desligar a lampada
      if (watch_buttons(BUTTON1_PIN)){
        printf("\nBotão 1 pressionado.");
        is_bulb_on = false;
        send_turn(is_bulb_on);
        set_led_color((Color){0,0,0});
        count_color = 0;
        send_colors_toggle(&count_color, &current_color);
        count_color--;
      }
      // Trocar cor
      if (watch_buttons(BUTTON2_PIN)){
        send_colors_toggle(&count_color, &current_color);
        printf("%i, %i, %i", current_color.red, current_color.green, current_color.blue);
      }
    }
    // Mudar a cor da placa para a cor da lampada
    if (!(last_color.blue == current_color.blue && last_color.green == current_color.green && last_color.red == current_color.red)){
      last_color = current_color;
      set_led_color(current_color);
      printf("\nChanging  Color on board.");
    }
    // Controle de estado quando a lampada está desligada
    if(!is_bulb_on){
      // Selecionar cor customizada
      if ((watch_buttons(BUTTONSTICK_PIN))){
        bool select = true;
        printf("\nSetting color.");
        Color color = {0,0,0};
        int color_choice = 0;
        
        while (select){
          while(color_choice == 0){
            if (watch_buttons(BUTTON1_PIN)){
              printf("Red");
              adc_select_input(1);
              uint adc_y_raw = adc_read();
              printf("%d\n",adc_y_raw);
              color.red = adc_to_color(adc_y_raw);
              set_led_color(color);
              color_choice++;
            }
          }
          while(color_choice == 1){
            if (watch_buttons(BUTTON1_PIN) && (color_choice == 1)){
              adc_select_input(1);
              printf("Green");
              uint adc_y_raw = adc_read();
              printf("%d",adc_y_raw);
              color.green = adc_to_color(adc_y_raw);
              set_led_color(color);
              color_choice++;
            }
          }
          while(color_choice == 2){
            if (watch_buttons(BUTTON1_PIN) && (color_choice == 2)){
              adc_select_input(1);
              printf("Blue");
              uint adc_y_raw = adc_read();
              printf("%d",adc_y_raw);
              color.blue = adc_to_color(adc_y_raw);
              set_led_color(color);
              color_choice++;
            }
          }
          if (watch_buttons(BUTTON2_PIN)){
            select = false;
            count_color = -1;
            is_bulb_on = true;
            send_turn(is_bulb_on);
            current_color = color;
            last_color = color;
            send_colors_toggle(&count_color, &color);
          }
        }
      }
      // Ligar Lampada
      if (watch_buttons(BUTTON1_PIN)){
        send_turn(&count_color);
        is_bulb_on = true;
        current_color = (Color){255,255,255};
        send_turn(is_bulb_on);
        set_led_color(current_color);
      }
      }
    }
  cyw43_arch_deinit(); 
  return 0;
}
