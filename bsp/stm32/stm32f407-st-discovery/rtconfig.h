#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */
#include "stdint.h"
/* RT-Thread Kernel */

#define RT_NAME_MAX 8
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 256
#define RT_USING_TIMER_SOFT
#define RT_TIMER_THREAD_PRIO 4
#define RT_TIMER_THREAD_STACK_SIZE 512
#define RT_DEBUG
#define RT_DEBUG_COLOR

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_SMALL_MEM
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 1024
#define RT_CONSOLE_DEVICE_NAME "uart1"
#define RT_VER_NUM 0x40002
#define ARCH_ARM
#define RT_USING_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10

/* C++ features */


/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 4096
#define FINSH_CMD_SIZE 80
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_USING_MSH_ONLY
#define FINSH_ARG_MAX 10

/* Device virtual file system */

#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define DFS_FILESYSTEMS_MAX 2
#define DFS_FILESYSTEM_TYPES_MAX 2
#define DFS_FD_MAX 16
#define RT_USING_DFS_DEVFS

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 1024
#define RT_USING_SYSTEM_WORKQUEUE
#define RT_SYSTEM_WORKQUEUE_STACKSIZE 2048
#define RT_SYSTEM_WORKQUEUE_PRIORITY 23
#define RT_USING_SERIAL
//#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 512
#define RT_USING_PIN

/* Using USB */


/* POSIX layer and C standard library */

#define RT_USING_LIBC
#define RT_USING_POSIX

/* Network */

/* Socket abstraction layer */

#define RT_USING_SAL

/* protocol stack implement */

#define SAL_USING_AT
#define SAL_USING_POSIX

/* Network interface device */

#define RT_USING_NETDEV
#define NETDEV_USING_IFCONFIG
#define NETDEV_USING_PING
#define NETDEV_USING_NETSTAT
#define NETDEV_USING_AUTO_DEFAULT
#define NETDEV_IPV4 1
#define NETDEV_IPV6 0

/* light weight TCP/IP stack */


/* AT commands */

#define RT_USING_AT
#define AT_DEBUG
#define AT_USING_CLIENT
#define AT_CLIENT_NUM_MAX 4
#define AT_USING_SOCKET
#define AT_USING_CLI
#define AT_PRINT_RAW_CMD
#define AT_CMD_MAX_LEN 512
#define AT_SW_VERSION_NUM 0x10300

/* VBUS(Virtual Software BUS) */


/* Utilities */


/* RT-Thread online packages */

/* IoT - internet of things */

#define PKG_USING_PAHOMQTT
#define PAHOMQTT_PIPE_MODE
#define PKG_USING_PAHOMQTT_EXAMPLE
#define RT_PKG_MQTT_THREAD_STACK_SIZE 4096
#define PKG_PAHOMQTT_SUBSCRIBE_HANDLERS 3
#define MQTT_DEBUG
#define PKG_USING_PAHOMQTT_V110

/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */

#define PKG_USING_AT_DEVICE
#define AT_DEVICE_USING_EC20
#define AT_DEVICE_EC20_INIT_ASYN
#define AT_DEVICE_EC20_SAMPLE
#define EC20_SAMPLE_POWER_PIN -1
#define EC20_SAMPLE_STATUS_PIN -1
#define EC20_SAMPLE_CLIENT_NAME "uart2"
#define EC20_SAMPLE_RECV_BUFF_LEN 1024
#define PKG_USING_AT_DEVICE_V203
#define PKG_AT_DEVICE_VER_NUM 0x20003

/* IoT Cloud */


/* security packages */


/* language packages */
extern int connest_01_socket;
extern int connest_02_socket;
extern uint8_t thread_socket_flag;
extern uint8_t usart1_rec_buff[1024];
extern uint16_t usart1_rec_length;

extern uint8_t find_usart1_date_flag;

/* multimedia packages */
//UART_HandleTypeDef  huart1
//UART_HandleTypeDef  huart2
//UART_HandleTypeDef  huart3
//UART_HandleTypeDef  huart6
/* tools packages */
extern struct rt_event event;

//事件标志

#define EVENT_FLAG_usart2 (1 << 3)
#define EVENT_FLAG_mqtt   (1 << 4)
#define EVENT_FLAG_port1  (1 << 5)
#define EVENT_FLAG_port2  (1 << 6)
#define EVENT_FLAG_usart1 (1 << 7)
#define EVENT_FLAG_usart_clent1   (1 << 8)
#define EVENT_FLAG_usart_clent2   (1 << 9)
#define EVENT_FLAG_usart_clent3   (1 << 10)

/* system packages */


/* peripheral libraries and drivers */


/* miscellaneous packages */


/* samples: kernel and components samples */

#define SOC_FAMILY_STM32
#define SOC_SERIES_STM32F4

/* Hardware Drivers Config */

#define SOC_STM32F407VG

/* Onboard Peripheral Drivers */

/* On-chip Peripheral Drivers */

#define BSP_USING_GPIO
#define BSP_USING_UART

#define BSP_USING_UART1
#define BSP_UART1_TX_PIN  "PA9"
#define BSP_UART1_RX_PIN  "PA10"

#define BSP_USING_UART2
#define BSP_UART2_TX_PIN  "PA2"
#define BSP_UART2_RX_PIN  "PA3"

#define BSP_USING_UART3
#define BSP_UART3_TX_PIN  "PB10"
#define BSP_UART3_RX_PIN  "PB11"

#define BSP_USING_UART6
#define BSP_UART6_TX_PIN  "PC6"
#define BSP_UART6_RX_PIN  "PC7"


enum
{
#ifdef BSP_USING_UART1
    UART1_INDEX,
#endif
#ifdef BSP_USING_UART2
    UART2_INDEX,
#endif
#ifdef BSP_USING_UART3
    UART3_INDEX,
#endif
#ifdef BSP_USING_UART4
    UART4_INDEX,
#endif
#ifdef BSP_USING_UART5
    UART5_INDEX,
#endif
#ifdef BSP_USING_UART6
    UART6_INDEX,
#endif
#ifdef BSP_USING_UART7
    UART7_INDEX,
#endif
#ifdef BSP_USING_UART8
    UART8_INDEX,
#endif
#ifdef BSP_USING_LPUART1
    LPUART1_INDEX,
#endif
};


/* Board extended module Drivers */


#endif
