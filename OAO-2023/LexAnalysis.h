#pragma once
#include "stdafx.h"

namespace Lex
{
	struct LEX
	{
		LT::LexTable lexTable;
		IT::IdTable idTable;
	};
	LEX LexAnalysis(In::IN, Log::LOG log);
}
