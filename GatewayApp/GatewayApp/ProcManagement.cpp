#include "stdafx.h"
#include "ProcManagement.h"

bool procinfo::Open()
{
	handle = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, id); 
	if (handle != NULL) return true;

	return false;
}

void procinfo::Close()
{
	CloseHandle(handle);
}


ProcManagement::ProcManagement()
{
}


ProcManagement::~ProcManagement()
{
}

std::vector<procinfo> GetAllProc()
{
	std::vector<procinfo> result;
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return result;

	cProcesses = cbNeeded / sizeof(DWORD);

	for (int i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0) {
			procinfo tmp;
			tmp.id = aProcesses[i];
			result.push_back(tmp);
		}
	}
	return result;
}

