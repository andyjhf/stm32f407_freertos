#include "RangeFinder.h"

typedef struct
{
	uint8_t		state;
	uint32_t	timer_count;
	uint32_t	range;
}RangeSensor;

typedef enum
{
	IDLE=0U,
	RUNNING,
	FINISHED,
}RangeState;

TIM_HandleTypeDef Tim3Handle;

uint32_t uwPrescalerValue = 0;

RangeSensor mRangeSensor;

uint32_t Range = 0xffffffff;

static void Timer_init(void);
static void Timer_start(void);
static void Timer_stop(void);
static void GPIO_Config(void);

void TIM3_IRQHandler(void)
{
	/* TIM Update event */
  if(__HAL_TIM_GET_FLAG(&Tim3Handle, TIM_FLAG_UPDATE) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(&Tim3Handle, TIM_IT_UPDATE) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(&Tim3Handle, TIM_IT_UPDATE);
      mRangeSensor.timer_count++;
    }
  }
}

void EXTI15_10_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
  {
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
    if(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_15) == GPIO_PIN_SET)
		{
			Timer_stop();
			Timer_start();
		}else
		{
			Timer_stop();
			
		}
		
		
  }

}

void Rangefinder_init(void)
{
	/****************timer*********************/
	Timer_init();

	/****************gpio**********************/
	GPIO_Config();
}

static void Delay_Us(uint16_t myUs)   
{
  uint16_t i;
  while(myUs--)
  {
    i = 15;
    while(i--);
  }
}

static void Timer_init(void)
{
	/* TIMx Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();
	/* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
  uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

  /* Set TIMx instance */
  Tim3Handle.Instance = TIM3;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = ((SystemCoreClock / 2)/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  Tim3Handle.Init.Period            = 10000 - 1;
  Tim3Handle.Init.Prescaler         = uwPrescalerValue;
  Tim3Handle.Init.ClockDivision     = 0;
  Tim3Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Tim3Handle.Init.RepetitionCounter = 0;

  if (HAL_TIM_Base_Init(&Tim3Handle) != HAL_OK)
  {
    /* Initialization Error */
//    Error_Handler();
  }

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set Interrupt Group Priority */ 
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

}

static void Timer_start(void)
{
	mRangeSensor.timer_count = 0;
	
	if (HAL_TIM_Base_Start_IT(&Tim3Handle) != HAL_OK)
  {
    /* Starting Error */
//    Error_Handler();
  }

}

static void Timer_stop(void)
{

	if (HAL_TIM_Base_Stop_IT(&Tim3Handle) != HAL_OK)
  {
    /* Starting Error */
//    Error_Handler();
  }
}	


static void GPIO_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOC clock */
  __HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Enable and set EXTI lines 15 to 10 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
}


