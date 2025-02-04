#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <processthreadsapi.h>



DWORD PID = NULL, TID = NULL;
LPVOID rBuffer = NULL;
HMODULE hKernel32 = NULL;
HANDLE hProcess = NULL, hThread = NULL;

wchar_t dllPath[MAX_PATH] = L"C:\\Users\\monika\\source\\repos\\randDLL.dll";
size_t dllPathSize = sizeof(dllPath);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("usage: %s <PID>", argv[0]);
		return EXIT_FAILURE;
	}
	PID = atoi(argv[1]);
	printf("trying to connect to process (%ld)\n", PID);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (hProcess == NULL) {
		printf("failed to get a handle to the process (%ld): (%ld)\n", PID, GetLastError());
		return EXIT_FAILURE;
	}
	printf("got 0x%p handle to the process (%ld)\n", hProcess, PID);

	rBuffer = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	printf("allocated buffer to process memory\n");


	if (WriteProcessMemory(hProcess, rBuffer, dllPath, dllPathSize, NULL)) {
		printf("wrote bytes to Process memory\n");
	}

	hKernel32 = GetModuleHandleW(L"Kernel32");
	if (hKernel32 == NULL) { printf("failed to get handle to dll: %ld\n", GetLastError()); CloseHandle(hProcess); return EXIT_FAILURE; }
	printf("got 0x%p handle to dll\n", hKernel32);

	LPTHREAD_START_ROUTINE startThis = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryW");
	printf("got the address to the LoadLibraryW() ---> 0x%p\n", startThis);

	hThread = CreateRemoteThread(hProcess, NULL, 0, startThis, rBuffer, 0, &TID);
	if (hThread == NULL) { printf("Problem in Opening Thread, %ld\n", GetLastError()); CloseHandle(hProcess); return EXIT_FAILURE; }
	printf("got 0x%p handle to thread (%ld)\n", hThread, PID);

	printf("waiting for the thread to finist executing\n");
	WaitForSingleObject(hThread, INFINITE);
	printf("Thread Finished executing\n");

	CloseHandle(hThread);
	CloseHandle(hProcess);

	printf("-------finished :)--------");


		return EXIT_SUCCESS;
	}
