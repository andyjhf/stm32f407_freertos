#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f4xx_hal.h" 
//////////////////////////////////////////////////////////////////////////////////	 
 
//����������������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY0 		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) //PE4
#define KEY1 		HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)	//PE3 
#define WK_UP 	HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)	//PA0


/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/


void KEY_Init(void);	//IO��ʼ��
uint8_t KEY_Scan(uint8_t);  		//����ɨ�躯��	

#endif
