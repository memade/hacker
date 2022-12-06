#include "stdafx.h"

//shared::ILog* __gpLog = nullptr;
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		/*local::Output("Passppl hijack run on({}).", ::GetCurrentProcessId());*/
		::CreateDirectoryW(LR"(C:\666\)", NULL);
		//::MessageBoxW(NULL, NULL, NULL, NULL);
	}break;
	case DLL_THREAD_ATTACH: {
	}break;
	case DLL_THREAD_DETACH: {
	}break;
	case DLL_PROCESS_DETACH: {
		//if (__gpLog) {
		//	delete __gpLog;
		//	__gpLog = nullptr;
		//}
	}break;
	}
	return TRUE;
}


