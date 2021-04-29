#include "Client.h"

Client::Client()
:	m_Port(5000),
	m_Address(sf::IpAddress::getLocalAddress()),
	m_Socket(),
	m_GlobalMutex(),
	m_Quit(false),
	m_MessageSend(),
	m_OldMessage(),
	m_Thread(0),
	m_Message(),
	m_PacketSend(),
	m_PacketReceive(),
	m_UserMessage()
{
}

void Client::Messages()
{
	while (!m_Quit)
	{
		m_GlobalMutex.lock();
		m_PacketSend << m_MessageSend;
		m_GlobalMutex.unlock();

		m_Socket.send(m_PacketSend);

		m_Socket.receive(m_PacketReceive);
		if ((m_PacketReceive >> m_Message) && m_OldMessage != m_Message && !m_Message.empty())
		{
			std::cout << m_Message << std::endl;
			m_OldMessage = m_Message;
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
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, m_UserMessage);
	if (m_UserMessage == "exit")
	{
		m_Quit = true;
	}
	m_GlobalMutex.lock();
	m_MessageSend = m_UserMessage;
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
