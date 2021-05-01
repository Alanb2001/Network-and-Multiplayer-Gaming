#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Server
{
public:
	void Messages();
	void GameServer();
	void GetInput();
	int Run();

	const unsigned short m_Port = 5000;
	sf::IpAddress m_Address = sf::IpAddress::getLocalAddress();
	sf::TcpSocket m_Socket;
	sf::Mutex m_GlobalMutex;
	bool m_Quit = false;
	std::string m_MessageSend;
};