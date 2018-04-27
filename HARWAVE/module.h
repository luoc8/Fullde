#ifndef _MODULE_H
#define _MODULE_H

u8 Module_Config(DEVER_MODULE *modulex, u32 vol, u32 cur);
u8 Module_Control(DEVER_MODULE *modulex, u8 cmd);
u8 Module_Start(DEVER_MODULE *modulex);
u8 Module_Stop(DEVER_MODULE *modulex);
u8 Module_Pause(DEVER_MODULE *modulex);
void CS_Control(u16 cs, FlagStatus state);
u8 Stop_All_Module(void);
#endif

