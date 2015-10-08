/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 14/05/2015 17:09:52
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_spi.h"

#include "tm_stm32f3_fonts.h"
#include "tm_stm32f3_ili9341.h"
#include "delay.h"
#include "youbeep_3.h" 

#define led3_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_9  , GPIO_PIN_SET		);
#define led3_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_9  , GPIO_PIN_RESET	);
#define led4_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_8  , GPIO_PIN_SET		);
#define led4_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_8  , GPIO_PIN_RESET	);
#define led5_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_10 , GPIO_PIN_SET		);
#define led5_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_10 , GPIO_PIN_RESET	);
#define led6_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_15 , GPIO_PIN_SET		);
#define led6_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_15 , GPIO_PIN_RESET	);
#define led7_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_11 , GPIO_PIN_SET		);
#define led7_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_11 , GPIO_PIN_RESET	);
#define led8_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_14 , GPIO_PIN_SET		);
#define led8_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_14 , GPIO_PIN_RESET	);
#define led9_h  HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_12 , GPIO_PIN_SET		);
#define led9_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_12 , GPIO_PIN_RESET	);
#define led10_h HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_13 , GPIO_PIN_SET		);
#define led10_l	HAL_GPIO_WritePin ( GPIOE , GPIO_PIN_13 , GPIO_PIN_RESET	);

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	TM_Delay_Init();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	
  MX_SPI1_Init();

  // CODE
	// Initialize ILI9341
	TM_ILI9341_Init();
	// Rotate LCD for 90 degrees 
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_1);
	//TM_ILI9341_Fill( ILI9341_COLOR_YOUBEEP);
	//TM_ILI9341_Puts(10,10,"Funca",&TM_Font_7x10,ILI9341_COLOR_BLACK,ILI9341_TRANSPARENT);
	drawbmp(youbeep_3,TM_ILI9341_Image_Horizontal);
	
	TM_ILI9341_Puts(10,10,"isto e uma string com paragrafo",&TM_Font_16x26,ILI9341_COLOR_WHITE,ILI9341_TRANSPARENT);
	while (1)
  {
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_15);
		TM_DelayMillis(500);
  }

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  __SYSCFG_CLK_ENABLE();

}

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;								//default LOW
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;										//default 1Edge
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLED;
  HAL_SPI_Init(&hspi1);
	
	
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA11   ------> USB_DM
     PA12   ------> USB_DP
     PB6   ------> I2C1_SCL
     PB7   ------> I2C1_SDA
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PE2 PE4 PE5 PE0 
                           PE1 */
  /*GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_0 
                          |GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);*/

  /*Configure GPIO pins : PE3 PE8 PE9 PE10 
                           PE11 PE12 PE13 PE14 
                           PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PA1 - CS | PA2 - DC | PA3 - RST  */
	GPIO_InitStruct.Pin = GPIO_PIN_1 |GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 */
  /*GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF14_USB;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/

  /*Configure GPIO pins : PB6 PB7 */
  /*GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);*/

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
