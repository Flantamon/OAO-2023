#pragma once
#include "stdafx.h"
#define LT_NOPRIORITY 0xffffffff		// нет приоритета
#define LEXEMA_FIXSIZE 1 //‘» —»–ќ¬јЌЌџ… –ј«ћ≈– Ћ≈ —≈ћ
#define LT_MAXSIZE 4096 //max кол-во строк в таблице лексем
#define LT_TI_NULLIDX -1 // нет элемента таблицы идентификаторов
#define LEX_T 't'
#define LEX_USINT 'u'
#define LEX_STRING 's'
#define LEX_SYMBOL 'c'	
#define LEX_LIB_FUNC 'x'
#define LEX_ID 'i'
#define LEX_LITERAL 'l'
#define LEX_DEF 'f'
#define LEX_HEAD 'm'
#define LEX_NEW 'n'
#define LEX_OPERATOR 'v'
#define LEX_LOG_OPERATOR 'z'
#define LEX_RET 'r'
#define LEX_OUT 'o'
#define LEX_DOT '.'
#define LEX_COMMA ','
#define LEX_OPEN '{'
#define LEX_CLOSE '}'
#define LEX_LEFTHESIS '('
#define LEX_RIGHTHESIS ')'
#define LEX_LEFTSHIFT '0'
#define LEX_RIGHTSHIFT '1'
#define LEX_LESS '<'
#define LEX_GREATER '>'
#define LEX_NEQU '!'
#define LEX_EQU '='
#define LEX_ASSIGN ':'
#define LEX_CYCLE 'q'
#define LEX_ENDC '['
#define LITERAL_NAME 'L'
#define LEFT_SHIFT_OPERATOR "<~"
#define RIGHT_SHIFT_OPERATOR "~>"
#define LEX_SUBST '@'
#define DATE "DATE"
#define RANDOM "RANDOM"
#define LT_EL(value)  lexTable.table[value]

namespace LT
{
	struct Entry //строка таблицы лексем
	{
		char lexema;		// лексема
		int sn;				//номер строки в исходном тексте
		int pn;				// номер позиции в исходном коде
		int idxTI;			// индекс в таблице идентификаторов или LT_TI_NULLDIX
		int priority;		// приоритет операции
		std::string operation;		// ’ранит значение операции (либо 0x00, если не €вл€етс€ операцией)
	};
	struct LexTable
	{
		int maxsize;	//Ємкость таблицы лексем < LT_MAXSIZE
		int size;		//текущий размер таблицы лексем < maxsize
		std::vector<Entry> table;	//массив строк таблицы лексем
	};
	LexTable& Create( //создать таблицу лексем
			 //Ємкость таблицы лексем < LT_MAXSIZE
	);
	void Add(	//добавить строку в таблицу лексем
		LexTable& lextable, //экземпл€р таблицы лексем
		Entry entry				//строка таблицы лексем
	);
	Entry GetEntry( //получить строку таблицы лексем
		LexTable& lextable,		//экземпл€р таблицы лексем
		int n					//номер получаемой строки
	);

	Entry CreateLex(
		char lex,
		int line,
		int pos,
		int id_in_TI, 
		int prior
	);

	void Delete(LexTable& lextable); //удалить таблицу лексем(освободить пам€ть)

	void WriteTable(Log::LOG log, LexTable& lexTable);
};

