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

/* 指向信号量的指针 */
rt_sem_t usart2_sem = RT_NULL;
rt_sem_t usart1_sem = RT_NULL;
/* 指向互斥量的指针 */
rt_mutex_t sim_contect_mutex = RT_NULL;

/* 事件控制块 */
struct rt_event event;
		/* 线程句柄声明*/
//static struct rt_thread thread_clent1;
//static struct rt_thread thread_clent2;

/* 线程堆栈声明 */
//static rt_uint8_t thread_clent1_stack[ 2048 ];
//static rt_uint8_t thread_clent2_stack[ 2048 ];
#define SERVER_HOST_1   "106.13.60.139"
#define SERVER_PORT_1   8000

#define SERVER_HOST_2   "106.13.60.139"
#define SERVER_PORT_2   8090


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




///* define MQTT client context */
//static MQTTClient client;



char *senddate = "hello rt_thread !";

//重新建立连接，并且设置互斥量，当服务器有链接的情况下此时不能进行连接
//当多链路进行连接时需要先获取设备信息，载创建相应结构体，此时就可以开始连接服务器的操作
//当连接上服务器后进行必要的数据通信后，将关闭连接，以供其他设备使用，连接断开后释放互斥量
/* clent1 线程函数入口 */
void thread_clent_1_entry(void* parameter)
{
	
		int ret = -1;
		char *rsp;
		static char senddate[] = "hello!";
		static char sendbuff[] = "rt_thread!";

		
   	while(1)
		{
				//获取互斥量
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

/* clent2 线程函数入口 */
void thread_clent_2_entry(void* parameter)
{
		int ret;
		char recvbuf[128] = {0};
		uint8_t buffdate[128] = {1,2,3,4,5,6,7,8,9,11};
		char data[20] = "nice to meet you!";
		int sockfd = -1;
		rt_uint32_t e;
		struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    struct netdev *netdev = RT_NULL; 
		
		struct at_socket *socket_01;
		
		
	
		while(1)
		{
				//获取互斥量
				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
				while(1)
				{
						/* 通过名称获取 netdev 网卡对象 */
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
						
						/* 初始化需要绑定的客户端地址 */
						client_addr.sin_family = AF_INET;
						client_addr.sin_port = htons(8080);
						/* 获取网卡对象中 IP 地址信息 */
						client_addr.sin_addr.s_addr = netdev->ip_addr.addr;
						rt_memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));

						if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
						{
								rt_kprintf("socket bind failed.\n");
								closesocket(sockfd);
						}
						rt_kprintf("socket bind network interface device(%s) success!\n", netdev->name);

						/* 初始化预连接的服务端地址 */
						server_addr.sin_family = AF_INET;
						server_addr.sin_port = htons(SERVER_PORT_1);
						server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST_1);
						rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
						/* 连接到服务端 */
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
				//可以进行互相通信，存在问题，串口数据帧的区分
				while(1)
				{
						
						if(rt_event_recv(&event, EVENT_FLAG_port1,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)
						{

							//在这需要获取串口的使用权，只有当通讯结束之后才释放串口，（串口1和串口2 均需要）
							
							//获得串口的使用权
							rt_sem_take(usart1_sem,RT_WAITING_FOREVER);
							rt_sem_take(usart2_sem,RT_WAITING_FOREVER);
							
							//rt_kprintf("enter port1_2 enent success!");
							ret = recv(sockfd,recvbuf,512,0);
							if(ret < 0)
							{
								/* 关闭连接 */
								closesocket(sockfd);
				
								break;
							}
							else
							{
								//rt_kprintf(" thread_01 receive date = %s\r\n",recvbuf);
								//发送数据
								ret = send(sockfd,data,rt_strlen(data),0);
								if(ret < 0)
								{
									closesocket(sockfd);
					
									break;
								}
								
								//通过串口将数据发送出去
							  rt_usart1_sendbuff((uint8_t *)recvbuf);
								
								//等待RTU返回的数据
								if(rt_event_recv(&event, EVENT_FLAG_usart1,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)//调试时更改为  RT_WAITING_FOREVER
								{
									//处理返回数据 数据接收成功则发送数据到服务器
									//rt_kprintf("enter rtu date receive event success!");
									ret = rt_usart1_receive();
									if(ret == RT_EOK)
									{
										//将返回数据发送到服务器
										//发送数据
										ret = send(sockfd,usart1_rec_buff,usart1_rec_length,0);
										//ret = send(sockfd,data,rt_strlen(data),0);
										if(ret < 0)
										{
											closesocket(sockfd);
											
											break;
										}
										usart1_rec_length = 0;
									}
									
								}
								else
								{
									//超时返回超时错误  串口返回数据超时
									/* 关闭连接 */
									closesocket(sockfd);
									break;
								}
								
								//
			
							}
						}
						rt_sem_release(usart1_sem);
						rt_sem_release(usart2_sem);
				}
				//释放信号量和互斥量
				rt_mutex_release(sim_contect_mutex);
//				rt_sem_release(usart1_sem);
//				rt_sem_release(usart2_sem);
				rt_thread_delay(200);

		}

		
}



/* clent2 线程函数入口 */
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
		uint8_t buffdate[128] = {1,2,3,4,5,6,7,8,9};
		
		struct at_socket *socket_02;
	
		while(1)
		{
				//获取互斥量
				rt_mutex_take(sim_contect_mutex, RT_WAITING_FOREVER);
				while(1)
				{
						/* 通过名称获取 netdev 网卡对象 */
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

						/* 初始化需要绑定的客户端地址 */
						client_addr.sin_family = AF_INET;
						client_addr.sin_port = htons(8010);
						/* 获取网卡对象中 IP 地址信息 */
						client_addr.sin_addr.s_addr = netdev->ip_addr.addr;
						rt_memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));

						if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
						{
								rt_kprintf("socket bind failed.\n");
								closesocket(sockfd);
						}
						rt_kprintf("socket bind network interface device(%s) success!\n", netdev->name);

						/* 初始化预连接的服务端地址 */
						server_addr.sin_family = AF_INET;
						server_addr.sin_port = htons(SERVER_PORT_2);
						server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST_2);
						rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
						/* 连接到服务端 */
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

							//获得串口的使用权
							rt_sem_take(usart1_sem,RT_WAITING_FOREVER);
							rt_sem_take(usart2_sem,RT_WAITING_FOREVER);
							
							//rt_kprintf("enter port1_2 enent success!");
							ret = recv(sockfd,recvbuf,128,0);
							if(ret < 0)
							{
								/* 关闭连接 */
								closesocket(sockfd);
								break;
							}
							else
							{
								//rt_kprintf(" thread_01 receive date = %s\r\n",recvbuf);
								//发送数据
								ret = send(sockfd,data,rt_strlen(data),0);
								if(ret < 0)
								{
									closesocket(sockfd);
									break;
								}
								
								//通过串口将数据发送出去
							  rt_usart1_sendbuff((uint8_t *)recvbuf);
								
								//等待RTU返回的数据
								if(rt_event_recv(&event, EVENT_FLAG_usart1,
                RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
                RT_WAITING_FOREVER, &e) == RT_EOK)//调试时更改为  RT_WAITING_FOREVER
								{
									//处理返回数据 数据接收成功则发送数据到服务器
									//rt_kprintf("enter rtu date receive event success!");
									ret = rt_usart1_receive();
									if(ret == RT_EOK)
									{
										//将返回数据发送到服务器
										//发送数据
										ret = send(sockfd,usart1_rec_buff,usart1_rec_length,0);
										//ret = send(sockfd,data,rt_strlen(data),0);
										if(ret < 0)
										{
											closesocket(sockfd);
											break;
										}
										usart1_rec_length = 0;
									}
									
								}
								else
								{
									//超时返回超时错误
									/* 关闭连接 */
									closesocket(sockfd);
									break;
								}
								//
							}
						}
		
				}
				//未接收到事件标志，释放互斥量，延时一段时间等待其他事件进入
				rt_mutex_release(sim_contect_mutex);
				//rt_thread_delay(200);
	
				
		}

		
}

