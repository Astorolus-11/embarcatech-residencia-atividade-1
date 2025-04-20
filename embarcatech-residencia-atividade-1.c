//Bibliotecas:
#include <stdio.h>
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "fonte.h"
#include "matriz_leds.pio.h"



//PIO: 
static PIO pio;
static uint statemachine;
static uint offset;
static uint32_t valor_led;

#define i²c_port i2c1 // Interface 1 que será utilizada do i²c

//Pinos:
const uint led_verde = 11, led_azul = 12, led_vermelho = 13; //LEDs do RGB
const uint botao_a = 5, botao_b = 6; //Botões esquerdo e direito
const uint buzzer_a = 10, buzzer_b = 21; //Buzzer esquerdo e direito 
const uint joy_x = 26, joy_y = 27; //Eixos do Joystick
const uint pin_matriz = 7; //Matriz de LEDs 5x5
const uint SDA = 14, SCL = 15; //Entrada/Saida de dados do I²c e o sinal de clock
const uint endereco = 0x3C; //Endereço do display SSD1306

//Variáveis globais:
static volatile uint32_t last_time = 0; 
ssd1306_t ssd;
uint pixels = 25;
uint escolha = 0;

//Protótipos das funções:
void setup();
static void gpio_irq_handler(uint gpio, uint32_t events);
void adc_setup();
void i²c_setup();
uint32_t intensidade(double g, double r, double b);
void animacao(uint escolha);
//Frames:
double desenho[4][25]={
        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.1, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.1, 0.1, 0.1, 0.0,
         0.0, 0.1, 0.1, 0.1, 0.0,
         0.0, 0.1, 0.1, 0.1, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.1, 0.1, 0.1, 0.1, 0.1,
         0.1, 0.1, 0.1, 0.1, 0.1,
         0.1, 0.1, 0.1, 0.1, 0.1,
         0.1, 0.1, 0.1, 0.1, 0.1,
         0.1, 0.1, 0.1, 0.1, 0.1},

        {0.0, 0.1, 0.0, 0.1, 0.0,
         0.1, 0.1, 0.1, 0.1, 0.1,
         0.0, 0.1, 0.0, 0.1, 0.0,
         0.1, 0.1, 0.1, 0.1, 0.1,
         0.0, 0.1, 0.0, 0.1, 0.0},

        
};



//Função principal:
int main()
{   
    bool clk;
    //Chamada das funções de configuração:
    setup();
    adc_setup();
    i²c_setup();
    stdio_init_all();

    //Configuração do PIO para matriz 5x5:
    pio = pio0;
    offset = pio_add_program(pio, &matriz_leds_program);
    statemachine = pio_claim_unused_sm(pio,true);
    matriz_leds_program_init(pio, statemachine, offset,pin_matriz);

    clk=set_sys_clock_khz(128000,false);
    if(clk){ // Se o clock de 128000 foi habilitado com sucesso
        printf("Clock definido: %ld\n",clock_get_hz(clk_sys));
    }

    //Interrupções para os botões:
    gpio_set_irq_enabled_with_callback(botao_a,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(botao_b,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);
    
    
    while (true) {

       adc_select_input(0);
       uint16_t valor_y = adc_read();
       adc_select_input(1);
       uint16_t valor_x = adc_read();    

       printf("Valor do eixo x: %d\t  Valor do eixo y: %d\n",valor_x,valor_y); 

       uint16_t cursor_y = ((4087 - valor_y)*44)/4095; //Eixo do y modificado, abaixo dele sobra 8 pixels para a caixa de informações do cursor, antes era 52, 52-8 = 44
       uint16_t cursor_x = (valor_x * 113)/4095;

       ssd1306_fill(&ssd, false);
       //Display SSD: Borda
       ssd1306_rect(&ssd,0,0,127,63,true,false);
       //Cursor:
       ssd1306_rect(&ssd, cursor_y, cursor_x, 8, 8, true, true);
       //Caixa de informações sobre o cursor:
       ssd1306_hline(&ssd,1,126,53,true);
       char strx[4],stry[4];
       sprintf(strx,"%d",valor_x);
       sprintf(stry,"%d",valor_y);
       ssd1306_draw_string(&ssd,"x",2,54);
       ssd1306_draw_string(&ssd,strx,19,54);
       ssd1306_draw_string(&ssd,"y", 65, 54);
       ssd1306_draw_string(&ssd,stry,82,54);
       ssd1306_vline(&ssd,64,54,61,true);
       
       ssd1306_send_data(&ssd);
       animacao(escolha);
       sleep_ms(10);

    }
}


//Campo das funções:

void setup (){
    //Inicializando os pinos:
    gpio_init(led_verde);
    gpio_init(led_azul);
    gpio_init(led_vermelho);
    gpio_init(botao_a);
    gpio_init(botao_b);
    gpio_init(buzzer_a);
    gpio_init(buzzer_b);
    //Definindo como entrada/saida:
    gpio_set_dir(led_verde, GPIO_OUT);
    gpio_set_dir(led_azul, GPIO_OUT);
    gpio_set_dir(led_vermelho, GPIO_OUT);
    gpio_set_dir(botao_a, GPIO_IN);
    gpio_set_dir(botao_b, GPIO_IN);
    gpio_set_dir(buzzer_a, GPIO_OUT);
    gpio_set_dir(buzzer_b, GPIO_OUT);
    //Habilitando o pull-up dos botões:
    gpio_pull_up(botao_a);
    gpio_pull_up(botao_b);
}

void gpio_irq_handler(uint gpio, uint32_t events){
    static bool bastate=false,bbstate=false,gstate=false,bstate=false,rstate=false;
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time()); //Tempo atual desde o inicio do sistema

    if(tempo_atual-last_time>200000){
        last_time = tempo_atual; //Atualiza o last time

        if(gpio_get(botao_a)==0){ //Rotina para o botão A
            //Incrementa e altera a animação:
            escolha++;
            bastate=!bastate;
            gpio_put(buzzer_a,bastate);

            if(escolha%2==0){ //ok
                gstate=!gstate;
                gpio_put(led_verde,gstate);

            }
            else if(escolha%2!=0){ //ok
                bstate=!bstate;
                gpio_put(led_azul,bstate);

            }
            else if(escolha%3==0){ //ok
                rstate=!rstate;
                gpio_put(led_vermelho,rstate);
            }
            if(escolha%5==0){ //ok
                rstate=!rstate;
                gpio_put(led_vermelho,rstate);
                bstate=!bstate;
                gpio_put(led_azul,bstate);
                gstate=!gstate;
                gpio_put(led_verde,gstate);
            }
            

        }
        if(gpio_get(botao_b)==0){ //Rotina para o botão B
            //Decrementa e altera a animação:
            escolha--;
            bbstate=!bbstate;
            gpio_put(buzzer_b,bbstate);

            if(escolha%2==0){ //ok
                gstate=!gstate;
                gpio_put(led_verde,gstate);

            }
            else if(escolha%2!=0){ //ok
                bstate=!bstate;
                gpio_put(led_azul,bstate);

            }
            else if(escolha%3==0){ //ok
                rstate=!rstate;
                gpio_put(led_vermelho,rstate);
            }
            if(escolha%5==0){ //ok
                rstate=!rstate;
                gpio_put(led_vermelho,rstate);
                bstate=!bstate;
                gpio_put(led_azul,bstate);
                gstate=!gstate;
                gpio_put(led_verde,gstate);
            }
            
        }
    
    }


}
void adc_setup(){
    //Inicializar o ADC:
    adc_init();
    //Inicializar o Joystick como ADC:
    adc_gpio_init(joy_x);
    adc_gpio_init(joy_y);
    //Inicializando os pinos:
    gpio_init(joy_x);
    gpio_init(joy_y);
    //Definindo como entrada:
    gpio_set_dir(joy_x,GPIO_IN);
    gpio_set_dir(joy_y,GPIO_IN);
    //Habilitando o pull-up:
    gpio_pull_up(joy_x);
    gpio_pull_up(joy_y);
}

