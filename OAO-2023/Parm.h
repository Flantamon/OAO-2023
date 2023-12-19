#pragma once
#include "stdafx.h"
#define PARM_IN	 L"/in:"					//ключ для файла исходного кода
#define PARM_OUT L"/out:"				//ключ для файла объектного кода
#define PARM_LOG L"/log:"				//ключ для файла журнала
#define PARM_AN  L"/an:"	
#define PARM_MAX_SIZE 300				//max длина строки параметра
#define PARM_OUT_DEFAULT_EXT L".asm"	//расширение файла объектного кода по умолчанию
#define PARM_LOG_DEFAULT_EXT L".log"	//расширение файла протокола  по умолчанию
#define PARM_AN_DEFAULT_EXT L".an"	


namespace Parm			//обработка входных параметров
{
	struct PARM			//входные параметры
	{
		wchar_t in[PARM_MAX_SIZE];		// /in: имя файла исходного кода
		wchar_t out[PARM_MAX_SIZE];		// /out: имя файла объектного кода
		wchar_t log[PARM_MAX_SIZE];		// /in: имя файла протокола
		wchar_t an[PARM_MAX_SIZE];		
	};
											//int _tmain(int argc, _TCHAR* argv[])
	PARM getparm(int argc, _TCHAR* argv[]); //сформировать struct PARM на основе параметров функции main
}