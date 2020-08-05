/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     misonyo   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "at.h"
#include "drv_usart.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdev.h>
#include "at_socket.h"
#include "at_device_ec20.h"
#include "sal_socket.h"
#include "uart_config.h"
#include "paho_mqtt.h"
#include "mb.h"


UART_HandleTypeDef  huart1;
UART_HandleTypeDef  huart2;
UART_HandleTypeDef  huart3;
UART_HandleTypeDef  huart6;

/* ָ���ź�����ָ�� */
rt_sem_t usart2_sem = RT_NULL;
rt_sem_t usart1_sem = RT_NULL;
/* ָ�򻥳�����ָ�� */
rt_mutex_t sim_contect_mutex = RT_NULL;

/* �¼����ƿ� */
struct rt_event event;
/* �߳̾������*/
static struct rt_thread thread_usart1;

/* �̶߳�ջ���� */
static rt_uint8_t thread_usart1_stack[ 4096 ];
#define SERVER_HOST_1   "106.13.60.139"
#define SERVER_PORT_1   8000

#define SERVER_HOST_2   "106.13.60.139"
#define SERVER_PORT_2   8090

char *serve1_ip = "106.13.60.139";
uint16_t serve1_port = 8000;

char *serve2_ip = "106.13.60.139";
uint16_t serve2_port = 8090;

char *mqtt1_uri = "tcp://106.13.60.139:1883";
char *mqtt1_usename = "admin";
char *mqtt1_password = "admin";
char *mqtt1_subtopic = "/mqtt/test";
char *mqtt1_pubtopic = "/mqtt/test";

char *mqtt2_uri = "tcp://106.13.60.139:1883";
char *mqtt2_usename = "admin";
char *mqtt2_password = "admin";
char *mqtt2_subtopic = "/mqtt/test_02";
char *mqtt2_pubtopic = "/mqtt/test_02";

#define MQTT_URI                "tcp://106.13.60.139:1883"
#define MQTT_USERNAME           "admin"
#define MQTT_PASSWORD           "admin"
#define MQTT_SUBTOPIC           "/mqtt/test"
#define MQTT_PUBTOPIC           "/mqtt/test"
#define MQTT_WILLMSG            "Goodbye!"

#define MQTT_SUBTOPIC_sec           "/mqtt/test_02"
#define MQTT_PUBTOPIC_sec           "/mqtt/test_02"

int connest_01_socket = 100;
int connest_02_socket = 100;

uint8_t usart1_rec_buff[1024] = {0};
uint16_t usart1_rec_length = 0;

uint8_t thread_socket_flag;



char *senddate = "hello rt_thread !";

//���½������ӣ��������û��������������������ӵ�����´�ʱ���ܽ�������
//������·��������ʱ��Ҫ�Ȼ�ȡ�豸��Ϣ���ش�����Ӧ�ṹ�壬��ʱ�Ϳ��Կ�ʼ���ӷ������Ĳ���
//�������Ϸ���������б�Ҫ������ͨ�ź󣬽��ر����ӣ��Թ������豸ʹ�ã����ӶϿ����ͷŻ�����
/* clent1 �̺߳������ */
void thread_clent_1_entry(void* parameter)
{
	
		int ret = -1;
		char *rsp;
		
   	while(1)
		{
				//��ȡ������
				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
			
				ret = mqtt_start(1);
			
				ret = mqtt_subscribe(1,rsp);
				if(ret == 0)
				{
					rt_kprintf ("topic  /mqtt/test sub success!");
				}
				rt_thread_delay(1000);
				
				ret = mqtt_subscribe(2,rsp);
				if(ret == 0)
				{
					rt_kprintf ("topic /mqtt/test_02 sub success!");
				}
				
				rt_mutex_release(sim_contect_mutex);
				rt_thread_delay(1000);
				while(1)
				{
					rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
//					ret = mqtt_publish(2,senddate);
//					if(ret == -1)
//					{
//							is_1_started =0;
//							mqtt_stop(1,rsp);
//							break;
//					}
//					rt_thread_delay(10000);
//					ret = mqtt_publish(3,sendbuff);
//					if(ret == -1)
//					{
//							is_1_started =0;
//							mqtt_stop(1,rsp);
//							break;
//					}
					rt_mutex_release(sim_contect_mutex);
					rt_thread_delay(1000);
				}
				//rt_mutex_release(sim_contect_mutex);
		}
		
}

