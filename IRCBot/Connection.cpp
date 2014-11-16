#include "Connection.h"


IrcMessage::IrcMessage(const string &msg)
	: m_SourceMessage(msg),
	m_NickOrServer(""), m_User(""), m_Host(""),
	m_Command("")
{
	size_t e = m_SourceMessage.find(' ');
	// PREFIX
	if(m_SourceMessage[0] == ':')
	{
		// Detailed prefix..
		size_t u = m_SourceMessage.find('!');
		size_t h = m_SourceMessage.find('@');
		if(u != string::npos && u < e)
		{
			m_NickOrServer = m_SourceMessage.substr(1, u);
			m_User = m_SourceMessage.substr(u + 1, h - u);
			m_Host = m_SourceMessage.substr(h + 1, e - h);
		}
		else
		{
			m_NickOrServer = m_SourceMessage.substr(1, e - 1);
		}
		e++;
	}
	else 
		e = 0;

	// COMMAND
	size_t cmdend = m_SourceMessage.find(' ', e);
	m_Command = m_SourceMessage.substr(e, cmdend - e);

	// PARAMLIST
	size_t lastparam = m_SourceMessage.find(':', cmdend + 1);
	// PING :a454554656
	size_t currspace = m_SourceMessage.find_first_not_of(' ', cmdend);
	while(currspace != lastparam && currspace != std::string::npos)
	{
		size_t nextspace = m_SourceMessage.find_first_of(' ', currspace + 1);

		m_ParamList.push_back(m_SourceMessage.substr(currspace, nextspace - currspace));

		currspace = m_SourceMessage.find_first_not_of(' ', nextspace);
		//size_t paramend = m_SourceMessage.find(' ', currspace + 1);
	}

	// inert last param (without :)
	m_ParamList.push_back(m_SourceMessage.substr(lastparam + 1, m_SourceMessage.length()));
}

Connection::Connection(const string &server, const string &nick, ConnectionBase *client, unsigned int port)
	: m_SockID(), m_Client(client), m_ThreadDelegate(*this, &Connection::ReceiveThread)//, m_Thread(&m_ThreadDelegate)
{
	WSADATA w;
	if(int result = WSAStartup(MAKEWORD(2,2), &w) != 0) {
		std::cout << "--> Winsock 2 konnte nicht gestartet werden! Error #" << result << std::endl;
		return;
    }

	m_SockID = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	// Try to resolve host name list
	hostent* phe = gethostbyname(server.c_str());

	std::cout << "[Info]\t" << "Trying to resolve host name : " << server.c_str() << std::endl;

	// Check if host is valid..
	if(phe == NULL)
	{
		cout << "[Error]\t" << "Unable to resolve host name" << endl;
		return;
	}
	if(phe->h_addrtype != AF_INET)
	{
		cout << "[Error]\t" << "Host is using an invalid address type" << endl;
		return;
	}
	if(phe->h_length != 4)
	{
		cout << "[Error]\t" << "Host is using an unknown IP type (IP4 only)" << endl;
		return;
	}

	if(m_SockID == -1)
	{
		cout << "[Error]\t" << "Creating socket failed with error: " << WSAGetLastError() << endl;
		return;
	}

	cout << "[Info]\t" << "Resolved host name and created socket, trying to connect to host now" << endl;

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	char** p = phe->h_addr_list;

	service.sin_addr.s_addr = *reinterpret_cast<unsigned long*>(*p);
    int result = ::connect(m_SockID, reinterpret_cast<sockaddr*>(&service), sizeof(service));

	if(result == -1)
	{
		cout << "[Error]\t" << "Connecting failed" << endl;
		return;
	}

	cout << "[Info]\t" << "Successfully connected to " << server.c_str() << endl;
	cout << "[Info]\t" << "Trying handshake with server" << endl;

	SendLine("NICK "+ nick +"\r\n");
	SendLine("USER "+ nick +" \"\" \"\" :"+ nick +"\r\n");

	cout << "[Info]\t Handshake successful" << endl;
	cout << "[Info] starting recv thread" << endl;
	m_Thread = new Thread(&m_ThreadDelegate);
}

Connection::~Connection()
{
	//delete m_Thread;
}

bool Connection::SendLine(const string &line)
{
	return ::send(m_SockID, line.c_str(), line.length(), 0) >= 0;
}

void Connection::ReceiveThread()
{
	char buffer[128];

	int result = 0;
	int readbytes = 0;
	string tmp;

	do
	{	
		tmp = "";
		while((readbytes = ::recv(m_SockID, &buffer[0], 128, 0)) > 0)
		{
			tmp.append(buffer, readbytes);
			
			size_t pos = 0; tmp.find("\r\n");

			do
			{
				pos = tmp.find("\r\n");
				if(pos == string::npos)
				{
					pos = tmp.find("\n");
					if(pos == string::npos)
						break;
				}


				string s = tmp.substr(0, pos);
				tmp = tmp.substr(pos + 1, tmp.length());
				IrcMessage msg(s);
				m_Client->Handle(msg);


				cout << s << endl;
			} while(pos != string::npos);

			/*if(pos == string::npos)
				pos = tmp.find("\n");*/

			/*if(pos != string::npos)
			{
				string s = tmp.substr(0, pos);
				tmp = tmp.substr(pos + 1, tmp.length());
				IrcMessage msg(s);
				m_Client->Handle(msg);


				cout << s << endl;
				;
			}*/
		}
		
		if(readbytes < 0)
		{
			cout << "[Error] Recv failed" << endl;
			break;
		}



	}while(true);
}