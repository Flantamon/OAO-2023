#include"stdafx.h"

namespace PN
{	
	void AddToResult(IT::IdTable& idTable, LT::Entry* result, LT::Entry elementLT, int& pos, int& flag, int lenout, int lextablePos)
	{
		if (TI_EL(elementLT.idxTI).idxfirstLE >= lextablePos && TI_EL(elementLT.idxTI).idxfirstLE < (lextablePos + lenout))
			TI_EL(elementLT.idxTI).idxfirstLE = lextablePos + flag;

		result[pos] = elementLT;
		flag++;
		pos++;
	}

	void AddToResult(std::stack<LT::Entry>& stack, IT::IdTable& idTable, LT::Entry* result, int& pos, int& flag, int lenout, int lextablePos)
	{	
		if (stack.top().idxTI != LT_TI_NULLIDX)
		{
			if (TI_EL(stack.top().idxTI).idxfirstLE >= lextablePos && TI_EL(stack.top().idxTI).idxfirstLE < (lextablePos + lenout))
				TI_EL(stack.top().idxTI).idxfirstLE = lextablePos + flag;
		}

		result[pos] = stack.top();
		flag++;
		pos++;
		stack.pop();
	}

	bool PolishNotation(int lextablePos, LT::LexTable & lexTable, IT::IdTable & idTable)
	{
		std::stack<LT::Entry> stackLTelements;
		LT::Entry* elementsLT = new LT::Entry[lexTable.size];
		LT::Entry tempLTEntry, bufEntry;
		int ncomma, flag, waste, funcPositionTI, lenght, lenout, dotId;					
		bool wasLogOper;			

		wasLogOper = false;
		ncomma = flag = waste = lenght = lenout = 0;
		dotId = funcPositionTI = -1;

		for (int i = lextablePos; LT_EL(i).lexema != LEX_DOT; i++)
		{
			lenout = i + 1;
			dotId = i + 1;
		}

		for (int i = lextablePos; i < lenout; i++)
		{
			tempLTEntry = LT_EL(i);

			if (tempLTEntry.lexema == LEX_ID || tempLTEntry.lexema == LEX_LITERAL || tempLTEntry.lexema == LEX_LIB_FUNC)
			{
				if (TI_EL(tempLTEntry.idxTI).idtype == IT::F)
				{
					funcPositionTI = tempLTEntry.idxTI;
					continue;
				}
				AddToResult(idTable, elementsLT, tempLTEntry, lenght, flag, lenout, lextablePos);
			}
			else
			{
				if (tempLTEntry.lexema == LEX_OPERATOR || tempLTEntry.lexema == LEX_LOG_OPERATOR)
				{
					if (wasLogOper && tempLTEntry.lexema == LEX_OPERATOR)
						throw ERROR_THROW_IN(312, LT_EL(lextablePos).sn, LT_EL(lextablePos).pn);

					// Проверка на корректность выражения
					if (tempLTEntry.lexema == LEX_LOG_OPERATOR) wasLogOper = true;

					while (!stackLTelements.empty() && stackLTelements.top().priority >= tempLTEntry.priority)
					{
						if (stackLTelements.top().lexema != LEX_LEFTHESIS)
							AddToResult(stackLTelements, idTable, elementsLT, lenght, flag, lenout, lextablePos);
						else
							break;
					}
					stackLTelements.push(tempLTEntry);
				}

				if (tempLTEntry.lexema == LEX_COMMA)
				{
					ncomma++;
					while (stackLTelements.top().lexema == LEX_OPERATOR)
					{
						AddToResult(stackLTelements, idTable, elementsLT, lenght, flag, lenout, lextablePos);
					}
				}
				else if (tempLTEntry.lexema != LEX_RIGHTHESIS)
				{
					if (stackLTelements.empty() || stackLTelements.top().lexema == LEX_LEFTHESIS || tempLTEntry.lexema == LEX_LEFTHESIS)
						stackLTelements.push(tempLTEntry);
				}

				if (tempLTEntry.lexema == LEX_RIGHTHESIS && tempLTEntry.priority != 4)
				{
					waste += 2;
					while (stackLTelements.top().lexema != LEX_LEFTHESIS)
					{
						AddToResult(stackLTelements, idTable, elementsLT, lenght, flag, lenout, lextablePos);
					}
					stackLTelements.pop();
				}

				if (tempLTEntry.lexema == LEX_RIGHTHESIS && tempLTEntry.priority == 4)
				{
					bufEntry.lexema = LEX_SUBST;
					bufEntry.idxTI = funcPositionTI;
					bufEntry.sn = tempLTEntry.sn;
					bufEntry.pn = tempLTEntry.pn;
					// количество параметров в функции
					bufEntry.priority = ncomma + 1;		
					elementsLT[lenght] = bufEntry;
					lenght++;

					if (ncomma != 0)
					{
						waste += ncomma;
						ncomma = 0;
					}

					while (stackLTelements.top().lexema != LEX_LEFTHESIS)
					{
						AddToResult(stackLTelements, idTable, elementsLT, lenght, flag, lenout, lextablePos);
					}
					stackLTelements.pop();
					waste += 2;
				}
			}
		}
		while (!stackLTelements.empty())
		{
			AddToResult(stackLTelements, idTable, elementsLT, lenght, flag, lenout, lextablePos);
		}

		for (int i = lextablePos, k = 0; i < lextablePos + lenght; i++, k++)
		{
			// Запись в ТЛ
			LT_EL(i) = elementsLT[k];										
		}

		// Вставка элемента с точкой
		LT_EL(lextablePos + lenght) = LT_EL(dotId);		

		for (int i = 0; i < waste; i++)
		{
			lexTable.size--;

			// сдвиг таблицы лексем, т.к после преобразования она может быть меньше 
			for (int j = lextablePos + lenght + 1; j < lexTable.size; j++)	
			{
				LT_EL(j) = LT_EL(j + 1);
				if (LT_EL(j).idxTI != LT_TI_NULLIDX && TI_EL(LT_EL(j).idxTI).idxfirstLE == (j + 1))
					TI_EL(LT_EL(j).idxTI).idxfirstLE--;
			}
		}
		return true;
	}

	void FindExpressions(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		for (int i = 0; i < lexTable.size; i++)
		{
			if (LT_EL(i).lexema == LEX_ASSIGN)
				PolishNotation(++i, lexTable, idTable);
		}
	}
}
