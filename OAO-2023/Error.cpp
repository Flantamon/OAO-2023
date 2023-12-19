#include "stdafx.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		//	 0 - 99  -  системные ошибки 
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "[System] Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30),
		ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60),
		ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		//			  100 - 109 -  ошибки параметров
		ERROR_ENTRY(100, "[PARM] Ошибка параметра. Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "[PARM] Ошибка параметра. Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105),
		ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		//		      110 - 119 -  ошибки открытия и чтения файлов
		ERROR_ENTRY(110, "[IN] Ошибка при открытии файла с исходным кодом (/in)"),
		ERROR_ENTRY(111, "[IN] Недопустимый символ в исходном файле (/in)"),
		ERROR_ENTRY(112, "[IN] Ошибка при создании файла протокола"),
		ERROR_ENTRY(113, "[IN] Ошибка в записи строкового литерала"),
		ERROR_ENTRY(114, "[IN] Превышена длина файла с исходным кодом"),
		ERROR_ENTRY(115, "[IN] Превышена максимальная длина слова в файле с исходным кодом"),
		ERROR_ENTRY_NODEF(116),
		ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		//			  120 - 199  -  ошибки лексического анализа
		ERROR_ENTRY(120, "[LexA] Цепочка символов не разобрана"),
		ERROR_ENTRY(121, "[LexA] Переполнение таблицы лексем"),
		ERROR_ENTRY(122, "[LexA] Переполнение таблицы идентификаторов"),
		ERROR_ENTRY_NODEF(123),
		ERROR_ENTRY_NODEF(124),
		ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126),
		ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		//			  200 - 299  -  ошибки синтаксического анализа
		ERROR_ENTRY(200, "[SynA] Неверная структура программы"),
		ERROR_ENTRY(201, "[SynA] Ошибочный сепаратор"),
		ERROR_ENTRY(202, "[SynA] Ошибка в выражении"),
		ERROR_ENTRY(203, "[SynA] Ошибка в параметрах функции"),
		ERROR_ENTRY(204, "[SynA] Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(205, "[SynA] Ошибка в условном операторе"),
		ERROR_ENTRY(206, "[SynA] Синтаксический анализ завершён с ошибками(подробности в log)"),
		ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208),
		ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210),ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230),
		ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270),
		ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		//  		  300 - 399  -  ошибки семантического анализа
		ERROR_ENTRY(300, "[SemA] Переопределение идентификатора"),
		ERROR_ENTRY(301, "[SemA] Несоответствие возвращаемого значения функции с её типом"),
		ERROR_ENTRY(302, "[SemA] Переданы неверные типы параметров"),
		ERROR_ENTRY(303, "[SemA] Деление на 0 в выражении"),
		ERROR_ENTRY(304, "[SemA] Вызов функции не в точке входа"),
		ERROR_ENTRY(305, "[SemA] Превышено максимальное количество функций в программе"),
		ERROR_ENTRY(306, "[SemA] Необъявленный идентификатор"),
		ERROR_ENTRY(307, "[SemA] Несоответствие типов в операторе присваивания"),
		ERROR_ENTRY(308, "[SemA] Более одной главной функции HEAD"),
		ERROR_ENTRY(309, "[SemA] Имя пользовательской функции не должно совпадать с именем функций CESlib"),
		ERROR_ENTRY(310, "[SemA] Отсутствие главной функции HEAD"),
		ERROR_ENTRY(311, "[SemA] Объявление идентификатора без его использования"),
		ERROR_ENTRY(312, "[SemA] Применение разных типов операторов в одном выражении"), 
		ERROR_ENTRY(313, "[SemA] Превышено максимальное количество параметров в функции"),
		ERROR_ENTRY(314, "[SemA] Значение целочисленного литерала не входит в диапазон допустимых значений"),
		ERROR_ENTRY(315, "[SemA] Превышена максимальная длина строкового литерала"),
		ERROR_ENTRY(316, "[SemA] Несоответствие операндов с операцией"),
		ERROR_ENTRY(317, "[SemA] Неверное выражение в условной конструкции"),
		ERROR_ENTRY(318, "[SemA] Передано неверное количество параметров"),
		ERROR_ENTRY_NODEF(319),
		ERROR_ENTRY_NODEF(320),
		ERROR_ENTRY_NODEF(321),
		ERROR_ENTRY_NODEF(322),
		ERROR_ENTRY_NODEF(323),
		ERROR_ENTRY_NODEF(324),
		ERROR_ENTRY_NODEF(325), ERROR_ENTRY_NODEF(326), ERROR_ENTRY_NODEF(327), ERROR_ENTRY_NODEF(328),
		ERROR_ENTRY_NODEF(329),
		ERROR_ENTRY_NODEF10(330),
		ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360),
		ERROR_ENTRY_NODEF10(370), ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390),

		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY_NODEF100(600),

		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id) {
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		return errors[id];
	}
	ERROR geterrorin(int id, int line = -1, int col = -1) {
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		errors[id].inext.line = line;
		errors[id].inext.col = col;
		return errors[id];
	}
}