#pragma once
#include <iostream>
#include <thread>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#include "SharedData.h"
#include "Car.h"

class Client
{
public:
	Client();
	void ReceivePackets(sf::TcpSocket* socket);
	void SendPacket(sf::Packet& packet);
	bool GameClient();
	void Run();
	void Events();
	void Movement();
	void Collision();

private:
	const unsigned short m_port = 5000;
	sf::IpAddress m_address = sf::IpAddress::getLocalAddress();
	sf::TcpSocket m_socket;
	sf::Packet m_packet;
	bool m_isConnected = false;
	std::string m_username;
	sf::RenderWindow m_window;
	sf::Texture m_backgroundTexture;
	sf::Texture m_carTexture;
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_carSprite;
	float m_speed;
	float m_angle;
	float m_maxSpeed;
	float m_accelerate;
	float m_decelerate;
	float m_turnSpeed;
	int m_offsetX;
	int m_offsetY;
	sf::Clock m_clock;
	float m_packetTimer;
	float m_timeElapsed;
	std::vector<Car> m_carContainer;
	sf::Image m_backgroundMask;
};

