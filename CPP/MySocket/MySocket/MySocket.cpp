#include "MySocket.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////
// 1. My Socket
/////////////////////////////////////////////////////////////////////////////////

MySocket::MySocket()
{
	m_port = 50000;	// Default Port: 50000 (Just Private Port!)
	m_ipAddr = "127.0.0.1";	// Default IP: localhost

	strcpy(m_buf, "\0");

	int iniResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iniResult != 0)
	{
		std::cerr << "Can't Initialize WinSock!" << std::endl;
		exit(1); // How To End?
	}
}

MySocket::MySocket(std::string ipAddr, int port)
{
	m_port = port;	
	m_ipAddr = ipAddr;	

	strcpy(m_buf, "\0");

	int iniResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (iniResult != 0)
	{
		std::cerr << "Can't Initialize WinSock!" << std::endl;
		exit(1); // How To End?
	}
}

MySocket::~MySocket()
{
	closesocket(m_sock);
	WSACleanup();
}

/////////////////////////////////////////////////////////////////////////////////
// 2. UDP Socket
/////////////////////////////////////////////////////////////////////////////////

UDPSocket::UDPSocket()
{
	memset(&m_sockAddr, 0, sizeof(m_sockAddr)); // Why? Essential?
	m_sockAddr.sin_addr.s_addr = inet_addr(m_ipAddr.c_str());
	m_sockAddr.sin_family = IPv4;
	m_sockAddr.sin_port = htons(m_port);
}

SOCKET UDPSocket::createSock()
{
	m_sock = socket(IPv4, UDP, 0);
	if (m_sock == INVALID_SOCKET)
	{
		std::cerr << "Can't Create Socket!" << std::endl;
		exit(1); // How To End?
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 3. TCP Socket
/////////////////////////////////////////////////////////////////////////////////

TCPSocket::TCPSocket()
{
	m_hint.sin_family = IPv4;
	m_hint.sin_port = htons(m_port);

	int convResult = inet_pton(IPv4, m_ipAddr.c_str(), &m_hint.sin_addr);

	if (convResult != 1)
	{
		std::cerr << "Can't Convert IP adderss" << std::endl;
		WSACleanup();
		exit(1); // How To End?
	}
}

SOCKET TCPSocket::createSock()
{
	m_sock = socket(IPv4, TCP, 0);
	if (m_sock == INVALID_SOCKET)
	{
		std::cerr << "Can't Create Socket!" << std::endl;
		exit(1); // How To End?
	}
}

/////////////////////////////////////////////////////////////////////////////////
// 4. UDP Client
/////////////////////////////////////////////////////////////////////////////////

void UDPClient::send()
{	
	std::cout << "UDP SEND TEST: ";
	std::cin >> m_buf;
	sendto(m_sock, m_buf, MAX, 0, (struct sockaddr*)
		&m_sockAddr, sizeof(m_sockAddr)); // beta: datagram len is static
}

/////////////////////////////////////////////////////////////////////////////////
// 5. UDP Server
/////////////////////////////////////////////////////////////////////////////////

UDPServer::UDPServer()
{
	m_clientAddr.sin_family = IPv4;
	m_clientAddr.sin_port = htons(60000); // Default: 60000
	int convResult = inet_pton(IPv4, m_ipAddr.c_str(), &m_clientAddr.sin_addr);

	c_len = sizeof(m_clientAddr);
}

UDPServer::UDPServer(std::string ipAddr, int port)
{
	m_clientAddr.sin_family = IPv4;
	m_clientAddr.sin_port = htons(60000); // Default: 60000
	int convResult = inet_pton(IPv4, m_ipAddr.c_str(), &m_clientAddr.sin_addr);

	c_len = sizeof(m_clientAddr);
}

void UDPServer::bindServer()
{
	if (bind(m_sock, (struct sockaddr*)&m_sockAddr, sizeof(m_sockAddr))
		== SOCKET_ERROR)
	{
		std::cout << "Bind Error!" << std::endl;
		WSACleanup();
		closesocket(m_sock);
		exit(1);
	}
}

void UDPServer::recv()
{
	recvfrom(m_sock, m_buf, MAX, 0, (struct sockaddr*)&m_clientAddr, &c_len);
	std::cout << m_buf << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////
// 6. TCP Client
/////////////////////////////////////////////////////////////////////////////////

// Write Code

/////////////////////////////////////////////////////////////////////////////////
// 7. TCP Server
/////////////////////////////////////////////////////////////////////////////////

// Write Code
