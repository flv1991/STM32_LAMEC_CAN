#include "stm32f10x_lib.h"
#include <stdint.h>
volatile int flagrx=0;//flagrx é activada na interrupçao da usart
extern void lcd_info(void);
void delay( uint32_t time2delay);


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


void rcc(void){

RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD , ENABLE );

}

void gpio(void){

// SCK and MISO are Output, Alternate Function floating (PP) as Master
//PB13
// MISO -> input (as Master, input floating!)

//SPI CS - PD2


    /* GPIO configuration */
    GPIO_InitTypeDef GPIO_InitStructure;
     GPIO_DeInit(GPIOB);//função deinit faz reset as configs do porto
     GPIO_DeInit(GPIOD);

//CS
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
//MOSI miso sck

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15  | GPIO_Pin_13; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void spi(void){
//sensor ligado ao SPI2
	//SPI MOSI - PB15 
	//SPI MISO - PB14
	//SPI_SCK - PB13
	//SPI CS - PD2


/* SPI_MASTER configuration ------------------------------------------------*/
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  //SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

SPI_Cmd(SPI2, ENABLE);
}


int SPI_send(uint8_t address, uint8_t data){
int ReceiveData;
char buffer[15];
//ativar o cs PD2
	 GPIO_WriteBit(GPIOD,GPIO_Pin_2,Bit_RESET);
	sprintf(buffer,"CS");
	DRAW_DisplayString( 1, 120, buffer, strlen(buffer));
//esperar que TX esteja livre
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_TXE);
	SPI_I2S_SendData(SPI2, address);
	sprintf(buffer,"add");
	DRAW_DisplayString( 5, 120, buffer, strlen(buffer));
 //esperar que TX esteja livre
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_TXE);
	SPI_I2S_SendData(SPI2, data);
	sprintf(buffer,"data");
	DRAW_DisplayString( 20, 120, buffer, strlen(buffer));
//Esperar pela info
while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_RXNE);
	ReceiveData= SPI_I2S_ReceiveData(SPI2);
	sprintf(buffer,"ReceiveData");
	DRAW_DisplayString( 5, 110, buffer, strlen(buffer));
//desativar o cs PD
GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET); 
sprintf(buffer,"notCS");
DRAW_DisplayString( 1, 100, buffer, strlen(buffer));
		return (ReceiveData);

}







int main(void)
{   char buffer2[15];  
	RCC_Config_HSE_PLL_Max();
	rcc();
	gpio();
	spi();
int i=0;
uint32_t addr=0;
GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET); 
//Set the data register to the byte to be transmitted
//Wait for data transmission on MOSI to complete
//Wait for data reception on MISO to complete
//Wait until SPI is no longer busy
//Transfer the received data from the data register
	DRAW_Init ();
//configacc();
SPI_send(0x20, 0b11000000);
addr=SPI_send(0x0f, 0x00); 

sprintf(buffer2,"addr:%d",addr);
DRAW_DisplayString( 1, 18, buffer2, strlen(buffer2));
    while (1)
    {   
		i++;
sprintf(buffer2,"addr:%d",i);
DRAW_DisplayString( 1, 50, buffer2, strlen(buffer2));

 
        
                
    }
    return (0);
}


void delay(volatile uint32_t time2delay)
{
    for ( ; time2delay > 0; time2delay--)
    {
    }
}

