#ifndef __drv_flash_h
#define __drv_flash_h

#include "stm32f4xx_hal_def.h"


extern char *serve1_ip ;
extern uint16_t serve1_port ;

extern char *serve2_ip ;
extern uint16_t serve2_port ;

extern char *mqtt1_uri ;
extern char *mqtt1_usename ;
extern char *mqtt1_password ;
extern char *mqtt1_subtopic ;
extern char *mqtt1_pubtopic ;

extern char *mqtt2_uri ;
extern char *mqtt2_usename ;
extern char *mqtt2_password ;
extern char *mqtt2_subtopic ;
extern char *mqtt2_pubtopic ;


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */


/* FLASH大小 */
#define STM32FLASH_SIZE   0x00100000UL

/* FLASH读写配置起始地址 */
#define STM32FLASH_BASE  0x08050000

/* FLASH结束地址 */
#define STM32FLASH_END   (STM32FLASH_BASE | STM32FLASH_SIZE)

/* FLASH页大小：1K */
#define STM32FLASH_PAGE_SIZE 1024

/* FLASH总页数 */
#define STM32FLASH_PAGE_NUM  (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)

/* 获取页地址，X=0~STM32FLASH_PAGE_NUM */
#define ADDR_FLASH_PAGE_X(X)    (STM32FLASH_BASE | (X * STM32FLASH_PAGE_SIZE))


/////////////////////////////////////////// 导出函数声明 ////////////////////////////////////////////
void FLASH_Init(void);
uint32_t FLASH_Read(uint32_t Address, uint16_t *Buffer, uint32_t NumToRead);
uint32_t FLASH_Write(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);
uint32_t FLASH_WriteNotCheck(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite);

uint32_t GetSector(uint32_t Address);
void read_config(void);


#endif
