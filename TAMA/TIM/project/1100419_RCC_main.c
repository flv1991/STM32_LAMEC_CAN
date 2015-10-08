#include "stm32f10x_lib.h"
#include <stdint.h>

extern void lcd_info(void);
void delay( uint32_t time2delay);
void RCC_Config_HSI_default(void){
    RCC_DeInit();
    /*configurar clock interno a 8Mhz*/
    RCC_HSICmd(ENABLE); // ou RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2);
   RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
//garantir que o clock está correcto
   while(RCC_GetSYSCLKSource()!=0x00);

}

void RCC_Config_HSI_PLL_Max(void){
RCC_DeInit();
FLASH_SetLatency(FLASH_Latency_2);

/*configurar clock a 64Mhz (max)*/




//divisão do prescaler AHB por 1
RCC_HCLKConfig(RCC_SYSCLK_Div1);
//divisão do prescaler APB1 por 2
RCC_PCLK1Config(RCC_HCLK_Div2);
//divisão do prescaler APB2 por 1
RCC_PCLK2Config(RCC_HCLK_Div1);
//é usado a PLL com a source HSI (clock interno) e 16 de factor multiplicativo
RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);

//depois de configurado a PLL fazer enable
RCC_PLLCmd(ENABLE);
while(RCC_GetFlagStatus( RCC_FLAG_PLLRDY)==RESET);

RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//garantir que o clock da pll está correcto
while(RCC_GetSYSCLKSource()!=0x08);




}

void RCC_Config_HSE_Default(void){
RCC_DeInit();
/*configurar clock externo a 12Mhz */
//bypass pois está ligado no OSC_IN um circuito oscilatorio externo
RCC_HSEConfig(RCC_HSE_Bypass);
//Após a ativação do HSE deveremos esperar até que o sinal de relógio esteja pronto e estável
ErrorStatus HSEStartUpStatus;
HSEStartUpStatus=RCC_WaitForHSEStartUp();

if(HSEStartUpStatus==SUCCESS) /*retorna SUCCESS ou ERROR*/
{

//divisão do prescaler AHB por 1
RCC_HCLKConfig(RCC_SYSCLK_Div1);
//divisão do prescaler APB1 por 1
RCC_PCLK1Config(RCC_HCLK_Div1);
//divisão do prescaler APB2 por 1
RCC_PCLK2Config(RCC_HCLK_Div1);

    //arrancar com o clock externo
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    //confirmar que o clock está correcto logo esperar que a fonte de relógio esteja correta
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


void RCC_Config_30MHz(void){
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
    RCC_PCLK1Config(RCC_HCLK_Div1);
    //divisão do prescaler APB2 por 1
    RCC_PCLK2Config(RCC_HCLK_Div1);
    //configuar a PLL para 30Mhz
    RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_5);
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
{   uint8_t cnt=0, flag=0;
    RCC_Config_30MHz();
    /* Enable GPIOB clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );

    /* GPIO configuration */
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_DeInit(GPIOB);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    lcd_info();
    while (1)
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
        delay(1000000);
        GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
        GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
        delay(1000000);
    
    
    
        cnt++;
        if (cnt==10) //ocorre quando o led pisca 10x
            {   if(flag==0) //flag indica a freq de clk flag=0 clk max flag=1 clk=30
                {
                RCC_Config_HSE_PLL_Max();
                lcd_info();
                cnt=0;
                flag=1;
                }
                else 
                {
                RCC_Config_30MHz();
                lcd_info();
                cnt=0;
                flag=0;
                }
            }*/
    }
    retutn (0);
}


void delay(volatile uint32_t time2delay)
{
    for ( ; time2delay > 0; time2delay--)
    {
    }
}

