/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     SummerGift   first version
 * 2020-05-23     chenyaxing   modify stm32_uart_config
 */
#include "string.h"
#include "stdlib.h"
#include "drv_common.h"
#include "uart_config.h"
#include "board.h"

uint8_t find_usart1_date_flag = 0;

static uint16_t usart1_count = 0;


//这些地址从40000开始  modbus寄存器
const  config_info_t config_info __attribute__((section(".ARM.__AT_0x08050000")))={
	
    .ID = 0X0A,
		.rtu_id = 0x01,
		
		//初始化串口配置数据
		.usart_config_t[0].usart_BaudRate = 115200,
		.usart_config_t[0].usart_parity = 0,
		.usart_config_t[0].usart_stopbits = 1,
		.usart_config_t[0].usart_datebits = 8,
	
		.usart_config_t[1].usart_BaudRate = 115200,
		.usart_config_t[1].usart_parity = 0,
		.usart_config_t[1].usart_stopbits = 1,
		.usart_config_t[1].usart_datebits = 8,
	
		.usart_config_t[2].usart_BaudRate = 115200,
		.usart_config_t[2].usart_parity = 0,
		.usart_config_t[2].usart_stopbits = 1,
		.usart_config_t[2].usart_datebits = 8,
	
		.usart_config_t[3].usart_BaudRate = 115200,
		.usart_config_t[3].usart_parity = 0,
		.usart_config_t[3].usart_stopbits = 1,
		.usart_config_t[3].usart_datebits = 8,
    
    //初始化服务器配置数据
		.clent_config_t[0].clent_id = 0x02,
		.clent_config_t[0].clent_ip ="106.13.60.139",
		.clent_config_t[0].clent_port = 8000,
		
		.clent_config_t[1].clent_id = 0x03,
		.clent_config_t[1].clent_ip ="106.13.60.139",
		.clent_config_t[1].clent_port = 8090,
		
		.clent_config_t[2].clent_id = 0x04,
		.clent_config_t[2].clent_ip ="106.13.60.139",
		.clent_config_t[2].clent_port = 8088,
		
		//初始化mqtt配置参数
		.mqtt_config_t[0].mqtt_id = 0x05,
		.mqtt_config_t[0].mqtt_uri = "tcp://106.13.60.139:1883",
		.mqtt_config_t[0].mqtt_SUBTopicNAME = "/mqtt/test",
		.mqtt_config_t[0].mqtt_PUBTopicNAME = "/mqtt/test",
		.mqtt_config_t[0].mqtt_username = "admin",
		.mqtt_config_t[0].mqtt_password = "admin",
		
		.mqtt_config_t[1].mqtt_id = 0x06,
		.mqtt_config_t[1].mqtt_uri = "tcp://106.13.60.139:1883",
		.mqtt_config_t[1].mqtt_SUBTopicNAME = "/mqtt/test",
		.mqtt_config_t[1].mqtt_PUBTopicNAME = "/mqtt/test",
		.mqtt_config_t[1].mqtt_username = "admin",
		.mqtt_config_t[1].mqtt_password = "admin",
		
		.mqtt_config_t[2].mqtt_id = 0x07,
		.mqtt_config_t[2].mqtt_uri = "tcp://106.13.60.139:1883",
		.mqtt_config_t[2].mqtt_SUBTopicNAME = "/mqtt/test",
		.mqtt_config_t[2].mqtt_PUBTopicNAME = "/mqtt/test",
		.mqtt_config_t[2].mqtt_username = "admin",
		.mqtt_config_t[2].mqtt_password = "admin",
		
		//初始化其他配置信息
		.relink_time = 1000,
		.heard_beat_time = 2000,
		.timeout_tx = 1000,
		.heard_beat_date = "asdasdasd",
		
};


#ifdef RT_USING_SERIAL

#include <rtdevice.h>

