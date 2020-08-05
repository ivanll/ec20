#include "board.h"
#include "uart_config.h"

//
uint16_t mqtt_linknum_max = 0;
uint16_t re_link_time = 0;
uint16_t time_heard_beat = 0;
char  *date_heard_beat;
uint16_t tx_timeout = 0;

void read_config(void)
{
	config_info_t info_config;
	
	//读取配置信息
	//读取服务器连接配置
	serve1_ip = info_config.clent_config_t[0].clent_ip;
	serve1_port = info_config.clent_config_t[0].clent_port;
	
	serve2_ip = info_config.clent_config_t[1].clent_ip;
	serve2_port = info_config.clent_config_t[1].clent_port;
	
	//读取mqtt配置信息
	mqtt1_uri = info_config.mqtt_config_t[0].mqtt_uri;
	mqtt1_subtopic = info_config.mqtt_config_t[0].mqtt_SUBTopicNAME;
	mqtt1_pubtopic = info_config.mqtt_config_t[0].mqtt_PUBTopicNAME;
	mqtt1_usename = info_config.mqtt_config_t[0].mqtt_username;
	mqtt1_password = info_config.mqtt_config_t[0].mqtt_password;
	
	mqtt2_uri = info_config.mqtt_config_t[1].mqtt_uri;
	mqtt2_subtopic = info_config.mqtt_config_t[1].mqtt_SUBTopicNAME;
	mqtt2_pubtopic = info_config.mqtt_config_t[1].mqtt_PUBTopicNAME;
	mqtt2_usename = info_config.mqtt_config_t[1].mqtt_username;
	mqtt2_password = info_config.mqtt_config_t[1].mqtt_password;
	
	mqtt_linknum_max = info_config.MQTT_SUBSCRIBE_HANDLERS;
	re_link_time = info_config.relink_time;
	time_heard_beat = info_config.heard_beat_time;
	date_heard_beat = info_config.heard_beat_date;
	tx_timeout = info_config.timeout_tx;
	
}


static uint16_t FlashBuffer[STM32FLASH_PAGE_SIZE >> 1];


/// 初始化FLASH
void FLASH_Init(void)
{
	HAL_FLASH_Unlock();
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	HAL_FLASH_Lock();
}


/**
 * 读FLASH
 * @param  Address   要读的起始地址，！！！要求2字节对齐！！！
 * @param  Buffer    存放读取的数据，！！！要求2字节对齐！！！
 * @param  NumToRead 要读取的数据量，单位：半字，！！！要求2字节对齐！！！
 * @return           实际读取到的数据量，单位：字节
 */
uint32_t FLASH_Read(uint32_t Address, uint16_t *Buffer, uint32_t NumToRead)
{
	uint32_t num_read = NumToRead;
	uint32_t AddrMax = STM32FLASH_END - 2;

	if (NumToRead == 0 || Buffer == NULL || Address > AddrMax)
		return 0;

	while (Address <= AddrMax && num_read)
	{
		*Buffer++ = *(__IO uint16_t *)Address;
		Address += 2;
		num_read--;
	}

	return ((NumToRead - num_read) << 1);
}


///**
// * 写FLASH
// * @param  Address    写入起始地址，！！！要求2字节对齐！！！
// * @param  Buffer     待写入的数据，！！！要求2字节对齐！！！
// * @param  NumToWrite 要写入的数据量，单位：半字，！！！要求2字节对齐！！！
// * @return            实际写入的数据量，单位：字节
// */
//uint32_t FLASH_Write(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite)
//{
//	
//}

uint32_t FLASH_WriteNotCheck(uint32_t Address, const uint16_t *Buffer, uint32_t NumToWrite)
{
	uint32_t nwrite = NumToWrite;
	uint32_t addrmax = STM32FLASH_END - 2;
	uint32_t StartSector;
	uint32_t EndSector;
	
	HAL_FLASH_Unlock(); //解锁FLASH后才能向FLASH中写数据。


  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	
	 /* Get the number of the start and end sectors */
  StartSector = GetSector(STM32FLASH_BASE);  //获取FLASH的Sector编号
  EndSector = GetSector(STM32FLASH_BASE+NumToWrite);
	
	//擦除FLASH
  for (uint32_t i = StartSector; i <= EndSector; i += 8)  //每次FLASH编号增加8，可参考上边FLASH Sector的定义。
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    FLASH_Erase_Sector(i, FLASH_VOLTAGE_RANGE_3);

  }
	//擦除完成开始写入
	while (nwrite)
	{
		if (Address > addrmax)
			break;

		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, *Buffer);
		
		if ( (*(__IO uint16_t *)Address) != *Buffer )
			break;

		nwrite--;
		Buffer++;
		Address += 2;
	}
	
	return (NumToWrite - nwrite);
}


uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_SECTOR_11;  
  }

  return sector;
}



