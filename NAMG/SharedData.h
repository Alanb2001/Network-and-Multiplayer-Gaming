#pragma once
#include <SFML/Network.hpp>

struct CarData
{
	CarData() = default;

	CarData(const std::string& username, const std::string& message, float speed, float acc, float dec, sf::Vector2f& posXY) : 
		m_username(username),
		m_message(message),
		m_speed(speed),
		m_accelerate(acc),
		m_decelerate(dec),
		m_position(posXY)
	{
	}

	std::string m_username;
	std::string m_message;
	float m_speed;
	float m_accelerate;
	float m_decelerate;
	sf::Vector2f m_position;
};

inline sf::Packet& operator <<(sf::Packet& packet, const CarData& cardata)
{
	return packet << cardata.m_username << cardata.m_message << cardata.m_speed << cardata.m_accelerate << cardata.m_decelerate << cardata.m_position.x << cardata.m_position.y;
}

inline sf::Packet& operator >>(sf::Packet& packet, CarData& cardata)
{
	return packet >> cardata.m_username >> cardata.m_message >> cardata.m_speed >> cardata.m_accelerate >> cardata.m_decelerate >> cardata.m_position.x >> cardata.m_position.y;
}

inline std::ostream& operator <<(std::ostream& os, const sf::Vector2f& vec)
{
	return os << "(" << vec.x << ", " << vec.y << ")";
}