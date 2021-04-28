#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Server
{
public:
	Server();
	void GameServer();

private:
	const unsigned short m_Port;
	sf::TcpSocket m_Socket;
	sf::TcpListener m_Listener;
};