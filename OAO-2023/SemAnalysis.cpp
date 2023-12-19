#include "SemAnalysis.h"

namespace SA
{	
	Function::Function()
	{
		size = 0;
		name = "";
		arrayOfDataType = new IT::IDDATATYPE[FUNC_MAX_SIZE]
		{
			IT::IDDATATYPE::OFF, IT::IDDATATYPE::OFF, IT::IDDATATYPE::OFF,
			IT::IDDATATYPE::OFF, IT::IDDATATYPE::OFF
		};
	};

	LocalFunctions::LocalFunctions()
	{
		size = -1;
		arrayOfFunction = new Function[FUNC_MAX_SIZE];
	}

	void startSemAnalysis(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		isStaticFunc(lexTable, idTable);
		isCorrectType(lexTable, idTable);
		isIdWasUsed(lexTable, idTable);
	}

	//ПОСЛЕ ПОСТРОЕНИЯ ПОЛЬСКОЙ ЗАПИСИ
	void isCorrectType(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		for (int i = 0; i < lexTable.size; i++)
		{ 
			if (lexTable.table[i].lexema == LEX_ID || lexTable.table[i].lexema == LEX_LITERAL)
			{
				// Проверяет, что логические выражения используются в правильном месте
				//if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::BOOLEAN)
				//{
				//	if (lexTable.table[i + 1].lexema == LEX_ASSIGN && i + 4 < lexTable.size)
				//	{
				//		if (lexTable.table[i + 2].idxTI != LT_TI_NULLIDX &&
				//			idTable.table[lexTable.table[i + 2].idxTI].iddatatype == IT::USINT &&
				//			lexTable.table[i + 3].idxTI != LT_TI_NULLIDX &&
				//			idTable.table[lexTable.table[i + 3].idxTI].iddatatype == IT::USINT &&
				//			lexTable.table[i + 4].lexema == LEX_LOG_OPERATOR ||
				//			idTable.table[lexTable.table[i + 2].idxTI].iddatatype == IT::BOOLEAN
				//			)
				//		{
				//			i += 4;
				//			continue;
				//		}
				//		else
				//			throw ERROR_THROW_IN(307, lexTable.table[i].sn, lexTable.table[i].pn);
				//	}	// Проверка на то, что в условной конструкции только булевая переменная
				//	else if (lexTable.table[i - 2].lexema == LEX_CYCLE)
				//	{
				//		if(lexTable.table[i - 1].lexema != LEX_LEFTHESIS
				//			|| lexTable.table[i+1].lexema != LEX_RIGHTHESIS)
				//			throw ERROR_THROW_IN(317, lexTable.table[i].sn, lexTable.table[i].pn);
				//	}
				//}
				//else
				//{
					// Проверка на то, что в условной конструкции логич. выражение
					if (lexTable.table[i - 2].lexema == LEX_CYCLE)
					{
						if (idTable.table[lexTable.table[i].idxTI].iddatatype != IT::USINT ||
							lexTable.table[i + 1].lexema != LEX_LOG_OPERATOR ||
							idTable.table[lexTable.table[i + 2].idxTI].iddatatype != IT::USINT)
							throw ERROR_THROW_IN(317, lexTable.table[i].sn, lexTable.table[i].pn);
					}
					if (idTable.table[lexTable.table[i].idxTI].iddatatype != IT::USINT)
					{
						i++;
						while (i < lexTable.size && 
							lexTable.table[i].lexema != LEX_DOT && 
							lexTable.table[i].lexema != LEX_OPERATOR &&
							lexTable.table[i].lexema != LEX_LOG_OPERATOR)
							i++;

						if (lexTable.table[i].lexema == LEX_OPERATOR
							|| lexTable.table[i].lexema == LEX_LOG_OPERATOR)
							throw ERROR_THROW_IN(316, lexTable.table[i].sn, lexTable.table[i].pn);
					}
				//}
			}
		}
	}
	
	void isIdWasUsed(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		std::string tempidName = "";
		for (int i = 0, j = 0; i < lexTable.size; i++)
		{
			bool isFind = false;
			if (lexTable.table[i].lexema == LEX_NEW || lexTable.table[i].lexema == LEX_DEF)
			{
				j = i; 

				if(lexTable.table[i].lexema == LEX_NEW)	// nti
					tempidName = idTable.table[lexTable.table[j + 2].idxTI].id;
				else     // tfi
					tempidName = idTable.table[lexTable.table[j + 1].idxTI].id;

				j += 3;	// сдвигаем указатель

				while (j < lexTable.size && !isFind)
				{
					if (lexTable.table[j].lexema == LEX_ID || lexTable.table[j].lexema == LEX_SUBST)
					{
						if (tempidName == idTable.table[lexTable.table[j].idxTI].id)
								isFind = true;
					}
					j++;
				}

				if(!isFind) throw ERROR_THROW_IN(311, lexTable.table[i].sn, lexTable.table[i].pn);

			}
		}
	}
	
	void isStaticFunc(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		for (int i = 0, j = 0; i < lexTable.size; i++)
		{
			if(lexTable.table[i].lexema == LEX_SUBST 
				&& (idTable.table[lexTable.table[i].idxTI].id == DATE
				|| idTable.table[lexTable.table[i].idxTI].id == RANDOM))
			{
			if (lexTable.table[i - 1].lexema == LEX_LITERAL || lexTable.table[i - 1].lexema == LEX_ID)
					throw ERROR_THROW_IN(305, lexTable.table[i].sn, lexTable.table[i - 2].pn);
			}
		}
	}

	ERROR CheckParms(LocalFunctions* localFunctions, Function* arr_of_it_entry, std::string name)
	{
		for (int i = 0; i < localFunctions->size + 1; i++)
		{
			if (localFunctions->arrayOfFunction[i].name == name)
			{
				if (localFunctions->arrayOfFunction[i].size == arr_of_it_entry->size)
				{
					for (int j = 0; j < localFunctions->arrayOfFunction[i].size; j++)
					{
						if (localFunctions->arrayOfFunction[i].arrayOfDataType[j] != arr_of_it_entry->arrayOfDataType[j])
							return ERROR::PARMS_TYPE;
					}
				}
				else
					return ERROR::PARMS_COUNT;
			}
		}
		return ERROR::NO_ERRORS;
	}
}