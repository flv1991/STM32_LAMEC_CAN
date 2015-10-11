#include "stm32f10x_lib.h"
#include <stdint.h>

extern void lcd_info(void);
void delay( uint32_t time2delay);
void RCC_Config_HSI_default(void){
    RCC_DeInit();
    /*configurar clock interno a 8Mhz*/
    RCC_HSICmd(ENABLE); // ou RCC_PLLConfig(RCC_PLLSource_HSI_Div2,�RCC_PLLMul_2);
   RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
//garantir que o clock est� correcto
   while(RCC_GetSYSCLKSource()!=0x00);

}

void RCC_Config_HSI_PLL_Max(void){
RCC_DeInit();
FLASH_SetLatency(FLASH_Latency_2);

/*configurar clock a 64Mhz (max)*/




//divis�o do prescaler AHB por 1
RCC_HCLKConfig(RCC_SYSCLK_Div1);
//divis�o do prescaler APB1 por 2
RCC_PCLK1Config(RCC_HCLK_Div2);
//divis�o do prescaler APB2 por 1
RCC_PCLK2Config(RCC_HCLK_Div1);
//� usado a PLL com a source HSI (clock interno) e 16 de factor multiplicativo
RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);

//depois de configurado a PLL fazer enable
RCC_PLLCmd(ENABLE);
while(RCC_GetFlagStatus( RCC_FLAG_PLLRDY)==RESET);

RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//garantir que o clock da pll est� correcto
while(RCC_GetSYSCLKSource()!=0x08);




}

void RCC_Config_HSE_Default(void){
RCC_DeInit();
/*configurar clock externo a 12Mhz */
//bypass pois est� ligado no OSC_IN um circuito oscilatorio externo
RCC_HSEConfig(RCC_HSE_Bypass);
//Ap�s a ativa��o do HSE deveremos esperar at� que o sinal de rel�gio esteja pronto e est�vel
ErrorStatus HSEStartUpStatus;
HSEStartUpStatus=RCC_WaitForHSEStartUp();

if(HSEStartUpStatus==SUCCESS) /*retorna�SUCCESS�ou�ERROR*/
{

//divis�o do prescaler AHB por 1
RCC_HCLKConfig(RCC_SYSCLK_Div1);
//divis�o do prescaler APB1 por 1
RCC_PCLK1Config(RCC_HCLK_Div1);
//divis�o do prescaler APB2 por 1
RCC_PCLK2Config(RCC_HCLK_Div1);

    //arrancar com o clock externo
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    //confirmar que o clock est� correcto logo esperar que a fonte de rel�gio esteja correta
    while(RCC_GetSYSCLKSource()!=0x04);
    //clock configurado com sucesso 
}
else
 while(1); /*se deu erro o micro nao arranca*/

}

void RCC_Config_HSE_PLL_Max(void){
RCC_DeInit();
FLASH_SetLatency(FLASH_Latency_2);
/*configurar clock externo a 72Mhz */

//bypass pois est� ligado no OSC_IN um circuito oscilatorio externo
RCC_HSEConfig(RCC_HSE_Bypass);
//Ap�s a ativa��o do HSE deveremos esperar at� que o sinal de rel�gio esteja pronto e est�vel
ErrorStatus HSEStartUpStatus;
HSEStartUpStatus=RCC_WaitForHSEStartUp();

if(HSEStartUpStatus==SUCCESS) /*retorna�SUCCESS�ou�ERROR*/
{   

	    //divis�o do prescaler AHB por 1
	    RCC_HCLKConfig(RCC_SYSCLK_Div1);
	    //divis�o do prescaler APB1 por 2
	    RCC_PCLK1Config(RCC_HCLK_Div2);
	    //divis�o do prescaler APB2 por 1
	    RCC_PCLK2Config(RCC_HCLK_Div1);
    //configuar a PLL para 72Mhz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_6);
    //activar a pll
    RCC_PLLCmd(ENABLE);
    
    while(RCC_GetFlagStatus( RCC_FLAG_PLLRDY)==RESET);
    //activa o sysclk pll
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    
    //confirmar que o clock est� correcto logo esperar que a fonte de rel�gio esteja correta
    while(RCC_GetSYSCLKSource()!=0x08);
    //clock configurado com sucesso 
}
else
 while(1); /*se deu erro o micro nao arranca*/


}


