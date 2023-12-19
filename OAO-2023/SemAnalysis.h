#pragma once
#include "stdafx.h"
#define FUNC_MAX_SIZE 5	// Максимальное количество функций и параметров в них
namespace SA
{
	enum ERROR { NO_ERRORS, PARMS_COUNT, PARMS_TYPE };

	// Локальная функция
	struct Function
	{
		int size = 0;
		std::string name;
		IT::IDDATATYPE* arrayOfDataType;	// массив типов параметров
		Function();
	};

	// Локальные функции в программе
	struct LocalFunctions
	{
		Function* arrayOfFunction;
		int size = 0;
		LocalFunctions();
	};

	// проверка на соотвествие типов параметров с типом возвращаемого значения функции
	ERROR CheckParms(LocalFunctions* localFunctions, Function* arr_of_it_entry, std::string name);

	// Начать семантический анализ
	void startSemAnalysis(LT::LexTable& lexTable, IT::IdTable& idTable);

	// Функция для проверки частного случая при присваивании
	//(когда переменная типа Bool, а в присваивании выражение 1>2)
	void isCorrectType(LT::LexTable &lexTable, IT::IdTable &idTable);

	// Функция, которая проверяет, используется ли в программе объявленная переменная
	void isIdWasUsed(LT::LexTable& lexTable, IT::IdTable& idTable);

	// Проверка на то, что функции стандартной библиотеки вызываются без параметров
	void isStaticFunc(LT::LexTable& lexTable, IT::IdTable& idTable);
}