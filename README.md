# DLL Injection and Payload Example

## Overview

This project demonstrates a basic **DLL Injection** technique using two components:

1. **`dllinj.cpp`**: The injector that injects a DLL into a target process.
2. **`dllpayload.cpp`**: The payload DLL that displays a message box when injected into the target process.

The injector creates a remote thread in the target process and uses `LoadLibraryW()` to load the payload DLL into the target's memory space. The payload DLL is a simple Windows message box that shows a warning message when the target process is injected with the DLL.

---

## Files

### `dllinj.cpp` (DLL Injector)

This C++ file serves as the injector, responsible for injecting the `randDLL.dll` into a running process. The steps performed are as follows:

1. Takes the target process ID (PID) as a command-line argument.
2. Opens the target process with `OpenProcess()`.
3. Allocates memory in the target process using `VirtualAllocEx()`.
4. Writes the path of the DLL (`randDLL.dll`) into the allocated memory using `WriteProcessMemory()`.
5. Retrieves the address of `LoadLibraryW()` from `Kernel32.dll`.
6. Creates a remote thread in the target process to call `LoadLibraryW()`, which loads the injected DLL.
7. Waits for the remote thread to finish executing and then closes handles.

### `dllpayload.cpp` (Payload DLL)

This C++ file is the payload that will be injected into the target process. The steps performed are as follows:

1. The `DllMain()` function is called when the DLL is loaded into the target process.
2. When the DLL is attached to the target process, a message box is displayed with the warning message: `"Install Antivirus"`.

When compiled, `dllpayload.cpp` generates a DLL (`randDLL.dll`) which is injected into the target process by the `dllinj.exe`.

---

## Compilation Instructions

To compile the project, you will need a C++ compiler (e.g., MSVC) for the Windows platform.

### For `dllinj.cpp` (Injector)

```bash
cl dllinj.cpp
