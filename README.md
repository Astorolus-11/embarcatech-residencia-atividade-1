# Pequeno projeto para revisar o conteudo aprendido na capacitação do Embarcatech em Sistemas Embarcados


## 1. Propósito


Como o próprio título diz, o objetivo desse projeto é revisar e consolidar os conhecimentos adquiridos na fase 1 de capacitação do Embarcatech. Os seguintes periféricos foram utilizados: ADC, UART, Matriz de LEDs 5x5 (WS2812), I²C
, LED RGB, botões, buzzers, joystick e o display SSD1306. Foi utilizado também técnicas de tratamento de bounce dos botões e interrupções.


## 2. Hardware


Para o desenvolvimento desse projeto foi utilizado a placa BitDogLab fornecida pela Embarcatech, com o Raspberry Pi Pico W com o microcontrolador RP2040.
Os componentes utilizados presentes na placa foram:


1. Botões : A, B e JOY (GPIOs 5, 6 e 22 respectivamente).
2. LEDs do RGB : Verde azul e vermelho (GPIOs 11, 12 e 13).
3. Buzzers : A, B (GPIOs 10 e 21).
4. Matriz de LEDs 5x5 WS2812 : (GPIO 7).
5. Joystick : Eixo x e y (GPIOs 27 e 26).
6. Comuinicação serial I2C : SDA e SCL (GPIOs 14 e 15).
7. Display SSD1306 : Endereço 0x3C.





## 3. Funcionalidades


1. Botão A - Altera a animação, liga/desliga o buzzer esquerdo e o LED RGB.
2. Botão B - Altera a animação, liga/desliga o buzzer direito e o LED RGB.
3. Joystick - Ao movimentar os eixos do joystick um quadrado de 8x8 é movimentado no display. Inicialmente está centralizado.
4. Display - Como foi dito anteriormente, movimenta um quadrado 8x8 e exibe suas coordenadas.
5. UART USB - Também exibe as coordenadas. (Utilizado para testar a lógica/depurar os eixos)

   
   


   
   
## 4. Pré-requisitos


1. Ter o [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado na máquina;
2. Ter o [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) instalado;
3. Ter o [Visual Studio Code](https://code.visualstudio.com/download) instalado;
4. Ter este repositório clonado na sua máquina;
5. Ter as seguintes extensões instaladas no seu VS Code para o correto funcionamento:
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools);
- [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake);
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools);
- [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico);
  
  

##  5. Como executar o projeto:


1. Clonar o repositório: [https://github.com/Astorolus-11/embarcatech-projeto-final](https://github.com/Astorolus-11/embarcatech-residencia-atividade-1)
2. Abrir a pasta clonada através do import project:

   ![image](https://github.com/user-attachments/assets/9ea528e1-0253-4cf8-b6c6-8532be0fc1b4)
   

3. Para executar na placa clique em Run que está localizada no rodapé do vscode (A placa precisa já está conectada e com o BootSel ativado):

   ![image](https://github.com/user-attachments/assets/36b14dce-1309-4f0c-a7f3-3cd7edb2b336)


## 6. Vídeo de demonstração



   
  
  # Pronto! já está tudo pronto para testar o projeto!
