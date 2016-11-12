#pragma once

struct procinfo
{
	HANDLE	handle;
	DWORD	id;
	bool Open();
	void Close();
};

class ProcManagement
{
public:
	ProcManagement();
	~ProcManagement();
};

std::vector<procinfo> GetAllProc();