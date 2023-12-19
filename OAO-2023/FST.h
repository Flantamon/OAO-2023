#pragma once
#include "stdafx.h"
#define FST_WORDS_SIZE 21
#define FST_SINGLE_LEX_SIZE 8
#define FST_USINT_DEC_INDEX 0
#define FST_USINT_HEX_INDEX 1

namespace FST
{
	// тип целочисленного литерала
	enum USINT_LIT_TYPE { NO_USINT_LIT, DEC_LIT, HEX_LIT};	
	enum LEX_TYPE { UNIDENTIFIED = -1, ID, NEED_PRIORITY, DEF_LEX, OPERATION, LOG_OPERATION, STATIC_LIB, LITERAL };
	
	struct RELATION //ребро:символ -> вершина графа переходов КА
	{
		char symbol; //символ перехода
		short nnode; //номер смежной вершины
		RELATION(
			char c = 0x00,  //символ перехода
			short ns = NULL //новое состояние
		);
	};
	struct NODE
	{
		short n_relation;		//количество инцидентых ребер
		RELATION* relations;	//инцидентные рёбра
		NODE();
		NODE(
			short n,			//количество инцидентых ребер
			RELATION rel, ...	//список рёбер
		);
	};
	struct STATE_MACHINE
	{
		LEX_TYPE lextype;
		char lexema;
		USINT_LIT_TYPE ult = NO_USINT_LIT;
	};
	struct FST //недетерминированный конечный автомат
	{
		char  lexema;		//цепочка (строка, завершается 0x00)
		LEX_TYPE lexType;	// тип лексемы
		std::string str;	// цепочка (строка, завершается 0х00)
		short position;		//текущая позиция в цепочке
		short nstates;		//количество состояний автомата
		NODE* nodes;		//граф переходов: [0] - начальное состояние, [nstate - 1] - конечное
		short* rstates;		//возможные состояния автомата на данной позиции
		FST();
		FST(
			char c,					// лексема
			LEX_TYPE l_t,			// тип лексемы
			std::string s,		//цепочка (строка, завершается 0x00)
			short ns,		//количество состояний автомата
			NODE n, ...		//список состояний (граф переходов)
		);
	};
	bool execute(	//выполнить распознавание цепочки
		FST& fst	
	);
	FST* fstSingleLex();	
	FST* fstWords();		
};
