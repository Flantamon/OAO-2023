#include "stdafx.h"

namespace FST {

	RELATION::RELATION(char c, short nn)
	{
		symbol = c;
		nnode = nn;
	};

	NODE::NODE()
	{
		n_relation = 0;
		RELATION* relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...) //� �����������
	{
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++) relations[i] = p[i];
	};
	FST::FST() {}
	FST::FST(char c, LEX_TYPE l_t, std::string s, short ns, NODE n, ...)
	{
		lexema = c;
		str = s;
		nstates = ns;
		lexType = l_t;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	};
	bool step(FST& fst, short*& rstates) // 1 ��� ��������
	{
		bool rc = false;
		std::swap(rstates, fst.rstates); //����� ��������
		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++) {
					if (fst.nodes[i].relations[j].symbol == fst.str[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};
	bool execute(FST& fst) // ��������� ������������� �������
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		int lstring = fst.str.size();
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;		//���������� �������
			rc = step(fst, rstates); //1 ��� ��������
		};
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}
	FST* fstSingleLex()
	{
		FST fstLogicOperators(LEX_LOG_OPERATOR, LOG_OPERATION, "", 2,
			NODE(4, RELATION('<', 1), RELATION('>', 1),
					RELATION('!', 1), RELATION('=', 1)),
			NODE()
		);

		FST fstLeftHesis(LEX_LEFTHESIS, NEED_PRIORITY, "", 2,
			NODE(1, RELATION('(', 1)),
			NODE()
		);
		FST fstRightHesis(LEX_RIGHTHESIS, NEED_PRIORITY, "", 2,
			NODE(1, RELATION(')', 1)),
			NODE()
		);
		FST fstAssign(LEX_ASSIGN, DEF_LEX, "", 2,
			NODE(1, RELATION(':', 1)),
			NODE()
		);

		FST fstSemicolon(LEX_DOT, DEF_LEX, "", 2,
			NODE(1, RELATION('.', 1)),
			NODE()
		);

		FST fstComma(LEX_COMMA, NEED_PRIORITY, "", 2,
			NODE(1, RELATION(',', 1)),
			NODE()
		);

		FST fstUsintDecLit(LEX_USINT, LITERAL, "", 2,
			NODE(10, RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
					 RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
				     RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
					 RELATION('9', 1)),
			NODE()
		);

		FST fstId(LEX_ID, ID, "", 2,
			NODE(26, RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
					 RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
					 RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
					 RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
					 RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
					 RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
					 RELATION('y', 1), RELATION('z', 1)),
			NODE()
		);

		FST* result = new FST[FST_SINGLE_LEX_SIZE];
		result[0] = fstLogicOperators;
		result[1] = fstUsintDecLit;
		result[2] = fstSemicolon;
		result[3] = fstComma;
		result[4] = fstLeftHesis;
		result[5] = fstRightHesis;
		result[6] = fstAssign;
		result[7] = fstId;
		return result;
	}

	FST* fstWords()
	{
		FST fstUsint(LEX_USINT, DEF_LEX, "", 6,
			NODE(1, RELATION('u', 1)),
			NODE(1, RELATION('s', 2)),
			NODE(1, RELATION('i', 3)),
			NODE(1, RELATION('n', 4)),
			NODE(1, RELATION('t', 5)),
			NODE()
		);
		FST fstString(LEX_STRING, DEF_LEX, "", 7,
			NODE(1, RELATION('s', 1)),
			NODE(1, RELATION('t', 2)),
			NODE(1, RELATION('r', 3)),
			NODE(1, RELATION('i', 4)),
			NODE(1, RELATION('n', 5)),
			NODE(1, RELATION('g', 6)),
			NODE()
		);
		FST fstSymbol(LEX_SYMBOL, DEF_LEX, "", 7,
			NODE(1, RELATION('s', 1)),
			NODE(1, RELATION('y', 2)),
			NODE(1, RELATION('m', 3)),
			NODE(1, RELATION('b', 4)),
			NODE(1, RELATION('o', 5)),
			NODE(1, RELATION('l', 6)),
			NODE()
		);
		FST fstFunc(LEX_DEF, DEF_LEX, "", 4,
			NODE(1, RELATION('D', 1)),
			NODE(1, RELATION('E', 2)),
			NODE(1, RELATION('F', 3)),
			NODE()
		);
		FST fstVariable(LEX_NEW, DEF_LEX, "", 4,
			NODE(1, RELATION('n', 1)),
			NODE(1, RELATION('e', 2)),
			NODE(1, RELATION('w', 3)),
			NODE()
		);
		FST fstHead(LEX_HEAD, DEF_LEX, "", 5,
			NODE(1, RELATION('H', 1)),
			NODE(1, RELATION('E', 2)),
			NODE(1, RELATION('A', 3)),
			NODE(1, RELATION('D', 4)),
			NODE()
		);
		FST fstLeftBrace(LEX_OPEN, DEF_LEX, "", 5,
			NODE(1, RELATION('O', 1)),
			NODE(1, RELATION('P', 2)),
			NODE(1, RELATION('E', 3)),
			NODE(1, RELATION('N', 4)),
			NODE()
		);
		FST fstRightBrace(LEX_CLOSE, DEF_LEX, "", 6,
			NODE(1, RELATION('C', 1)),
			NODE(1, RELATION('L', 2)),
			NODE(1, RELATION('O', 3)),
			NODE(1, RELATION('S', 4)),
			NODE(1, RELATION('E', 5)),
			NODE()
		);
		FST fstWrite(LEX_OUT, DEF_LEX, "", 6,
			NODE(1, RELATION('w', 1)),
			NODE(1, RELATION('r', 2)),
			NODE(1, RELATION('i', 3)),
			NODE(1, RELATION('t', 4)),
			NODE(1, RELATION('e', 5)),
			NODE()
		);
		FST fstWriteLine(LEX_OUT, DEF_LEX, "", 10,
			NODE(1, RELATION('w', 1)),
			NODE(1, RELATION('r', 2)),
			NODE(1, RELATION('i', 3)),
			NODE(1, RELATION('t', 4)),
			NODE(1, RELATION('e', 5)),
			NODE(1, RELATION('l', 6)),
			NODE(1, RELATION('i', 7)),
			NODE(1, RELATION('n', 8)),
			NODE(1, RELATION('e', 9)),
			NODE()
		);

		FST fstRet(LEX_RET, DEF_LEX, "", 4,
			NODE(1, RELATION('R', 1)),
			NODE(1, RELATION('E', 2)),
			NODE(1, RELATION('T', 3)),
			NODE()
		);

		FST fstSymbolLit(LEX_SYMBOL, LITERAL, "", 4,
			NODE(1, RELATION('\'', 1)),
			NODE(138,
				RELATION('a', 2), RELATION('b', 2), RELATION('c', 2), RELATION('d', 2), RELATION('e', 2), RELATION('f', 2),
				RELATION('g', 2), RELATION('h', 2), RELATION('i', 2), RELATION('j', 2), RELATION('k', 2), RELATION('l', 2),
				RELATION('m', 2), RELATION('n', 2), RELATION('o', 2), RELATION('p', 2), RELATION('q', 2), RELATION('r', 2),
				RELATION('s', 2), RELATION('t', 2), RELATION('u', 2), RELATION('v', 2), RELATION('w', 2), RELATION('x', 2),
				RELATION('y', 2), RELATION('z', 2), RELATION('1', 2), RELATION('2', 2), RELATION('3', 2), RELATION('4', 2),
				RELATION('5', 2), RELATION('6', 2), RELATION('7', 2), RELATION('8', 2), RELATION('9', 2), RELATION('0', 2),
				RELATION('A', 2), RELATION('B', 2), RELATION('C', 2), RELATION('D', 2), RELATION('E', 2), RELATION('F', 2),
				RELATION('G', 2), RELATION('H', 2), RELATION('I', 2), RELATION('J', 2), RELATION('K', 2), RELATION('L', 2),
				RELATION('M', 2), RELATION('N', 2), RELATION('O', 2), RELATION('P', 2), RELATION('Q', 2), RELATION('R', 2),
				RELATION('S', 2), RELATION('T', 2), RELATION('U', 2), RELATION('V', 2), RELATION('W', 2), RELATION('X', 2),
				RELATION('Y', 2), RELATION('Z', 2),

				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION(' ', 2), RELATION('.', 2), RELATION(',', 2),
				RELATION('?', 2), RELATION('!', 2), RELATION(';', 2), RELATION(':', 2), RELATION('-', 2), RELATION(')', 2),
				RELATION('(', 2)),

			NODE(1, RELATION('\'', 3)),
			NODE()
		);

		FST fstUsintDecLit(LEX_USINT, LITERAL, "", 3,
			NODE(30,
				RELATION('0', 0), RELATION('1', 0), RELATION('2', 0),
				RELATION('3', 0), RELATION('4', 0), RELATION('5', 0),
				RELATION('6', 0), RELATION('7', 0), RELATION('8', 0), 
				RELATION('9', 0), 
				RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
				RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
				RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
				RELATION('9', 1),
				RELATION('0', 2), RELATION('1', 2), RELATION('2', 2),
				RELATION('3', 2), RELATION('4', 2), RELATION('5', 2),
				RELATION('6', 2), RELATION('7', 2), RELATION('8', 2),
				RELATION('9', 2)),
			NODE(1, RELATION('d', 2)),
			NODE()
		);

		FST fstUsintHexLit(LEX_USINT, LITERAL, "", 3,
			NODE(32,
				RELATION('0', 0), RELATION('1', 0), RELATION('2', 0),
				RELATION('3', 0), RELATION('4', 0), RELATION('5', 0),
				RELATION('6', 0), RELATION('7', 0), RELATION('8', 0),
				RELATION('9', 0),
				RELATION('A', 0), RELATION('B', 0), RELATION('C', 0),
				RELATION('D', 0), RELATION('E', 0), RELATION('F', 0),
				RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
				RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
				RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
				RELATION('9', 1), 
				RELATION('A', 1), RELATION('B', 1), RELATION('C', 1),
				RELATION('D', 1), RELATION('E', 1), RELATION('F', 1)),
			NODE(1, RELATION('h', 2)),
			NODE()
		);

		FST fstId(LEX_ID, ID, "", 3, // [a-z][a-z | 0-9]*
			NODE(26,
					RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
					RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
					RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
					RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
					RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
					RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
					RELATION('y', 1), RELATION('z', 1)),
			NODE(72,
					RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
					RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
					RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
					RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
					RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
					RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
					RELATION('y', 1), RELATION('z', 1), RELATION('0', 1), RELATION('1', 1),
					RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
					RELATION('6', 1), RELATION('7', 1), RELATION('8', 1), RELATION('9', 1),

					RELATION('a', 2), RELATION('b', 2), RELATION('c', 2), RELATION('d', 2),
					RELATION('e', 2), RELATION('f', 2), RELATION('g', 2), RELATION('h', 2),
					RELATION('i', 2), RELATION('j', 2), RELATION('k', 2), RELATION('l', 2),
					RELATION('m', 2), RELATION('n', 2), RELATION('o', 2), RELATION('p', 2),
					RELATION('q', 2), RELATION('r', 2), RELATION('s', 2), RELATION('t', 2),
					RELATION('u', 2), RELATION('v', 2), RELATION('w', 2), RELATION('x', 2),
					RELATION('y', 2), RELATION('z', 2), RELATION('0', 2), RELATION('1', 2),
					RELATION('2', 2), RELATION('3', 2), RELATION('4', 2), RELATION('5', 2),
					RELATION('6', 2), RELATION('7', 2), RELATION('8', 2), RELATION('9', 2)),
			NODE()
		);

		FST fstStringLit(LEX_STRING, LITERAL, "", 4,
			NODE(2,
				RELATION('`', 1),
				RELATION('`', 2)),
			NODE(276,
				RELATION('A', 1), RELATION('B', 1), RELATION('C', 1), RELATION('D', 1), RELATION('E', 1), RELATION('F', 1),
				RELATION('G', 1), RELATION('H', 1), RELATION('I', 1), RELATION('J', 1), RELATION('K', 1), RELATION('L', 1),
				RELATION('M', 1), RELATION('N', 1), RELATION('O', 1), RELATION('P', 1), RELATION('Q', 1), RELATION('R', 1),
				RELATION('S', 1), RELATION('T', 1), RELATION('U', 1), RELATION('V', 1), RELATION('W', 1), RELATION('X', 1),
				RELATION('Y', 1), RELATION('Z', 1),

				RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION('e', 1), RELATION('f', 1),
				RELATION('g', 1), RELATION('h', 1), RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
				RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1), RELATION('q', 1), RELATION('r', 1),
				RELATION('s', 1), RELATION('t', 1), RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
				RELATION('y', 1), RELATION('z', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1),
				RELATION('5', 1), RELATION('6', 1), RELATION('7', 1), RELATION('8', 1), RELATION('9', 1), RELATION('0', 1),

				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),

				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION('�', 1),
				RELATION('�', 1), RELATION('�', 1), RELATION('�', 1), RELATION(' ', 1), RELATION('.', 1), RELATION(',', 1),
				RELATION('?', 1), RELATION('!', 1), RELATION(';', 1), RELATION(':', 1), RELATION('-', 1), RELATION(')', 1),
				RELATION('(', 1),

				RELATION('A', 2), RELATION('B', 2), RELATION('C', 2), RELATION('D', 2), RELATION('E', 2), RELATION('F', 2),
				RELATION('G', 2), RELATION('H', 2), RELATION('I', 2), RELATION('J', 2), RELATION('K', 2), RELATION('L', 2),
				RELATION('M', 2), RELATION('N', 2), RELATION('O', 2), RELATION('P', 2), RELATION('Q', 2), RELATION('R', 2),
				RELATION('S', 2), RELATION('T', 2), RELATION('U', 2), RELATION('V', 2), RELATION('W', 2), RELATION('X', 2),
				RELATION('Y', 2), RELATION('Z', 2),
				RELATION('a', 2), RELATION('b', 2), RELATION('c', 2), RELATION('d', 2), RELATION('e', 2), RELATION('f', 2),
				RELATION('g', 2), RELATION('h', 2), RELATION('i', 2), RELATION('j', 2), RELATION('k', 2), RELATION('l', 2),
				RELATION('m', 2), RELATION('n', 2), RELATION('o', 2), RELATION('p', 2), RELATION('q', 2), RELATION('r', 2),
				RELATION('s', 2), RELATION('t', 2), RELATION('u', 2), RELATION('v', 2), RELATION('w', 2), RELATION('x', 2),
				RELATION('y', 2), RELATION('z', 2), RELATION('1', 2), RELATION('2', 2), RELATION('3', 2), RELATION('4', 2),
				RELATION('5', 2), RELATION('6', 2), RELATION('7', 2), RELATION('8', 2), RELATION('9', 2), RELATION('0', 2),

				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),

				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),
				RELATION('�', 2), RELATION('�', 2), RELATION('�', 2),

				RELATION(' ', 2), RELATION('.', 2), RELATION(',', 2), RELATION('?', 2), RELATION('!', 2), RELATION(';', 2),
				RELATION(':', 2), RELATION('-', 2), RELATION(')', 2), RELATION('(', 2)),
			NODE(1, RELATION('`', 3)),
			NODE()
		);

		FST fstDate(LEX_LIB_FUNC, STATIC_LIB, "", 5,
			NODE(1, RELATION('D', 1)), \
			NODE(1, RELATION('A', 2)), \
			NODE(1, RELATION('T', 3)), \
			NODE(1, RELATION('E', 4)), \
			NODE()
		);

		FST fstRandom(LEX_LIB_FUNC, STATIC_LIB, "", 7,
			NODE(1, RELATION('R', 1)), \
			NODE(1, RELATION('A', 2)), \
			NODE(1, RELATION('N', 3)), \
			NODE(1, RELATION('D', 4)), \
			NODE(1, RELATION('O', 5)), \
			NODE(1, RELATION('M', 6)), \
			NODE()
		);

		FST fstShiftOp(LEX_OPERATOR, OPERATION, "", 4,
			NODE(2, RELATION('<', 1), RELATION('~', 2)), \
			NODE(1, RELATION('~', 3)), \
			NODE(1, RELATION('>', 3)), \
			NODE()
		);

		FST fstCycle(LEX_CYCLE, DEF_LEX, "", 6,
			NODE(1, RELATION('C', 1)),
			NODE(1, RELATION('Y', 2)),
			NODE(1, RELATION('C', 3)),
			NODE(1, RELATION('L', 4)),
			NODE(1, RELATION('E', 5)),
			NODE()
		);

		FST fstEndc(LEX_ENDC, DEF_LEX, "", 5,
			NODE(1, RELATION('E', 1)),
			NODE(1, RELATION('N', 2)),
			NODE(1, RELATION('D', 3)),
			NODE(1, RELATION('C', 4)),
			NODE()
		);

		FST* result = new FST[FST_WORDS_SIZE];
		result[0] = fstUsintDecLit;
		result[1] = fstUsintHexLit;
		result[2] = fstShiftOp;
		result[3] = fstUsint;
		result[4] = fstString;
		result[5] = fstSymbol;
		result[6] = fstVariable;
		result[7] = fstFunc;
		result[8] = fstHead;
		result[9] = fstLeftBrace;
		result[10] = fstRightBrace;
		result[11] = fstRet;
		result[12] = fstSymbolLit;
		result[13] = fstStringLit;
		result[14] = fstWrite;
		result[15] = fstWriteLine;
		result[16] = fstDate;
		result[17] = fstRandom;
		result[18] = fstCycle;
		result[19] = fstEndc;
		result[20] = fstId;
		
		return result;
	}
}