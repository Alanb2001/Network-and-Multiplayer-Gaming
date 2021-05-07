#include "Client.h"
#include <SFML/Main.hpp>

const int num = 8; //checkpoints
// TODO: use checkpoint to make sure we are on the track.
// Slow speed when not on the track.
int points[num][2] = { 300, 610,
  1270,430,
  1380,2380,
  1900,2460,
  1970,1700,
  2550,1680,
  2560,3150,
  500, 3300 };

struct Car
{
	sf::Vector2f position;
	float speed;
	float angle;
	int n;
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
			angle += 0.005 * speed;
		}
		else
		{
			angle -= 0.005 * speed;
		}
		// Check if passed a checkpoint
		if ((position.x - tx) * (position.x - tx) + (position.y - ty) * (position.y - ty) < 25 * 25)
		{
			n = (n + 1) % num; // TODO: simplify
		}
	}
};

float R = 22;
const int N = 5;
Car car[N];

Client::Client()
	: m_window(sf::VideoMode(640, 480), "NAMG", sf::Style::Default),
	m_backgroundTexture(),
	m_carTexture(),
	m_backgroundSprite(),
	m_carSprite(),
	m_speed(0),
	m_angle(0),
	m_maxSpeed(12.0),
	m_accelerate(0.2),
	m_decelerate(0.3),
	m_turnSpeed(0.08),
	m_offsetX(0),
	m_offsetY(0),
	m_packetTimer(1.f)
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

}

void Client::ReceivePackets(sf::TcpSocket* socket)
{
	//std::string oldMessage;
	///*while (!m_Quit)
	//{*/
	//sf::Packet packetSend;
	////m_GlobalMutex.lock();
	//packetSend << m_MessageSend;
	////m_GlobalMutex.unlock();

	//if (m_Socket.send(packetSend) != sf::Socket::Done)
	//{
	//    std::cout << "Error" << std::endl;
	//}

	//std::string message;
	//sf::Packet packetReceive;

	//m_Socket.receive(packetReceive);
	//if ((packetReceive >> message) && oldMessage != message && !message.empty())
	//{
	//    std::cout << message << std::endl;
	//    oldMessage = message;
	//}
	///*}*/

	while (true)
	{
		sf::Packet packet;
		// m_Packet.clear();
		if (socket->receive(packet) == sf::Socket::Done)
		{
			//std::string receviedString;
			//std::string senderAddress;
			//unsigned short senderPort;
			//m_Packet >> receviedString >> senderAddress >> senderPort;

			CarData inCarData;
			packet >> inCarData;

			// std::cout << "From (" << senderAddress << ":" << senderPort << "): " << receviedString <<std::endl;
			std::cout << "From " << inCarData.m_username << ": " << inCarData.m_postion << std::endl;
		}

		// std::this_thread::sleep_for((std::chrono::milliseconds)100);
	}
}

void Client::SendPacket(sf::Packet& packet)
{
	m_timeElapsed += m_clock.getElapsedTime().asSeconds();

	if (m_timeElapsed >= m_packetTimer) {
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

//void Client::GetInput()
//{
//  std::string userMessage;
//	std::cout << "\nEnter \"exit\" to quit or message to send: ";
//	getline(std::cin, userMessage);
//	if (userMessage == "exit")
//	{
//		m_Quit = true;
//	}
//	//m_GlobalMutex.lock();
//	m_MessageSend = userMessage;
//	//m_GlobalMutex.unlock();
//}

int Client::Run()
{
	//   //sf::Thread* thread = 0;

	   ////thread = new sf::Thread(&Client::Messages, this);
	   ////thread->launch();

	   //while (!m_Quit)
	   //{
	   //	//GameClient();
	   //	Messages();
	   //	GetInput();
	   //}

	   //if (thread)
	   //{
	   //	thread->wait();
	   //	delete thread;
	   //}

	  /* while (!m_Quit)
	   {*/

	   /* }*/

	GameClient();

	std::thread receptionThread(&Client::ReceivePackets, this, &m_socket);

	// Starting positions
	for (int i = 0; i < N; i++)
	{
		car[i].position.x = 300 + i * 50;
		car[i].position.y = 1700 + i * 80;
		car[i].speed = 7 + i;
	}

	sf::Clock clock;
	constexpr float lastTime = std::numeric_limits<float>::max();

	while (m_window.isOpen())
	{

		m_fpsCounter.update();

		m_window.setTitle("Racing Game: " + std::to_string(m_fpsCounter.getFPS()));

		if (m_isConnected)
		{
			//std::string userMessage;

			//getline(std::cin, userMessage);

			//std::cin >> userMessage;

			// std::cin.getline()

			//scanf_s("%[^\t\n]", userMessage.c_str());

			/* if (userMessage == "exit")
			{
				m_Quit = true;
			}*/

			sf::Packet replyPacket;

			CarData outData(m_username, "hello", m_speed, m_accelerate, m_decelerate, car[0].position);

			// std::cout << outData.m_message << "\n\n";

			// std::cout << /*outData.m_speed << " " << outData.m_accelerate << " " << outData.m_decelerate << " " << */ outData.m_postion << std::endl;

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
			m_carSprite.setPosition(car[i].position.x - m_offsetX, car[i].position.y - m_offsetY);
			m_carSprite.setRotation(car[i].angle * 180 / 3.141593);
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
	car[0].speed = m_speed;
	car[0].angle = m_angle;
	for (int i = 0; i < N; i++)
	{
		car[i].move();
	}
	for (int i = 1; i < N; i++)
	{
		car[i].findTarget();
	}
}

void Client::Collision()
{
	//collision
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == j) {
				break;
			}
			int dx = 0, dy = 0;
			while (dx * dx + dy * dy < 4 * R * R)
			{
				car[i].position.x += dx / 10.0;
				car[i].position.x += dy / 10.0;
				car[j].position.x -= dx / 10.0;
				car[j].position.y -= dy / 10.0;
				dx = car[i].position.x - car[j].position.x;
				dy = car[i].position.y - car[j].position.y;
				if (!dx && !dy) break;
			}
		}
	}

	// TODO: Stay within the limit of the map.
	// TODO: Don't show white at bottom/right.
	if (car[0].position.x > 320)
	{
		m_offsetX = car[0].position.x - 320;
	}
	if (car[0].position.y > 240)
	{
		m_offsetY = car[0].position.y - 240;
	}
}
