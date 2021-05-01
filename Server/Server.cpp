#include "Server.h"

void Server::Messages()
{
	std::string oldMessage;
	while (!m_Quit)
	{
		sf::Packet packetSend;
		m_GlobalMutex.lock();
		packetSend << m_MessageSend;
		m_GlobalMutex.unlock();

		m_Socket.send(packetSend);

		std::string message;
		sf::Packet packetReceive;

		m_Socket.receive(packetReceive);
		if ((packetReceive >> message) && oldMessage != message && !message.empty())
		{
			std::cout << message << std::endl;
			oldMessage = message;
		}
	}
}

void Server::GameServer()
{
	sf::TcpListener listener;
	listener.listen(m_Port);
	listener.accept(m_Socket);
	std::cout << "New client connected: " << m_Socket.getRemoteAddress() << std::endl;
}

void Server::GetInput()
{
	std::string userMessage;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, userMessage);
	if (userMessage == "exit")
	{
		m_Quit = true;
	}
	m_GlobalMutex.lock();
	m_MessageSend = userMessage;
	m_GlobalMutex.unlock();
}

int Server::Run()
{
	sf::Thread* thread = 0;

	GameServer();

	thread = new sf::Thread(&Server::Messages, this);
	thread->launch();

	while (!m_Quit)
	{
		GetInput();
	}

	if (thread)
	{
		thread->wait();
		delete thread;
	}

	return 0;
}
