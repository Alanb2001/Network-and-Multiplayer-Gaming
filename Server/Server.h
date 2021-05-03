#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <list>

class Server
{
public:
	void Messages();
	void GameServer();
	void GetInput();
	int Run();

private:
	const unsigned short m_Port = 5000;
	sf::IpAddress m_Address = sf::IpAddress::getLocalAddress();
	//std::list<sf::TcpSocket*> m_Clients;
	sf::TcpSocket m_Clients;
	sf::Mutex m_GlobalMutex;
	bool m_Quit = false;
	std::string m_MessageSend;
	sf::SocketSelector m_Selector;
	sf::TcpListener m_Listener;
};