#include "stdafx.h"
#include "MFST.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	Log::LOG log = Log::INITLOG;
	Parm::PARM parm;
	try 
	{	
		parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		//Считывание исходного кода
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);

		// Лексический анализ
		log = Log::getlog(parm.an);
		Lex::LEX lex = Lex::LexAnalysis(in, log);

		// Синтаксический анализ
		MFST::Mfst mfst(lex.lexTable, GRB::getGreibach());
		mfst.start(log, parm);
		mfst.printRules(log);

		// Построение польской записи
		PN::FindExpressions(lex.lexTable, lex.idTable);
		IT::WriteTable(log, lex.idTable);
		LT::WriteTable(log, lex.lexTable);

		// Семантический анализ
		SA::startSemAnalysis(lex.lexTable, lex.idTable);

		// Генерация кода в ассемблер
		log = Log::getlog(parm.out);
		CodeGen::Generation(lex, log);

		Log::Close(log);

	}
	catch (Error::ERROR e)
	{
		if (log.stream)
		{
			Log::Close(log);
			log = Log::appendLog(parm.log);
		}
		ERROR_TRACE
		Log::WriteError(log, e);
		Log::Close(log);
		system("pause");
	}
	catch (...)
	{
		Error::ERROR e = Error::geterror(1);

		if (log.stream)
		{
			Log::Close(log);
			log = Log::appendLog(parm.log);
		}
		ERROR_TRACE
		Log::WriteError(log, e);
		Log::Close(log);
		system("pause");
	}
	return 0;
}
