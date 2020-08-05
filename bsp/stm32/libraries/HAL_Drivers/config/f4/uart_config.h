/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     SummerGift   first version
 * 2019-01-03     zylx         modify dma support
 */

#ifndef __UART_CONFIG_H__
#define __UART_CONFIG_H__

#include <rtthread.h>
#include <board.h>


#ifdef __cplusplus
extern "C" {
#endif



#if defined(BSP_USING_UART1)
#ifndef UART1_CONFIG
#define UART1_CONFIG                                                \
    {                                                               \
        .name = "uart1",                                            \
        .Instance = USART1,                                         \
        .irq_type = USART1_IRQn,                                    \
        .tx_pin_name = BSP_UART1_TX_PIN,                            \
        .rx_pin_name = BSP_UART1_RX_PIN,                            \
    }
#endif /* UART1_CONFIG */

#if defined(BSP_UART1_RX_USING_DMA)
#ifndef UART1_DMA_RX_CONFIG
#define UART1_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART1_RX_DMA_INSTANCE,                          \
        .channel = UART1_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART1_RX_DMA_RCC,                                \
        .dma_irq = UART1_RX_DMA_IRQ,                                \
    }
#endif /* UART1_DMA_RX_CONFIG */
#endif /* BSP_UART1_RX_USING_DMA */

#if defined(BSP_UART1_TX_USING_DMA)
#ifndef UART1_DMA_TX_CONFIG
#define UART1_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART1_TX_DMA_INSTANCE,                          \
        .channel = UART1_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART1_TX_DMA_RCC,                                \
        .dma_irq = UART1_TX_DMA_IRQ,                                \
    }
#endif /* UART1_DMA_TX_CONFIG */
#endif /* BSP_UART1_TX_USING_DMA */
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
#ifndef UART2_CONFIG
#define UART2_CONFIG                                                \
    {                                                               \
        .name = "uart2",                                            \
        .Instance = USART2,                                         \
        .irq_type = USART2_IRQn,                                    \
        .tx_pin_name = BSP_UART2_TX_PIN,                            \
        .rx_pin_name = BSP_UART2_RX_PIN,                            \
    }
#endif /* UART2_CONFIG */

#if defined(BSP_UART2_RX_USING_DMA)
#ifndef UART2_DMA_RX_CONFIG
#define UART2_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART2_RX_DMA_INSTANCE,                          \
        .channel = UART2_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART2_RX_DMA_RCC,                                \
        .dma_irq = UART2_RX_DMA_IRQ,                                \
    }
#endif /* UART2_DMA_RX_CONFIG */
#endif /* BSP_UART2_RX_USING_DMA */

#if defined(BSP_UART2_TX_USING_DMA)
#ifndef UART2_DMA_TX_CONFIG
#define UART2_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART2_TX_DMA_INSTANCE,                          \
        .channel = UART2_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART2_TX_DMA_RCC,                                \
        .dma_irq = UART2_TX_DMA_IRQ,                                \
    }
#endif /* UART2_DMA_TX_CONFIG */
#endif /* BSP_UART2_TX_USING_DMA */
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
#ifndef UART3_CONFIG
#define UART3_CONFIG                                                \
    {                                                               \
        .name = "uart3",                                            \
        .Instance = USART3,                                         \
        .irq_type = USART3_IRQn,                                    \
        .tx_pin_name = BSP_UART3_TX_PIN,                            \
        .rx_pin_name = BSP_UART3_RX_PIN,                            \
    }
#endif /* UART3_CONFIG */

#if defined(BSP_UART3_RX_USING_DMA)
#ifndef UART3_DMA_RX_CONFIG
#define UART3_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART3_RX_DMA_INSTANCE,                          \
        .channel = UART3_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART3_RX_DMA_RCC,                                \
        .dma_irq = UART3_RX_DMA_IRQ,                                \
    }
#endif /* UART3_DMA_RX_CONFIG */
#endif /* BSP_UART3_RX_USING_DMA */

#if defined(BSP_UART3_TX_USING_DMA)
#ifndef UART3_DMA_TX_CONFIG
#define UART3_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART3_TX_DMA_INSTANCE,                          \
        .channel = UART3_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART3_TX_DMA_RCC,                                \
        .dma_irq = UART3_TX_DMA_IRQ,                                \
    }
#endif /* UART3_DMA_TX_CONFIG */
#endif /* BSP_UART3_TX_USING_DMA */
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
#ifndef UART4_CONFIG
#define UART4_CONFIG                                                \
    {                                                               \
        .name = "uart4",                                            \
        .Instance = UART4,                                          \
        .irq_type = UART4_IRQn,                                     \
        .tx_pin_name = BSP_UART4_TX_PIN,                            \
        .rx_pin_name = BSP_UART4_RX_PIN,                            \
    }
