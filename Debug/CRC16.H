
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CRC16_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CRC16_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CRC16_EXPORTS
#define CRC16_API __declspec(dllexport)
#else
#define CRC16_API __declspec(dllimport)
#endif

CRC16_API unsigned short CRC16(unsigned char *puchMsg,unsigned short usDataLen);
//puchMsg:   /* 要进行CRC校验的消息 */
//usDataLen: /* 消息中字节数 */