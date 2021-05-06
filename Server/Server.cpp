#include "Server.h"

Server::Server()
{
	std::cout << "Chat server started" << std::endl;

	if (m_Listener.listen(m_Port) != sf::Socket::Done)
	{
		std::cout << "Could not listen" << std::endl;
	}
}

void Server::BroadcastPacket(sf::Packet& packet, sf::IpAddress address, unsigned short port)
{
	for (size_t iterator = 0; iterator < m_Clients.size(); iterator++)
	{
		sf::TcpSocket* client = m_Clients[iterator];
		//if (client->getRemoteAddress() != m_Address || client->getLocalPort() != m_Port)
		//{
			if (client->send(packet) != sf::Socket::Done)
			{
				std::cout << "Could not send packet on broadcast" << std::endl;
			}
		//}
	}
	//std::cout << "Checking if there's any new messages..." << std::endl;
	//std::string oldMessage;
	////while (!m_Quit)
	////{
	//	//sf::Packet packetSend;
	//	//m_GlobalMutex.lock();
	//	//packetSend << m_MessageSend;
	//	//m_GlobalMutex.unlock();

	//	//m_Clients.send(packetSend);

	//	//std::string message;
	//	//sf::Packet packetReceive;

	//	//m_Clients.receive(packetReceive);

	//	//if ((packetReceive >> message) && oldMessage != message && !message.empty())
	//	//{
	//	//	std::cout << message << std::endl;
	//	//	oldMessage = message;
	//	//}

	//	for (std::list<sf::TcpSocket*>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
	//	{
	//		sf::TcpSocket& client = **it;
	//		if (m_Selector.isReady(client))
	//		{
	//			sf::Packet packetSend;
	//			//m_GlobalMutex.lock();
	//			packetSend << m_MessageSend;
	//			//m_GlobalMutex.unlock();

	//			if (client.send(packetSend) == sf::Socket::Done)
	//			{
	//				std::string message;
	//				sf::Packet packetReceive;

	//				if (client.receive(packetReceive) == sf::Socket::Done)
	//				{
	//					if ((packetReceive >> message) && oldMessage != message && !message.empty())
	//					{
	//						std::cout << message << std::endl;
	//						oldMessage = message;
	//					}
	//				}
	//			}
	//		}
	//	}
	/*}*/
}

void Server::ReceivePacket(sf::TcpSocket* client, size_t iterator)
{
	sf::Packet packet;
	if (client->receive(packet) == sf::Socket::Disconnected)
	{
		DisconnectClient(client, iterator);
	}
	else
	{
		if (packet.getDataSize() > 0)
		{
			//std::string receivedMessage;
			
			CarData inData;
			
			//packet >> receivedMessage;
			
			packet >> inData;

			std::cout << inData.m_username << ": " << inData.m_message <<  inData.m_speed << inData.m_accelerate << inData.m_decelerate<<std::endl;

			packet.clear();

			// packet << receivedMessage << client->getRemoteAddress().toString() << client->getRemotePort();

			// Push the received data out to all the other clients
			packet << inData;

			BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
		}
	}
}

void Server::ConnectClients(std::vector<sf::TcpSocket*>* client) 
{
	while (true)
	{
		sf::TcpSocket* newClient = new sf::TcpSocket();
		if (m_Listener.accept(*newClient) == sf::Socket::Done)
		{
			newClient->setBlocking(false);
			client->push_back(newClient);
			std::cout << "Added client " << newClient->getRemoteAddress() << " on slot " << client->size() << std::endl;
		}
		else
		{
			std::cout << "Server listener error, restart the server" << std::endl;
			delete(newClient);
			break;
		}
	}
	//m_Listener.listen(m_Port);
	//m_Listener.accept(m_Clients);
	//std::cout << "New client connected: " << m_Clients.getRemoteAddress() << std::endl;
	//std::cout << "Entered the server loop" << std::endl;
	//m_Listener.listen(m_Port);
	//m_Selector.add(m_Listener);
	///*while (!m_Quit)
	//{*/
	//	if (m_Selector.wait())
	//	{
	//		if (m_Selector.isReady(m_Listener))
	//		{
	//			sf::TcpSocket* client = new sf::TcpSocket;
	//			if (m_Listener.accept(*client) == sf::Socket::Done)
	//			{
	//				m_Clients.push_back(client);
	//				
	//				m_Selector.add(*client);
	//				std::cout << "Added a new client" << std::endl;
	//			}
	//			else
	//			{
	//				delete client;
	//			}
	//		}
	//		/*else
	//		{*/
	//			for (std::list<sf::TcpSocket*>::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
	//			{
	//				sf::TcpSocket& client = **it;
	//				if (m_Selector.isReady(client)  && *it)
	//				{
	//					sf::Packet packet;
	//					if (client.receive(packet) == sf::Socket::Done)
	//					{
	//						std::cout << "New client connected: " << client.getRemoteAddress() << std::endl;
	//					}
	//				}
	//			}
	//		/*}*/
	//	}
	/*}*/
}

void Server::DisconnectClient(sf::TcpSocket* socket, size_t position)
{
	std::cout << "Client " << socket->getRemoteAddress() << ":" << " disconnected, removing" << std::endl;
	socket->disconnect();
	delete(socket);
	m_Clients.erase(m_Clients.begin() + position);
}

//void Server::GetInput()
//{
////	std::string userMessage;
////	std::cout << "\nEnter \"exit\" to quit or message to send: " << std::endl;
////	getline(std::cin, userMessage);
////	if (userMessage == "exit")
////	{
////		m_Quit = true;
////	}
////	//m_GlobalMutex.lock();
////	m_MessageSend = userMessage;
////	//m_GlobalMutex.unlock();
////}

void Server::ManagePackets()
{
	while (true)
	{
		for (size_t iterator = 0; iterator < m_Clients.size(); iterator++)
		{
			ReceivePacket(m_Clients[iterator], iterator);
		}
		std::this_thread::sleep_for((std::chrono::milliseconds)100);
	}
}

int Server::Run()
{
	std::thread connectionThread(&Server::ConnectClients, this, &m_Clients);

	ManagePackets();

	////sf::Thread* thread = 0;

	////GameServer();

	////thread = new sf::Thread(&Server::Messages, this);
	////thread->launch();

	//while (!m_Quit)
	//{
	//	GameServer();
	//	Messages();
	////	//GetInput();
	//}

	////if (thread)
	////{
	////	thread->wait();
	////	delete thread;
	////}

	return 0;
}
