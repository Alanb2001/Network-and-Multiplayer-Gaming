#include "Server.h"

Server::Server()
{
	std::cout << "Server started" << std::endl;

	if (m_Listener.listen(m_Port) != sf::Socket::Done)
	{
		std::cout << "Could not listen" << std::endl;
	}
}

void Server::BroadcastPacket(sf::TcpSocket* sender, sf::Packet& packet, sf::IpAddress address, unsigned short port)
{
	for (size_t iterator = 0; iterator < m_Clients.size(); iterator++)
	{
		sf::TcpSocket* client = m_Clients[iterator];
		if (client != sender) 
		{
			if (client->send(packet) != sf::Socket::Done)
			{
				std::cout << "Could not send packet on broadcast" << std::endl;
			}
		}
	}
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
			CarData inData;
				
			packet >> inData;

			std::cout << inData.m_username << ": " << inData.m_angle << inData.m_position << std::endl;

			packet.clear();

			packet << inData;

			BroadcastPacket(client, packet, client->getRemoteAddress(), client->getRemotePort());
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
}

void Server::DisconnectClient(sf::TcpSocket* socket, size_t position)
{
	std::cout << "Client " << socket->getRemoteAddress() << ":" << " disconnected, removing" << std::endl;
	socket->disconnect();
	delete(socket);
	m_Clients.erase(m_Clients.begin() + position);
}

void Server::ManagePackets()
{
	while (true)
	{
		for (size_t iterator = 0; iterator < m_Clients.size(); iterator++)
		{
			ReceivePacket(m_Clients[iterator], iterator);
		}
	}
}

/* This function checks if any clients are trying to connect on a separate
	thread as well as going through each client to see if the server is 
	receiving any packets	*/
void Server::Run()
{
	std::thread connectionThread(&Server::ConnectClients, this, &m_Clients);

	ManagePackets();
}