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
	float m_Speed = 0;
	float m_Angle = 0;
	float m_MaxSpeed = 12.0;
	float m_Acc = 0.2; 
	float m_Dec = 0.3;
	float m_TurnSpeed = 0.08;
	int m_OffsetX = 0; 
	int m_OffsetY = 0;
};

