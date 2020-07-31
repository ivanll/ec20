
#include "board.h"

TIM_HandleTypeDef TIM2_Handler; // ���嶨ʱ����ʼ���ṹ�����

void time_config(void )
{
	#ifdef tim1_config
	
	__HAL_RCC_TIM2_CLK_ENABLE(); //ʹ��TIM3ʱ��
	
	TIM2_Handler.Channel = HAL_TIM_ACTIVE_CHANNEL_1; // ѡ��ʱ��ͨ��
	TIM2_Handler.Instance = TIM2; // ѡ��ʱ��1
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; // ��ʱ��ʱ�ӷ�Ƶ ����Ƶ
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_DOWN; //���¼���
	TIM2_Handler.Init.Period = 0xFFFFFFFF; // �Զ���װ��ֵ
	TIM2_Handler.Init.Prescaler = 335; // Ԥ��Ƶ
	TIM2_Handler.Init.RepetitionCounter = 0x00; // �����ظ�����
	HAL_TIM_Base_Init(&TIM2_Handler); // ��ʼ����ʱ��

	HAL_TIM_Base_Start_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE

	
	#endif
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
		{
			HAL_NVIC_SetPriority(TIM2_IRQn,1,1);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
			//HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
		}
}

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
	rt_kprintf("enter time2_irq  success!\r\n");
	for(uint8_t i=0;i<usart1_rec_length;i++)
	{
		rt_kprintf("usart1_rec_buff DATE = %d\r\n",usart1_rec_buff[i]);
		rt_kprintf("usart1_rec_length DATE = %d\r\n",usart1_rec_length);
	}
	usart1_rec_length = 0;
	stop_timer();
	
	//���ݽ������ �����¼���־
	rt_event_send(&event,EVENT_FLAG_usart1);

}

void start_timer(void)
{
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж� 
}

void stop_timer(void)
{
	HAL_NVIC_DisableIRQ(TIM2_IRQn);          //�ر�tim2�ж�
}

void restart_timer(void)
{
	HAL_NVIC_DisableIRQ(TIM2_IRQn);          //�ر�tim2�ж�
	
	//rt_thread_delay(100);
	
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж� 
}
