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
	void Run();
	void Events();
	void Movement();
	void Collision();

private:
	const unsigned short m_Port;
	sf::IpAddress m_Address;
	sf::TcpSocket m_Socket;
	sf::Mutex m_GlobalMutex;
	bool m_Quit;
	std::string m_MessageSend;
	std::string m_OldMessage;
	sf::Thread* m_Thread;
	sf::Packet m_PacketSend;
	std::string m_Message;
	sf::Packet m_PacketReceive;
	std::string m_UserMessage;
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

