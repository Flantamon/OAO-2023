#pragma once
#include "stdafx.h"
typedef short GRBALPHABET; // символы алфавита грамматики, терминалы > 0, нетерминалы < 0
#define GRB_ERROR_SERIES 200
namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
	struct Rule	 // правило в грамматике Грейбах
	{
		GRBALPHABET nn;		// нетерминал(левый символ правила) < 0
		int iderror;		// индетификатор диагностического сообщения
		short size;			// кол-во цепочек - правых частей правила
		struct Chain		// цепочка(правая часть правила)
		{
			short size;			// длина цепочки
			GRBALPHABET* nt;	// цепочка терминалов и нетерминалов 
			Chain() { size = 0; nt = 0; };
			Chain(
				short psize,		// кол-во символов в цепочке
				GRBALPHABET s, ...  // символы(терминал и нетерминал)
			);
			char* getCChain(char* b);	// получить правую сторону правила
			static GRBALPHABET T(char t) { return  GRBALPHABET(t); }; // терминал
			static GRBALPHABET N(char n) { return -GRBALPHABET(n); }; // нетерминал
			static bool isT(GRBALPHABET s) { return s > 0; };	// это терминал?
			static bool isN(GRBALPHABET s) { return !isT(s); };	// это нетерминал?
			static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };
		}* chains;
		Rule() { nn = 0x00; size = 0; iderror = 0; chains = nullptr; }
		Rule(
			GRBALPHABET pnn,	// нетерминал
			int iderror,		// идент. диагностического сообщения (Error)
			short psize,		// кол-во цепочек - правых частей правила
			Chain c, ...		// множество цепочек- правых частей правила
		);
		char* getCRule(		// получить правило в виде N->цепочка (для распечатки)
			char* b,		// буфер
			short nchain	// номер цепочки (правой части в правиле
		);
		short getNextChain(  // получить следующую за j подходящую цепочку, вернуть её номер или -1
			GRBALPHABET t,		   // первый символ цепочки
			Rule::Chain& pchain,   // возвращаемая цепочка
			short j				   // номер цепочки
		);
	};

	struct Greibach	 // грамматика Грейбах
	{
		short size;				// кол-во правил
		GRBALPHABET startN;		// стартовый символ
		GRBALPHABET stbottomT;  // дно стека
		Rule* rules;			// множество правил 
		Greibach() {  size = 0; startN = 0; stbottomT = 0; rules = 0; };
		Greibach(
			GRBALPHABET pstartN,	// стартовый символ
			GRBALPHABET pstBottomT, // дно стека
			short psize,			// кол-во правил 
			Rule r, ...				// правила
		);
		short getRule(		// получить правило, возвращается номер правила или -1
			GRBALPHABET pnn,	// левый символ правила
			Rule& prule			// возвращаемое правило грамматики
		);
		Rule getRule(short n);  // получить правило по номеру
	};
	Greibach getGreibach();   //получить грамматику
};