#include "stm32f4xx_hal.h"

#define PWM_CHANNEL_3 TIM_CHANNEL_3
#define PWM_CHANNEL_4 TIM_CHANNEL_4

void pwm1_init(void);
int pwm1_start(char channel ,char speed);
int pwm1_stop(char channel);	





