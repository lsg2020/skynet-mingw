/****************************************************************************
Copyright (c) 2015-2017      dpull.com
http://www.dpull.com
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "platform.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include "epoll.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call,
		      LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		epoll_startup();
		break;

	case DLL_PROCESS_DETACH:
		epoll_cleanup();
		break;
	}
	return TRUE;
}

struct tm *localtime_r(const time_t *timer, struct tm *buf)
{
	struct tm *tm = localtime(timer);
	if (!tm)
		return NULL;
	memcpy(buf, tm, sizeof(*tm));
	return buf;
}

// 获取毫秒时间
__int64 __GetTickMs()
{
	static __int64 Freq = 0;
	static __int64 Start = 0;
	static __int64 StartTime = 0;
	if (Freq == 0) {
		StartTime = time(NULL);
		QueryPerformanceFrequency((LARGE_INTEGER *)&Freq);
		QueryPerformanceCounter((LARGE_INTEGER *)&Start);
	}
	__int64 Count = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&Count);

	//乘以1000，把秒化为毫秒
	return (__int64)((double)(Count - Start) / (double)Freq * 1000.0) +
	       StartTime * 1000;
}
