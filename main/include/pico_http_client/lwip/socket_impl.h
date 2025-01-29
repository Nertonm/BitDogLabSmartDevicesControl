#ifndef INKYHOMEASSITANT_SOCKET_IMPL_H
#define INKYHOMEASSITANT_SOCKET_IMPL_H

#include <stdlib.h>

#include <lwip/tcp.h>
#include <lwip/dns.h>
#include <pico/cyw43_arch.h>

#ifndef sleep
#define sleep(us) cyw43_arch_wait_for_work_until(make_timeout_time_us(us))
#endif

typedef struct tcp_client {
    struct tcp_pcb *pcb;
    void *data;
    uint32_t data_size;
    void *rx_buffer;
    int rx_buffer_size;
} tcp_client_t;

tcp_client_t *pico_new_tcp_client();

void pico_free_tcp_client(tcp_client_t *);

int pico_tcp_connect(tcp_client_t *tcp_client, const char *host, const char *port);

int pico_is_tcp_connected(tcp_client_t *tcp_client);

int pico_tcp_poll(tcp_client_t *tcp_client, int timeout_us);

int pico_tcp_write(tcp_client_t *tcp_client, void *data, size_t size);

int pico_tcp_read(tcp_client_t *tcp_client);

#endif 
