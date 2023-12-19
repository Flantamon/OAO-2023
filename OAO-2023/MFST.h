#pragma once
#include "stdafx.h"
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

//отладка
#define MFST_TRACE_START *(log.stream) << std::setfill(' ') << std::setw(4)<<std::left<<"Шаг"<<":"  \
								  <<std::setw(20)<<std::left<<" Правило" \
								  <<std::setw(30)<<std::left<<" Входная лента" \
								  <<std::setw(20)<<std::left<<" Стек" \
								  <<std::endl;

static int FST_TRACE_n = -1;
static char rbuf[205], sbuf[205], lbuf[1024]; // печать

#define ISNS(n) GRB::Rule::Chain::isN(n)
#define MFST_TRACE1	*(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf, nrulechain)	\
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf) \
									<<std::endl;

#define MFST_TRACE2	*(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<< FST_TRACE_n<<": " \
									<<std::setw(20)<<std::left<<" " \
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf) \
									<<std::endl;

#define MFST_TRACE3	*(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<<++FST_TRACE_n<<": " \
									<<std::setw(20)<<std::left<<" " \
									<<std::setw(30)<<std::left<<getCLenta(lbuf, lenta_position)	\
									<<std::setw(20)<<std::left<<getCSt(sbuf) \
									<<std::endl;

#define MFST_TRACE4(c) *(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<<++FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE5(c) *(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<< FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<std::endl;

#define MFST_TRACE6(c, k)  *(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<< FST_TRACE_n<<": "<<std::setw(20)<<std::left<<c<<k<<std::endl;

#define MFST_TRACE7 *(log.stream) << std::setfill(' ')<<std::setw(4)<<std::left<<state.lenta_position<<": " \
								<<std::setw(20)<<std::left<<rule.getCRule(rbuf,state.nrulechain) \
								<<std::endl;


typedef std::vector<short> MFSTSTSTACK;		// стек автомата
namespace MFST
{
	struct MfstState			// состояние автомата (для сохранения)
	{
		short lenta_position;	// позиция на ленте
		short nrule;			// номер текущего правила
		short nrulechain;		// номер текущей цепочки, текущего правила
		MFSTSTSTACK st;			// стек автомата
		MfstState();
		MfstState(
			short pposition,	// позиция на ленте
			MFSTSTSTACK pst,	// стек автомата
			short pnrulechain	// номер текущей цепочки, текущего правила
		);
		MfstState(
			short pposition,	// позиция на ленте
			MFSTSTSTACK pst,	// стек автомата
			short pnrule,
			short pnrulechain	// номер текущей цепочки, текущего правила
		);
	};

	struct Mfst		// магазинный автомат
	{
		enum RC_STEP {	// код возврата функции step
			NS_OK,		// найдено правило и цепочка, цепочка записана в стек
			NS_NORULE,	// не найдено правило грамматики (ошибка в грамматике)
			NS_NORULECHAIN,	// не найдена подходящая цепочка правила (ошибка в исходном коде)
			NS_ERROR,	// неизвестный нетерминальный символ грамматики
			TS_OK,		// тек. символ ленты == вершине стека, продвинулась лента, pop стека
			TS_NOK,		// тек. символ ленты != вершине стека, восстановлено состояние
			LENTA_END,	// текущая позиция ленты >= lenta_size
			SURPRISE	// неожиданный код возврата (ошибка в step)
		};
		struct MfstDiagnosis		// диагностика
		{
			short lenta_position;	// позиция на ленте
			RC_STEP rc_step;		// код завершения шага
			short nrule;			// номер правила
			short nrule_chain;		// номер цепочки правила
			MfstDiagnosis();
			MfstDiagnosis(
				short plenta_position, // позиция на ленте
				RC_STEP prc_step,	   // код завершения шага
				short pnrule,		   // номер правила
				short pnrule_chain	   // номер цепочки правила
			);
		}diagnosis[MFST_DIAGN_NUMBER];	// последние самые глубокие сообщения

		GRBALPHABET* lenta;		// перекодированная (TS/NS) лента (из LEX)
		short lenta_position;	// текущая позиция на ленте
		short nrule;			// номер текущего правила
		short nrulechain;		// номер текущей цепочки, текущего правила
		short lenta_size;		// размер ленты
		GRB::Greibach greibach; // грамматика Грейбах
		LT::LexTable lex;		// результат работы лексич. анализатора
		MFSTSTSTACK st;			// стек автомата
		std::vector<MfstState> storestate; // стек для сохранения состояний
		Mfst();
		Mfst(
			LT::LexTable plex,	// результат работы лексич. анализатора
			GRB::Greibach pgreibach // грамматика Грйбах
		);
		char* getCSt(char* buf);	// получить содержимое стека
		char* getCLenta(char* buf, short pos, short n = 25); // лента: n символов с pos
		char* getDiagnosis(short n, char* buf);	// получить n-ую строку диагностики или 0x00
		bool saveState(Log::LOG log);			// сохранить состояние автомата
		bool restState(Log::LOG log);			// восстановить состояние автомата
		bool push_chain(			// поместить цепочку правила в стеку
			GRB::Rule::Chain chain	// цепочка правила
		);
		RC_STEP step(Log::LOG log);				// выполинть шаг автомата
		bool start(Log::LOG &log, Parm::PARM& parm);				// запустить автомат
		bool saveDiagnosis(
			RC_STEP pprc_step		// код завершения шага
		);
		void printRules(Log::LOG& log);			// вывести последовательность правил

		struct Deducation			// вывод
		{
			short size;				// кол-во шагов в выводе
			short* nrules;			// номера правил грамматики
			short* nrulechains;		// номера цепочек правил грамматики (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool saveDeducation();		// сохранить дерево вывода
	};
};