#endif /* UART4_CONFIG */

#if defined(BSP_UART4_RX_USING_DMA)
#ifndef UART4_DMA_RX_CONFIG
#define UART4_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART4_RX_DMA_INSTANCE,                          \
        .channel = UART4_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART4_RX_DMA_RCC,                                \
        .dma_irq = UART4_RX_DMA_IRQ,                                \
    }
#endif /* UART4_DMA_RX_CONFIG */
#endif /* BSP_UART4_RX_USING_DMA */

#if defined(BSP_UART4_TX_USING_DMA)
#ifndef UART4_DMA_TX_CONFIG
#define UART4_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART4_TX_DMA_INSTANCE,                          \
        .channel = UART4_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART4_TX_DMA_RCC,                                \
        .dma_irq = UART4_TX_DMA_IRQ,                                \
    }
#endif /* UART4_DMA_TX_CONFIG */
#endif /* BSP_UART4_RX_USING_DMA */
#endif /* BSP_USING_UART4 */

#if defined(BSP_USING_UART5)
#ifndef UART5_CONFIG
#define UART5_CONFIG                                                \
    {                                                               \
        .name = "uart5",                                            \
        .Instance = UART5,                                          \
        .irq_type = UART5_IRQn,                                     \
        .tx_pin_name = BSP_UART5_TX_PIN,                            \
        .rx_pin_name = BSP_UART5_RX_PIN,                            \
    }
#endif /* UART5_CONFIG */

#if defined(BSP_UART5_RX_USING_DMA)
#ifndef UART5_DMA_RX_CONFIG
#define UART5_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART5_RX_DMA_INSTANCE,                          \
        .channel = UART5_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART5_RX_DMA_RCC,                                \
        .dma_irq = UART5_RX_DMA_IRQ,                                \
    }
#endif /* UART5_DMA_RX_CONFIG */
#endif /* BSP_UART5_RX_USING_DMA */

#if defined(BSP_UART5_TX_USING_DMA)
#ifndef UART5_DMA_TX_CONFIG
#define UART5_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART5_TX_DMA_INSTANCE,                          \
        .channel = UART5_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART5_TX_DMA_RCC,                                \
        .dma_irq = UART5_TX_DMA_IRQ,                                \
    }
#endif /* UART5_DMA_TX_CONFIG */
#endif /* BSP_UART5_TX_USING_DMA */
#endif /* BSP_USING_UART5 */

#if defined(BSP_USING_UART6)
#ifndef UART6_CONFIG
#define UART6_CONFIG                                                \
    {                                                               \
        .name = "uart6",                                            \
        .Instance = USART6,                                         \
        .irq_type = USART6_IRQn,                                    \
        .tx_pin_name = BSP_UART6_TX_PIN,                            \
        .rx_pin_name = BSP_UART6_RX_PIN,                            \
    }
#endif /* UART6_CONFIG */

#if defined(BSP_UART6_RX_USING_DMA)
#ifndef UART6_DMA_RX_CONFIG
#define UART6_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART6_RX_DMA_INSTANCE,                          \
        .channel = UART6_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART6_RX_DMA_RCC,                                \
        .dma_irq = UART6_RX_DMA_IRQ,                                \
    }
#endif /* UART6_DMA_RX_CONFIG */
#endif /* BSP_UART6_RX_USING_DMA */

#if defined(BSP_UART6_TX_USING_DMA)
#ifndef UART6_DMA_TX_CONFIG
#define UART6_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART6_TX_DMA_INSTANCE,                          \
        .channel = UART6_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART6_TX_DMA_RCC,                                \
        .dma_irq = UART6_TX_DMA_IRQ,                                \
    }
#endif /* UART6_DMA_TX_CONFIG */
#endif /* BSP_UART6_TX_USING_DMA */
#endif /* BSP_USING_UART6 */

#if defined(BSP_USING_UART7)
#ifndef UART7_CONFIG
#define UART7_CONFIG                                                \
    {                                                               \
        .name = "uart7",                                            \
        .Instance = UART7,                                          \
        .irq_type = UART7_IRQn,                                     \
        .tx_pin_name = BSP_UART7_TX_PIN,                            \
        .rx_pin_name = BSP_UART7_RX_PIN,                            \
    }
#endif /* UART7_CONFIG */

