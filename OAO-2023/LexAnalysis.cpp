#include "stdafx.h"
namespace Lex
{
#pragma region FST_CHECK
	FST::STATE_MACHINE SearchFST(std::string src)
	{
		int size;
		FST::STATE_MACHINE result;
		FST::FST* fstArray;
		if (src.size() == 1)
		{
			fstArray = FST::fstSingleLex();
			size = FST_SINGLE_LEX_SIZE;
		}
		else
		{
			fstArray = FST::fstWords();
			size = FST_WORDS_SIZE;
		}
		// последовательно проходим по автоматам
		for (int i = 0; i < size; i++)
		{
			fstArray[i].str = src;
			if (execute(fstArray[i]))
			{
				if (size == FST_WORDS_SIZE && i == FST_USINT_HEX_INDEX)
					result.ult = FST::HEX_LIT;
				else if (i == FST_USINT_DEC_INDEX)
					result.ult = FST::DEC_LIT;
				
				result.lexema = fstArray[i].lexema;
				result.lextype = fstArray[i].lexType;
				delete []fstArray;
				fstArray = nullptr;
				return result;
			}
		}
		delete[] fstArray;
		fstArray = nullptr;
		return { FST::UNIDENTIFIED, 0 };
	}
#pragma endregion

	LEX LexAnalysis(In::IN in, Log::LOG log)
	{
		LT::LexTable lexTable = LT::Create();
		IT::IdTable idTable = IT::Create();

		bool isInFunc = false, isParmInFunc = false, isVariable = false, isRet = false,
			 isFunction = false, isParm = false, isAssign = false, isLibFunc = false;	
		short isHead = 0;
		LEX tables;
		std::string funcName = "",		// для семантического  анализа  параметров функции
				funcHesisName = "";		//для области видимости 
		SA::Function *arrayOfITEntry = new SA::Function();
		SA::LocalFunctions *localFunctions = new SA::LocalFunctions();
		In::WORD streamWord;
		LT::Entry elementLT;
		FST::STATE_MACHINE lexFromFst;	
		IT::IDDATATYPE dataType = IT::OFF;		
		int nLit = 0;
		int size = in.text.size();

		for (int i = 0; i < size; i++) 
		{
			streamWord = in.text.front();
			in.text.pop();
			lexFromFst = SearchFST(streamWord.word);
			elementLT = LT::CreateLex(lexFromFst.lexema, streamWord.line, streamWord.pos, LT_TI_NULLIDX, LT_NOPRIORITY);
			
			switch (lexFromFst.lextype) 
			{
				case FST::UNIDENTIFIED:
				{
					throw ERROR_THROW_IN(120, elementLT.sn, elementLT.pn);
					break;
				}
				case FST::ID:		
				{
					int index = 0;
					if (isFunction)
					{
						isParmInFunc = true;
						funcName = streamWord.word;
						isFunction = false;

						if (IT::IsDublId(idTable, streamWord.word.substr(0, ID_MAXSIZE)) != TI_NULLIDX)
						{
							throw ERROR_THROW_IN(300, elementLT.sn, elementLT.pn);
						}
						else
						{	// если тип возвр. значения ф-ции равен одному из стандартых типов
							if (dataType != IT::D)		
							{
								idTable.table.push_back(CreateId(lexTable.size, streamWord.word.substr(0, ID_MAXSIZE), dataType, IT::F));
								elementLT.idxTI = idTable.size++;
								localFunctions->size++;
							}
							else
								throw ERROR_THROW_IN(301, elementLT.sn, elementLT.pn)
						}
						break;
					}
					if (isParmInFunc)
					{	//если нет в ТИ
						if ((index = IT::IsId(idTable, funcName + streamWord.word.substr(0, ID_MAXSIZE))) == TI_NULLIDX)		
						{
								idTable.table.push_back(CreateId(lexTable.size, funcName + streamWord.word.substr(0, ID_MAXSIZE), dataType, IT::P));
								elementLT.idxTI = idTable.size++;
						}
						else
						{
							if (isHead)
							{
								arrayOfITEntry->arrayOfDataType[arrayOfITEntry->size++] = TI_EL(index).iddatatype;
							}
							elementLT.idxTI = index;
						}
					}
					if (isInFunc)
					{
						if ((index = IT::IsId(idTable, funcName + streamWord.word.substr(0, ID_MAXSIZE))) == TI_NULLIDX)
						{
							if ((index = IT::IsId(idTable, streamWord.word.substr(0, ID_MAXSIZE))) != TI_NULLIDX)
							{
								elementLT.idxTI = index;
								if (TI_EL(index).idtype == IT::F)
								{	// Вызов функции не из точки входа (HEAD)
									if (!isHead) throw ERROR_THROW_IN(304, elementLT.sn, LT_EL(lexTable.size - 1).pn);

									funcHesisName = TI_EL(index).id;
									isParmInFunc = true;
								}
							}

							if (isVariable)
							{
								if (dataType == IT::STRING)
								{
									idTable.table.push_back(CreateId(lexTable.size, funcName + streamWord.word.substr(0, ID_MAXSIZE), dataType, IT::V, (std::string)""));
								}
								else if(dataType == IT::USINT)
								{
									idTable.table.push_back(CreateId(lexTable.size, funcName + streamWord.word.substr(0, ID_MAXSIZE), dataType, IT::V, 0));
								}
								else //symbol
								{
									idTable.table.push_back(CreateId(lexTable.size, funcName + streamWord.word.substr(0, ID_MAXSIZE), dataType, IT::V, (char)'0'));
								}
								elementLT.idxTI = idTable.size++;
							}
							//семантика
							else if (index == TI_NULLIDX)
								throw ERROR_THROW_IN(306, streamWord.line, elementLT.pn);
						}
						else		
						{ // если есть индекс вхождения в ТИ
							elementLT.idxTI = index;
							if (isVariable)
							{
								throw ERROR_THROW_IN(300, streamWord.line, elementLT.pn)
							}
							if (isAssign && !isParmInFunc)			
							{
								if (TI_EL(index).iddatatype != dataType)
									if (TI_EL(elementLT.idxTI).iddatatype != IT::USINT)
										throw ERROR_THROW_IN(307, streamWord.line, elementLT.pn);
							}
						}
					}

					if (isRet)
					{
						if (TI_EL(IT::IsId(idTable, funcName)).iddatatype != TI_EL(elementLT.idxTI).iddatatype)
							throw ERROR_THROW_IN(301, streamWord.line, elementLT.pn);	
					}

					break;
				}

				case FST::STATIC_LIB:
				{
					int index;

					if (isFunction)
						throw ERROR_THROW_IN(309, elementLT.sn, elementLT.pn);

					if ((index = IT::IsId(idTable, streamWord.word)) == TI_NULLIDX)
					{
						idTable.table.push_back(CreateId(lexTable.size, streamWord.word, IT::STRING, IT::F));
						elementLT.idxTI = idTable.size++;
					}
					else
						elementLT.idxTI = index;

					elementLT.lexema = LEX_LIB_FUNC;
					isLibFunc = true;

					break;
				}

				case FST::LITERAL:
				{
					switch (lexFromFst.lexema) 
					{
						case LEX_USINT:
						{
							char *temp;
							int number = 0;

							//конвертация в десятичную систему счисления
							if(lexFromFst.ult == FST::HEX_LIT)
								number = strtol(streamWord.word.c_str(), &temp, 16);
							else
								number = strtol(streamWord.word.c_str(), &temp, 10);

							// проверка на допустимое значение целочисленного литерала
							if (number > TI_USINT_MAXVALUE || number < TI_USINT_DEFAULT)
								throw ERROR_THROW_IN(314, streamWord.line, elementLT.pn);

							int index = IT::CheckLiteralId(idTable, number);

							if (index == TI_NULLIDX)
							{	
								idTable.table.push_back(CreateId(lexTable.size, LITERAL_NAME + std::to_string(nLit++), IT::USINT, IT::L, number));
								elementLT.idxTI = idTable.size++;
							}
							else
								elementLT.idxTI = index;

							break;
						}
						case LEX_STRING:
						{	
							std::string str;

							//убираются кавычки
							if (streamWord.word.length() > 2)
								str = streamWord.word.substr(1, streamWord.word.length() - 2);
							else
								str = "";

							//проверка на допустимую длину строкового литерала
							if(str.length() > TI_STR_MAXSIZE) 
								throw ERROR_THROW_IN(315, streamWord.line, elementLT.pn);

							int index = IT::CheckId(idTable, str);

							if (index == TI_NULLIDX)
							{
								idTable.table.push_back(CreateId(lexTable.size, LITERAL_NAME + std::to_string(nLit++), IT::STRING, IT::L, str));
								elementLT.idxTI = idTable.size++;
							}
							else
								elementLT.idxTI = index;

							break;
						}

						case LEX_SYMBOL:
						{										//символ достаётся из кавычек
							int index = IT::CheckId(idTable, (char)(streamWord.word).c_str()[1]);	

							if (index == TI_NULLIDX)
							{																
								idTable.table.push_back(CreateId(lexTable.size, LITERAL_NAME + std::to_string(nLit++), IT::SYMBOL, IT::L, (char)(streamWord.word).c_str()[1]));
								elementLT.idxTI = idTable.size++;
							}
							else
								elementLT.idxTI = index;
							break;
						}
					}

					if (isRet)
					{
						if(TI_EL(IT::IsId(idTable, funcName)).iddatatype != TI_EL(elementLT.idxTI).iddatatype)
							throw ERROR_THROW_IN(301, elementLT.sn, elementLT.pn);
					}

					if (isHead && isParmInFunc)
					{
						arrayOfITEntry->arrayOfDataType[arrayOfITEntry->size++] = TI_EL(elementLT.idxTI).iddatatype;
					}

					if (isAssign && !isParmInFunc)
					{	
						if (TI_EL(elementLT.idxTI).iddatatype != dataType)
							if (TI_EL(elementLT.idxTI).iddatatype != IT::USINT)
								throw ERROR_THROW_IN(307, streamWord.line, elementLT.pn);		 // несоответствие типов в операторе присваивания
					}
					elementLT.lexema = LEX_LITERAL;
					break;
				}

				case FST::NEED_PRIORITY:
				{
					switch (streamWord.word[0])
					{
						case LEX_COMMA:
						{
							elementLT.priority = 1;
							break;
						}
							
						case LEX_LEFTHESIS:
						{
							if (isParmInFunc && elementLT.lexema == LEX_LEFTHESIS)
							{
								elementLT.priority = 4;
								isParm = true;
							}
							else if (isLibFunc)
								elementLT.priority = 4;	
							else
								elementLT.priority = 0;

							break;
						}
							
						case LEX_RIGHTHESIS:
						{
							if (isParmInFunc && isParm)
							{
								if (isHead)
								{
									SA::ERROR rc = CheckParms(localFunctions, arrayOfITEntry, funcHesisName);

									if (rc == SA::ERROR::PARMS_TYPE)
										throw ERROR_THROW_IN(302, elementLT.sn, elementLT.pn);

									if (rc == SA::ERROR::PARMS_COUNT)											
										throw ERROR_THROW_IN(318, elementLT.sn, elementLT.pn);

									funcHesisName = "";
									arrayOfITEntry->size = 0;
								}
								
								elementLT.priority = 4;
								isParm = false;
								isParmInFunc = false;
							}
							else if (isLibFunc)
							{
								elementLT.priority = 4;
								isLibFunc = false;
							}
							else
								elementLT.priority = 0;

							break;
						}
					}
					break;
				}

				case FST::DEF_LEX:
				{
					switch (lexFromFst.lexema) 
					{
						case LEX_HEAD:
						{
							isHead++;
							funcName = streamWord.word;
							break;
						}

						case LEX_DEF:
						{
							isFunction = true;
							break;
						}

						case LEX_SYMBOL:
						case LEX_USINT:
						case LEX_STRING:
						{
							IT::IDDATATYPE type = IT::OFF;

							if (lexFromFst.lexema == LEX_STRING)
								type = IT::STRING;
							else if(lexFromFst.lexema == LEX_USINT)
								type = IT::USINT;
							else
								type = IT::SYMBOL;

							if (isParmInFunc && !isHead) 
							{
								if (localFunctions->size >= FUNC_MAX_SIZE)
									throw ERROR_THROW(305);

								// Возвращает позицию, где начинается название функции
								if (localFunctions->arrayOfFunction[localFunctions->size].size >= FUNC_MAX_SIZE)
									throw ERROR_THROW_IN(313, elementLT.sn, LT_EL(TI_EL(IT::IsId(idTable, funcName)).idxfirstLE).pn);

								localFunctions->arrayOfFunction[localFunctions->size].arrayOfDataType[localFunctions->arrayOfFunction[localFunctions->size].size++] = type;
								localFunctions->arrayOfFunction[localFunctions->size].name = funcName;
								
							}
							dataType = type;
							elementLT.lexema = LEX_T;
							break;
						}

						case LEX_NEW:
						{
							if (isInFunc)
								isVariable = true;
							break;
						}							

						case LEX_RET:
						{
							isRet = true;
							break;
						}

						case LEX_ASSIGN:
						{
							isAssign = true;

							if(LT_EL(lexTable.size - 1).idxTI != LT_TI_NULLIDX)
								dataType = TI_EL(LT_EL(lexTable.size - 1).idxTI).iddatatype;
							break;
						}
							
						case LEX_DOT:
						{	
							isVariable = false;
							isAssign = false;
							isRet = false;
							break;
						}
						
						case LEX_OPEN:
						{
							isInFunc = true;
							break;
						}
							
						case LEX_CLOSE:
						{
							isInFunc = false;
							break;
						}

						case LEX_OUT:
						{
							elementLT.operation = streamWord.word;
							break;
						}
					}
					break;
				}
	
				case FST::LOG_OPERATION:
				case FST::OPERATION:
				{
					elementLT.operation = streamWord.word; 
					if (streamWord.word.size() == 2)
					{
						elementLT.priority = 3;
					}
					else
					{
						switch (streamWord.word[0])
						{
						case LEX_LESS:
						case LEX_GREATER:
							elementLT.priority = 2;
							break;

						case LEX_EQU:
						case LEX_NEQU:
							elementLT.priority = 3;
							break;
						}
					}

					elementLT.idxTI = TI_NULLIDX;
					break;
				}

			}
			lexTable.table.push_back(elementLT);
			lexTable.size++;

			if (lexTable.size > LT_MAXSIZE)
				throw ERROR_THROW(121);

			if (idTable.size > TI_MAXSIZE)
				throw ERROR_THROW(122);
		}

		delete[]arrayOfITEntry->arrayOfDataType;
		delete arrayOfITEntry;

		delete[] localFunctions->arrayOfFunction;
		delete localFunctions;
		

		if (isHead < 1)
			throw ERROR_THROW(310);	

		if (isHead > 1)
			throw ERROR_THROW(308);

		IT::WriteTable(log, idTable);
		LT::WriteTable(log, lexTable);
		tables.idTable = idTable;
		tables.lexTable = lexTable;
		return tables;
	}
}