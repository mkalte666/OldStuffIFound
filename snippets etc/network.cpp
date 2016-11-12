#include "StdAfx.h"
#include "network.h"

#ifdef _WIN32
WSAData g_wsadata;
#pragma comment(lib, "Ws2_32.lib")
#endif
bool g_network_init = false;

std::string IpToString(sockaddr_in &data);

/*
* @name: InitNetwork
* @param: ~
* @return:  ~
* @description: Creates Network. Means Initialisation of special systems needed by different platforms
*/
void InitNetwork()
{
	std::cout << "INFO: Network initialisation started..." << std::endl;
#ifdef _WIN32
	int result = WSAStartup(MAKEWORD(2,2), &g_wsadata);
	if(result!=0) {
		std::cout << "ERROR: WSAStartup failed : " << GetLastError() << std::endl;
		g_network_init = false;
		return;
	}
#endif
	std::cout << "...done successfull" << std::endl;
	g_network_init = true;
}

/*
* @name: StopNetwork
* @param: ~
* @return: ~
* @description: Stops Network. Means that it stops special systems needed by differen platforms
*/
void StopNetwork()
{
	std::cout << "INFO: Stopping network..." << std::endl;
#ifdef _WIN32
	int result = WSACleanup();
	if(result!=0) {
		std::cout << "ERROR: WSACleanup failed: " << GetLastError() << std::endl;
		g_network_init = false;
		return;
	}
#endif
	std::cout << "...done successfull" << std::endl;
	g_network_init = false;
}

/*
* @name: Client::Client
* @param: ~
* @return: ~
* @description: Creates a Client Object. Creates Socket etc. 
*/
Client::Client(void) 
	: m_connected(false)
{
	if(!g_network_init) {
		std::cout << "ERROR: Cant create client: Network not initialised! "<< std::endl;
		m_valid = false;
		return;
	}

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_socket == INVALID_SOCKET) {
		std::cout << "ERROR: Cant create client: Socket creation failed! " << std::endl;
		m_valid = false;
		return;
	}
	
	m_addr.sin_family = AF_INET;
	m_valid = true;
}

/*
* @name: Client::~Client
* @param: ~
* @return: ~
* @description: Destructor. Removes Client and delets socket
*/
Client::~Client(void)
{
	closesocket(m_socket);
}

/*
* @name: Client::Connect
* @param:	ip: const char* that contanis the ip of the server to connect to
			port: port to connect to
* @return:	bool: true if successfull, false otherwise
* @description: Connects to a server at a given ip (hostname) and port. 
*/
bool Client::Connect(const char* ip, int port)
{
	if(!m_valid || m_connected) return false;
	m_addr.sin_addr.s_addr = inet_addr(ip);
	m_addr.sin_port = htons(port);
	
	int result = connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr));
	if(result=SOCKET_ERROR) {
		std::cout << "ERROR: Cannot connect to server at " << IpToString(m_addr).c_str() << std::endl;
		m_connected = false;
		return false;
	}

	m_connected = true;
	return true;
}

/*
* @name: Client::Discconnect
* @param: ~
* @return: bool: true if successfull, false otherwise.
* @description: Dissconnects
*/
bool Client::Disconnect()
{
	if(!m_valid || !m_connected) return false;
	m_connected = false;
	closesocket(m_socket);

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_socket == INVALID_SOCKET) {
		std::cout << "ERROR: Cant create client: Socket creation failed! " << std::endl;
		m_valid = false;
		return false;
	}

	return true;
}

/*
* @name: Client::SendData
* @param:	src: char* that contais the data to send
			len: unsigned int that contains the lenght of the data
* @return:	bool: true if successfull and sizes that has been sent and that should have been sent match, false otherwise
* @description: sends data to a server that the Client is connected to
*/
bool Client::SendData(char* src, unsigned int len)
{
	if(!m_valid || !m_connected) return false;
	int result = send(m_socket, src, len, 0);
	if(result!=len) return false;
	return true;
}

/*
* @name: Client::RecvData
* @param:	dst: char* that will recive the data
			len: length of data to recive
* @return:	bool: true if successfull and the sizes of the data to recive and the recived data match, false otherwise
* @description
*/
bool Client::RecvData(char* dst, unsigned int len)
{
	if(!m_valid || !m_connected) return false;
	int result = recv(m_socket, dst, len, 0);
	if(result!=len) return false;
	return true;
}

