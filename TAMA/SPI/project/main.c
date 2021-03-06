#include <stdlib.h>
#include "stm32f10x_lib.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x_lib.h"
#include <stdint.h>
#include <math.h>

volatile int flagrx=0;//flagrx � activada na interrup�ao da usart
extern void lcd_info(void);
void delay( uint32_t time2delay);
volatile uint8_t RxData;
    int32_t left_part_z,right_part_z;
    int32_t left_part_x,right_part_x;
 int32_t left_part_y,right_part_y;
 long int i=0, MSB, LSB, Xg,Yg,Zg;
 float x1, z1;
 char xusart,yusart,zusart;
 int flaginitbola=0;
 int flagxesq, flagxdir,flagycima,flagybaixo;
 //float y1;
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


void rcc(void){
 
RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB1Periph_USART2, ENABLE);
 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD , ENABLE );

}

void gpio(void){

// SCK and MISO are Output, Alternate Function floating (PP) as Master
//PB13
// MISO -> input (as Master, input floating!)

//SPI CS - PD2


    /* GPIO configuration */
    GPIO_InitTypeDef GPIO_InitStructure;
     GPIO_DeInit(GPIOB);//fun��o deinit faz reset as configs do porto
     GPIO_DeInit(GPIOD);
    GPIO_DeInit(GPIOA);
//CS
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
  
//MOSI PB15
//miso PB14
//sck PB13

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15  | GPIO_Pin_13; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
//
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    

    
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8; //pino 8 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //CAN
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//inicia o GPIOB8 como saida



    //PA2 USART TX Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2; //pino 2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//inicia o GPIOA2
    //PA3 USART RX GPIO_Mode_IN_FLOATING
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3; //pino 3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//inicia o GPIOA3
    
}

void spi(void){
//sensor ligado ao SPI2
  //SPI MOSI - PB15 
  //SPI MISO - PB14
  //SPI_SCK - PB13
  //SPI CS - PD2


/* --------------SPI_MASTER configuration------------------------------*/
SPI_InitTypeDef SPI_InitStructure;
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
SPI_Init(SPI2, &SPI_InitStructure);
SPI_Cmd(SPI2, ENABLE);
}

uint8_t SPI_read(uint8_t address)
{
    uint8_t value;
    GPIO_WriteBit(GPIOD, GPIO_Pin_2,RESET); 
    address = 0x80 | address;                         // 0b10 - reading and clearing status
  
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_SendData(SPI2, address);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
 
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_SendData(SPI2, 0xFF);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  value=SPI_I2S_ReceiveData(SPI2);
    GPIO_WriteBit(GPIOD, GPIO_Pin_2,SET);
 
  return value ;
}


void SPI_send(uint8_t address, uint8_t data)
{
    GPIO_WriteBit(GPIOD, GPIO_Pin_2,RESET);
 
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_SendData(SPI2, address);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
 
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
    SPI_I2S_SendData(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI2);
 
    GPIO_WriteBit(GPIOD, GPIO_Pin_2,SET);
}


void acc(void){
float y1;
MSB = SPI_read(0x2D);                     // Z-axis MSB  
      LSB = SPI_read(0x2C);                     // Z-axis LSB
      Zg = (MSB << 8) | (LSB);                  // Merging
      Zg=Zg&0xfff; //mascara para ficarmos s� com 12bits
      if(Zg>=0&&Zg<=2047)
        {z1=1.0*Zg/988;
       zusart=100*Zg/3060-32.29;}
      else
       { z1=1.0*Zg/1068-3.83;
     zusart=-100*Zg/988+100;}
        left_part_z=(int) z1;
        right_part_z = (z1 - left_part_z) * 100000;
    
      MSB = SPI_read(0x2B);                     // Z-axis MSB
      LSB = SPI_read(0x2A);                     // Z-axis LSB
      Yg = (MSB << 8) | (LSB);  
      Yg=Yg&0xfff;    // Merging  
      if(Yg>=0&&Yg<=1015){
        //y1=1.0*Yg/1015;
      y1=63*Yg/1015+64;
        yusart=100*Yg/1015;
      }
      else
        {
        // y1=-1.0*Yg/1015+4.03;
        y1=64.0*Yg/1015-194.2;
yusart=-(-100*Yg/1015+403.45);
    }
        left_part_y=(int) y1;
        right_part_y = (y1 - left_part_y) * 100000;
      MSB = SPI_read(0x29);
      LSB = SPI_read(0x28);                     
      Xg = (MSB << 8) | (LSB);
      Xg=Xg&0xfff; 
      if(Xg>=0&&Xg<=1015){
         x1=63*Xg/1015+64;
        xusart=100*Xg/1015;
      }
      else
       { x1=64.0*Xg/1015-194.2;
        xusart=-(-100*Xg/1015+403.45);
    }
        left_part_x=(int) x1;
        right_part_x = (x1 - left_part_x) * 100000;

}

void clearcircle(int xCenter, int yCenter, int radius)
{int x, y, r2,cnt,r;
r2 = radius * radius;

for (x = -radius; x <= radius; x++) {
 y = (int) (sqrt(r2 - x*x) + 0.5);
LCD_DrawPixel( xCenter + x, yCenter + y, 0xFFFF );
LCD_DrawPixel( xCenter + x, yCenter - y, 0xFFFF );
LCD_DrawPixel( xCenter - x, yCenter + y,0xFFFF );
LCD_DrawPixel( xCenter - x, yCenter - y, 0xFFFF );

}

for (y = -radius; y <= radius; y++) {
 x = (int) (sqrt(r2 - y*y) + 0.5);
LCD_DrawPixel( xCenter + x, yCenter + y, 0xFFFF );
LCD_DrawPixel( xCenter + x, yCenter - y, 0xFFFF );
LCD_DrawPixel( xCenter - x, yCenter + y, 0xFFFF );
LCD_DrawPixel( xCenter - x, yCenter - y, 0xFFFF );


}


}

