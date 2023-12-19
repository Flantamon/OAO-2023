#include "stdafx.h"

namespace LT
{
	LexTable& Create()
	{ 
		LexTable *lt = new LexTable();
		lt->maxsize = LT_MAXSIZE;
		lt->size = 0;
		lt->table = std::vector<Entry>();
		return *lt;
	}
	void Add(LexTable& lexTable, Entry entry)
	{
		lexTable.table.push_back(entry);
		lexTable.size++;
	}
	Entry GetEntry(LexTable& lexTable, int n)
	{
		return LT_EL(n);
	}

	Entry CreateLex(char lex, int line, int pos, int id_in_TI, int prior)
	{
		LT::Entry result;
		result.priority = prior;
		result.lexema = lex;
		result.sn = line;
		result.pn = pos;
		result.idxTI = id_in_TI;
		result.operation = '\0';
		return result;
	}

	void Delete(LexTable& lextable)
	{
		delete &lextable;
	}
	void WriteTable(Log::LOG log, LexTable& lexTable)
	{
		int k = 0;

		for (int i = 0; i < lexTable.size; i++)
		{
			if (LT_EL(i).sn != k)
			{
				*(log.stream) << "\n" << LT_EL(i).sn << ": ";
				k = LT_EL(i).sn;

				if (LT_EL(i).idxTI != (int)LT_TI_NULLIDX)
					*(log.stream) << LT_EL(i).lexema;
				else
					*(log.stream) << LT_EL(i).lexema;
			}
			else
			{
				if (LT_EL(i).idxTI != (int)LT_TI_NULLIDX)
					*(log.stream) << LT_EL(i).lexema;
				else
					*(log.stream) << LT_EL(i).lexema;
			}
		}

		*(log.stream) << "\nКоличество лексем - " << lexTable.size << std::endl;
	}

}