#endif
#if (defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32H7)) && defined(RT_SERIAL_USING_DMA)
#error "The STM32L0 and STM32H7 devices DO NOT support UART DMA feature."
#elif defined(RT_SERIAL_USING_DMA)
#include "dma_config.h"
#endif

//#define DRV_DEBUG
#define DBG_TAG              "drv.usart"
#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif /* DRV_DEBUG */
#include <rtdbg.h>

#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32F1) || defined(SOC_SERIES_STM32L4) \
    || defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32L1) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32G4)
#define DMA_INSTANCE_TYPE              DMA_Channel_TypeDef
#elif defined(SOC_SERIES_STM32F2) || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32H7)
#define DMA_INSTANCE_TYPE              DMA_Stream_TypeDef
#endif /*  defined(SOC_SERIES_STM32F1) || defined(SOC_SERIES_STM32L4) */

#if defined(SOC_SERIES_STM32F1) || defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32F2) \
    || defined(SOC_SERIES_STM32F4) || defined(SOC_SERIES_STM32L0) ||  defined(SOC_SERIES_STM32L1) \
    || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32G4)
#define UART_INSTANCE_CLEAR_FUNCTION    __HAL_UART_CLEAR_FLAG
#elif defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32H7)
#define UART_INSTANCE_CLEAR_FUNCTION    __HAL_UART_CLEAR_IT
#endif



/* stm32 config class */
struct stm32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;

    const char *tx_pin_name;
    const char *rx_pin_name;
};

/* stm32 uart dirver class */
struct stm32_uart
{
    UART_HandleTypeDef handle;
    struct stm32_uart_config *config;

    rt_uint16_t uart_dma_flag;
    struct rt_serial_device serial;
};

#if !defined(BSP_USING_UART1) && !defined(BSP_USING_UART2) && !defined(BSP_USING_UART3) && \
    !defined(BSP_USING_UART4) && !defined(BSP_USING_UART5) && !defined(BSP_USING_UART6) && \
    !defined(BSP_USING_UART7) && !defined(BSP_USING_UART8) && !defined(BSP_USING_LPUART1)
    #error "Please define at least one BSP_USING_UARTx"
    /* this driver can be disabled at menuconfig -> RT-Thread Components -> Device Drivers */
#endif



static struct stm32_uart_config uart_config[] =
{
#ifdef BSP_USING_UART1
    UART1_CONFIG,
#endif
#ifdef BSP_USING_UART2
    UART2_CONFIG,
#endif
#ifdef BSP_USING_UART3
    UART3_CONFIG,
#endif
#ifdef BSP_USING_UART4
    UART4_CONFIG,
#endif
#ifdef BSP_USING_UART5
    UART5_CONFIG,
#endif
#ifdef BSP_USING_UART6
    UART6_CONFIG,
#endif
#ifdef BSP_USING_UART7
    UART7_CONFIG,
#endif
#ifdef BSP_USING_UART8
    UART8_CONFIG,
#endif
#ifdef BSP_USING_LPUART1
    LPUART1_CONFIG,
#endif
};

