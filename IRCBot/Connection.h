#pragma once

#include <WinSock2.h>
#include <Windows.h>

#include <string>
#include <iostream>
#include <vector>

#include "Thread.h"

using namespace std;





struct IrcMessage
{
	IrcMessage(const string &msg);

	string m_SourceMessage;

	string m_NickOrServer;
	string m_User, m_Host;

	string m_Command;

	vector<string> m_ParamList;
	
};

struct ConnectionBase
{
	virtual void Handle(IrcMessage &message) = 0;
};

class Connection
{
public:
	Connection(const string &server, const string &nick, ConnectionBase *client, unsigned int port = 6667);
	~Connection();
	bool SendLine(const string &line);

	void ReceiveThread();

private:
	Thread *m_Thread;
	ThreadDelegate<Connection> m_ThreadDelegate;
	ConnectionBase *m_Client;
	::SOCKET m_SockID;
};