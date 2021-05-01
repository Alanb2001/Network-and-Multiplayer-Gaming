#include "Server.h"

void Server::Messages()
{
	static std::string m_OldMessage;
	while (!m_Quit)
	{
		sf::Packet m_PacketSend;
		m_GlobalMutex.lock();
		m_PacketSend << m_MessageSend;
		m_GlobalMutex.unlock();

		m_Socket.send(m_PacketSend);

		std::string m_Message;
		sf::Packet m_PacketReceive;

		m_Socket.receive(m_PacketReceive);
		if ((m_PacketReceive >> m_Message) && m_OldMessage != m_Message && !m_Message.empty())
		{
			std::cout << m_Message << std::endl;
			m_OldMessage = m_Message;
		}
	}
}

void Server::GameServer()
{
	sf::TcpListener m_Listener;
	m_Listener.listen(m_Port);
	m_Listener.accept(m_Socket);
	std::cout << "New client connected: " << m_Socket.getRemoteAddress() << std::endl;
}

void Server::GetInput()
{
	std::string m_UserMessage;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, m_UserMessage);
	if (m_UserMessage == "exit")
	{
		m_Quit = true;
	}
	m_GlobalMutex.lock();
	m_MessageSend = m_UserMessage;
	m_GlobalMutex.unlock();
}

int Server::Run()
{
	sf::Thread* m_Thread = 0;

	GameServer();

	m_Thread = new sf::Thread(&Server::Messages, this);
	m_Thread->launch();

	while (!m_Quit)
	{
		GetInput();
	}

	if (m_Thread)
	{
		m_Thread->wait();
		delete m_Thread;
	}

	return 0;
}
