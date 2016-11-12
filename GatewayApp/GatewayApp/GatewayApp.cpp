// GatewayApp.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "ProcManagement.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//First we need a process list.
	std::vector<procinfo> tmp = GetAllProc();

	for (auto i = tmp.begin(); i != tmp.end(); i++) {
		if (i->Open()) {
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HMODULE hMod;
			DWORD cbNeeded;

			if (EnumProcessModules(i->handle, &hMod, sizeof(hMod),
				&cbNeeded))
			{
					GetModuleBaseName(i->handle, hMod, szProcessName,
					sizeof(szProcessName) / sizeof(TCHAR));
			}
			_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, i->id);
			i->Close();
		}
	}

	std::cin.get();
	return 0;
}

