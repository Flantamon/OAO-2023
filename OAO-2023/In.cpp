#include "stdafx.h"

namespace In
{	
	IN getin(wchar_t infile[])
	{
		std::ifstream file(infile);
		IN in;
		int readChar, position, line = 1;
		std::string tempLexema = "";
		bool isShiftOperator = false;

		if (!file.is_open())
			throw ERROR_THROW(110);

		position = in.size = in.lines = in.ignor = 0;

		while (!file.eof())  
		{
			readChar = file.get();

			if (tempLexema.length() > IN_MAX_WORD_LEN)
				throw ERROR_THROW_IN(115, line + 1, position - tempLexema.length());

			if (readChar == '`')		
			{
				// обработка строкового литерала

				tempLexema += readChar;
				position++;

				do
				{
					if (readChar == IN_CODE_ENDL || readChar == IN_CODE_TAB)
						throw ERROR_THROW_IN(113, in.lines + 1, position);

					readChar = file.get();
					tempLexema += readChar;
					in.size++;
					position++;
				} while (readChar != '`');

				in.text.push({ tempLexema, line, position - (int)tempLexema.length()});
				tempLexema.clear();
				in.size++;

				continue;
			}

			if (readChar == IN_CODE_SPACE || in.code[readChar] == IN::S ||
				readChar == IN_CODE_ENDL || readChar == IN_CODE_TAB)
			{

				if (tempLexema.length())
				{
					in.text.push({ tempLexema, line, position - (int)tempLexema.length() });
					tempLexema.clear();
				}

				position++;

				if (readChar == IN_CODE_SPACE || readChar == IN_CODE_TAB)
				{
					in.ignor++;
				}
	
				if (in.code[readChar] == IN::S)
				{
					// проверка на операторы сдвига
					if (isShiftOperator &&
						(readChar == '>' || readChar == '~'))
					{
						if (in.text.back().word == "~" && readChar == '>'
						 || in.text.back().word == "<" && readChar == '~')
						{
							in.text.back().word += readChar;
							isShiftOperator = false;
							continue;
						}
					}
					else
						isShiftOperator = false;

					if (readChar == '~' || readChar == '<')
						isShiftOperator = true;
	
					std::string buf;
					buf += readChar;
					in.text.push({ buf, line, position - (int)buf.length() });
					tempLexema.clear();
				}

				if (readChar == IN_CODE_ENDL)
				{
					in.lines++;
					line++;
					position = 0;
				}
			}
			else if (in.code[readChar] != IN::F)
			{
				position++;
				tempLexema += readChar;
				in.size++;
			}
			else
				throw  ERROR_THROW_IN(111, in.lines + 1, position);

			if (in.size > IN_MAX_LEN_TEXT)
				throw ERROR_THROW(114);
		}
		file.close();
		return in;
	}
}