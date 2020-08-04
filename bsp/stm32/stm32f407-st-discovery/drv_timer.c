
#include "board.h"
#include "uart_config.h"

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
	TIM2_Handler.Init.Prescaler = 71; // Ԥ��Ƶ
	TIM2_Handler.Init.RepetitionCounter = 0x00; // �����ظ�����
	HAL_TIM_Base_Init(&TIM2_Handler); // ��ʼ����ʱ��

	HAL_TIM_Base_Start_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE
//	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж� 
	
	#endif
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
		{
			HAL_NVIC_SetPriority(TIM2_IRQn,1,2);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
			//HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
		}
}
uint16_t count = 0;
uint8_t count_sec = 0;
//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{

		count++;
		if(count ==5000)
		{
				stop_timer();
				
				count = 0;

				//���ݽ������ �����¼���־
				if(usart1_rec_length > 0)
				{
					rt_event_send(&event,EVENT_FLAG_usart1);
				}
		}

}

void start_timer(void)
{
	HAL_TIM_Base_Start_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж� 
}

void stop_timer(void)
{
	HAL_TIM_Base_Stop_IT(&TIM2_Handler);
	HAL_NVIC_DisableIRQ(TIM2_IRQn);          //�ر�tim2�ж�
}

void restart_timer(void)
{
	HAL_TIM_Base_Stop_IT(&TIM2_Handler);
	HAL_NVIC_DisableIRQ(TIM2_IRQn);          //�ر�tim2�ж�

	HAL_TIM_Base_Start_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM2�ж� 
}
