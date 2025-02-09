# BitDogLab Smart Devices Control

Este projeto utiliza um Raspberry Pi Pico W para controlar LEDs RGB e interagir com um servidor via Wi-Fi. Ele responde a eventos de botões físicos para alterar o estado dos LEDs e enviar atualizações ao servidor.

## Funcionalidades

- **Conexão Wi-Fi**: Conecta-se a uma rede Wi-Fi e mantém a conexão ativa.
- **Controle de LEDs RGB**: Permite alterar as cores dos LEDs RGB.
- **Interação com Botões**: Responde a eventos de botões físicos para alterar o estado dos LEDs e enviar requisições HTTP.
- **Requisições HTTP**: Envia requisições HTTP PUT para um servidor para atualizar o estado dos dispositivos.

## Componentes

- **Raspberry Pi Pico W**
- **LEDs RGB**
- **Botões**
- **Joystick**
- **Conexão Wi-Fi**

## Configuração

### Hardware

1. Conecte os LEDs RGB aos pinos GPIO do Raspberry Pi Pico W.
2. Conecte os botões aos pinos GPIO do Raspberry Pi Pico W.
3. Conecte o joystick aos pinos GPIO do Raspberry Pi Pico W.

Ou utilize a placa bitdoglab

### Software

1. Clone este repositório:
    ```sh
    git clone https://github.com/seu-usuario/BitDogLabSmartDevicesControl.git
    cd BitDogLabSmartDevicesControl
    ```

2. Configure o arquivo [config.h](http://_vscodecontentref_/0) com as informações da sua rede Wi-Fi e do servidor:
    ```c
    #define WIFI_SSID "seu-ssid" // Substitua pelo SSID da sua rede Wi-Fi
    #define WIFI_PASS "sua-senha" // Substitua pela senha da sua rede Wi-Fi
    #define HTTP_SERVER_IP "192.168.0.5" // Substitua pelo IP do seu servidor
    #define HTTP_SERVER_PORT "8000"
    ```

3. Compile o projeto:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

4. Carregue o firmware no Raspberry Pi Pico W.

## Uso

### Inicialização

1. Conecte o Raspberry Pi Pico W à energia.
2. O dispositivo tentará se conectar à rede Wi-Fi configurada.
3. O LED RGB inicializará com a cor branca.

### Controle de LEDs

- **Botão 1**: Alterna o estado da lâmpada (liga/desliga).
- **Botão 2**: Troca a cor dos LEDs RGB.
- **Botão Stick**: Entra no modo de seleção de cor customizada.

### Seleção de Cor Customizada

1. Pressione o botão Stick com o led desligado para entrar no modo de seleção de cor.
2. Use o botão 1 para ajustar o componente vermelho (Red).
3. Use o botão 1 novamente para ajustar o componente verde (Green).
4. Use o botão 1 novamente para ajustar o componente azul (Blue).

### Requisições HTTP

O dispositivo envia requisições HTTP PUT para o servidor configurado para atualizar o estado dos dispositivos.

## Estrutura do Código

- **main.c**: Contém a lógica principal do programa.
- **functions.c**: Contém funções auxiliares para controle de LEDs, Wi-Fi e requisições HTTP.
- **config.h**: Contém as configurações do projeto, como credenciais Wi-Fi e informações do servidor.
- **colors.h**: Define a estrutura de cores e o vetor de cores.

## Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para abrir issues e pull requests.

## Licença

Este projeto está licenciado sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.