static rt_err_t stm32_uart_clk_enable(struct stm32_uart_config *config)
{
    /* uart clock enable */
    switch ((uint32_t)config->Instance)
    {
#ifdef BSP_USING_UART1
    case (uint32_t)USART1:
        __HAL_RCC_USART1_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART1 */
#ifdef BSP_USING_UART2
    case (uint32_t)USART2:
        __HAL_RCC_USART2_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART2 */
#ifdef BSP_USING_UART3
    case (uint32_t)USART3:
        __HAL_RCC_USART3_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART3 */
#ifdef BSP_USING_UART4
#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32L0) || \
   defined(SOC_SERIES_STM32G0)
    case (uint32_t)USART4:
        __HAL_RCC_USART4_CLK_ENABLE();
#else
    case (uint32_t)UART4:
        __HAL_RCC_UART4_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART4 */
#ifdef BSP_USING_UART5
#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32L0) || \
   defined(SOC_SERIES_STM32G0)
    case (uint32_t)USART5:
        __HAL_RCC_USART5_CLK_ENABLE();
#else
    case (uint32_t)UART5:
        __HAL_RCC_UART5_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART5 */
#ifdef BSP_USING_UART6
    case (uint32_t)USART6:
        __HAL_RCC_USART6_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART6 */
#ifdef BSP_USING_UART7
#if defined(SOC_SERIES_STM32F0)
    case (uint32_t)USART7:
        __HAL_RCC_USART7_CLK_ENABLE();
#else
    case (uint32_t)UART7:
        __HAL_RCC_UART7_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART7 */
#ifdef BSP_USING_UART8
#if defined(SOC_SERIES_STM32F0)
    case (uint32_t)USART8:
        __HAL_RCC_USART8_CLK_ENABLE();
#else
    case (uint32_t)UART8:
        __HAL_RCC_UART8_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART8 */
#ifdef BSP_USING_LPUART1
    case (uint32_t)LPUART1:
       __HAL_RCC_LPUART1_CLK_ENABLE();
        break;
#endif /* BSP_USING_LPUART1 */
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t stm32_gpio_clk_enable(GPIO_TypeDef *gpiox)
{
    /* check the parameters */
    RT_ASSERT(IS_GPIO_ALL_INSTANCE(gpiox));

    /* gpio ports clock enable */
    switch ((uint32_t)gpiox)
    {
#if defined(__HAL_RCC_GPIOA_CLK_ENABLE)
    case (uint32_t)GPIOA:
        __HAL_RCC_GPIOA_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOB_CLK_ENABLE)
    case (uint32_t)GPIOB:
        __HAL_RCC_GPIOB_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOC_CLK_ENABLE)
    case (uint32_t)GPIOC:
        __HAL_RCC_GPIOC_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOD_CLK_ENABLE)
    case (uint32_t)GPIOD:
        __HAL_RCC_GPIOD_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOE_CLK_ENABLE)
    case (uint32_t)GPIOE:
        __HAL_RCC_GPIOE_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOF_CLK_ENABLE)
    case (uint32_t)GPIOF:
        __HAL_RCC_GPIOF_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOG_CLK_ENABLE)
    case (uint32_t)GPIOG:
        __HAL_RCC_GPIOG_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOH_CLK_ENABLE)
    case (uint32_t)GPIOH:
        __HAL_RCC_GPIOH_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOI_CLK_ENABLE)
    case (uint32_t)GPIOI:
        __HAL_RCC_GPIOI_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOJ_CLK_ENABLE)
    case (uint32_t)GPIOJ:
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOK_CLK_ENABLE)
    case (uint32_t)GPIOK:
        __HAL_RCC_GPIOK_CLK_ENABLE();
        break;
#endif
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}
static int up_char(char * c)
{
    if ((*c >= 'a') && (*c <= 'z'))
    {
        *c = *c - 32;
    }
    return 0;
}
static void get_pin_by_name(const char* pin_name, GPIO_TypeDef **port, uint16_t *pin)
{
    int pin_num = atoi((char*) &pin_name[2]);
    char port_name = pin_name[1];
    up_char(&port_name);
    up_char(&port_name);
    *port = ((GPIO_TypeDef *) ((uint32_t) GPIOA
            + (uint32_t) (port_name - 'A') * ((uint32_t) GPIOB - (uint32_t) GPIOA)));
    *pin = (GPIO_PIN_0 << pin_num);
}
static rt_err_t stm32_gpio_configure(struct stm32_uart_config *config)
{
    int uart_num = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_TypeDef *tx_port;
    GPIO_TypeDef *rx_port;
    uint16_t tx_pin;
    uint16_t rx_pin;
    uart_num = config->name[4] - '0';
    get_pin_by_name(config->rx_pin_name, &rx_port, &rx_pin);
    get_pin_by_name(config->tx_pin_name, &tx_port, &tx_pin);
    /* gpio ports clock enable */
    stm32_gpio_clk_enable(tx_port);
    if (tx_port != rx_port)
    {
        stm32_gpio_clk_enable(rx_port);
    }
    
    /* rx pin initialize */
    GPIO_InitStruct.Pin = tx_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#if defined(SOC_SERIES_STM32F2) || defined(SOC_SERIES_STM32F4) || \
    defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32G4) || \
    defined(SOC_SERIES_STM32L1) || defined(SOC_SERIES_STM32L4)
