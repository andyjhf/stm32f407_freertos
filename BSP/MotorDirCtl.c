#include "MotorDirCtl.h"

void motor_dir_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
}

void motor_dir_ctl(char motor, char dir)
{
	if(motor == MOTOR_RIGHT)
	{
		if(dir == MOTOR_FORWARD)	
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
		}
	}
	else if(motor == MOTOR_LEFT)
	{
		if(dir == MOTOR_FORWARD)	
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
		}	
	}
}
	
	