#if defined(BSP_UART7_RX_USING_DMA)
#ifndef UART7_DMA_RX_CONFIG
#define UART7_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART7_RX_DMA_INSTANCE,                          \
        .channel = UART7_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART7_RX_DMA_RCC,                                \
        .dma_irq = UART7_RX_DMA_IRQ,                                \
    }
#endif /* UART7_DMA_RX_CONFIG */
#endif /* BSP_UART7_RX_USING_DMA */

#if defined(BSP_UART7_TX_USING_DMA)
#ifndef UART7_DMA_TX_CONFIG
#define UART7_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART7_TX_DMA_INSTANCE,                          \
        .channel = UART7_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART7_TX_DMA_RCC,                                \
        .dma_irq = UART7_TX_DMA_IRQ,                                \
    }
#endif /* UART7_DMA_TX_CONFIG */
#endif /* BSP_UART7_TX_USING_DMA */
#endif /* BSP_USING_UART7 */

#if defined(BSP_USING_UART8)
#ifndef UART8_CONFIG
#define UART8_CONFIG                                                \
    {                                                               \
        .name = "uart8",                                            \
        .Instance = UART8,                                          \
        .irq_type = UART8_IRQn,                                     \
        .tx_pin_name = BSP_UART8_TX_PIN,                            \
        .rx_pin_name = BSP_UART8_RX_PIN,                            \
    }
#endif /* UART8_CONFIG */

#if defined(BSP_UART8_RX_USING_DMA)
#ifndef UART8_DMA_RX_CONFIG
#define UART8_DMA_RX_CONFIG                                         \
    {                                                               \
        .Instance = UART8_RX_DMA_INSTANCE,                          \
        .channel = UART8_RX_DMA_CHANNEL,                            \
        .dma_rcc = UART8_RX_DMA_RCC,                                \
        .dma_irq = UART8_RX_DMA_IRQ,                                \
    }
#endif /* UART8_DMA_RX_CONFIG */
#endif /* BSP_UART8_RX_USING_DMA */

#if defined(BSP_UART8_TX_USING_DMA)
#ifndef UART8_DMA_TX_CONFIG
#define UART8_DMA_TX_CONFIG                                         \
    {                                                               \
        .Instance = UART8_TX_DMA_INSTANCE,                          \
        .channel = UART8_TX_DMA_CHANNEL,                            \
        .dma_rcc = UART8_TX_DMA_RCC,                                \
        .dma_irq = UART8_TX_DMA_IRQ,                                \
    }
#endif /* UART8_DMA_TX_CONFIG */
#endif /* BSP_UART8_TX_USING_DMA */
#endif /* BSP_USING_UART8 */


void rt_usart1_sendbuff(uint8_t *sendbuff,uint16_t date_length );
int rt_usart1_receive(void);
void thread_usart1_entry(void* parameter);


typedef  struct _config_info_{
    uint16_t ID;  //����ID 
	
		uint16_t rtu_id;
	
		//��������
		struct usart_config {
			int usart_BaudRate;    //������
			uint16_t usart_parity;      //����
			uint16_t usart_stopbits;    //ֹͣλ
			uint16_t usart_datebits;    //����λ
		}__attribute__((packed)) usart_config_t[4];//�ֱ�Ϊ����1,2,3,6��������Ϣ
	 
		//��������������
		struct clent_config {
			uint16_t clent_id;         //���ӷ�������ID
			char clent_ip[15];             //ip��ַ
			uint16_t clent_port;       //���ӵĶ˿ں�
		}__attribute__((packed)) clent_config_t[3];//�ֱ�Ϊclent1,2,3��������Ϣ

		//mqtt1 ������Ϣ
		struct mqtt_config {
			uint16_t mqtt_id;           //mqtt���Ӷ�Ӧ��ID
			char mqtt_uri[27];          //URI : "TCP://IP:PORT"
			char mqtt_SUBTopicNAME[20];     //mqtt���ĵ�����
			char mqtt_PUBTopicNAME[20];     //mqtt����������
			char mqtt_username[10];         //�û���
			char mqtt_password[10];         //����    �û������������ڼ�������
		}__attribute__((packed)) mqtt_config_t[3];//�ֱ�Ϊ����1,2,3,6��������Ϣ
		
		//��������
		uint16_t relink_time;        //��������ʱ��
		uint16_t heard_beat_time;    //������ʵ��
		uint16_t timeout_tx;         //���䳬ʱʱ��
		char heard_beat_date[10];        //�������ַ�
		uint16_t MQTT_SUBSCRIBE_HANDLERS ; //mqtt���������ӵ��������
		
}__attribute__((packed)) config_info_t;




#ifdef __cplusplus
}
#endif

#endif
