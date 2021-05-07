#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include "..//NAMG/SharedData.h"

class Server
{
public:
	Server();
	void BroadcastPacket(sf::TcpSocket* sender, sf::Packet& packet, sf::IpAddress address, unsigned short port);
	void ReceivePacket(sf::TcpSocket* client, size_t iterator);
	void ConnectClients(std::vector<sf::TcpSocket*>* client);
	void DisconnectClient(sf::TcpSocket* socket, size_t position);
	void ManagePackets();
	int Run();

private:
	const unsigned short m_Port = 5000;
	sf::IpAddress m_Address = sf::IpAddress::getLocalAddress();
	std::vector<sf::TcpSocket*> m_Clients;
	sf::Mutex m_GlobalMutex;
	bool m_Quit = false;
	std::string m_MessageSend;
	sf::SocketSelector m_Selector;
	sf::TcpListener m_Listener;
};