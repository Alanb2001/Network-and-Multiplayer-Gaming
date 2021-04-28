#pragma once
#include <iostream>
#include <SFML/Network.hpp>

class Client
{
public:
	Client();
	void Messages();
	bool GameClient();
	void GetInput();
	void Run();

private:
	const unsigned short m_Port;
	sf::IpAddress m_Address;
	sf::TcpSocket m_Socket;
	sf::Mutex m_GlobalMutex;
	bool m_Quit;
	std::string m_MessageSend;
	std::string m_OldMessage;
	sf::Thread* m_Thread;
};