void RCC_Config_30MHz(void){
RCC_DeInit();
FLASH_SetLatency(FLASH_Latency_2);
/*configurar clock externo a 72Mhz */

//bypass pois est� ligado no OSC_IN um circuito oscilatorio externo
RCC_HSEConfig(RCC_HSE_Bypass);
//Ap�s a ativa��o do HSE deveremos esperar at� que o sinal de rel�gio esteja pronto e est�vel
ErrorStatus HSEStartUpStatus;
HSEStartUpStatus=RCC_WaitForHSEStartUp();

if(HSEStartUpStatus==SUCCESS) /*retorna�SUCCESS�ou�ERROR*/
{   

    //divis�o do prescaler AHB por 1
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    //divis�o do prescaler APB1 por 2
    RCC_PCLK1Config(RCC_HCLK_Div1);
    //divis�o do prescaler APB2 por 1
    RCC_PCLK2Config(RCC_HCLK_Div1);
    //configuar a PLL para 30Mhz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_5);
    //activar a pll
    RCC_PLLCmd(ENABLE);
    
    while(RCC_GetFlagStatus( RCC_FLAG_PLLRDY)==RESET);
    //activa o sysclk pll
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    
    //confirmar que o clock est� correcto logo esperar que a fonte de rel�gio esteja correta
    while(RCC_GetSYSCLKSource()!=0x08);
    //clock configurado com sucesso 
}
else
 while(1); /*se deu erro o micro nao arranca*/




}


void timer2config(void) //timer 2 usado para fazer os 1Hz ou seja pisca a cada 1 segundo
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 

	TIM_TimeBaseStructure.TIM_Period=4999;//auto�reload de 4999
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=7199; //prescaler�de�7199
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //Enviar a estrutura preenchida
	TIM_Cmd(TIM2, ENABLE); //Ligar timer apos config

}

void timer4config_toggle(void) //timer 4 est� ligado diretamente ao GPIOB9
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period=4999;//auto�reload�4999
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=7199; //prescaler�de�7199
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//Enviar a estrutura preenchida
 
 	/*os valores sao os mesmos pois queremos a mesma base de tempo*/

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;  //escolher modo do timer, no caso modo toggle
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0; //no modo toggle nao � necess�rio definir o pulse (PWM) logo =0
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low; // Low ou High serve para definir o 1� estado do toggle no caso est� a Low
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);// envia a estrutura da conig
	TIM_Cmd(TIM4, ENABLE);//Ligar timer apos configs
}

void timer3config_toggle(void) //timer 4 est� ligado diretamente ao GPIOB9
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period=35;//auto�reload�4999
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=719; //prescaler�de�7199
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//Enviar a estrutura preenchida
 
 	/*os valores sao os mesmos pois queremos a mesma base de tempo*/

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Toggle;  //escolher modo do timer, no caso modo toggle
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0; //no modo toggle nao � necess�rio definir o pulse (PWM) logo =0
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low; // Low ou High serve para definir o 1� estado do toggle no caso est� a Low
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);// envia a estrutura da conig
	TIM_Cmd(TIM3, ENABLE);//Ligar timer apos configs
}




void timer4config_pwm(void) //timer 4 est� ligado diretamente ao GPIOB9
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period=99;//auto�reload�99, � usado os 99 para simplificar, pois ao colocar o pulse a 99=100% de PWM 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=719; //prescaler�de�719
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);// envia a estrutura da conig


	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;//modo 2 pk o modo 1 � invertido
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//liga o timer diretamente ao LED
	TIM_OCInitStructure.TIM_Pulse=0; //iniciar o led apagado dutty cycle de 0%
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low; // Low ou High serve para definir o 1� estado do PWM
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);// envia a estrutura da conig
	TIM_Cmd(TIM4, ENABLE);//Ligar timer apos configs
}