#define GPIO_AF7   ((uint8_t)0x07)
#define GPIO_AF8   ((uint8_t)0x08)
    /* uart1-3 -> AF7, uart4-8 -> AF8 */
    if (uart_num <= 3)
    {
        GPIO_InitStruct.Alternate = GPIO_AF7;
    }
    else
    {
        GPIO_InitStruct.Alternate = GPIO_AF8;
    }
#endif
    HAL_GPIO_Init(tx_port, &GPIO_InitStruct);

    /* rx pin initialize */
    GPIO_InitStruct.Pin = rx_pin;
    HAL_GPIO_Init(rx_port, &GPIO_InitStruct);

    return RT_EOK;
}

struct stm32_uart uart_obj[sizeof(uart_config) / sizeof(uart_config[0])] = {0};

static rt_err_t stm32_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct stm32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = rt_container_of(serial, struct stm32_uart, serial);

    /* uart clock enable */
    stm32_uart_clk_enable(uart->config);
    /* uart gpio clock enable and gpio pin init */
    stm32_gpio_configure(uart->config);

    uart->handle.Instance          = uart->config->Instance;
    uart->handle.Init.BaudRate     = cfg->baud_rate;
    uart->handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    uart->handle.Init.Mode         = UART_MODE_TX_RX;
    uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;
    switch (cfg->data_bits)
    {
    case DATA_BITS_8:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    case DATA_BITS_9:
        uart->handle.Init.WordLength = UART_WORDLENGTH_9B;
        break;
    default:
        uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
        break;
    }
    switch (cfg->stop_bits)
    {
    case STOP_BITS_1:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    case STOP_BITS_2:
        uart->handle.Init.StopBits   = UART_STOPBITS_2;
        break;
    default:
        uart->handle.Init.StopBits   = UART_STOPBITS_1;
        break;
    }
    switch (cfg->parity)
    {
    case PARITY_NONE:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    case PARITY_ODD:
        uart->handle.Init.Parity     = UART_PARITY_ODD;
        break;
    case PARITY_EVEN:
        uart->handle.Init.Parity     = UART_PARITY_EVEN;
        break;
    default:
        uart->handle.Init.Parity     = UART_PARITY_NONE;
        break;
    }

    if (HAL_UART_Init(&uart->handle) != HAL_OK)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t stm32_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct stm32_uart *uart;
#ifdef RT_SERIAL_USING_DMA
    rt_ubase_t ctrl_arg = (rt_ubase_t)arg;
#endif

    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    switch (cmd)
    {
    /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        NVIC_DisableIRQ(uart->config->irq_type);
        /* disable interrupt */
        __HAL_UART_DISABLE_IT(&(uart->handle), UART_IT_RXNE);
        break;
    /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
        NVIC_EnableIRQ(uart->config->irq_type);
        /* enable interrupt */
        __HAL_UART_ENABLE_IT(&(uart->handle), UART_IT_RXNE);
        break;

#ifdef RT_SERIAL_USING_DMA
    case RT_DEVICE_CTRL_CONFIG:
        stm32_dma_config(serial, ctrl_arg);
        break;
#endif
    }
    return RT_EOK;
}

