#include "stdafx.h"
using namespace std;
namespace IT
{

	IdTable& Create()
	{
		IdTable *it = new IdTable();
		it->maxsize = LT_MAXSIZE;
		it->size = 0;
		it->table = std::vector<Entry>();
		return *it;
	}
	void Add(IdTable& idTable, Entry entry)
	{
		idTable.table.push_back(entry);
		idTable.size++;
	}
	Entry GetEntry(IdTable& idTable, int n)
	{
		return TI_EL(n);
	}

	Entry CreateId(int line, std::string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type)
	{
		IT::Entry result;
		result.id = id;
		result.idxfirstLE = line;
		result.iddatatype = id_data_type;
		result.idtype = id_type;
		return result;
	}

	Entry CreateId(int line, std::string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, int vUsint)
	{
		IT::Entry result;
		result.id = id;
		result.idxfirstLE = line;
		result.iddatatype = id_data_type;
		result.idtype = id_type;
		result.value.vUsint = vUsint;
		return result;
	}

	Entry CreateId(int line, std::string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, char vSymbol)
	{
		IT::Entry result;
		result.id = id;
		result.idxfirstLE = line;
		result.iddatatype = id_data_type;
		result.idtype = id_type;
		result.value.vSymbol = vSymbol;
		return result;
	}



	Entry CreateId(int line, std::string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, std::string vstr)
	{
		IT::Entry result;
		result.id = id;
		result.idxfirstLE = line;
		result.iddatatype = id_data_type;
		result.idtype = id_type;
		result.value.vString.str = vstr;
		result.value.vString.len = vstr.size();
		return result;
	}

	int IsId(IdTable& idTable, std::string id)
	{
		for (int i = 0; i < idTable.size; i++)
		{
			if (TI_EL(i).id == id)
				return i;
		}
		return TI_NULLIDX;
	}


	int IsDublId(IdTable& idTable, string id)
	{
		for (int i = 0; i < idTable.size - 1; i++)
		{
			if (TI_EL(i).id == id)
				return i;
		}

		return TI_NULLIDX;
	}


	int CheckId(IdTable& idTable, string value)
	{
		for (int i = 0; i < idTable.size; i++) 
		{
			if (TI_EL(i).iddatatype == IT::STRING)
			{
				if (TI_EL(i).value.vString.str == value)
					return i;
			}
		}

		return TI_NULLIDX;
	}

	int CheckId(IdTable& idTable, int value)
	{
		for (int i = 0; i < idTable.size; i++)
		{
			if (TI_EL(i).iddatatype == IT::USINT)
			{
				if (TI_EL(i).value.vUsint == value)
					return i;
			}
		}
		return TI_NULLIDX;
	}

	int CheckLiteralId(IdTable& idTable, int value)
	{
		for (int i = 0; i < idTable.size; i++)
		{
			if (TI_EL(i).iddatatype == IT::USINT && TI_EL(i).idtype == IT::L)
			{
				if (TI_EL(i).value.vUsint == value)
					return i;
			}
		}
		return TI_NULLIDX;
	}

	int CheckId(IdTable& idTable, char value)
	{
		for (int i = 0; i < idTable.size; i++)
		{
			if (TI_EL(i).iddatatype == IT::SYMBOL)
			{
				if (TI_EL(i).value.vSymbol == value)
					return i;
			}
		}
		return TI_NULLIDX;
	}

	void WriteTable(Log::LOG log, IdTable& idtable)
	{
		static bool flag = false;
		int i, numberOP;
		i = numberOP = 0;

		if(!flag)
			*(log.stream) << setw(30) << setfill('-') <<  "-"  << " Лексический анализ " << setw(50) << setfill('-') << "-" << endl;
		else
			*(log.stream) << setw(40) << setfill('-') << "-" << " ПОЛИЗ " << setw(50) << setfill('-') << "-" << endl;

		*(log.stream) << setfill('-') << setw(100) << '-' << endl;
		*(log.stream) << "№ id" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << "Значение" << endl;
		*(log.stream) << setw(100) << '-' << endl;

		for (Entry ident:idtable.table)
		{
			*(log.stream) << setfill(' ') << setw(4) << std::right << i << " | ";
			*(log.stream) << setw(13) << left << ident.id << " | ";

			switch (ident.iddatatype)
			{
				case  IT::USINT:
				{
					*(log.stream) << setw(10) << left;
					*(log.stream) << "usint" << " | ";
					break;
				}
				case  IT::STRING:
				{
					*(log.stream) << setw(10) << left;
					*(log.stream) << "string" << " | ";

					break;
				}
				case  IT::SYMBOL:
				{
					*(log.stream) << setw(10) << left;
					*(log.stream) << "symbol" << " | ";
					break;
				}
				default:
				{
					*(log.stream) << setw(10) << left << "unknown" << " | ";

					break;
				}
			}

			switch (ident.idtype)
			{
				case IT::V:
				{
					*(log.stream) << setw(18) << left << "переменная" << " | ";
					break;
				}
				case IT::F:
				{
					*(log.stream) << setw(18) << left << "функция" << " | ";
					break;
				}
				case IT::P:
				{
					*(log.stream) << setw(18) << left << "параметр" << " | ";
					break;
				}
				case IT::L:
				{
					*(log.stream) << setw(18) << left << "литерал" << " | ";
					break;
				}
				case IT::OP:
				{
					*(log.stream) << setw(18) << left << "оператор" << " | ";
					numberOP++;
					break;
				}
				default:
				{
					*(log.stream) << setw(18) << left << "unknown" << " | ";
					break;
				}
			}

			*(log.stream) << setw(11) << left << ident.idxfirstLE << " | ";



			if (ident.iddatatype == IT::USINT && (ident.idtype == IT::V || ident.idtype == IT::L))
			{
				*(log.stream) << setw(18) << left << ident.value.vUsint;
			}
			else if (ident.iddatatype == IT::STRING && (ident.idtype == IT::V || ident.idtype == IT::L))
			{
				if(ident.value.vString.str.length() < 10)
					*(log.stream) << "`" << ident.value.vString.str << "`(" << ident.value.vString.len << ")";
				else
					*(log.stream) << "`" << ident.value.vString.str.substr(0,10) << "...`(" << ident.value.vString.len << ")";
			}
			else if (ident.iddatatype == IT::SYMBOL && (ident.idtype == IT::L || ident.idtype == IT::V))
			{
				*(log.stream) << "'" << ident.value.vSymbol << "'";
			}
			else
				*(log.stream) << "null";

			*(log.stream) << endl;
			i++;
		}

		*(log.stream) << setfill('-') << setw(100) << '-' << endl;
		*(log.stream) << "Количество идентификаторов: " << i - numberOP << endl;
		*(log.stream) << setw(100) << '-' << endl;

		flag = true;
	}

}