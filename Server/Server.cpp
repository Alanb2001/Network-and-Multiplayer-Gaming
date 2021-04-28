#include "Server.h"

Server::Server()
:	m_Port(5000),
	m_Socket(),
	m_Listener(),
	m_MessageSend(),
	m_OldMessage(),
	m_GlobalMutex(),
	m_Quit(false),
	m_Thread(0)
{
}

void Server::Messages()
{
	while (!m_Quit)
	{
		sf::Packet packetSend;
		m_GlobalMutex.lock();
		packetSend << m_MessageSend;
		m_GlobalMutex.unlock();

		m_Socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		m_Socket.receive(packetReceive);
		if ((packetReceive >> msg) && m_OldMessage != msg && !msg.empty())
		{
			std::cout << msg << std::endl;
			m_OldMessage = msg;
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
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit")
	{
		m_Quit = true;
	}
	m_GlobalMutex.lock();
	m_MessageSend = s;
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