/* clent2 �̺߳������ */
void thread_clent_2_entry(void* parameter)
{
		int ret;
		char recvbuf[128] = {0};
		char data[20] = "nice to meet you!";
		int sockfd = -1;
		rt_uint32_t e;
		struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    struct netdev *netdev = RT_NULL; 
		
		while(1)
		{
				//��ȡ������
				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
				while(1)
				{
						/* ͨ�����ƻ�ȡ netdev �������� */
					  thread_socket_flag = 2;
						netdev = netdev_get_by_name(EC20_SAMPLE_DEIVCE_NAME);
						if (netdev == RT_NULL)
						{
								rt_kprintf("get network interface device(%s) failed.\n", EC20_SAMPLE_DEIVCE_NAME);
						}

						if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
						{
								rt_kprintf("Socket create failed.\n");
						}
						
						/* ��ʼ����Ҫ�󶨵Ŀͻ��˵�ַ */
						client_addr.sin_family = AF_INET;
						client_addr.sin_port = htons(8080);
						/* ��ȡ���������� IP ��ַ��Ϣ */
						client_addr.sin_addr.s_addr = netdev->ip_addr.addr;
						rt_memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));

						if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
						{
								rt_kprintf("socket bind failed.\n");
								closesocket(sockfd);
						}
						rt_kprintf("socket bind network interface device(%s) success!\n", netdev->name);

						/* ��ʼ��Ԥ���ӵķ���˵�ַ */
						server_addr.sin_family = AF_INET;
						server_addr.sin_port = htons(SERVER_PORT_1);
						server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST_1);
						rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
						/* ���ӵ������ */
						if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
						{
								rt_kprintf("socket connect failed!\n");
								closesocket(sockfd);
								continue;
						}
						else
						{
							rt_kprintf("socket_thread02 connect success!\n");
							break;
						}
				}
				rt_mutex_release(sim_contect_mutex);
				
				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
				while(1)
				{
						
					  if(rt_event_recv(&event, EVENT_FLAG_port1,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)
						{

							//��ô��ڵ�ʹ��Ȩ
							rt_sem_take(usart1_sem,RT_WAITING_FOREVER);
							rt_sem_take(usart2_sem,RT_WAITING_FOREVER);
							
							//rt_kprintf("enter port1_2 enent success!");
							ret = recv(sockfd,recvbuf,1024,0);
							if(ret < 0)
							{
								/* �ر����� */
								closesocket(sockfd);
								break;
							}
							else
							{

//								//��������  ����ʹ��  
//								ret = send(sockfd,data,rt_strlen(data),0);
//								if(ret < 0)
//								{
//									closesocket(sockfd);
//									break;
//								}
								
								//ͨ�����ڽ����ݷ��ͳ�ȥ  �˴���Ҫ�����ݽ��д����modbus���ݰ�
							  rt_usart1_sendbuff((uint8_t *)recvbuf,rt_strlen(recvbuf));
								
								//�ȴ�RTU���ص�����  ���ڽ��յ����ݾ����жϺ������¼���־
								if(rt_event_recv(&event, EVENT_FLAG_usart_clent2,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)//����ʱ����Ϊ  RT_WAITING_FOREVER
								{
									//���������� ���ݽ��ճɹ��������ݵ�������

									//��������  �����ڽ��յ������ݷ��͵�������
									ret = send(sockfd,usart1_rec_buff,usart1_rec_length,0);
									if(ret < 0)
									{
										closesocket(sockfd);
										break;
									}
									//���ͳɹ��󽫻���������
									usart1_rec_length = 0;
									for(uint16_t i=0;usart1_rec_buff[i] == '\0';i++)
									{
										usart1_rec_buff[i] = 0;
									}
									
									
								}
								else
								{
									//��ʱ���س�ʱ����  ���ش������
									
									rt_sem_release(usart1_sem);
									rt_sem_release(usart2_sem);
									break;
								}
								//
							}
						}
		
				}
				//δ���յ��¼���־���ͷŻ���������ʱһ��ʱ��ȴ������¼�����
				rt_mutex_release(sim_contect_mutex);

		}

		
}



