#include "Client.h"

const int N = 5;

Client::Client() : 
	m_window(sf::VideoMode(640, 480), "NAMG", sf::Style::Default),
	m_backgroundTexture(),
	m_carTexture(),
	m_backgroundSprite(),
	m_carSprite(),
	m_speed(0.f),
	m_angle(0.f),
	m_maxSpeed(12.0f),
	m_accelerate(0.2f),
	m_decelerate(0.3f),
	m_turnSpeed(0.08f),
	m_offsetX(0),
	m_offsetY(0),
	m_packetTimer(1.f),
	m_timeElapsed(0.f)
{
	std::cout << "Chat client started" << std::endl;

	m_window.setFramerateLimit(60);

	m_backgroundTexture.loadFromFile("images/background.png");
	m_backgroundTexture.setSmooth(true);
	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_backgroundSprite.scale(2, 2);

	m_carTexture.loadFromFile("images/car.png");
	m_carTexture.setSmooth(true);
	m_carSprite.setTexture(m_carTexture);
	m_carSprite.setOrigin(22, 22);

	std::cout << "Enter a username: ";

	std::cin >> m_username;

	m_carContainer.reserve(N);
	for (int i = 0; i < N; ++i) 
	{
		m_carContainer.emplace_back();
	}
}

void Client::ReceivePackets(sf::TcpSocket* socket)
{
	CarData inCarData;
	sf::Packet packet;
	while (true)
	{
		if (socket->receive(packet) == sf::Socket::Done)
		{
			packet >> inCarData;

			std::cout << "From " << inCarData.m_username << ": " << inCarData.m_angle << inCarData.m_position << std::endl;

		}
	}

	switch (inCarData.m_type)
	{
	case eDataPackets::e_None:
		break;
	case eDataPackets::e_Connect:
		break;
	case eDataPackets::e_GameStart:
		break;
	case eDataPackets::e_Disconnect:
		break;
	default:
		break;
	}
}

void Client::SendPacket(sf::Packet& packet)
{
	m_timeElapsed += m_clock.getElapsedTime().asSeconds();

	if (m_timeElapsed >= m_packetTimer) 
	{
		if (packet.getDataSize() > 0 && m_socket.send(packet) != sf::Socket::Done)
		{
			std::cout << "Could not send packet" << std::endl;
		}

		m_timeElapsed = 0.f;
	}

	m_clock.restart();
}

bool Client::GameClient()
{
	if (m_socket.connect(m_address, m_port) != sf::Socket::Done)
	{
		std::cout << "Could not connect to the server\n";
		return true;
	}
	else
	{
		m_isConnected = true;
		std::cout << "Connected to the server\n" << std::endl;
	}
	return false;
}


int Client::Run()
{
	GameClient();

	std::thread receptionThread(&Client::ReceivePackets, this, &m_socket);

	// Starting positions
	for (int i = 0; i < N; i++)
	{
		m_carContainer[i].position.x = 300 + i * 50;
		m_carContainer[i].position.y = 1700 + i * 80;
		m_carContainer[i].speed = 7 + i;
	}

	sf::Clock clock;
	constexpr float lastTime = std::numeric_limits<float>::max();

	while (m_window.isOpen())
	{
		m_fpsCounter.update();

		m_window.setTitle("Racing Game: " + std::to_string(m_fpsCounter.getFPS()));

		if (m_isConnected)
		{
			sf::Packet replyPacket;

			CarData outData(eDataPackets::e_GameStart, m_username, m_carContainer[0].angle, m_carContainer[0].position);

			replyPacket << outData;

			SendPacket(replyPacket);

		}

		Events();
		Movement();
		Collision();

		m_window.clear(sf::Color::White);

		m_backgroundSprite.setPosition(-m_offsetX, -m_offsetY);
		m_window.draw(m_backgroundSprite);
		sf::Color colors[10] = { sf::Color::Red, sf::Color::Green, sf::Color::Magenta, sf::Color::Blue, sf::Color::White };
		for (int i = 0; i < N; i++)
		{
			m_carSprite.setPosition(m_carContainer[i].position.x - m_offsetX, m_carContainer[i].position.y - m_offsetY);
			m_carSprite.setRotation(m_carContainer[i].angle * 180 / 3.141593f);
			m_carSprite.setColor(colors[i]);
			m_window.draw(m_carSprite);
		}
		m_window.display();
	}

	return 0;
}

void Client::Events()
{
	sf::Event e;
	while (m_window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

void Client::Movement()
{
	bool Up = 0, Right = 0, Down = 0, Left = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		Up = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		Right = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		Down = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		Left = 1;
	}
	//car movement
	if (Up && m_speed < m_maxSpeed)
	{
		if (m_speed < 0)
		{
			m_speed += m_decelerate;
		}
		else
		{
			m_speed += m_accelerate;
		}
	}
	if (Down && m_speed > -m_maxSpeed)
	{
		if (m_speed > 0)
		{
			m_speed -= m_decelerate;
		}
		else
		{
			m_speed -= m_accelerate;
		}
	}
	if (!Up && !Down)
	{
		if (m_speed - m_decelerate > 0)
		{
			m_speed -= m_decelerate;
		}
		else if (m_speed + m_decelerate < 0)
		{
			m_speed += m_decelerate;
		}
		else
		{
			m_speed = 0;
		}
	}
	if (Right && m_speed != 0)
	{
		m_angle += m_turnSpeed * m_speed / m_maxSpeed;
	}
	if (Left && m_speed != 0)
	{
		m_angle -= m_turnSpeed * m_speed / m_maxSpeed;
	}
	m_carContainer[0].speed = m_speed;
	m_carContainer[0].angle = m_angle;
	for (int i = 0; i < N; i++)
	{
		m_carContainer[i].move();
	}
	for (int i = 1; i < N; i++)
	{
		m_carContainer[i].findTarget();
	}
}

void Client::Collision()
{
	float R = 22;

	//collision
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == j) 
			{
				break;
			}
			int dx = 0, dy = 0;
			while (dx * dx + dy * dy < 4 * R * R)
			{
				m_carContainer[i].position.x += dx / 10.0f;
				m_carContainer[i].position.x += dy / 10.0f;
				m_carContainer[j].position.x -= dx / 10.0f;
				m_carContainer[j].position.y -= dy / 10.0f;
				dx = m_carContainer[i].position.x - m_carContainer[j].position.x;
				dy = m_carContainer[i].position.y - m_carContainer[j].position.y;
				if (!dx && !dy)
				{
					break;
				}
			}
		}
	}

	// TODO: Stay within the limit of the map.
	// TODO: Don't show white at bottom/right.
	if (m_carContainer[0].position.x > 320)
	{
		m_offsetX = m_carContainer[0].position.x - 320;
	}
	if (m_carContainer[0].position.y > 240)
	{
		m_offsetY = m_carContainer[0].position.y - 240;
	}
}
