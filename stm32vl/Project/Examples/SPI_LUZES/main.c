/**
  ******************************************************************************
  * @file    Examples/GPIOToggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Main program body.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "../../../../Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_spi.h"
#include "../../../../Libraries/STM32F10x_StdPeriph_Driver/inc/stm32f10x_tim.h"
//#include "defines.h"
//#include "../../../../Libraries/STM32F10x_StdPeriph_Driver/inc/tm_stm32f4_ili9341.h"
//#include "../../../../Libraries/STM32F10x_StdPeriph_Driver/inc/tm_stm32f4_fonts.h"

/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;

int i;
 int flag=0;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	
	void max7219_init(void){
 
		




SPI_InitTypeDef SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_StructInit (& GPIO_InitStructure);
SPI_StructInit (& SPI_InitStructure);
 
		
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  , ENABLE );
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  , ENABLE );
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE );
		
		GPIO_DeInit(GPIOA);
		
		    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
		
		//led debug
				GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_8 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		
		
		//iniciar pinos
		//MOSI PA7
		//miso PA6
		//sck PA5
		//cs PB12
		
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;  //cs
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		
		 //desativar o cs (negado)
		
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7  | GPIO_Pin_5; //mosi clk
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING; //miso
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
		
	GPIO_WriteBit(GPIOA, GPIO_Pin_5,0);	
		
SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;
SPI_InitStructure.SPI_CRCPolynomial = 7;
 SPI_I2S_DeInit(SPI1);
SPI_Init(SPI1,&SPI_InitStructure);
SPI_Cmd(SPI1,ENABLE);

	
}
	
	
	
uint8_t SPI_send(uint8_t adress ,uint8_t data)
{
int rev;	
	Delay(250000);
    GPIO_WriteBit(GPIOB, GPIO_Pin_2,0);
		GPIO_WriteBit(GPIOC, GPIO_Pin_9,1);
	
 
 
 
while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
SPI_I2S_SendData(SPI1, adress);
while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
SPI_I2S_ReceiveData(SPI1); //Clear RXNE bit
 
while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
SPI_I2S_SendData(SPI1, data); //Dummy byte to generate clock
while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
 
	rev= SPI_I2S_ReceiveData(SPI1);


	
		GPIO_WriteBit(GPIOC, GPIO_Pin_9,0);
    GPIO_WriteBit(GPIOB, GPIO_Pin_2,1);
		
return  rev;
}






void anima(void){
	
	i++;

		SPI_send(0x04,0x18);  
	 SPI_send(0x05,0x18); 

	
		SPI_send(0x03,0x3C);  
	 SPI_send(0x04,0x3C);
		SPI_send(0x05,0x3C);  
	 SPI_send(0x06,0x3C);
	

		SPI_send(0x02,0x7E);  
		SPI_send(0x03,0x7E);  
	 SPI_send(0x04,0x7E);
		SPI_send(0x05,0x7E);  
	 SPI_send(0x06,0x7E);
		SPI_send(0x07,0x7E);

	
		SPI_send(0x01,0xFF);
	SPI_send(0x02,0xFF);  
		SPI_send(0x03,0xFF);  
	 SPI_send(0x04,0xFF);
		SPI_send(0x05,0xFF);  
	 SPI_send(0x06,0xFF);
			 SPI_send(0x07,0xFF);
			 	SPI_send(0x08,0xFF);
	
	
	
	

	
	
	
		SPI_send(0x04,0xE7);  
	 SPI_send(0x05,0xE7); 
			

		SPI_send(0x03,0xC3);  
	 SPI_send(0x04,0xC3);
		SPI_send(0x05,0xC3);  
	 SPI_send(0x06,0xC3);


		SPI_send(0x02,0x81);  
		SPI_send(0x03,0x81);  
	 SPI_send(0x04,0x81);
		SPI_send(0x05,0x81);  
	 SPI_send(0x06,0x81);
		SPI_send(0x07,0x81);

	
		SPI_send(0x01,0x00);
	SPI_send(0x02,0x00);  
		SPI_send(0x03,0x00);  
	 SPI_send(0x04,0x00);
		SPI_send(0x05,0x00);  
	 SPI_send(0x06,0x00);
			 SPI_send(0x07,0x00);
			 	SPI_send(0x08,0x00);
	

	
}
	


void config_timer(void){
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period=60999;//auto­reload de 4999
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=60199; //prescaler de 7199
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


int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
		//SystemInit();
	
		max7219_init();
		 GPIO_WriteBit(GPIOB, GPIO_Pin_2,1);
				//SPI_send(0x0F00);
	//while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_RXNE) == RESET);
		//while (SPI_I2S_GetFlagStatus(SPI1 , SPI_I2S_FLAG_TXE) == RESET);
Delay(10000000);
		SPI_send(0x0C,0x00); //desliga
Delay(100000);
		SPI_send(0x09,0x00); //no decode

		//SPI_send(0x0A0F); //intensidade 	
		SPI_send(0x0A,0x02);

		SPI_send(0x0B,0x07); 	//scan limit  pinos todos

	  SPI_send(0x0C,0x01); //liga 

////////////////////////////////// config cima


//desliga a matrix
	  SPI_send(0x01,0x00);  

	  SPI_send(0x02,0x00);  

				  SPI_send(0x03,0x00);  
 
				  SPI_send(0x04,0x00);  
 
				  SPI_send(0x05,0x00);  
 
				  SPI_send(0x06,0x00);  
 
				  SPI_send(0x07,0x00);  
 
				  SPI_send(0x08,0x00);  
 
			
			
			
			 SPI_send(0x0F,0x01); 
 
			SPI_send(0x0F,0x00); 
 
				
			

			anima();


 


			//SPI_send(0x0204);  
			//Delay(10000000);
		
		//SPI_send(0x0400);
		
		Delay(10000);
		//SPI_send(0x0F00);
		//SPI_send(0x0600);
		
	GPIO_WriteBit(GPIOC, GPIO_Pin_8,1);
	
  while (1)
  {
    
			 anima();
		
		//GPIO_WriteBit(GPIOC, GPIO_Pin_9,0);
	//MAX7219 MAX7219 	MAX7219_writeData(0x08,0x0f);

  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
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

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
