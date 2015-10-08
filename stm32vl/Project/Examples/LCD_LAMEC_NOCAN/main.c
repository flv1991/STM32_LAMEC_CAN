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
#include "defines.h"
#include <stdint.h>
#include "../../../../Libraries/STM32F10x_StdPeriph_Driver/inc/tm_stm32f4_ili9341.h"
#include "../../../../Libraries/STM32F10x_StdPeriph_Driver/inc/tm_stm32f4_fonts.h"
#include "..\..\..\..\Libraries\STM32F10x_StdPeriph_Driver\src\stm32f10x_tim.c"

/* DEFINES ------------------------------------------*/


/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void gpiosonar (void);
void timer (void);
void RCC_Configuration(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
			SystemInit();
	
		TM_ILI9341_Init();
	
	/* Rotate LCD for 90 degrees */
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
	
	
	TM_ILI9341_Fill( ILI9341_COLOR_CYAN);
	
	TM_ILI9341_DrawFilledCircle(60, 60, 35, ILI9341_COLOR_RED);
	
	RCC_Configuration();
	
	gpiosonar();
	timer();
	
	
	 

  while (1)
  {
   


		
		
		
		
		

  }
}



void RCC_Configuration(void)
{
  /* Enable peripheral clocks ------------------------------------------------*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); // sonar
	 RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE ); //timer
}






//configuraçao do pinos para o sonar
void gpiosonar(void){
	
	GPIO_InitTypeDef  GPIO_sonar;
	GPIO_InitTypeDef GPIO_InitStructure;
	//trigger configurado como saida
	//echo como entrada

	
	
	GPIO_sonar.GPIO_Pin = GPIO_Pin_0;
	GPIO_sonar.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_sonar.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_sonar);

	GPIO_sonar.GPIO_Pin = GPIO_Pin_1;  // TIM3_CH4 EXTI1
	GPIO_sonar.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_sonar.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_sonar);
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9; //pino 9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//inicia o GPIOB9 como saida para PWM

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 ; //pino 0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //modo AF(alternated funcion) para o botão no GPIOA0 poder operar em modo gated
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);//inicia o GPIOA0 como Entrada ligada ao timer2 
	
}

//configraçao do timer

void timer(void){
		NVIC_InitTypeDef NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);


	//TIMER2 configurado com uma freq de 2Hz
	TIM_TimeBaseStructure.TIM_Period=2499;//auto­reload de 2499
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //nao tem divisao de clk
	TIM_TimeBaseStructure.TIM_Prescaler=7199; //prescaler de 7199
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //modo de contagem crescente
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure); //Enviar a estrutura preenchida
	TIM_Cmd(TIM4, ENABLE); //Ligar timer apos config

	/* ------------------------------------------------------- */

	/* Configura o Priority Group com 1 bit */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	//Interrupção do timer 2 a 250ms
	/* Interrupção global do TIM4 com prioridade 0 sub-prioridade 2 */
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable da interrupção do Update Event do TIM4 */
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE );


	//Interrupção externa ligada no botao PA0
	/* Interrupção global do TIM4 com prioridade 0 sub-prioridade 2 */
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable da interrupção do Update Event do TIM4 */
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE );


	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;//ligado no PA0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //rising define que quando o botao passa de 0 para 1 a interrupçao acontece.
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);//Enviar a estrutura preenchida
	
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
