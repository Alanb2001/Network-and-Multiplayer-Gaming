#pragma once
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	void Events();
	void Movement();
	void Collision();
	void Run();

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

