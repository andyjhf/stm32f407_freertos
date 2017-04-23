#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
 
//����������������	   
//STM32F4����-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	 

//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
 
  GPIO_InitStructure.Pin = GPIO_PIN_3|GPIO_PIN_4; //KEY0 KEY1 ��Ӧ����
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//��ͨ����ģʽ
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;//100M
  GPIO_InitStructure.Pull = GPIO_PULLUP;//����
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE,3,4
	
	 
  GPIO_InitStructure.Pin = GPIO_PIN_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.Pull = GPIO_PULLDOWN ;//����
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
uint8_t KEY_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
//		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(WK_UP==1)return 3;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}




















