/*!
* ¨€¨€     ¨€¨€  ¨€¨€¨€¨€¨€  ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€¨€    ¨€¨€¨€
* ¨€¨€     ¨€¨€ ¨€¨€   ¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€      ¨€¨€      ¨€¨€ ¨€¨€¨€¨€  ¨€¨€¨€¨€
* ¨€¨€  ¨€  ¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€    ¨€¨€ ¨€¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€ ¨€¨€¨€¨€ ¨€¨€
* ¨€¨€ ¨€¨€¨€ ¨€¨€ ¨€¨€   ¨€¨€  ¨€¨€  ¨€¨€  ¨€¨€           ¨€¨€ ¨€¨€ ¨€¨€  ¨€¨€  ¨€¨€
*  ¨€¨€¨€ ¨€¨€¨€  ¨€¨€   ¨€¨€   ¨€¨€¨€¨€   ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€¨€¨€¨€¨€¨€ ¨€¨€ ¨€¨€      ¨€¨€
**********************************************************
* @file driver.cpp
* @author Simon
* @brief Wave Sim Driver
*/
#include <iostream>
#include <stdio.h>
#include <stdarg.h>

#include "driver.h"
#include "commander.h"


#pragma comment(lib,"Winmm.lib")

static std::vector<std::string> errorList;
static CRITICAL_SECTION csError;
static CRITICAL_SECTION csLog;

driver::driver(const char* name) {
	m_name = name;
}

driver::~driver() {
	
}

std::string driver::getLastError() {
	if (!errorList.size()) return "";

	std::string msg = errorList.back();
	errorList.pop_back();
	return msg;
}

void driver::setLog(const char* msg) {
	EnterCriticalSection(&csLog);

	LeaveCriticalSection(&csLog);
}

void driver::setError(const char* format, ...) {
	EnterCriticalSection(&csError);

	va_list aptr;
	char buffer[4096];
	va_start(aptr, format);
	vsprintf(buffer, format, aptr);
	va_end(aptr);

	std::string strMsg = m_name + std::string(": ") + std::string(buffer);
	errorList.push_back(strMsg);
	std::cout << strMsg << std::endl;

	LeaveCriticalSection(&csError);
}


commander* pCommander = nullptr;

void WINAPI SimThread(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	pCommander->sim();
}

int main()
{
	InitializeCriticalSection(&csError);
	InitializeCriticalSection(&csLog);

	pCommander = new commander();

	int gtime_ID = timeSetEvent(10, 1, (LPTIMECALLBACK)SimThread, 0, TIME_PERIODIC);
	if (gtime_ID == NULL) exit(-1);

	while (true)
	{

	}
}
