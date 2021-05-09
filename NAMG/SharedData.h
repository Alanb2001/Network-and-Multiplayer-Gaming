#pragma once
#include <SFML/Network.hpp>

enum class eDataPackets
{
	e_None,
	e_Connect,
	e_Disconnect,
	e_GameStart,
	e_Car
};

// Sending enums via sf::Packet https://en.sfml-dev.org/forums/index.php?topic=17075.0
template<typename T>
inline typename std::enable_if<std::is_enum<T>::value, sf::Packet&>::type
operator<<(sf::Packet& roPacket, const T& rkeMsgType)
{
	return roPacket << static_cast<typename std::underlying_type<T>::type>(rkeMsgType);
}

template<typename T>
inline typename std::enable_if<std::is_enum<T>::value, sf::Packet&>::type
operator>>(sf::Packet& roPacket, T& reMsgType)
{
	typename std::underlying_type<T>::type xMsgType{};
	roPacket >> xMsgType;
	reMsgType = static_cast<T>(xMsgType);

	return roPacket;
}

struct CarData
{
	CarData() = default;

	CarData(const eDataPackets type, const std::string& username, float angle, sf::Vector2f& posXY) : 
		m_type(type),
		m_username(username),
		m_angle(angle),
		m_position(posXY)
	{
	}
	eDataPackets m_type;
	std::string m_username;
	float m_angle;
	sf::Vector2f m_position;
};

inline sf::Packet& operator <<(sf::Packet& packet, const CarData& cardata)
{
	return packet << cardata.m_type << cardata.m_username << cardata.m_angle << cardata.m_position.x << cardata.m_position.y;
}

inline sf::Packet& operator >>(sf::Packet& packet, CarData& cardata)
{
	return packet >> cardata.m_type >> cardata.m_username >> cardata.m_angle >> cardata.m_position.x >> cardata.m_position.y;
}

inline std::ostream& operator <<(std::ostream& os, const sf::Vector2f& vec)
{
	return os << "(" << vec.x << ", " << vec.y << ")";
}