//___________________________Server Functions_________________________________

/*
* @name: Server::Server
* @param: ~
* @return: ~
* @description: Creates a Server. Creates the socket etc.
*/
Server::Server()
	: m_bound(false)
{
	if(!g_network_init) {
		std::cout << "ERROR: Cant create server: Network not initialised! "<< std::endl;
		m_valid = false;
		return;
	}

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_socket == INVALID_SOCKET) {
		std::cout << "ERROR: Cant create server: Socket creation failed! " << std::endl;
		m_valid = false;
		return;
	}
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_valid = true;
}

/*
* @name: Server::~Server
* @param: ~
* @return: ~
* @description: ~
*/
Server::~Server()
{
	if(!m_valid) return;
	closesocket(m_socket);
}

/*
* @name: Server::Create
* @param:	port: int that contains the port the server will bind to
			backlog: int that contains the number of connections maximal waiting in the queqe
* @return: bool: true if succsessfull, false otherwise
* @description: Binds the server to the given port with the given Backlog
*/
bool Server::Create(int port, int backlog)
{
	if(!m_valid || m_bound) return false;
	m_addr.sin_port = htons(port);
	int result = bind(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr));
	if(result == SOCKET_ERROR) {
		std::cout << "ERROR: Cant bind server." << std::endl;
		m_bound = false;
		return false;
	}
	listen(m_socket, backlog);
	m_bound = true;
}

/*
* @name: Server::Stop
* @param: ~
* @return: bool: true if succsessfull ,false otherwise
* @description: Stops the server (if its running). Recreates the server-socket
*/
bool Server::Stop()
{
	if(!m_valid || !m_bound) return false;
	m_bound = false;
	closesocket(m_socket);

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_socket == INVALID_SOCKET) {
		std::cout << "ERROR: Cant create server: Socket creation failed! " << std::endl;
		m_valid = false;
		return false;
	}

	return true;
}

/*
* @name: Server::NewClient()
* @param: ~
* @return: ClientConnection: Returns a ClientConnection that represents the client that connected
* @description: Waits for an incoming connection and returns a ClientConnection.
*/
ClientConnection Server::NewClient()
{
	SOCKADDR_IN client_addr;
	SOCKET		client_sock;
	int			client_addr_size = sizeof(client_addr);
	client_sock = accept(m_socket, (SOCKADDR*)&client_addr, &client_addr_size);
	bool		client_valid = false;
	if(client_sock != INVALID_SOCKET) client_valid = true;
	return ClientConnection(client_sock, client_addr, client_valid);
}

//_________ClientConnection Functions __________________________________

/*
* @name: ClientConnection::ClientConnection
* @param:	socket: SOCKET that the client is connected to
			addr:	SOCKADDR_IN containing the address of the connected client
			valid:	bool that contains information if the socket is valid
* @return: ~
* @description: Creates A ClientConnections as a Layer between the Socket and the user
*/
ClientConnection::ClientConnection(SOCKET socket, SOCKADDR_IN addr, bool valid)
	: m_socket(socket), m_addr(addr), m_valid(valid)
{
	if(!g_network_init) {
		std::cout << "ERROR: Cant create ClientConnection: Network not initialised! "<< std::endl;
		m_valid = false;
		return;
	}
}

/*
* @name: ClientConnection::SendData
* @param:	src: char* that contains the data that will be send
* @return:	len: int containting the length of src
* @description: Sends data to a connected client
*/
bool ClientConnection::SendData(char* src, unsigned int len)
{
	if(!m_valid) return false;
	int result = send(m_socket, src, len, 0);
	if(result!=len) return false;
	return true;
}

/*
* @name:  ClientConnection::RecvData
* @param:	src: char* that will recive the data
* @return:	len: int containting the length ro recive
* @description: Recives data from a connected client
*/
bool ClientConnection::RecvData(char* dst, unsigned int len)
{
	if(!m_valid ) return false;
	int result = recv(m_socket, dst, len, 0);
	if(result!=len) return false;
	return true;
}

/*
* @name: IpToString
* @param:	in: unsigned long that contains the raw ip-address
* @return:	std::string: the human-readable verison of the given Ip-address
* @description: Converts a raw ip-structure to a human-readable from (->"123.12.22.2")
*/
std::string IpToString(sockaddr_in &data)
{
	char buff[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &data.sin_addr.s_addr, buff, INET_ADDRSTRLEN);
	return std::string(buff);
}