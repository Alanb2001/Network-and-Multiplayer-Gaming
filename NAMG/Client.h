#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

class Client
{
public:
	Client();
	void Messages();
	bool GameClient();
	void GetInput();
	int Run();
	void Events();
	void Movement();
	void Collision();

	const unsigned short m_Port = 5000;
	sf::IpAddress m_Address = sf::IpAddress::getLocalAddress();
	sf::TcpSocket m_Socket;
	sf::Mutex m_GlobalMutex;
	bool m_Quit = false;
	std::string m_MessageSend;

private:
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

