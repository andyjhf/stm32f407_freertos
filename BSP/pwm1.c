#include "pwm1.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM1
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define TIMx_GPIO_PORT_CHANNEL3        GPIOE
#define TIMx_GPIO_PORT_CHANNEL4        GPIOE
#define TIMx_GPIO_PIN_CHANNEL3         GPIO_PIN_13
#define TIMx_GPIO_PIN_CHANNEL4         GPIO_PIN_14
#define TIMx_GPIO_AF_CHANNEL3          GPIO_AF1_TIM1
#define TIMx_GPIO_AF_CHANNEL4          GPIO_AF1_TIM1

TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef sConfig;
/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;
/* Exported macro ------------------------------------------------------------*/
#define  PERIOD_VALUE       (uint32_t)(666 - 1)  /* Period Value  */
#define  PULSE9_VALUE       (uint32_t)(PERIOD_VALUE*95/100)				/* Capture Compare 1 Value  */
#define  PULSE8_VALUE       (uint32_t)(PERIOD_VALUE*90/100)				/* Capture Compare 2 Value  */
#define  PULSE7_VALUE       (uint32_t)(PERIOD_VALUE*85/100)       /* Capture Compare 3 Value  */
#define  PULSE6_VALUE       (uint32_t)(PERIOD_VALUE*80/100)				/* Capture Compare 4 Value  */
#define  PULSE5_VALUE       (uint32_t)(PERIOD_VALUE*75/100)				/* Capture Compare 1 Value  */
#define  PULSE4_VALUE       (uint32_t)(PERIOD_VALUE*70/100)				/* Capture Compare 2 Value  */
#define  PULSE3_VALUE       (uint32_t)(PERIOD_VALUE*65/100)				/* Capture Compare 3 Value  */
#define  PULSE2_VALUE       (uint32_t)(PERIOD_VALUE*60/100)				/* Capture Compare 4 Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE*55/100)				/* Capture Compare 1 Value  */

uint32_t speed_value[10] = {0, PULSE1_VALUE, PULSE2_VALUE, PULSE3_VALUE, PULSE4_VALUE, PULSE5_VALUE, PULSE6_VALUE, PULSE7_VALUE, PULSE8_VALUE, PULSE9_VALUE,};
/* Exported functions ------------------------------------------------------- */

static void Error_Handler(void)
{
  /* Turn LED3 on */
  while (1)
  {
  }
}

void pwm1_init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /* Enable all GPIO Channels Clock requested */
  TIMx_CHANNEL_GPIO_PORT();

  /* Configure PC.06 (pin 58 (PC6) in CN7 connector) (TIM8_Channel1), PC.07 (pin 56 (PC7) in CN7 connector) (TIM8_Channel2), PC.08 (pin 51 (PC8) in CN7 connector) (TIM8_Channel3),
     PC.09 (pin 46 (PC9) in CN7 connector) (TIM8_Channel4) in output, push-pull, alternate function mode
  */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL3;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL3;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL4;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL4;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);
	
	TimHandle.Instance = TIMx;

	/* Compute the prescaler value to have TIM8 counter clock equal to 16000000 Hz */
  uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000) - 1;

  TimHandle.Init.Prescaler         = uhPrescalerValue;
  TimHandle.Init.Period            = PERIOD_VALUE;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

int pwm1_start(char channel ,char speed)
{
	/* Common configuration for all channels */
  sConfig.OCMode       = TIM_OCMODE_PWM1;
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the pulse value for channel 1 */
	if(speed > 9)
		speed = 9;
	if(speed == 0)
	{
		pwm1_stop(channel);
		return 0;
	}
  sConfig.Pulse = speed_value[speed];
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK)
  {
    /* Configuration Error */
    return 2;
  }
	if (HAL_TIM_PWM_Start(&TimHandle, channel) != HAL_OK)
  {
    /* PWM Generation Error */
    return 3;
  }
	return 0;
}

int pwm1_stop(char channel)
{
	if (HAL_TIM_PWM_Stop(&TimHandle, channel) != HAL_OK)
  {
    /* PWM Generation Error */
    return 1;
  }
	return 0;
}







