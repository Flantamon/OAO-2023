#include "OAO-2023L.h"

extern "C"
{
	char* DATE(void)
	{
		time_t rawtime;
		struct tm timeinfo;
		char* buffer = (char*)calloc(SIZE, sizeof(char));
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		strftime(buffer _Notnull_, SIZE, "Date: %d.%m.%y", &timeinfo);
		return buffer;
	}

	int RANDOM()
	{
		srand(static_cast<unsigned int>(time(0)));
		return rand() % 255 + 1;
	}

	void outStr(char* value)
	{
		system("chcp 1251 > nul");
		std::cout << value;
	}

	void outStrNewLine(char* value)
	{
		outStr(value);
		std::cout << std::endl;
	}

	void outUs(int value, int line = -1, int position = -1)
	{
		if(value <= USINT_MAXVALUE && value >= USINT_MINVALUE )
			std::cout << value;
		else
		{
			std::system("cls");
			std::cout << TRACE_STR << std::endl;

			if (value == DIVISION_BY_ZERO)
			{
				std::cout << "������ 303: [SemA] ������� �� 0 � ���������, ������ ";
				std::cout << line << ", ������� " << position << std::endl;
			}
			else
			{
				std::cout << "������ 314: [SemA] �������� �������������� �������� �� ������ � �������� ���������� ��������, ";
				std::cout << "������ " << line << ", ������� " << position << std::endl;
			}

			std::cout << TRACE_STR << std::endl;
		}	
	}

	void outUsNewLine(int value, int line = -1, int position = -1)
	{
		outUs(value, line, position);
		std::cout << std::endl;
	}

	void outSymb(char value)
	{
		std::cout << value;
	}

	void outSymbNewLine(char value)
	{
		outSymb(value);
		std::cout << std::endl;
	}
}