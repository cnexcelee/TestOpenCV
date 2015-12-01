/// @file WnLog.h
/// @brief 日志
/// @details 日志
/// @author		chenghuaiyu@sina.com
/// @date		2014-6-5
/// @version	A001
/// @par Copyright (c):
///        Science And Technology Co.,Ltd.
/// @par History:          
///		version: author, date, desc\n 

#ifndef __WN_LOG_H__
#define __WN_LOG_H__

#define WN_LOG_CLOSE	0
#define WN_LOG_CONSOLE	1
#define WN_LOG_FILE		2

//// 输出日志开关
#define WN_LOG WN_LOG_CLOSE
//#define WN_LOG WN_LOG_CONSOLE
//#define WN_LOG WN_LOG_FILE

//#define VERBOSE

#ifdef _DEBUG
	#define SAVE_IMAGE
#endif

#if WN_LOG == WN_LOG_CLOSE
	#define Wn_Printf
	#define WN_ENTRY
#elif WN_LOG == WN_LOG_CONSOLE
	#include <stdio.h>

	#define Wn_Printf printf
#elif WN_LOG == WN_LOG_FILE
	#include <stdarg.h>

	extern int __cdecl Wn_Printf(const char * _Format, ...);
	extern int __cdecl Wn_WPrintf(const wchar_t * _Format, ...);

	const int Wn_nLogMsgMaxCount = _MAX_PATH * _MAX_PATH;
	#define WN_LOG_FILENAME "WnLog"
#else
	#error WN_LOG_CLOSE, WN_LOG_CONSOLE, or WN_LOG_FILE required!
#endif

#ifndef WN_ENTRY
	#include <time.h>
	struct WN_EntryRaiiObject
	{
		WN_EntryRaiiObject(const char * pcszMsg) : m_pcszMsg(pcszMsg)
		{
			time_t t = time(0);
			tm tms = * localtime(& t);
			Wn_Printf("<%d-%d-%d %02d:%02d:%02d> Enter into %s\n", tms.tm_year + 1900, tms.tm_mon + 1, tms.tm_mday, tms.tm_hour, tms.tm_min, tms.tm_sec, m_pcszMsg);
		}

		~WN_EntryRaiiObject()
		{
			time_t t = time(0);
			tm tms = * localtime(& t);
			Wn_Printf("<%d-%d-%d %02d:%02d:%02d> Exit from %s\n", tms.tm_year + 1900, tms.tm_mon + 1, tms.tm_mday, tms.tm_hour, tms.tm_min, tms.tm_sec, m_pcszMsg);
		}

		const char * m_pcszMsg;
	};

	#define WN_ENTRY WN_EntryRaiiObject obj ## __LINE__ (__FUNCTION__);
#endif

#endif ///< __WNLOG_H__