void timer2config_gated(void) 
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//mesmos tempos da func timer2config
	TIM_TimeBaseStructure.TIM_Period=4999;//auto�reload de 4999
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=7199; //prescaler�de�7199
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //Enviar a estrutura preenchida
	
	

	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//o botao est� ligado ao canal 1
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;//falling o timer para quando o bot�o � pressionado, rising o timer s� trabalho quando o bot�o estiver a ser pressionado
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI; //TIM Input 1 is selected to be connected to IC1.
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//nao h� div
	TIM_ICInitStructure.TIM_ICFilter=0; // Specifies the Input Capture Filter.
	TIM_ICInit(TIM2, &TIM_ICInitStructure);// envia a estrutura da conig
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1); //Filtered Timer Input 1
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Gated);//indica o modo do timer no caso, Gated 
	TIM_Cmd(TIM2, ENABLE);//Ligar timer apos configs
}


void rccconfig(void){

 	//activar o GPIOB para os leds e o GPIOA para o bot�o
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA , ENABLE );
    //ativar o periferico TIM2 e TIM4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM3, ENABLE);


}

void gpioconfig(void){

    /* GPIO configuration */
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_DeInit(GPIOB);//fun��o deinit faz reset as configs do porto
    GPIO_DeInit(GPIOA);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8; //pino 8 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //modo saida
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//inicia o GPIOB8 como saida


    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9; //pino 9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //modo AF (alternated function) para o timer 4 poder controlar o led
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//inicia o GPIOB9 como saida para PWM

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 ; //pino 0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //modo AF(alternated funcion) para o bot�o no GPIOA0 poder operar em modo gated
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//inicia o GPIOA0 como Entrada ligada ao timer2 

GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 ; //pino 0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //modo AF(alternated funcion) para o bot�o no GPIOA0 poder operar em modo gated
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//inicia o GPIOA0 como Entrada ligada ao timer2 

}
void update(void){



}

int main(void)
{   uint8_t brilho=0,sentido=0,flag=0; //brilho � o pwm , sentido define se � para aumentar o brilho ou para diminuir, a flag conta os 2 segundos
   
    RCC_Config_HSE_PLL_Max(); //Configura o clk a 72Mhz
    rccconfig(); //activa os perifericos necess�rios, tim, gpios...
    //timer2config();  //timer2, flag_update a cada 500ms 
    //timer4config_toggle(); //timer 4 no modo toggle ligado diretamente ao led no GPIOB9
    timer4config_pwm(); // timer 4 no modo pwm (modo de contagem crescente) de forma a regular o brilho do led GPIOB9
    timer2config_gated(); //timer 2 modo gated  
timer3config_toggle();  //buzzer  
gpioconfig(); //iniciar a configura��o dos GPIO's
  
    //lcd_info();
    while (1)
    {update();
        //1
        if(sentido==0){
						brilho=brilho+24;
				}
				else {
						brilho=brilho-24;
				}
		
        GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET); //liga led
        while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET); //o prog s� avan�a quando ocorrer a flag_update
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);//logo ap�s deverificar a flag do timer � necess�rio limpar a flag para o timer continuar

        if(sentido==0){
						brilho=brilho+24;
				}
				else {
						brilho=brilho-24;
				}
		TIM_SetCompare4(TIM4, brilho);
       
        GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET); //desliga led 
        while(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==RESET); //o prog s� avan�a quando ocorrer a flag_update
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);//logo ap�s deverificar a flag do timer � necess�rio limpar a flag para o timer continuar
        
       	flag++;
				if(sentido==0&&flag==2){
						sentido=1;
						flag=0;
				}
				else if (sentido==1&&flag==2) {
						sentido=0;
						flag=0;
				}
        
            
         
        
    }
    return (0);
}


void delay(volatile uint32_t time2delay)
{
    for ( ; time2delay > 0; time2delay--)
    {
    }
}

