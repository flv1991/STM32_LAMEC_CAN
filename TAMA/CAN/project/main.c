

#include <stdlib.h>
#include "stm32f10x_lib.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x_lib.h"
#include <stdint.h>
#include <math.h>

int delay(int32_t var){
volatile int32_t i;
for(i=0;i<var;i++){
    asm("nop");
    }
}


CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg canMessage;

CanRxMsg rx_message;

void can_init(void){

    GPIO_InitTypeDef       GPIO_InitStructure;
   CAN_InitTypeDef       CAN_InitStructure;
   CAN_FilterInitTypeDef     CAN_FilterInitStructure;
   NVIC_InitTypeDef       NVIC_InitStructure;


   /* Enable GPIO clock */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
//RCC_APB2Periph_AFIO
   /* Connect CAN 1 pins to AF9 */
   //GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN);
   //GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN);

   /* Configure CAN 1 RX and TX pins */
   //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
   //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

   //GPIO_Init(GPIOB, &GPIO_InitStructure);


/* Configure CAN pin: RX */

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


GPIO_Init(GPIOB, &GPIO_InitStructure);


/* Configure CAN pin: TX */

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure);


// Remap2 is for PB8 and PB9

GPIO_PinRemapConfig(GPIO_Remap1_CAN , ENABLE);

   


   /* CAN 1 cell init */
   CAN_InitStructure.CAN_Prescaler = 18; //64
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
   CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	//2
   CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq; //11
   CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq; //4
   CAN_InitStructure.CAN_TTCM = DISABLE;
   CAN_InitStructure.CAN_ABOM = DISABLE;
   CAN_InitStructure.CAN_AWUM = DISABLE;
   CAN_InitStructure.CAN_NART = ENABLE;
   CAN_InitStructure.CAN_RFLM = DISABLE;
   CAN_InitStructure.CAN_TXFP = DISABLE;
   CAN_Init(&CAN_InitStructure );

   /* CAN1 filter init, accept every message */
   CAN_FilterInitStructure.CAN_FilterNumber = 0;  // 0..13 for CAN1, 14..27 for CAN2
   CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
   CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
   CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000 ;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
   CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

   /* Message Init */
   canMessage.StdId = 454;
   canMessage.ExtId = 0;
   canMessage.RTR = CAN_RTR_DATA;
   canMessage.IDE = CAN_ID_STD;
   canMessage.DLC = 1;

   canMessage.Data[0] = 111;
	 //canMessage.Data = 5;
     
     
}

void RCC_Config_HSE_PLL_Max(void){
RCC_DeInit();
FLASH_SetLatency(FLASH_Latency_2);
/*configurar clock externo a 72Mhz */

//bypass pois está ligado no OSC_IN um circuito oscilatorio externo
RCC_HSEConfig(RCC_HSE_Bypass);
//Após a ativação do HSE deveremos esperar até que o sinal de relógio esteja pronto e estável
ErrorStatus HSEStartUpStatus;
HSEStartUpStatus=RCC_WaitForHSEStartUp();

if(HSEStartUpStatus==SUCCESS) /*retorna SUCCESS ou ERROR*/
{   

	    //divisão do prescaler AHB por 1
	    RCC_HCLKConfig(RCC_SYSCLK_Div1);
	    //divisão do prescaler APB1 por 2
	    RCC_PCLK1Config(RCC_HCLK_Div2);
	    //divisão do prescaler APB2 por 1
	    RCC_PCLK2Config(RCC_HCLK_Div1);
    //configuar a PLL para 72Mhz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_6);
    //activar a pll
    RCC_PLLCmd(ENABLE);
    
    while(RCC_GetFlagStatus( RCC_FLAG_PLLRDY)==RESET);
    //activa o sysclk pll
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    
    //confirmar que o clock está correcto logo esperar que a fonte de relógio esteja correta
    while(RCC_GetSYSCLKSource()!=0x08);
    //clock configurado com sucesso 
}
else
 while(1); /*se deu erro o micro nao arranca*/


}

int main(void)
{

RCC_Config_HSE_PLL_Max(); //Configura o clk a 72Mhz
    /* Enable GPIO B clock*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
LCD_Init();
DRAW_Init();
can_init();
char buff[20];
int i=0;
    /*GPIO configuration*/
    GPIO_InitTypeDef GPIO_InitStructure;


    /*botao como entrada*/
    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

//iniciar o leds on
    //GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
   // GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
    uint8_t butao, cnt=0; //declaração de variavel de 8bits
    
    


  
    
    while (1)
    {
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    butao=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    if (butao==1){ //botao press
    
    i++;
        sprintf(buff,"entrou %d vezes     ",i);
        DRAW_DisplayString( 5, 2, buff, strlen(buff));
        
        CAN_Receive(CAN_FIFO0, &rx_message);
    
        sprintf(buff,"ID: %d        ",rx_message.StdId);
        DRAW_DisplayString( 7, 60, buff, strlen(buff));
        sprintf(buff,"TAMANHO: %d        ",rx_message.DLC);
        DRAW_DisplayString( 7, 40, buff, strlen(buff));    
    
        sprintf(buff,"DATA: %d        ",rx_message.Data[0]);
        DRAW_DisplayString( 7, 20, buff, strlen(buff));
        //CAN_Transmit(&canMessage);
        delay(100000);
    
    }
    
    

    }

}


