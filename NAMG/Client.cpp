#include "Client.h"

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
    float x, y, speed, angle; int n;
    Car() { speed = 2; angle = 0; n = 0; x = 0; y = 0; }
    void move()
    {
        x += sin(angle) * speed;
        y -= cos(angle) * speed;
    }
    void findTarget()
    {
        float tx = points[n][0];
        float ty = points[n][1];
        float beta = angle - atan2(tx - x, -ty + y);
        if (sin(beta) < 0)
        {
            angle += 0.005 * speed;
        }
        else
        {
            angle -= 0.005 * speed;
        }
        // Check if passed a checkpoint
        if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25)
        {
            n = (n + 1) % num; // TODO: simplify
        }
    }
};

float R = 22;
const int N = 5;
Car car[N];

Client::Client()
:	m_window(sf::VideoMode(640, 480), "NAMG", sf::Style::Close),
    m_BackgroundTexture(),
    m_CarTexture(),
    m_BackgroundSprite(),
    m_CarSprite(),
    m_Speed(0),
    m_Angle(0),
    m_MaxSpeed(12.0),
    m_Accelerate(0.2),
    m_Decelerate(0.3),
    m_TurnSpeed(0.08),
    m_OffsetX(0),
    m_OffsetY(0)
{
    std::cout << "Chat client started" << std::endl;

    m_window.setFramerateLimit(60);

    m_BackgroundTexture.loadFromFile("images/background.png");
    m_BackgroundTexture.setSmooth(true);
    m_BackgroundSprite.setTexture(m_BackgroundTexture);
    m_BackgroundSprite.scale(2, 2);

    m_CarTexture.loadFromFile("images/car.png");
    m_CarTexture.setSmooth(true);
    m_CarSprite.setTexture(m_CarTexture);
    m_CarSprite.setOrigin(22, 22);



	std::cout << "Enter a username: ";

	std::cin >> m_Username;

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
		// m_Packet.clear();
		if (socket->receive(m_Packet) == sf::Socket::Done)
		{
            //std::string receviedString;
            //std::string senderAddress;
            //unsigned short senderPort;
            //m_Packet >> receviedString >> senderAddress >> senderPort;

			CarData inCarData;
			m_Packet >> inCarData; // TODO: Remove the member packet - create local packets to send out

			// std::cout << "From (" << senderAddress << ":" << senderPort << "): " << receviedString <<std::endl;
			std::cout << "From " << inCarData.m_username << ": " << inCarData.m_message << std::endl;
		}

        std::this_thread::sleep_for((std::chrono::milliseconds)100);
	}
}

void Client::SendPacket(sf::Packet& packet)
{
    if (packet.getDataSize() > 0 && m_Socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Could not send packet" << std::endl;
    }
}

bool Client::GameClient()
{
	if (m_Socket.connect(m_Address, m_Port) != sf::Socket::Done)
	{
		std::cout << "Could not connect to the server\n";
		return true;
	}
    else
    {
        m_IsConnected = true;
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

	GameClient();

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

    std::thread receptionThread(&Client::ReceivePackets, this, &m_Socket);

    while (!m_Quit)
    {
        if (m_IsConnected)
        {
            std::string userMessage;
            
			getline(std::cin, userMessage);

			//std::cin >> userMessage;

			// std::cin.getline()

			//scanf_s("%[^\t\n]", userMessage.c_str());

            if (userMessage == "exit")
	        {
		        m_Quit = true;
	        }


            sf::Packet replyPacket;

			CarData outData(m_Username, userMessage, 123, 76839, 7238913);

			// std::cout << outData.m_message << "\n\n";

            replyPacket << outData;

            SendPacket(replyPacket);
        }
    }

    // Starting positions
    for (int i = 0; i < N; i++)
    {
        car[i].x = 300 + i * 50;
        car[i].y = 1700 + i * 80;
        car[i].speed = 7 + i;
    }

    while (m_window.isOpen())
    {
        Events();
        Movement();
        Collision();

        m_window.clear(sf::Color::White);

        m_BackgroundSprite.setPosition(-m_OffsetX, -m_OffsetY);
        m_window.draw(m_BackgroundSprite);
        sf::Color colors[10] = { sf::Color::Red, sf::Color::Green, sf::Color::Magenta, sf::Color::Blue, sf::Color::White };
        for (int i = 0; i < N; i++)
        {
            m_CarSprite.setPosition(car[i].x - m_OffsetX, car[i].y - m_OffsetY);
            m_CarSprite.setRotation(car[i].angle * 180 / 3.141593);
            m_CarSprite.setColor(colors[i]);
            m_window.draw(m_CarSprite);
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
    if (Up && m_Speed < m_MaxSpeed)
    {
		if (m_Speed < 0)
		{
			m_Speed += m_Decelerate;
		}
		else 
		{
			m_Speed += m_Accelerate;
		}
    }
    if (Down && m_Speed > -m_MaxSpeed)
    {
		if (m_Speed > 0)
		{
			m_Speed -= m_Decelerate;
		}
		else
		{
			m_Speed -= m_Accelerate;
		}
    }
    if (!Up && !Down)
    {
        if (m_Speed - m_Decelerate > 0)
        {
            m_Speed -= m_Decelerate;
        }
        else if (m_Speed + m_Decelerate < 0)
        {
            m_Speed += m_Decelerate;
        }
        else
        {
            m_Speed = 0;
        }
    }
    if (Right && m_Speed != 0)
    {
        m_Angle += m_TurnSpeed * m_Speed / m_MaxSpeed;
    }
    if (Left && m_Speed != 0)
    {
        m_Angle -= m_TurnSpeed * m_Speed / m_MaxSpeed;
    }
    car[0].speed = m_Speed;
    car[0].angle = m_Angle;
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
                car[i].x += dx / 10.0;
                car[i].x += dy / 10.0;
                car[j].x -= dx / 10.0;
                car[j].y -= dy / 10.0;
                dx = car[i].x - car[j].x;
                dy = car[i].y - car[j].y;
                if (!dx && !dy) break;
            }
        }
    }

    // TODO: Stay within the limit of the map.
    // TODO: Don't show white at bottom/right.
    if (car[0].x > 320)
    {
        m_OffsetX = car[0].x - 320;
    }
    if (car[0].y > 240)
    {
        m_OffsetY = car[0].y - 240;
    }
}
