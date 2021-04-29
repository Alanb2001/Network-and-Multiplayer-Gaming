#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Server
{
public:
	Server();
	void Messages();
	void GameServer();
	void GetInput();
	void Run();

private:
	const unsigned short m_Port;
	sf::TcpSocket m_Socket;
	sf::TcpListener m_Listener;
	std::string m_MessageSend;
	std::string m_OldMessage;
	sf::Mutex m_GlobalMutex;
	bool m_Quit;
	sf::Thread* m_Thread;
	sf::Packet m_PacketSend;
	std::string m_Message;
	sf::Packet m_PacketReceive;
	std::string m_UserMessage;
};