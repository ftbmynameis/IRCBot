

#include "Bot.h"
#include <vector>
#include <xfunctional>
#include <functional>
#include <cstdlib>

class Console
{
private:
	HANDLE m_ConsoleHandle;
	int m_Width, m_Height;
	std::vector<std::string> m_MessageBuffer;
	int m_BufferRefreshCount;
	int m_CurrentLine;
public:
	Console(HANDLE consolehandle = GetStdHandle(STD_OUTPUT_HANDLE), int w = 120, int h = 30)
		: m_ConsoleHandle(consolehandle), m_Width(w), m_Height(h), m_CurrentLine(0), m_MessageBuffer(h)
	{
		SMALL_RECT WindowSize = {0, 0, w, h};
		SetConsoleWindowInfo (GetStdHandle (STD_OUTPUT_HANDLE), TRUE, &WindowSize);
		COORD BufferSize = {w+1, h+1};
		SetConsoleScreenBufferSize (GetStdHandle (STD_OUTPUT_HANDLE), BufferSize);

		COORD cursorpos = {0, m_Height - 1};
		SetConsoleCursorPosition(m_ConsoleHandle, cursorpos);
		for(int i = 0; i < m_Width; i++)
		{
			//cursorpos.X++;
			//SetConsoleCursorPosition(m_ConsoleHandle, cursorpos);
			std::cout << "_";
		}
		std::cout << std::endl;
	}
	void PrintToConsole(const std::string& msg)
	{
		if(m_CurrentLine > m_MessageBuffer.size() - 1)
		{

		}
		else
		{
			m_MessageBuffer[m_CurrentLine] = msg;
			COORD cursorpos = {0, m_CurrentLine};
			SetConsoleCursorPosition(m_ConsoleHandle, cursorpos);
			std::cout << msg;
			m_CurrentLine++;
		}

		COORD cursorpos = {0, m_Height};
		SetConsoleCursorPosition(m_ConsoleHandle, cursorpos);
	}
};

Console g_Console;

int main()
{
	int x = 150, y = 50;
	SMALL_RECT WindowSize = {0, 0, x, y};
    SetConsoleWindowInfo (GetStdHandle (STD_OUTPUT_HANDLE), TRUE, &WindowSize);
    COORD BufferSize = {x+1, y+1};
    SetConsoleScreenBufferSize (GetStdHandle (STD_OUTPUT_HANDLE), BufferSize);


	Bot test("ftb_ist_super2", "irc.euirc.net");
	test.UserInput();

	cin.get();
	return 0;
}