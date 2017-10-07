
#pragma once
class CSecurity
{
public:
	void HideModule(HANDLE hModule);
	void RemovePeHeader(DWORD ModuleBase);
	void HideModuleFromPEB(HINSTANCE hInstance);
	bool DestroyModuleHeader(HMODULE hModule);
	bool HideThread(HANDLE hThread)
	{
		typedef NTSTATUS(NTAPI *pNtSetInformationThread)
			(HANDLE, UINT, PVOID, ULONG);

		NTSTATUS Status;

		// Get NtSetInformationThread
		pNtSetInformationThread NtSIT = (pNtSetInformationThread)
			GetProcAddress(GetModuleHandle(XorStr("ntdll.dll")), XorStr("NtSetInformationThread"));
		// Shouldn't fail
		if (NtSIT == NULL)
			return false;

		// Set the thread info
		if (hThread == NULL)
			Status = NtSIT(GetCurrentThread(),
				0x11, //ThreadHideFromDebugger
				0, 0);
		else
			Status = NtSIT(hThread, 0x11, 0, 0);

		if (Status != 0x00000000)
			return false;
		else
			return true;
	}
};
extern CSecurity Security;
