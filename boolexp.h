#ifndef BOOLEXP_H
#define BOOLEXP_H

/*
expr:		(expr) expr2 | left op1 right expr2
expr2:		op2 expr expr2 | e
op1:		'=' | '!=' | '<' | '<=' | '>' | '>=' | '=~' | '!~'
op2:		'or' | 'and'
left:		'req''.''code' | 'req''['string']'
right:		string | number | regular
*/

#include <stdio.h>
#include "str.h"

enum token_t
{
	T_TOKEN_INVALID = 0,
	T_EOL, // end of line
	T_OP_EQ, // =
	T_OP_NE, // !=
	T_OP_LT, // <
	T_OP_LE, // <=
	T_OP_GT, // >
	T_OP_GE, // >=
	T_OP_REG_EQ, // =~
	T_OP_REG_NE, // !~
	T_LBRACE, // (
	T_RBRACE, // )
	T_LZBRACE, // [
	T_RZBRACE, // ]
	T_DOT, // .
	T_NUM,
	T_STRING,
	T_BAREWORD,
	T_REG,
	T_OP_OR,
	T_OP_AND,
	T_TOKEN_LAST
};

struct token_name_number
{
	const char *name;
	enum token_t number;
};

class Exp
{
public:
	//enum BOOL_OP {AND, OR};
	//enum OPRAND_OP {EQUAL, NOTEQUAL, GREAT, LESS, GREATEQUAL, LESSEQUAL, REGULAR};

	Exp()
	{
		left = right = NULL;
	}

	virtual ~Exp()
	{
	}

	virtual bool Result() = 0;

protected:
	Exp *left;
	Exp *right;
};

class OPLeft
{
public:
	enum obj_type {OBJ_REQ};
	enum op_type {OP_CODE, OP_ATTRIB};

	OPLeft(obj_type t, op_type o, const char *att)
		:obj(t), op(o)
	{
		if (att) {
			attrib = att;
		}
	}

	obj_type GetObj()
	{
		return obj;
	}

	op_type GetOp()
	{
		return op;
	}

	const String &GetAttrib()
	{
		return attrib;
	}
protected:
	obj_type obj;
	op_type op;
	String attrib;
};

class OPRight
{
public:
	enum val_type {VAL_STRING, VAL_NUM, VAL_REG};

	OPRight(val_type t, const char *val)
		:type(t), value(val)
	{
	}

	val_type GetType()
	{
		return type;
	}

	const String &GetValue()
	{
		return value;
	}
protected:
	val_type type;
	String value;
};

class Operation : public Exp
{
public:
	Operation(OPLeft *op1, token_t op, OPRight *op2);

	bool Result();

protected:
	OPLeft *oprand1;
	token_t op;
	OPRight *oprand2;
};

class BoolExp : public Exp
{
public:
	BoolExp(Exp *op1, token_t op, Exp *op2);
	bool Result();

protected:
	token_t op;
};

class Expression
{
public:
	Expression();
	~Expression();

	int Parse(const char *text);

	bool Result();

protected:
	OPLeft *left(const char **p);
	OPRight *right(const char **p);
	Exp *expr(const char **p);
	Exp *expr2(const char **p, Exp *ex);

protected:
	Exp *root;
};

#endif
