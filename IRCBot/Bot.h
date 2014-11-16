#pragma once

#include "Connection.h"
#include <map>
#include <functional>

#include <fstream>

class BotBind;


class Bot : public ConnectionBase
{
public:
	Bot(const string &nick, const string &server);
	~Bot();

	void UserInput();
public:
	Connection m_Connection;

	void Handle(IrcMessage &message);

	void PING (IrcMessage &msg);

	void SayX(IrcMessage& msg);

	//std::map<string, BotBind> m_CommandAction;
	//typedef std::mem_fun1_t<void, Bot, IrcMessage> bla;
	typedef std::tr1::function<void(IrcMessage&)> func_type;
	typedef std::binder1st<std::mem_fun1_t<void, Bot, IrcMessage>> _Binder;
	std::map<string, func_type> m_CommandAction;
};

class BotBind
{
public:
	typedef void (Bot::*MemFunc)(IrcMessage&);

	BotBind(Bot &b, MemFunc func)
		: m_Bot(b), m_Func(func)
	{
	}

	void operator() (IrcMessage &msg)
	{
		(m_Bot.*m_Func)(msg);
	}

private:
	Bot &m_Bot;
	MemFunc m_Func;

};