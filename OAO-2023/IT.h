#pragma once
#include "stdafx.h"
#define ID_MAXSIZE 10 //max кол-во символов в id
#define TI_MAXSIZE 4096 //max кол-во строк в таблице id
#define TI_USINT_DEFAULT 0x00000000 //значение по умолчанию дл€ integer
#define TI_STR_DEFAULT 0x00		//значение по умолч дл€ string
#define TI_NULLIDX -1 //нет эл-та таблицы id
#define TI_STR_MAXSIZE 255
#define TI_USINT_MAXVALUE 255
#define TI_EL(value) idTable.table[value]

namespace IT // таблица id
{	//типы данных id
	enum IDDATATYPE { OFF = 0, USINT = 1, STRING = 2, SYMBOL = 3};
	//типы id: переменна€, функци€, параметр, литерал, оператор
	enum IDTYPE {D = 0, V = 1, F = 2, P = 3, L = 4, OP = 5};	  


	struct Entry //строка таблицы id
	{
		int idxfirstLE;		//индекс первой строки в “Ћ
		std::string id;		//id (автоматически усекаетс€ до ID_MAXSIZE)
		IDDATATYPE iddatatype; //тип данных
		IDTYPE idtype;			//тип идентификатора
		struct
		{
			char vSymbol;
			int vUsint; //значение usint
			struct
			{
				int len;	//кол-во символов в string
				std::string str; //символы string
			}vString; //значение string
		}value;
	};
	
	struct IdTable //экземпл€р таблицы id
	{
		int maxsize; //Ємкость таблицы id < TI_MAXSIZE
		int size;	//текущий размер таблицы id
		std::vector<Entry> table; //массив строк таблицы id
	};
	IdTable& Create(	//создать таблицу id
		 //Ємкость таблицы id < TI_MAXSIZE
	);

	void Add(	//добавить строку в таблицу id 
		IdTable& idtable, //экземпл€р таблицы id
		Entry entry		//строка таблицы идентификаторов
	);
	Entry GetEntry(	//получить строку таблицы id
		IdTable& idtable, //экземпл€р таблицы id
		int n		//номер получаемой строки
	);

	Entry CreateId(
		int line, 
		std::string id, 
		IDDATATYPE id_data_type,
		IDTYPE id_type
	);

	Entry CreateId(
		int line, 
		std::string id, 
		IDDATATYPE id_data_type, 
		IDTYPE id_type, int vUsint
	);

	Entry CreateId(
		int line,
		std::string id,
		IDDATATYPE id_data_type, 
		IDTYPE id_type,  char vSymbol);

	Entry CreateId(
		int line,
		std::string id, 
		IDDATATYPE id_data_type, 
		IDTYPE id_type, 
		std::string vstr
	);

	int IsId(		//возврат: номер строки(если есть), TI_NULLIDX(если нет)
		IdTable& idtable,	//экземпл€р таблицы идентификаторов
		std::string id //идентификатор
	);
	// ѕоиск значени€ в таблице идентификаторов
	int CheckId(
		IdTable& idtable,
		std::string value
	);

	int CheckId(
		IdTable& idtable,
		 char value
	);

	int CheckLiteralId(
		IdTable& idtable,
		int value
	);

	// ѕроверка на дублирование идентификатора
	int IsDublId(
		IdTable& idtable,
		std::string id
	);		
	int CheckId(
		IdTable& idtable,
		int value
	);
	void WriteTable(
		Log::LOG log,
		IdTable& idtable
	);

}