//Includes
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream>
#include "version.h"

void printHelp()
{
	std::cout	<< "Camerift - An Oculus Rift camera controler by Malte Kiessling\n";
	printVersionNumber();
	std::cout	<< "Please read the Readme first!\n"
				<< "This programm accepts the following options:\n"
				<< "'-h': Display this help\n"
				<< "'-v': Display version and exit\n"
				<< "'-ip <ip>: Set the IP of the client. If its your PC it's not needed.\n"
				<< "'-so': For debugging. Don't connect to a client and display headtracking only\n"
				<< "If you have any questions, feel free to contact me at:\n"
				<< "			mkalte666@googlemail.com\n"
				<< "(c) 2013 Malte Kiessling " << std::endl;
	return;
				
}