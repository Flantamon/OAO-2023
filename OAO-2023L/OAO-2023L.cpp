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
				std::cout << "Ошибка 303: [SemA] Деление на 0 в выражении, строка ";
				std::cout << line << ", позиция " << position << std::endl;
			}
			else
			{
				std::cout << "Ошибка 314: [SemA] Значение целочисленного литерала не входит в диапазон допустимых значений, ";
				std::cout << "строка " << line << ", позиция " << position << std::endl;
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