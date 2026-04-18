#include "MySocket.h"
#include <iostream>
using namespace std;


MySocket::MySocket()
{
	m_port = 8080;	// Default Port: 8080
	m_ipAddr = "127.0.0.1";	// Default IP: localhost

	int iniResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData); // Init Window Socket Network

	if (iniResult != 0)
	{
		cerr << "Can't Initialize WinSock!" << endl;
	}

	// How To End?
}


MySocket::~MySocket()
{
	closesocket(m_sock);
	WSACleanup();
}


SOCKET MySocket::createSock(int ipProtocol, int L4Protocol)
{
	m_sock = socket(ipProtocol, L4Protocol, 0);

	if (m_sock == INVALID_SOCKET)
	{
		cerr << "Can't Create Socket!" << endl;
	}

	// How To End?
}


void bind()
{

}


void connect()
{

}


void listen()
{

}


void send()
{

}
