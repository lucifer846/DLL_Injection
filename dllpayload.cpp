#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD Reason, LPVOID lpvReserved) {
	switch (Reason) {
		case DLL_PROCESS_ATTACH:
			MessageBoxW(NULL, L"Install Antivirsu", L"Warning", MB_ICONINFORMATION | MB_OKCANCEL);
			break;
	}
	return TRUE;
}