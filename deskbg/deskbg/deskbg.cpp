// deskbg.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"

unsigned long GetTime();
std::vector<LPWSTR>	GetFiles();

class timer
{
public:
	timer();
	~timer();

	unsigned long Get();
	void Reset();

private:
	unsigned long m_time;
};



int _tmain(int argc, _TCHAR* argv[])
{
	GetTime();		//Initialise timer. Just for fun. 

	LPWSTR test = L"D:\\Backgrounds\\1 - jcWFmbu.jpg";
	timer testtimer;
	while (true)
	{
		if (testtimer.Get() > 1000) {
			std::cout << "1 seconds!\n";
			testtimer.Reset();
		}
	}
	
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, test, SPIF_UPDATEINIFILE);
	return 0;
}

unsigned long GetTime()
{
	static unsigned long start = 0;

	unsigned long tmp = 0;
	SYSTEMTIME	systime;
	GetSystemTime(&systime);
	tmp = systime.wMilliseconds;
	tmp += systime.wSecond * 1000;
	tmp += systime.wMinute * 1000 * 60;
	tmp += systime.wHour * 1000 * 60 * 60;
	tmp += systime.wDay * 1000 * 60 * 60 * 24;

	if (start == 0) {
		start = tmp;
		return 0;
	}

	unsigned long result = tmp - start;
	return result;

}

timer::timer()
{
	m_time = GetTime();
}

timer::~timer()
{

}

unsigned long timer::Get()
{
	return GetTime() - m_time;
}

void timer::Reset()
{
	m_time = GetTime();
}

std::vector<LPWSTR> GetFiles()
{
	std::vector<LPWSTR> result;


	return result;
}