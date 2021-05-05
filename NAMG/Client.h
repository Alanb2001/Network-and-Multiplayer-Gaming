#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Client
{
public:
	Client();
	void ReceivePackets(sf::TcpSocket* socket);
	void SendPacket(sf::Packet& packet);
	bool GameClient();
	//void GetInput();
	int Run();
	void Events();
	void Movement();
	void Collision();

private:
	const unsigned short m_Port = 5000;
	sf::IpAddress m_Address = sf::IpAddress::getLocalAddress();
	sf::TcpSocket m_Socket;
	sf::Packet m_Packet;
	sf::Mutex m_GlobalMutex;
	bool m_IsConnected = false;
	bool m_Quit = false;
	std::string m_MessageSend;
	sf::RenderWindow m_window;
	sf::Texture m_BackgroundTexture;
	sf::Texture m_CarTexture;
	sf::Sprite m_BackgroundSprite;
	sf::Sprite m_CarSprite;
	float m_Speed;
	float m_Angle;
	float m_MaxSpeed;
	float m_Acc;
	float m_Dec;
	float m_TurnSpeed;
	int m_OffsetX;
	int m_OffsetY;
};

