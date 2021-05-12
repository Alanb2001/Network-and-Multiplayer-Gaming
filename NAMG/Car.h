#pragma once
#include <SFML/Graphics.hpp>

struct Car
{
	sf::Vector2f position;
	float speed;
	float angle;
	int n;

	int points[8][2] = 
	{ 
	  300, 610,
	  1270,430,
	  1380,2380,
	  1900,2460,
	  1970,1700,
	  2550,1680,
	  2560,3150,
	  500, 3300 
	};

	Car() :
		speed(2.f),
		angle(0.f),
		n(0),
		position
		(0.f, 0.f)
	{

	}

	void move()
	{
		position.x += sin(angle) * speed;
		position.y -= cos(angle) * speed;
	}
	void findTarget()
	{
		float tx = points[n][0];
		float ty = points[n][1];
		float beta = angle - atan2(tx - position.x, -ty + position.y);
		if (sin(beta) < 0)
		{
			angle += 0.005f * speed;
		}
		else
		{
			angle -= 0.005f * speed;
		}
		if ((position.x - tx) * (position.x - tx) + (position.y - ty) * (position.y - ty) < 25 * 25)
		{
			n = (n + 1) % 8;
		}
	}
};
