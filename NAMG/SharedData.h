#pragma once
#include <SFML/Network.hpp>

struct CarData
{
	CarData() = default;

	CarData(const std::string& username, float angle, sf::Vector2f& posXY) : 
		m_username(username),
		m_angle(angle),
		m_position(posXY)
	{
	}
	std::string m_username;
	float m_angle;
	sf::Vector2f m_position;
};

inline sf::Packet& operator <<(sf::Packet& packet, const CarData& cardata)
{
	return packet << cardata.m_username << cardata.m_angle << cardata.m_position.x << cardata.m_position.y;
}

inline sf::Packet& operator >>(sf::Packet& packet, CarData& cardata)
{
	return packet >> cardata.m_username >> cardata.m_angle >> cardata.m_position.x >> cardata.m_position.y;
}

inline std::ostream& operator <<(std::ostream& os, const sf::Vector2f& vec)
{
	return os << "(" << vec.x << ", " << vec.y << ")";
}