void i²c_setup(){
    //Inicializa a interface i²c com 400KHz de frequência:
    i2c_init(i²c_port,400*1000);
    //Habilitando a função do i²c nos pinos de SDA e SCL:
    gpio_set_function(SDA,GPIO_FUNC_I2C);
    gpio_set_function(SCL,GPIO_FUNC_I2C);
    //Habilitando o pull-up:
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
    //Configuração inicial do display:
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, i²c_port);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
}
uint32_t intensidade(double g, double r, double b){
    unsigned char G,B,R;
    G = g*255;
    B = b*255;
    R = r*255;

    return (G<<24) | (R<<16) | (B<<8);
}
void animacao(uint escolha){
    

    if(escolha==0){
        
        
        for(int i=0;i<pixels;i++){
            
            valor_led= intensidade(desenho[0][i],0.0,0.0);
            pio_sm_put_blocking(pio,statemachine,valor_led);
    
        }

    }
    else if(escolha==1){

        for(int i=0;i<pixels;i++){
            if(i!=12){
            valor_led= intensidade(0.0,desenho[1][i],0.0);
            

            }
            else{
                valor_led= intensidade(desenho[1][i],0.0,0.0);

            }
            pio_sm_put_blocking(pio,statemachine,valor_led);
            
    
        }

    }
    else if(escolha==2){

        for(int i=0;i<pixels;i++){
        
            if(i==12){
            valor_led= intensidade(desenho[2][i],0.0,0.0);
            

            }
            if( i==5 || i==9 || i==10 || i==14 || i==15 || i==19){
                valor_led= intensidade(0.0,0.0,desenho[2][i]);

            }
            if(i>=0 && i<=5 ||i>=20 && i<=24){
                valor_led= intensidade(0.0,0.0,desenho[2][i]);
            }
            if(i>=6&&i<=8 || i>=16 && i<18 || i==11 || i==13){
                valor_led= intensidade(0.0,desenho[2][i],0.0);

            }
            pio_sm_put_blocking(pio,statemachine,valor_led);
            
    
        }

    }
    else if(escolha==3){

        for(int i=0;i<pixels;i++){
            if(i>=5 &&i<=9 || i>=15 && i<=19){
                valor_led=intensidade(0.0,0.0,desenho[3][i]);
            }
            else{
                valor_led=intensidade(0.0,desenho[3][i],0.0);
                
            }
            pio_sm_put_blocking(pio,statemachine,valor_led);
            
        }
    }
    else if(escolha%2==0){
        for(int i=0;i<pixels;i++){
            if(i>=5 &&i<=9 || i>=15 && i<=19){
                valor_led=intensidade(0.0,desenho[3][i],0.0);
            }
            else{
                valor_led=intensidade(0.0,0.0,desenho[3][i]);
               
                
            }
            pio_sm_put_blocking(pio,statemachine,valor_led);
            
        }
    }
    else if(escolha%2!=0){
        for(int i=0;i<pixels;i++){
            if(i>=5 &&i<=9 || i>=15 && i<=19){
                valor_led=intensidade(0.0,0.0,desenho[3][i]);
            }
            else{
                valor_led=intensidade(0.0,desenho[3][i],0.0);
                
               
                
            }
            pio_sm_put_blocking(pio,statemachine,valor_led);
            
        }
    }
    
    }    
    





