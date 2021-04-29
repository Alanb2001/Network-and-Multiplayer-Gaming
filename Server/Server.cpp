#include "Server.h"

Server::Server()
:	m_Port(5000),
	m_Socket(),
	m_Listener(),
	m_MessageSend(),
	m_OldMessage(),
	m_GlobalMutex(),
	m_Quit(false),
	m_Thread(0),
	m_PacketSend(),
	m_Message(),
	m_PacketReceive(),
	m_UserMessage()
{
}

void Server::Messages()
{
	while (!m_Quit)
	{
		m_GlobalMutex.lock();
		m_PacketSend << m_MessageSend;
		m_GlobalMutex.unlock();

		m_Socket.send(m_PacketSend);

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
	m_Listener.listen(m_Port);
	m_Listener.accept(m_Socket);
	std::cout << "New client connected: " << m_Socket.getRemoteAddress() << std::endl;
}

void Server::GetInput()
{
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

void Server::Run()
{
	Server server;

	GameServer();

	m_Thread = new sf::Thread(&Server::Messages, &server);
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

}
