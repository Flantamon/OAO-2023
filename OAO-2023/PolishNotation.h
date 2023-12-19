#pragma once
#include "stdafx.h"
namespace PN
{
	void AddToResult(IT::IdTable& idTable, LT::Entry* result, LT::Entry elementLT, int& pos, int& flag, int lenout, int lextablePos);
	void AddToResult(std::stack<LT::Entry>& stack, IT::IdTable& idTable, LT::Entry* result, int& pos, int& flag, int lenout, int lextablePos);
	bool PolishNotation(int	lexTablePos, LT::LexTable & lextable, IT::IdTable & idtable);
	void FindExpressions(LT::LexTable& lextable, IT::IdTable& idtable);
}
	