static int stm32_putc(struct rt_serial_device *serial, char c)
{
    struct stm32_uart *uart;
    RT_ASSERT(serial != RT_NULL);

    uart = rt_container_of(serial, struct stm32_uart, serial);
    UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
#if defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32F0) \
    || defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7) \
    || defined(SOC_SERIES_STM32G4)
    uart->handle.Instance->TDR = c;
#else
    uart->handle.Instance->DR = c;
#endif
    while (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) == RESET);
    return 1;
}

static int stm32_getc(struct rt_serial_device *serial)
{
    int ch;
    struct stm32_uart *uart;
    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    ch = -1;
    if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
    {
#if defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32F0) \
    || defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7) \
    || defined(SOC_SERIES_STM32G4)
        ch = uart->handle.Instance->RDR & 0xff;
#else
        ch = uart->handle.Instance->DR & 0xff;
#endif
    }
    return ch;
}


static const struct rt_uart_ops stm32_uart_ops =
{
    .configure = stm32_configure,
    .control = stm32_control,
    .putc = stm32_putc,
    .getc = stm32_getc,
    //.dma_transmit = stm32_dma_transmit
};

uint16_t usart2_receive_flag = 0;
/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    struct stm32_uart *uart;


    RT_ASSERT(serial != RT_NULL);
    uart = rt_container_of(serial, struct stm32_uart, serial);

    /* UART in mode Receiver -------------------------------------------------*/
    if ((__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET) &&
            (__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_RXNE) != RESET))
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }
    else
    {
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_NE) != RESET)
        {
            __HAL_UART_CLEAR_NEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_FE) != RESET)
        {
            __HAL_UART_CLEAR_FEFLAG(&uart->handle);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_PE) != RESET)
        {
            __HAL_UART_CLEAR_PEFLAG(&uart->handle);
        }
#if !defined(SOC_SERIES_STM32L4) && !defined(SOC_SERIES_STM32F7) && !defined(SOC_SERIES_STM32F0) \
    && !defined(SOC_SERIES_STM32L0) && !defined(SOC_SERIES_STM32G0) && !defined(SOC_SERIES_STM32H7) \
    && !defined(SOC_SERIES_STM32G4)
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_LBD) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_LBD);
        }
#endif
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_CTS) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_CTS);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TXE) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TXE);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_TC) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_TC);
        }
        if (__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET)
        {
            UART_INSTANCE_CLEAR_FUNCTION(&(uart->handle), UART_FLAG_RXNE);
        }
    }
}


#if defined(BSP_USING_UART1)


void USART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
		struct rt_serial_device *serial = &(uart_obj[UART1_INDEX].serial);
		struct stm32_uart *uart;

	  //重启定时器
		restart_timer();
		
		RT_ASSERT(serial != RT_NULL);
		uart = rt_container_of(serial, struct stm32_uart, serial);

		/* UART in mode Receiver -------------------------------------------------*/
		if ((__HAL_UART_GET_FLAG(&(uart->handle), UART_FLAG_RXNE) != RESET) &&
						(__HAL_UART_GET_IT_SOURCE(&(uart->handle), UART_IT_RXNE) != RESET))
		{
			  
				usart1_rec_buff[usart1_count] = uart->handle.Instance->DR & 0xff;
				
		}

		usart1_count++;
		
		usart1_rec_length = usart1_count;
		
		__HAL_UART_CLEAR_FLAG(&(uart->handle), UART_FLAG_RXNE);


    /* leave interrupt */
    rt_interrupt_leave();
	
}
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
void USART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART2_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
void USART3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART3_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART3*/



#if defined(BSP_USING_UART6)
void USART6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&(uart_obj[UART6_INDEX].serial));

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART5*/


