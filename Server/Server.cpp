#include "Server.h"

Server::Server()
:	m_Port(5000),
	m_Socket(),
	m_Listener()
{
}

void Server::GameServer()
{
	m_Listener.listen(m_Port);
	m_Listener.accept(m_Socket);
	std::cout << "New client connected: " << m_Socket.getRemoteAddress() << std::endl;
}