/* clent2 �̺߳������ */
void thread_clent_3_entry(void* parameter)
{
		int ret;
		char recvbuf[128] = {0};
		char data[20] = "hello rt_thread!";
		int sockfd = -1;
		rt_uint32_t e;
		struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    struct netdev *netdev = RT_NULL; 

	
		while(1)
		{
				//��ȡ������
				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
				while(1)
				{
						/* ͨ�����ƻ�ȡ netdev �������� */
						thread_socket_flag = 3;
						netdev = netdev_get_by_name(EC20_SAMPLE_DEIVCE_NAME);
						if (netdev == RT_NULL)
						{
								rt_kprintf("get network interface device(%s) failed.\n", EC20_SAMPLE_DEIVCE_NAME);
						}

						if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
						{
								rt_kprintf("Socket create failed.\n");
						}

						/* ��ʼ����Ҫ�󶨵Ŀͻ��˵�ַ */
						client_addr.sin_family = AF_INET;
						client_addr.sin_port = htons(8010);
						/* ��ȡ���������� IP ��ַ��Ϣ */
						client_addr.sin_addr.s_addr = netdev->ip_addr.addr;
						rt_memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));

						if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
						{
								rt_kprintf("socket bind failed.\n");
								closesocket(sockfd);
						}
						rt_kprintf("socket bind network interface device(%s) success!\n", netdev->name);

						/* ��ʼ��Ԥ���ӵķ���˵�ַ */
						server_addr.sin_family = AF_INET;
						server_addr.sin_port = htons(SERVER_PORT_2);
						server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST_2);
						rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
						/* ���ӵ������ */
						if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
						{
								rt_kprintf("socket connect failed!\n");
								closesocket(sockfd);
								continue;
						}
						else
						{
							rt_kprintf("socket_thread03 connect success!\n");
							break;
						}
				}
				rt_mutex_release(sim_contect_mutex);

				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
				while(1)
				{
						
					  if(rt_event_recv(&event, EVENT_FLAG_port2,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)
						{

							//��ô��ڵ�ʹ��Ȩ
							rt_sem_take(usart1_sem,RT_WAITING_FOREVER);
							rt_sem_take(usart2_sem,RT_WAITING_FOREVER);
							
							//rt_kprintf("enter port1_2 enent success!");
							ret = recv(sockfd,recvbuf,128,0);
							if(ret < 0)
							{
								/* �ر����� */
								closesocket(sockfd);
								break;
							}
							else
							{
								//rt_kprintf(" thread_01 receive date = %s\r\n",recvbuf);
								//��������
								ret = send(sockfd,data,rt_strlen(data),0);
								if(ret < 0)
								{
									closesocket(sockfd);
									break;
								}
								
								//ͨ�����ڽ����ݷ��ͳ�ȥ
							  rt_usart1_sendbuff((uint8_t *)recvbuf,rt_strlen(recvbuf));
								
								//�ȴ�RTU���ص�����  c���ڽ��յ����ݾ����ж�
								if(rt_event_recv(&event, EVENT_FLAG_usart_clent3,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)//����ʱ����Ϊ  RT_WAITING_FOREVER
								{
									//���������� ���ݽ��ճɹ��������ݵ�������
									//���������ݷ��͵�������
									//��������
									ret = send(sockfd,usart1_rec_buff,usart1_rec_length,0);
									
									if(ret < 0)
									{
										closesocket(sockfd);
										break;
									}
									
									usart1_rec_length = 0;
									for(uint16_t i=0;usart1_rec_buff[i] == '\0';i++)
									{
										usart1_rec_buff[i] = 0;
									}
									
									
								}
								else
								{
									//��ʱ���س�ʱ����  ���ش������
									rt_sem_release(usart1_sem);
									rt_sem_release(usart2_sem);
									//break;

								}
								//
							}
						}
						else  //��ʱ
						{
							rt_sem_release(usart1_sem);
							rt_sem_release(usart2_sem);
						}
		
				}
				//δ���յ��¼���־���ͷŻ���������ʱһ��ʱ��ȴ������¼�����
				rt_sem_release(usart1_sem);
				rt_sem_release(usart2_sem);
				rt_mutex_release(sim_contect_mutex);
				//rt_thread_delay(200);
	
				
		}

		
}

int main(void)
{
    /* user app entry */
	  rt_err_t result;

		time_config();
		/* ����һ����̬�ź�������ʼֵ�� 1 */
		//������ռusart2��ʹ��Ȩ
    usart2_sem = rt_sem_create("dsem", 1, RT_IPC_FLAG_FIFO);
    if (usart2_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
				return -1;
    }
		//������ռusart1��ʹ��Ȩ
		usart1_sem = rt_sem_create("dsem", 1, RT_IPC_FLAG_FIFO);
    if (usart2_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
				return -1;
    }
		/* ����һ����̬������ */
    sim_contect_mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);
    if (sim_contect_mutex == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
        return -1;
    }
		/* ����һ���¼���־*/
		/* ��ʼ���¼����� */
    result = rt_event_init(&event,"event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init event failed.\n");
        return -1;
    }
	/* ��ʼ�� usart1 thread �����������ݴ���*/
		result = rt_thread_init(&thread_usart1,
														"clent2",
														thread_usart1_entry, 
														RT_NULL,
														(rt_uint8_t*)&thread_usart1_stack[0], 
														sizeof(thread_usart1_stack), 
														30, 
														5);
		if (result == RT_EOK)
		{
				rt_thread_startup(&thread_usart1);
		}
		else
		{
			rt_kprintf("usart1_thread  error\r\n");
		}

}
