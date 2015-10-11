/**
  ******************************************************************************
  * @file    TIM_PWMOutput/main.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This example shows how to configure the TIM peripheral in PWM 
  *         (Pulse Width Modulation) mode.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stm32f4xx_spi.h>
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_PWMOutput
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//CanTxMsg TxMessage;
CanTxMsg canMessage;
CanRxMsg RxMessage;
 int flag=0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void config_gpio(void);
void config_timer(void);
 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
	
	void init_myspi(void){
		
		
				
			GPIO_InitTypeDef  GPIO_Lcd;
	GPIO_InitTypeDef  GPIO_Spi;
	SPI_InitTypeDef SPI_InitStruct;
		
		
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
		
		
		
		  GPIO_Lcd.GPIO_Pin = GPIO_Pin_4;
  GPIO_Lcd.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Lcd.GPIO_OType = GPIO_OType_PP;
  GPIO_Lcd.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Lcd.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_Lcd);
	
  GPIO_Lcd.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_Lcd);
 
	
	
	GPIO_Spi.GPIO_Pin = GPIO_Pin_2| GPIO_Pin_6;
	GPIO_Spi.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Spi.GPIO_OType = GPIO_OType_PP;
	GPIO_Spi.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Spi.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_Spi);
	
	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	
	/* configure SPI1 in Mode 0 
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI4, &SPI_InitStruct); 
	
	SPI_Cmd(SPI4, ENABLE); // enable SPI1

		
	}
	
void init_SPI4(void){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* configure pins used by SPI4
	 * PA5 = SCK
	 * PA6 = MISO
	 * PA7 = MOSI
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	
	  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, 1); 
	
	
	// connect SPI4 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_SPI4);
	

	
	// enable peripheral clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
	
	/* configure SPI4 in Mode 0 
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI4, &SPI_InitStruct); 
	
	SPI_Cmd(SPI4, ENABLE); // enable SPI4
}
	
	
void SPI4_send(uint16_t data){
GPIO_WriteBit(GPIOC, GPIO_Pin_2, 0); 
 GPIO_WriteBit(GPIOG, GPIO_Pin_14, 1); 
 while (SPI_I2S_GetFlagStatus(SPI4, SPI_I2S_FLAG_TXE) == RESET) {}; 
	 SPI_I2S_SendData(SPI4,data);
	
	
	GPIO_WriteBit(GPIOG, GPIO_Pin_14, 0); 
	GPIO_WriteBit(GPIOC, GPIO_Pin_2, 1); 
}


int main(void)
{ int bot=0 ,i=0;
	SystemInit();
  config_gpio();
	config_timer();
	//config_can();
  //prvCANtwoInit();
 
	//init_SPI4();
	init_myspi();
	
	

		SPI4_send(0x0C00); //desliga
			 
		SPI4_send(0x0900); //no decode

		SPI4_send(0x0A08); //intensidade 	
		SPI4_send(0x0B07); 	//scan limit  pinos todos
	  SPI4_send(0x0C01); //liga 
		SPI4_send(0x04FF);
		

		//SPI_send(0x0F00);
		SPI4_send(0x0600);
	
	
	while (1)
  {
		
		
		
		
		
		   // GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    bot=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		
	 //canMessage.Data[0] = 0x55;
			if(bot==1 && flag==1){
				GPIO_WriteBit(GPIOG, GPIO_Pin_14, 1); 
		 
			GPIO_WriteBit(GPIOG, GPIO_Pin_14,0);
				i++;
				canMessage.Data[0] = i;
				flag=0;
			}
		
	}
}

 

 

 


void config_gpio(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* GPIOD Configuration: TIM4 CH1 (PD12), TIM4 CH2 (PD13), TIM4 CH3 (PD14) and TIM4 CH4 (PD15) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOG, &GPIO_InitStructure); 
	
	
	
	
	
}

void config_timer(void){
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period=4999;//auto­reload de 4999
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=7199; //prescaler de 7199
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); //Enviar a estrutura preenchida
	
	TIM_Cmd(TIM2, ENABLE); //Ligar timer apos config

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Interrupção global do TIM3 com prioridade 0 sub-prioridade 2 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable da interrupção do Update Event do TIM2 */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );



	

	
}







#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