int main(void)
{
    /* user app entry */
	  rt_err_t result;
	  rt_event_t ret;
	
		//modbus();
		time_config();
		/* 创建一个动态信号量，初始值是 1 */
		//用来抢占usart2的使用权
    usart2_sem = rt_sem_create("dsem", 1, RT_IPC_FLAG_FIFO);
    if (usart2_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
				return -1;
    }
		//用来抢占usart1的使用权
		usart1_sem = rt_sem_create("dsem", 1, RT_IPC_FLAG_FIFO);
    if (usart2_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
				return -1;
    }
		/* 创建一个动态互斥量 */
    sim_contect_mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);
    if (sim_contect_mutex == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
        return -1;
    }
		/* 创建一个事件标志*/
		/* 初始化事件对象 */
    result = rt_event_init(&event,"event", RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("init event failed.\n");
        return -1;
    }
		
		
//		/* 初始化 clent 1 thread */
//		result = rt_thread_init(&thread_clent1,
//														"clent1",
//														thread_clent_1_entry, 
//														RT_NULL,
//														(rt_uint8_t*)&thread_clent1_stack[0], 
//														sizeof(thread_clent1_stack), 
//														30, 
//														5);
//		if (result == RT_EOK)
//		{
//				rt_thread_startup(&thread_clent1);
//		}
//		
//		/* 初始化 clent 2 thread */
//		result = rt_thread_init(&thread_clent2,
//														"clent2",
//														thread_clent_2_entry, 
//														RT_NULL,
//														(rt_uint8_t*)&thread_clent2_stack[0], 
//														sizeof(thread_clent2_stack), 
//														30, 
//														5);
//		if (result == RT_EOK)
//		{
//				rt_thread_startup(&thread_clent2);
//		}
    else
        return -1;
}
