#include "stdafx.h"

namespace CodeGen
{
	void Head(Lex::LEX &lex, Log::LOG &log)
	{
		*(log.stream) << ".586\n";
		*(log.stream) << "\t.model flat, stdcall\n";
		*(log.stream) << "\tincludelib libucrt.lib\n";
		*(log.stream) << "\tincludelib kernel32.lib\n";
		*(log.stream) << "\tExitProcess PROTO : DWORD\n\n";
		*(log.stream) << "\tEXTRN outUs : proc\n";
		*(log.stream) << "\tEXTRN outStr : proc\n";
		*(log.stream) << "\tEXTRN outSymb : proc\n";
		*(log.stream) << "\tEXTRN outUsNewLine : proc\n";
		*(log.stream) << "\tEXTRN outStrNewLine : proc\n";
		*(log.stream) << "\tEXTRN outSymbNewLine : proc\n";
		*(log.stream) << "\tEXTRN DATE : proc \n";
		*(log.stream) << "\tEXTRN RANDOM : proc\n";
		*(log.stream) << "\n.stack 4096\n";
	}
	void Const(Lex::LEX &lex, Log::LOG &log)
	{
		*(log.stream) << ".const\n";
		for (int i = 0; i < lex.idTable.size; i++)
		{
			if (lex.TI_EL(i).idtype == IT::L)
			{
				*(log.stream) << "\t" << lex.TI_EL(i).id;
				if (lex.TI_EL(i).iddatatype == IT::STRING)
				{
					*(log.stream) << " BYTE '" << lex.TI_EL(i).value.vString.str << "', 0\n";
				}
				else if (lex.TI_EL(i).iddatatype == IT::USINT)
				{
					*(log.stream) << " DWORD " << lex.TI_EL(i).value.vUsint << std::endl;
				}
				else
				{
					*(log.stream) << " DWORD " << "\'" << lex.TI_EL(i).value.vSymbol << "\'" << std::endl;
				}
			
			}
		}
	}
	void Data(Lex::LEX &lex, Log::LOG &log)
	{
		*(log.stream) << ".data\n";
		for (int i = 0; i < lex.lexTable.size; i++)
		{
			if (lex.LT_EL(i).lexema == LEX_NEW)
			{
				*(log.stream) << "\t" << lex.TI_EL(lex.LT_EL(i + 2).idxTI).id;
				*(log.stream) << " DWORD ?\n";

				i += 3;
			}
		}
	}
	void Code(Lex::LEX &lex, Log::LOG &log)
	{
		std::stack<std::string> parms;		
		std::string funcName = "";			
		bool isInfuncion = false, isMain = false;
		int numOfPoint = 0, numOfCycle = 0, numOfOp = 0;				

		*(log.stream) << "\n.code\n\n";
		for (int i = 0; i < lex.lexTable.size; i++)
		{
			switch (lex.LT_EL(i).lexema)
			{
				case LEX_DEF:	
				{
					*(log.stream) << (funcName = lex.TI_EL(lex.LT_EL(++i).idxTI).id) << " PROC ";

					while (lex.LT_EL(i).lexema != LEX_RIGHTHESIS)
					{
						if (lex.LT_EL(i).idxTI != LT_TI_NULLIDX)
						{
							if (lex.TI_EL(lex.LT_EL(i).idxTI).idtype == IT::P)
							{
								*(log.stream) << lex.TI_EL(lex.LT_EL(i).idxTI).id << " : ";

								if (lex.TI_EL(lex.LT_EL(i).idxTI).iddatatype == IT::USINT)
								{
									*(log.stream) << "SDWORD";
								}
								else
								{
									*(log.stream) << "DWORD";
								}
							}
					
						}
						if (lex.LT_EL(i).lexema == LEX_COMMA)
							*(log.stream) << ", ";

						i++;
					}
					isInfuncion = true;
					*(log.stream) << std::endl;

					break;
				}
				case LEX_HEAD:
				{
					isMain = true;
					*(log.stream) << "main PROC\n";
					break;
				}
				case LEX_ASSIGN:
				{
					int resultPosition = i - 1;

					while (lex.LT_EL(i).lexema != LEX_DOT)
					{
						switch (lex.LT_EL(i).lexema)
						{
							case LEX_ID:
							case LEX_LITERAL:
							{

								if (lex.TI_EL(lex.LT_EL(i).idxTI).iddatatype == IT::STRING
									&& lex.TI_EL(lex.LT_EL(i).idxTI).idtype == IT::L)
								{
									*(log.stream) << "\tpush offset " << lex.TI_EL(lex.LT_EL(i).idxTI).id << std::endl;
									parms.push("offset " + lex.TI_EL(lex.LT_EL(i).idxTI).id);
								}
								else
								{
									*(log.stream) << "\tpush " << lex.TI_EL(lex.LT_EL(i).idxTI).id << std::endl;
									parms.push(lex.TI_EL(lex.LT_EL(i).idxTI).id);
								}

								break;
							}
							case LEX_SUBST:
							{
								for (int j = 1; j <= lex.LT_EL(i).priority; j++)
								{
									*(log.stream) << "\tpop edx\n";
								}

								if (!parms.empty())
								{
									for (int j = 1; j <= lex.LT_EL(i).priority; j++)
									{
										*(log.stream) << "\tpush " << parms.top() << std::endl;
										parms.pop();
									}
								}

								*(log.stream) << "\t\tcall " << lex.TI_EL(lex.LT_EL(i).idxTI).id << "\n\tpush eax\n";
								break;
							}
							case LEX_OPERATOR:
							case LEX_LOG_OPERATOR:
							{
								*(log.stream) << "\tpop ebx\n\tpop eax\n";

								if (lex.LT_EL(i).operation == LEFT_SHIFT_OPERATOR)
								{
									*(log.stream) << "\tcmp ebx, 8\n";
									*(log.stream) << "\tjg NOK" << numOfOp << std::endl;
									*(log.stream) << "\tmov cl, bl\n";
									*(log.stream) << "\tshl eax, cl\n";
									*(log.stream) << "\tcmp eax, " << TI_USINT_MAXVALUE + 1 << std::endl;
									*(log.stream) << "\tjl OK" << numOfOp << std::endl;
									*(log.stream) << "NOK" << numOfOp << ":\n";
									*(log.stream) << "\tpush " << lex.LT_EL(i).pn << std::endl;
									*(log.stream) << "\tpush " << lex.LT_EL(i).sn << std::endl;
									*(log.stream) << "\tpush eax\n";
									*(log.stream) << "\tcall outUsNewLine\n";
									if (isMain)
										*(log.stream) << "\tjmp ERROR\n";
									else
									{
										*(log.stream) << "\tpush 0\n";
										*(log.stream) << "\tcall ExitProcess\n";
									}
									*(log.stream) << "OK" << numOfOp++ << ":\n";
									*(log.stream) << "\tpush eax\n";
								}
								else if (lex.LT_EL(i).operation == RIGHT_SHIFT_OPERATOR)
								{
									*(log.stream) << "\tcmp ebx, 8\n";
									*(log.stream) << "\tjg NOK" << numOfOp << std::endl;
									*(log.stream) << "\tmov cl, bl\n";
									*(log.stream) << "\tshr eax, cl\n";
									*(log.stream) << "\tjmp OK" << numOfOp << std::endl;
									*(log.stream) << "NOK" << numOfOp << ":\n";
									*(log.stream) << "\txor eax, eax\n";
									*(log.stream) << "OK" << numOfOp++ << ":\n";
									*(log.stream) << "\tpush eax\n";
								}
								else if (lex.LT_EL(i).operation == std::string(1, LEX_LESS))
								{
									*(log.stream) << "\tcmp eax, ebx" << "\n";
									*(log.stream) << "\tjge M" << numOfPoint << "\n";
									*(log.stream) << "\tpush 1" << "\n";
									*(log.stream) << "\tjmp M" << numOfPoint + 1 << "\n";
									*(log.stream) << "M" << numOfPoint << ":\n";
									*(log.stream) << "\tpush 0" << "\n";
									*(log.stream) << "M" << numOfPoint + 1 << ":\n";
								}
								else if (lex.LT_EL(i).operation == std::string(1, LEX_GREATER))
								{
									*(log.stream) << "\tcmp eax, ebx" << "\n";
									*(log.stream) << "\tjle M" << numOfPoint << "\n";
									*(log.stream) << "\tpush 1" << "\n";
									*(log.stream) << "\tjmp M" << numOfPoint + 1 << "\n";
									*(log.stream) << "M" << numOfPoint << ":\n";
									*(log.stream) << "\tpush 0" << "\n";
									*(log.stream) << "M" << numOfPoint + 1 << ":\n";
								}
								else if (lex.LT_EL(i).operation == std::string(1, LEX_EQU))
								{
									*(log.stream) << "\tcmp eax, ebx" << "\n";
									*(log.stream) << "\tjne M" << numOfPoint << "\n";
									*(log.stream) << "\tpush 1" << "\n";
									*(log.stream) << "\tjmp M" << numOfPoint + 1 << "\n";
									*(log.stream) << "M" << numOfPoint << ":\n";
									*(log.stream) << "\tpush 0" << "\n";
									*(log.stream) << "M" << numOfPoint + 1 << ":\n";
								}
								else if (lex.LT_EL(i).operation == std::string(1, LEX_NEQU))
								{
									*(log.stream) << "\tcmp eax, ebx" << "\n";
									*(log.stream) << "\tje M" << numOfPoint << "\n";
									*(log.stream) << "\tpush 1" << "\n";
									*(log.stream) << "\tjmp M" << numOfPoint + 1 << "\n";
									*(log.stream) << "M" << numOfPoint << ":\n";
									*(log.stream) << "\tpush 0" << "\n";
									*(log.stream) << "M" << numOfPoint + 1 << ":\n";
								}

								break;
							}
						}
						i++;
					}
					numOfPoint++;
					*(log.stream) << "\tpop " << lex.TI_EL(lex.LT_EL(resultPosition).idxTI).id << "\n";

					break;
				}
				case LEX_RET:
				{
					*(log.stream) << "\tmov eax, ";

					i++;

					if (lex.TI_EL(lex.LT_EL(i).idxTI).idtype == IT::L)
					{
						if (lex.TI_EL(lex.LT_EL(i).idxTI).iddatatype == IT::USINT)
						{
							*(log.stream) << lex.TI_EL(lex.LT_EL(i++).idxTI).value.vUsint;
						}	

						else if (lex.TI_EL(lex.LT_EL(i).idxTI).iddatatype == IT::SYMBOL)
						{
							*(log.stream) << lex.TI_EL(lex.LT_EL(i++).idxTI).value.vSymbol;
						}

						else
						{
							*(log.stream) << "offset " << lex.TI_EL(lex.LT_EL(i++).idxTI).id;
						}
						
					}
					else
					{
						*(log.stream) << lex.TI_EL(lex.LT_EL(i++).idxTI).id;
					}

					*(log.stream) << "\n\tret\n";
					break;
				}
				case LEX_CLOSE:
				{
					if (isMain && !isInfuncion)
					{
						*(log.stream) << "ERROR:\n";
						*(log.stream) << "\tpush 0\n";
						*(log.stream) << "\tcall ExitProcess\n";
						*(log.stream) << "main ENDP\n";
						*(log.stream) << "end main";
					}
					if (isInfuncion)
					{
						*(log.stream) << funcName << " ENDP\n\n";
						isInfuncion = false;
					}
					break;
				}
				case LEX_OUT:
				{
					bool isNewLine = lex.LT_EL(i).operation == "writeline";
				
					if (lex.TI_EL(lex.LT_EL(i + 1).idxTI).iddatatype == IT::USINT)
					{
						*(log.stream) << "\tpush " << lex.TI_EL(lex.LT_EL(i + 1).idxTI).id << (isNewLine ? "\n\t\tcall outUsNewLine\n" : "\n\t\tcall outUs\n");
					}
					else if(lex.TI_EL(lex.LT_EL(i + 1).idxTI).iddatatype == IT::STRING)
					{
						if (lex.TI_EL(lex.LT_EL(i + 1).idxTI).idtype == IT::L)
						{
							*(log.stream) << "\tpush offset " << lex.TI_EL(lex.LT_EL(i + 1).idxTI).id << (isNewLine ? "\n\t\tcall outStrNewLine\n" : "\n\t\tcall outStr\n");
						}

						else
						{
							*(log.stream) << "\tpush  " << lex.TI_EL(lex.LT_EL(i + 1).idxTI).id << (isNewLine ? "\n\t\tcall outStrNewLine\n" : "\n\t\tcall outStr\n");
						}
					
					}
					else
					{
						*(log.stream) << "\tpush " << lex.TI_EL(lex.LT_EL(i + 1).idxTI).id << (isNewLine ? "\n\t\tcall outSymbNewLine\n" : "\n\t\tcall outSymb\n");
					}

					break;
				}
				case LEX_CYCLE:
				{
					*(log.stream) << ";--------------------------блок цикла------------------------------\n";
					*(log.stream) << "CYCLE_START" << numOfCycle << ":\n";

					*(log.stream) << "\tmov eax, " << lex.TI_EL(lex.LT_EL(i + 2).idxTI).id << "\n";
					*(log.stream) << "\tcmp eax, " << lex.TI_EL(lex.LT_EL(i + 4).idxTI).id << "\n";

					if (lex.LT_EL(i + 3).operation == std::string(1, LEX_GREATER))
					{
						*(log.stream) << "\tjle CYCLE_END" << numOfCycle << "\n";
					}
					else if (lex.LT_EL(i + 3).operation == std::string(1, LEX_LESS))
					{
						*(log.stream) << "\tjge CYCLE_END" << numOfCycle << "\n";
					}
					else if (lex.LT_EL(i + 3).operation == std::string(1, LEX_EQU))
					{
						*(log.stream) << "\tjne CYCLE_END" << numOfCycle << "\n";
					}
					else if (lex.LT_EL(i + 3).operation == std::string(1, LEX_NEQU))
					{
						*(log.stream) << "\tje CYCLE_END" << numOfCycle << "\n";
					}
					
					break;
				}
				case LEX_ENDC:
				{
					*(log.stream) << "jmp CYCLE_START" << numOfCycle << "\n";
					*(log.stream) << "CYCLE_END" << numOfCycle << ":\n";

					*(log.stream) << ";----------------------------------------------------------------------\n";

					numOfCycle += 1;

					break;
				}
			}
		}

	}

	void ChangeNames(Lex::LEX &lex)
	{
		for (int i = 0; i < lex.idTable.size; i++)
		{
			if (lex.TI_EL(i).idtype != IT::L &&
				lex.TI_EL(i).id != DATE && 
				lex.TI_EL(i).id != RANDOM)
			{
				lex.TI_EL(i).id = lex.TI_EL(i).id + std::to_string(i);
			}
				
		}
	}

	void Generation(Lex::LEX &lex, Log::LOG log)
	{
		ChangeNames(lex);

		*(log.stream) << ";========================= шапка ==============================\n";
		Head(lex, log);

		*(log.stream) << ";=================== сегмент констант =========================\n";
		Const(lex, log);

		*(log.stream) << ";=================== сегмент данных ============================\n";
		Data(lex, log);

		*(log.stream) << ";=================== сегмент кода ==============================\n";
		Code(lex, log);

		*(log.stream) << "\n;=============================================================\n";
	}
}