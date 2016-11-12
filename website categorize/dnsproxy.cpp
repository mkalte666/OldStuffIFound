#include <iostream>
#include <thread>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else

#endif

#include "dnsstruct.h"

//function: netInit
//note: returns 0 if net init is succsesfull. handles platform spec. magic
int netInit()
{
#ifdef _WIN32
	WSADATA wsa;
	std::cout << "Trying to init WinSock" << std::endl;
	return WSAStartup(MAKEWORD(2,0), &wsa);
#else
	return 0;
#endif
}

unsigned short sushort(unsigned short in)
{
	return (in<<8)|(in>>8);
}

std::string fieldToDNSString(char* in, int len, int offset) {
	std::string result;
	if(len>offset) {
		int i = offset;
		while(i+1<len) {
			int secLen = in[i];
			i++;
			if(secLen == 0) {
				break;
			}
			for(int j=0;j<secLen&&secLen+i<len;j++) {
				result.push_back(in[i+j]);
			}
			result.push_back('.');
			i+=secLen;
		}
	}
	return result;
}


//function: main
//note: starts everything
int main(int argc, char**argv)
{
	std::cout << sizeof(short) << std::endl;
	if(netInit()!=0) {
		std::cout << "Error while NetInit" << std::endl;
		return 1;
	}
	std::cout << "Done Init. Creating UDP/TCP listening socks" << std::endl;
	int udpListener=0;
	udpListener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udpListener<=0) {
		std::cout << "could not create UDP listener socket" << std::endl;
		return 1;
	}
	
	sockaddr_in myAddr, dstAddr, partnerAddr; 
	int socklen = sizeof(sockaddr_in);
	myAddr.sin_family = AF_INET;
	dstAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(53);
	dstAddr.sin_port = htons(53);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	dstAddr.sin_addr.s_addr = inet_addr("8.8.8.8");
	
	if(bind(udpListener, (sockaddr*)&myAddr, sizeof(myAddr))!=0) {
		std::cout << "Error binding" << std::endl;
		return 1;
	}
	char * data = new char[1024];
	while(true) {
		int len = recvfrom(udpListener,data,1024,NULL,(sockaddr*)&partnerAddr,&socklen);
		std::cout << "recived a " << len << " bytes request!" << std::endl;
		DNSRequestHeader header = *(DNSRequestHeader*)data;
		std::cout << "id=" << sushort(header.id) << " qdcount="<<sushort(header.qdcount) << std::endl;
		std::cout << fieldToDNSString(data, len,sizeof(DNSRequestHeader)) << std::endl;
		sendto(udpListener, data, len, NULL, (sockaddr*)&dstAddr, socklen);
		len = recvfrom(udpListener,data,1024,NULL,NULL,NULL);
		sendto(udpListener, data, len, NULL, (sockaddr*)&partnerAddr, socklen);
	}
	closesocket(udpListener);
	return 0;
}