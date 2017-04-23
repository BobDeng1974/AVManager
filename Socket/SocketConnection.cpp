#include "stdafx.h"
#include "SocketConnection.h"



SocketConnection::SocketConnection(int uid)
{
	m_uid = uid;
}

SocketConnection::~SocketConnection()
{

}

bool SocketConnection::Read()
{
	return true;
}

bool SocketConnection::Write()
{
	return true;
}