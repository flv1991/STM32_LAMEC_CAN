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
void config_can(void);
void prvCANoneInit( void );
void prvCANtwoInit( void );
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
	
	
	
int main(void)
{ int bot=0 ,i=0;
	SystemInit();
  config_gpio();
	config_timer();
	//config_can();
  //prvCANtwoInit();
	prvCANoneInit();
	while (1)
  {
		
		   // GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    bot=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		
	 //canMessage.Data[0] = 0x55;
			if(bot==1 && flag==1){
				GPIO_WriteBit(GPIOG, GPIO_Pin_14, 1); 
		CAN_Transmit(CAN1, &canMessage);
			GPIO_WriteBit(GPIOG, GPIO_Pin_14,0);
				i++;
				canMessage.Data[0] = i;
				flag=0;
			}
		
	}
}


void prvCANtwoInit( void )
{
   GPIO_InitTypeDef       GPIO_InitStructure;
   CAN_InitTypeDef       CAN_InitStructure;
   CAN_FilterInitTypeDef     CAN_FilterInitStructure;
   NVIC_InitTypeDef       NVIC_InitStructure;


   /* Enable GPIO clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

   /* Connect CAN 1 pins to AF9 */
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

   /* Configure CAN 1 RX and TX pins */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* CAN 1 cell init */
   CAN_InitStructure.CAN_Prescaler = 21; //64
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
   CAN_Init( CAN2, &CAN_InitStructure );

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
   canMessage.StdId = 236;
   canMessage.ExtId = 0;
   canMessage.RTR = CAN_RTR_DATA;
   canMessage.IDE = CAN_ID_STD;
   canMessage.DLC = 1;

   canMessage.Data[0] = 165;
	 //canMessage.Data = 5;
   }


void prvCANoneInit( void )
{
   GPIO_InitTypeDef       GPIO_InitStructure;
   CAN_InitTypeDef       CAN_InitStructure;
   CAN_FilterInitTypeDef     CAN_FilterInitStructure;
   NVIC_InitTypeDef       NVIC_InitStructure;


   /* Enable GPIO clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

   /* Connect CAN 1 pins to AF9 */
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
   GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

   /* Configure CAN 1 RX and TX pins */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

   /* CAN 1 cell init */
   CAN_InitStructure.CAN_Prescaler = 21; //64
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
   CAN_Init( CAN1, &CAN_InitStructure );

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
   canMessage.StdId = 236;
   canMessage.ExtId = 0;
   canMessage.RTR = CAN_RTR_DATA;
   canMessage.IDE = CAN_ID_STD;
   canMessage.DLC = 2;

   canMessage.Data[0] = 222;
	 //canMessage.Data = 5;
   }


void config_can(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
  /* Configure CAN1 and CAN2 IOs **********************************************/
	
	//can1
	//RX = PA11
	//TX = PA12
	//can2  
	//RX = PB12
	//TX = PB13
	
	
	

	
  /* GPIOB, GPIOD and AFIO clocks enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
     
  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_AF_CAN1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_AF_CAN2;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure CAN1 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_AF_CAN1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure CAN2 TX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_AF_CAN2;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Remap CAN1 and CAN2 GPIOs */
  //GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);
  //GPIO_PinRemapConfig(GPIO_Remap_CAN2, ENABLE);

  /* Configure CAN1 and CAN2 **************************************************/  
  /* CAN1 and CAN2 Periph clocks enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);  
  
   /* CAN1 and CAN2 register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);

  /* Struct init*/
  CAN_StructInit(&CAN_InitStructure);

  /* CAN1 and CAN2  cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;
  CAN_InitStructure.CAN_AWUM = ENABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = ENABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
  CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
 

  CAN_InitStructure.CAN_Prescaler =6;


  
  /*Initializes the CAN1  and CAN2 */
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_Init(CAN2, &CAN_InitStructure);

 
  
  /* Transmit */
  /*TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;  
	
	*/
	
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