/**
  * @brief  UART error callbacks
  * @param  huart: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    RT_ASSERT(huart != NULL);
    struct stm32_uart *uart = (struct stm32_uart *)huart;
    LOG_D("%s: %s %d\n", __FUNCTION__, uart->config->name, huart->ErrorCode);
    UNUSED(uart);
}


int rt_hw_usart_init(void)
{
    rt_size_t obj_num = sizeof(uart_obj) / sizeof(struct stm32_uart);
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;
		struct stm32_uart *uart;
		//struct _config_info_ *info_config;
	  //config_info_t info_config;
	
		//读取配置信息
		//read_config();
    
    for (int i = 0; i < obj_num; i++)
    {
				if(i < 3)
				{
					config.baud_rate = config_info.usart_config_t[i].usart_BaudRate;
					config.parity = config_info.usart_config_t[i].usart_parity;
					config.data_bits = config_info.usart_config_t[i].usart_datebits;
					config.stop_bits = config_info.usart_config_t[i].usart_stopbits;
				}
				if(i == 5)
				{
					config.baud_rate = config_info.usart_config_t[3].usart_BaudRate;
					config.parity = config_info.usart_config_t[3].usart_parity;
					config.data_bits = config_info.usart_config_t[3].usart_datebits;
					config.stop_bits = config_info.usart_config_t[3].usart_stopbits;
				}
			  
        uart_obj[i].config = &uart_config[i];
        uart_obj[i].serial.ops    = &stm32_uart_ops;
        uart_obj[i].serial.config = config;
        /* register UART device */
        result = rt_hw_serial_register(&uart_obj[i].serial, uart_obj[i].config->name,
                                       RT_DEVICE_FLAG_RDWR
                                       | RT_DEVICE_FLAG_INT_RX
                                       | RT_DEVICE_FLAG_INT_TX
                                       , uart);
        RT_ASSERT(result == RT_EOK);
    }
		


    return result;
}


//串口1 发送数据  将数据打包成modbus数据包后进行发送
void rt_usart1_sendbuff(uint8_t *sendbuff,uint16_t date_length )
{
	  struct rt_serial_device *serial = &(uart_obj[UART1_INDEX].serial);
	
    RT_ASSERT(serial != RT_NULL);
		
		rt_interrupt_enter();
		
		//进行modbus数据打包
		_modbus_rtu_send_msg_pre(sendbuff,date_length);
		//发送数据
		for(uint8_t i=0;i<date_length;i++)
		{
			stm32_putc(serial, sendbuff[i]);
		}
		date_length = 0;

    /* leave interrupt */
    rt_interrupt_leave();
}




