#ifndef FLASH_IF_H
#define FLASH_IF_H
#include "stm32f10x.h"

//最后8个扇区用于储存一些配置参数
#define ADDR_FLASH_PAGE_120   ((uint32_t)0x0803C000) /* Base @ of Page 120, 2 Kbytes */
#define ADDR_FLASH_PAGE_121   ((uint32_t)0x0803C800) /* Base @ of Page 121, 2 Kbytes */
#define ADDR_FLASH_PAGE_122   ((uint32_t)0x0803D000) /* Base @ of Page 122, 2 Kbytes */
#define ADDR_FLASH_PAGE_123   ((uint32_t)0x0803D800) /* Base @ of Page 123, 2 Kbytes */
#define ADDR_FLASH_PAGE_124   ((uint32_t)0x0803E000) /* Base @ of Page 124, 2 Kbytes */
#define ADDR_FLASH_PAGE_125   ((uint32_t)0x0803E800) /* Base @ of Page 125, 2 Kbytes */
#define ADDR_FLASH_PAGE_126   ((uint32_t)0x0803F000) /* Base @ of Page 126, 2 Kbytes */
#define ADDR_FLASH_PAGE_127   ((uint32_t)0x0803F800) /* Base @ of Page 127, 2 Kbytes */

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define STM32_FLASH_SIZE 256 	 		//所选STM32的FLASH容量大小(单位为K)

#define FLASH_PAGE_SIZE   			2048

#define ADC_CORRECT_ADDRESS     ADDR_FLASH_PAGE_120    
#define DAC_CORRECT_ADDRESS			ADDR_FLASH_PAGE_121


#define USER_FLASH_END_ADDRESS  ((uint32_t)0x08040000)

#endif
