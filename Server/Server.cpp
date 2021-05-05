#include "Server.h"

void Server::Messages()
{
	std::cout << "Checking if there's any new messages..." << std::endl;
	std::string oldMessage;
	//while (!m_Quit)
	//{
		//sf::Packet packetSend;
		//m_GlobalMutex.lock();
		//packetSend << m_MessageSend;
		//m_GlobalMutex.unlock();

		//m_Clients.send(packetSend);

		//std::string message;
		//sf::Packet packetReceive;

		//m_Clients.receive(packetReceive);

		//if ((packetReceive >> message) && oldMessage != message && !message.empty())
		//{
		//	std::cout << message << std::endl;
		//	oldMessage = message;
		//}

		for (std::list<sf::TcpSocket*>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
		{
			sf::TcpSocket& client = **it;
			if (m_Selector.isReady(client))
			{
				sf::Packet packetSend;
				m_GlobalMutex.lock();
				packetSend << m_MessageSend;
				m_GlobalMutex.unlock();

				if (client.send(packetSend) == sf::Socket::Done)
				{
					std::string message;
					sf::Packet packetReceive;

					if (client.receive(packetReceive) == sf::Socket::Done)
					{
						if ((packetReceive >> message) && oldMessage != message && !message.empty())
						{
							std::cout << message << std::endl;
							oldMessage = message;
						}
					}
				}
			}
		}
	/*}*/
}

void Server::GameServer() 
{
	//m_Listener.listen(m_Port);
	//m_Listener.accept(m_Clients);
	//std::cout << "New client connected: " << m_Clients.getRemoteAddress() << std::endl;
	std::cout << "Entered the server loop" << std::endl;
	m_Listener.listen(m_Port);
	m_Selector.add(m_Listener);
	/*while (!m_Quit)
	{*/
		if (m_Selector.wait())
		{
			if (m_Selector.isReady(m_Listener))
			{
				sf::TcpSocket* client = new sf::TcpSocket;
				if (m_Listener.accept(*client) == sf::Socket::Done)
				{
					m_Clients.push_back(client);
					
					m_Selector.add(*client);
					std::cout << "Added a new client" << std::endl;
				}
				else
				{
					delete client;
				}
			}
			/*else
			{*/
				for (std::list<sf::TcpSocket*>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
				{
					sf::TcpSocket& client = **it;
					if (m_Selector.isReady(client)  && *it)
					{
						sf::Packet packet;
						if (client.receive(packet) == sf::Socket::Done)
						{
							std::cout << "New client connected: " << client.getRemoteAddress() << std::endl;
						}
					}
				}
			/*}*/
		}
	/*}*/
}

void Server::GetInput()
{
	std::string userMessage;
	std::cout << "\nEnter \"exit\" to quit or message to send: " << std::endl;
	getline(std::cin, userMessage);
	if (userMessage == "exit")
	{
		m_Quit = true;
	}
	//m_GlobalMutex.lock();
	m_MessageSend = userMessage;
	//m_GlobalMutex.unlock();
}

int Server::Run()
{
	//sf::Thread* thread = 0;

	//GameServer();

	//thread = new sf::Thread(&Server::Messages, this);
	//thread->launch();

	while (!m_Quit)
	{
		GameServer();
		Messages();
	//	//GetInput();
	}

	//if (thread)
	//{
	//	thread->wait();
	//	delete thread;
	//}

	return 0;
}