/* usart1线程函数入口 */
void thread_usart1_entry(void* parameter)
{
	rt_uint32_t e;
	uint32_t ret;
	struct rt_serial_device *serial = &(uart_obj[UART1_INDEX].serial);
	config_info_t info_config_rx;
//	struct stm32_uart *uart;
	RT_ASSERT(serial != RT_NULL);
	//定义modbus通信变量

	uint16_t mb_addr = 0;
	uint16_t mb_length = 0;
	uint16_t read_date_buff[300] = {0};
	
	uint8_t rx_date[1024] = {0};
	uint8_t tx_date[1024] = {0};

//  uart = rt_container_of(serial, struct stm32_uart, serial);
	//等待RTU返回的数据
	while(1)
	{
		if(rt_event_recv(&event, EVENT_FLAG_usart1,
		RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
		RT_WAITING_FOREVER, &e) == RT_EOK)//调试时更改为  RT_WAITING_FOREVER
		{
			
			usart1_count = 0;
			
			for(uint16_t i=0;i<usart1_rec_length;i++)
			{
				rx_date[i] = usart1_rec_buff[i];
			}
//			rt_usart1_sendbuff(rx_date,usart1_rec_length);
			if(usart1_rec_length > 0)//接收到数据
			{
				
//				rt_usart1_sendbuff(rx_date,usart1_rec_length);
				//进行数据校验  
				if(_modbus_rtu_check_integrity(&rx_date[0],  usart1_rec_length)==usart1_rec_length)
				{

					//对数据进行处理
//					rt_usart1_sendbuff(rx_date,usart1_rec_length);//调试时使用
					
					
					
					//判断是给哪个服务器的数据
					if(rx_date[0] == 0x01)
					{
						//数据接收完成 发送事件标志
						rt_event_send(&event,EVENT_FLAG_usart_clent1);
						
					}
					
					//判断是给哪个服务器的数据
					if(rx_date[0] == 0x02)
					{
						//数据接收完成 发送事件标志
						rt_event_send(&event,EVENT_FLAG_usart_clent2);
						
					}
					
					//判断是给哪个服务器的数据
					if(rx_date[0] == 0x03)
					{
						//数据接收完成 发送事件标志
						rt_event_send(&event,EVENT_FLAG_usart_clent3);
						
					}
					
					//当ID为0X0A时说明是RTU发送给自己的配置信息
					if(rx_date[0] == 0x0A)
					{
						mb_addr = (rx_date[2] << 8 ) + rx_date[3];
						//mb_addr = mb_addr + STM32FLASH_BASE;
						mb_length = (rx_date[4] << 8 ) + rx_date[5];
						
						if(mb_length % 2 != 0)
						{
							mb_length = mb_length+1;
						}
						//校验成功将配置信息更新并保存  进行配置更新时应该对串口2进行互斥处理
						//判断功能码
						if(usart1_rec_buff[1] == 0x03)
						{
							//读取flash中的数据
							ret = FLASH_Read((uint32_t)mb_addr+STM32FLASH_BASE, read_date_buff, mb_length);
							
							if(ret == mb_length*2)//读取完成
							{
								//将数据进行打包并发送出去
								tx_date[0] = rx_date[0];
								tx_date[1] = rx_date[1];
								tx_date[2] = mb_length * 2;
								
								for(uint16_t i=0;i<mb_length/2;i++)
								{
									tx_date[3+i*2] = read_date_buff[i] >> 8;
									tx_date[3+i*2+1] = read_date_buff[i] & 0x00ff;
								}
								
								mb_length = _modbus_rtu_send_msg_pre(tx_date,mb_length+3);
								
								rt_usart1_sendbuff(tx_date,mb_length);
								
							}
							else//读取错误
							{
								
							}
						}
						
						if(rx_date[1] == 0x06)
						{
							
						}
						
						if(rx_date[1] == 0x10)
						{
							//组合数据
							for(uint16_t i=0;i<mb_length;i++)
							{
								read_date_buff[i] = (rx_date[7+i*2] << 8) + rx_date[7+i*2+1];
							}
							//将数据写入flash
							FLASH_WriteNotCheck((uint32_t)mb_addr+STM32FLASH_BASE,read_date_buff,mb_length);
							
							//将数据进行打包并发送出去
							tx_date[0] = rx_date[0];
							tx_date[1] = rx_date[1];
							tx_date[2] = rx_date[2];
							tx_date[3] = rx_date[3];
							tx_date[4] = rx_date[4];
							tx_date[5] = rx_date[5];
							
							mb_length = _modbus_rtu_send_msg_pre(tx_date,6);
							//发送返回数据
							rt_usart1_sendbuff(tx_date,mb_length);
							
							//重启
						}
						
						//返回接收完成应答
	
					}
					//清除数据   测试使用
					
					for(uint16_t i=0;i<usart1_rec_length;i++)
					{
						usart1_rec_buff[i] = 0;
					}
					usart1_rec_length = 0; 
				}
				else
				{
					//数据校验错误  返回错误代码
					for(uint16_t i=0;i<usart1_rec_length;i++)
					{
						usart1_rec_buff[i] = 0;
					}
					usart1_rec_length = 0;
				}
			}
			else//数据无效
			{
				for(uint16_t i=0;i<usart1_rec_length;i++)
				{
					usart1_rec_buff[i] = 0;
				}
				usart1_rec_length = 0; 
			}
		}
	}
}


