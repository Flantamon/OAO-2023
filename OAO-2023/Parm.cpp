#include "stdafx.h"
namespace Parm
{
		PARM getparm(int argc, _TCHAR* argv[])
		{
			PARM par;
			bool isIN, isOUT, isLOG , isAN;
			wchar_t in[PARM_MAX_SIZE], out[PARM_MAX_SIZE], log[PARM_MAX_SIZE], an[PARM_MAX_SIZE];

			isIN = isOUT = isLOG = isAN = false;

			for (int i = 0; i < argc; i++) 
			{
				if (wcslen(argv[i]) >= PARM_MAX_SIZE)
					throw ERROR_THROW(101);

				if (wcsstr(argv[i], PARM_IN))
				{
					wcscpy_s(in, argv[i] + wcslen(PARM_IN)); // + 1 для обхода двоеточия
					isIN = true;
				}
				else if (wcsstr(argv[i], PARM_OUT))
				{
					wcscpy_s(out, argv[i] + wcslen(PARM_OUT));
					isOUT = true;
				}
				else if (wcsstr(argv[i], PARM_AN))
				{
					wcscpy_s(an, argv[i] + wcslen(PARM_AN));
					isAN = true;
				}
				else if (wcsstr(argv[i], PARM_LOG)) 
				{
					wcscpy_s(log, argv[i] + wcslen(PARM_LOG));
					isLOG = true;
				}
			}

			if(!isIN) 
				throw ERROR_THROW(100);

			if (!isOUT)
			{
				wcscpy_s(out, in);
				wcscat_s(out, PARM_OUT_DEFAULT_EXT);
			}

			if (!isLOG)
			{
				wcscpy_s(log, in);
				wcscat_s(log, PARM_LOG_DEFAULT_EXT);
			}

			if (!isAN)
			{
				wcscpy_s(an, in);
				wcscat_s(an, PARM_AN_DEFAULT_EXT);
			}

			wcscpy_s(par.in, in); wcscpy_s(par.out, out); wcscpy_s(par.log, log); wcscpy_s(par.an, an);
			return par;
		}
}