void drawcircle(int xCenter, int yCenter, int radius)
{int x, y, r2;
r2 = radius * radius;

for (x = -radius; x <= radius; x++) {
 y = (int) (sqrt(r2 - x*x) + 0.5);
LCD_DrawPixel( xCenter + x, yCenter + y, 0 );
LCD_DrawPixel( xCenter + x, yCenter - y, 0 );
LCD_DrawPixel( xCenter - x, yCenter + y, 0 );
LCD_DrawPixel( xCenter - x, yCenter - y, 0 );

}

for (y = -radius; y <= radius; y++) {
 x = (int) (sqrt(r2 - y*y) + 0.5);
LCD_DrawPixel( xCenter + x, yCenter + y, 0 );
LCD_DrawPixel( xCenter + x, yCenter - y, 0 );
LCD_DrawPixel( xCenter - x, yCenter + y, 0 );
LCD_DrawPixel( xCenter - x, yCenter - y, 0 );


}


}

void alvo(void){
LCD_DrawRect( 64, 0, 1, 128, 0 );
LCD_DrawRect( 0, 64, 128, 1, 0 );
drawcircle(64,64,65);
drawcircle(64,64,55);
drawcircle(64,64,45);
drawcircle(64,64,35);
drawcircle(64,64,25);
drawcircle(64,64,15);
drawcircle(64,64,5);

}
void bola(int x, int y)
{

int bmp;
int cnt;
if (flaginitbola==0){
drawcircle(64,64,1);
drawcircle(64,64,2);
drawcircle(64,64,3);
drawcircle(64,64,4);
flaginitbola=1;
}
drawcircle(x,y,0);
drawcircle(x,y,1);
drawcircle(x,y,2);
drawcircle(x,y,3);
drawcircle(x,y,4);




for (cnt = 0; cnt < 20; ++cnt)
{
 clearcircle(x,y,5+cnt);
}
 alvo();
//delay(90000);
     }
 

void nvic(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configura o Priority Group com 1 bit */

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);


 }

void usart(int baud){

USART_DeInit(USART2);
USART_InitTypeDef USART_InitStructure;
USART_InitStructure.USART_BaudRate = baud;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1;
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

USART_Init(USART2, &USART_InitStructure);

USART_Cmd(USART2, ENABLE);

USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);



}

 
 
int main(void)
{  

    char buffer2[15];  
    
  RCC_Config_HSE_PLL_Max();
LCD_Init();
DRAW_Init ();

  rcc();
  gpio();
  spi();
usart(9600);
nvic();
uint32_t addr=0,bmp;
GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET); 
//Set the data register to the byte to be transmitted
//Wait for data transmission on MOSI to complete
//Wait for data reception on MISO to complete
//Wait until SPI is no longer busy
//Transfer the received data from the data register
  
//configacc();
    SPI_send(0x20, 0xC7);                        
    //SPI_send(0x21, 0b10000001);    

    //SPI_send(0x22, 0x00);     
addr=SPI_read(0x0f);
// sprintf(buffer2,"addr1:%d",addr);
// DRAW_DisplayString( 1, 5, buffer2, strlen(buffer2));


// alvo();
// bola(0,0);
// delay(5000000);
// bola(20,20);
// delay(5000000);
// bola(40,40);
// delay(5000000);
char buff[15];
char valor=xusart;
    while (1)
      {   
acc();

      
      if (flagrx==1)
          {     flagrx=0;//flag rx ?ctivada quando recebe alguma coisa por USART
                RxData = USART_ReceiveData(USART2);//o que receber na usart2 ?olocado na variavel rxdata  
                switch ( RxData ){ 
                
                case 'X': case 'x':
sprintf(buff, "%2d\n",xusart);
      while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[0]);  
       while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[1]);
             while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[2]); 
                          while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[3]);
                break;
                
                                case 'Y': case 'y':
sprintf(buff, "%2d\n",yusart);
      while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[0]);  
       while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[1]);
             while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[2]); 
                          while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    USART_SendData(USART2,buff[3]);
                break;
                                                  // case 'Z': case 'z'://nao funca 
                  // while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		                    // USART_SendData(USART2, zusart);  
                // break;
                
                
                
                
                
                
                }
            
          
          
          
          }
     bola(left_part_x,left_part_y);
     //LCD_DisplayChar( left_part_x,left_part_y,'O',0,0xFFFF,1);
    
        // sprintf(buffer2,"x:%d.%d           ",left_part_x,right_part_x);
// DRAW_DisplayString( 1, 115, buffer2, strlen(buffer2));
              // sprintf(buffer2,"y:%d         ",left_part_y);
// DRAW_DisplayString( 1, 100, buffer2, strlen(buffer2));
          // sprintf(buffer2,"z:%d         ",(int)Xg);
      // DRAW_DisplayString( 1, 85, buffer2, strlen(buffer2));
                // sprintf(buffer2,"x:%d         ",(int8_t)xusart);
      // DRAW_DisplayString( 1, 70, buffer2, strlen(buffer2));
                        
                // sprintf(buffer2,"z:%f        ",valor);
      // DRAW_DisplayString( 1, 50, buffer2, strlen(buffer2));
 // delay(500000);

              
    }
    return (0);
}


void delay(volatile uint32_t time2delay)
{
    for ( ; time2delay > 0; time2delay--)
    {
    }
}

