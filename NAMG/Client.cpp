#include "Client.h"

Client::Client()
:	m_Port(5000),
    m_Address(sf::IpAddress::getLocalAddress()),
    m_Socket(),
    m_GlobalMutex(),
    m_Quit(false),
	m_MessageSend(),
	m_OldMessage(),
	m_Thread(0)
{
}

void Client::Messages()
{
	while (!m_Quit)
	{
		sf::Packet packetSend;
		m_GlobalMutex.lock();
		packetSend << m_MessageSend;
		m_GlobalMutex.unlock();

		m_Socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		m_Socket.receive(packetReceive);
		if ((packetReceive >> msg) && m_OldMessage != msg && !msg.empty())
		{
			std::cout << msg << std::endl;
			m_OldMessage = msg;
		}
	}
}

bool Client::GameClient()
{
	if (m_Socket.connect(m_Address, m_Port) == sf::Socket::Done)
	{
		std::cout << "Connected\n";
		return true;
	}
	return false;
}

void Client::GetInput()
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit")
	{
		m_Quit = true;
	}
	m_GlobalMutex.lock();
	m_MessageSend = s;
	m_GlobalMutex.unlock();
}

void Client::Run()
{
	Client client;

	GameClient();

	m_Thread = new sf::Thread(&Client::Messages, &client);
	m_Thread->launch();

	while (!m_Quit)
	{
		GetInput();
	}

	if (m_Thread)
	{
		m_Thread->wait();
		delete m_Thread;
	}

}
