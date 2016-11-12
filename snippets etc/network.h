#pragma once
#include "stdafx.h"

extern void InitNetwork();
extern void StopNetwork();

class Client;
class Server;
class ClientConnection;

class Client
{
public:
	Client(void);
	~Client(void);

	bool Connect(const char* ip, int port);
	bool Disconnect();
	
	bool SendData(char* src, unsigned int len);
	bool RecvData(char* dst, unsigned int len);

private:
	SOCKET			m_socket;
	SOCKADDR_IN		m_addr;
	bool			m_valid;
	bool			m_connected;
};


class Server
{
public:
	Server(void);
	~Server(void);
	
	bool			Create(int port, int backlog);
	bool			Stop();
	ClientConnection NewClient();

private:
	SOCKET			m_socket;
	SOCKADDR_IN		m_addr;
	bool			m_valid;
	bool			m_bound;
};


class ClientConnection 
{
public:
	ClientConnection(SOCKET socket, SOCKADDR_IN addr, bool valid);
	~ClientConnection(void);

	bool SendData(char* src, unsigned int len);
	bool RecvData(char* dst, unsigned int len);
private:
	SOCKET			m_socket;
	SOCKADDR_IN		m_addr;
	bool			m_valid;
};