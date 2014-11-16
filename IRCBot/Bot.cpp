#include "Bot.h"


std::ofstream g_File("log.txt");

Bot::Bot(const string &nick, const string &server)
	: m_Connection(server, nick, this)
{
	//m_CommandAction.insert(std::pair<string, BotBind>("PING", BotBind(*this, &Bot::PING)));
	m_CommandAction["PING"] = std::tr1::bind(&Bot::PING, *this, std::tr1::placeholders::_1);
	m_CommandAction["PRIVMSG"] = std::tr1::bind(&Bot::SayX, *this, std::tr1::placeholders::_1);

}

Bot::~Bot()
{
}

void Bot::Handle(IrcMessage &message)
{
	std::string cmd = message.m_Command;
	// remove all \n from the command
	std::size_t st = std::string::npos;
	while ((st = cmd.find_first_of("\n")) != std::string::npos) // bad bad bad but no idea how to make it more pretty :(
	{
		cmd.erase(st, 1);
	}

	std::map<string, func_type>::iterator it = m_CommandAction.find(cmd);

	if (it != m_CommandAction.end())
	{
		it->second(message);
	}
	else
		//cout << message.m_SourceMessage << endl;
		;

	g_File << message.m_SourceMessage << std::endl;

}


void Bot::UserInput()
{
	while(true)
	{
		string str;
		getline(cin, str);

		m_Connection.SendLine(str + "\r\n");

	}
}


void Bot::PING(IrcMessage &msg)
{
	m_Connection.SendLine("PONG :" + msg.m_ParamList[0] + "\r\n");
	/*cout << "#PONG" << endl;*/
}

void Bot::SayX(IrcMessage& msg)
{
	if(msg.m_ParamList[1][0] == '!')
	{
		string s = "PRIVMSG #sppro :" + msg.m_ParamList[1].substr(1, msg.m_ParamList[1].length() - 1) + "\r\n";
		m_Connection.SendLine(s);
	}
}