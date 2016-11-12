// BasicRun.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <CamProject.h>

void loop(int none)
{
	while (true) {
		std::cout << "blah" << std::endl;
	}
}


int main(int argc, char* argv[])
{
	CamInit(loop);
	return 0;
}

