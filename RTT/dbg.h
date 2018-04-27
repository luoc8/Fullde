#ifndef DBG_H_
#define DBG_H_

#include"SEGGER_RTT.h"
int RTT_dbg_msg(const char * sFormat, ...);
int RTT_dbg_error(const char * sFormat, ...);
int SEGGER_RTT_Read(unsigned BufferIndex, char* pBuffer, unsigned BufferSize);

#endif /* DBG_H_ */
