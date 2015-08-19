#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "boolexp.h"
#include "str.h"

Operation::Operation(OPLeft *op1, token_t op, OPRight *op2)
{
	oprand1 = op1;
	this->op = op;
	oprand2 = op2;
}

bool Operation::Result()
{
/*
	switch(op)
	{
		case T_OP_EQ: // =
		case T_OP_NE: // !=
		case T_OP_LT: // <
		case T_OP_LE: // <=
		case T_OP_GT: // >
		case T_OP_GE: // >=
		case T_OP_REG_EQ: // =~
		case T_OP_REG_NE: // !~
	}
*/
	return false;
}



BoolExp::BoolExp(Exp *op1, token_t op, Exp *op2)
{
	left = op1;
	this->op = op;
	right = op2;
}

bool BoolExp::Result()
{
	switch(op)
	{
	case T_OP_AND:
		return left->Result() && right->Result();
	case T_OP_OR:
		return left->Result() || right->Result();
	}

	return false;
}

static const token_name_number op_tokens[] =
{
	{"!=" , T_OP_NE},
	{"<=" , T_OP_LE},
	{"<" , T_OP_LT},
	{">=" , T_OP_GE},
	{">" , T_OP_GT},
	{"=~" , T_OP_REG_EQ},
	{"=" , T_OP_EQ},
	{"!~" , T_OP_REG_NE},
	{"(" , T_LBRACE},
	{")" , T_RBRACE},
	{"[" , T_LZBRACE},
	{"]" , T_RZBRACE},
	{"." , T_DOT},
	{NULL, T_TOKEN_INVALID}
};


#define TOKEN_MATCH(a, t) \
	( (t)[0] == (a)[0] && \
	 ((t)[1] == (a)[1] || (t)[1] == 0))

static token_t gettoken(const char **ptr, char *buf, int buflen, int peek)
{
	const char *p = *ptr;
	const token_name_number *t;
	char *s;

	while (*p && isspace((int) *p))
		p++;

	if (*p == 0) {
		*ptr = p;
		return T_EOL;
	}

	if (!isalnum(*p)) for (t = op_tokens; t->name; t++) {
		if (TOKEN_MATCH(p, t->name)) {
			strcpy(buf, t->name);
			p += strlen(t->name);
			while (isspace((int) *p))
				p++;
			if (!peek)
				*ptr = p;
			return t->number;
		}
	}

	int quote = 0;
	int quote_ok = 0;
	if (*p == '"'||
		*p == '/') {
		quote = *p;
		p++;
	}

	s = buf;
	while (*p && buflen-- > 1) {
		if (quote) {
			if (*p == '"' ||
				*p == '/') {
				p++;
				quote_ok = 1;
				break;
			}
		} else {
			if (!isalnum(*p)) {
				break;
			}
		}
		*s++ = *p++;
	}
	if (quote && !quote_ok) {
		return T_TOKEN_INVALID;
	}

	*s = '\0';
	while (*p && isspace((int) *p))
		p++;

	if (!peek)
		*ptr = p;
	if (quote == '"') {
		return T_STRING;
	} else if (quote == '/') {
		return T_REG;
	} else {
		if (strcasecmp(buf, "or") == 0) {
			return T_OP_OR;
		} else if (strcasecmp(buf, "and") == 0) {
			return T_OP_AND;
		} else if (s > buf) {
			char *x = NULL;
			strtol(buf, &x, 10);
			if (*x == '\0') {
				return T_NUM;
			}
		}
	}

	if (s == buf)
		return T_TOKEN_INVALID;

	return T_BAREWORD;
}


Expression::Expression()
	: root(NULL)
{
}

Expression::~Expression()
{
	if (root) {
		delete root;
	}
}

OPLeft *Expression::left(const char **p)
{
	char buf[128];
	token_t t = gettoken(p, buf, sizeof(buf), 0);
	if (t != T_BAREWORD || strcasecmp(buf, "req") != 0)
		return NULL;

	t = gettoken(p, buf, sizeof(buf), 0);
	if (t == T_DOT) {
		t = gettoken(p, buf, sizeof(buf), 0);
		if (t != T_BAREWORD)
			return NULL;

		if (strcasecmp(buf, "code") == 0) {
			return new OPLeft(OPLeft::OBJ_REQ, OPLeft::OP_CODE, NULL);
		}
		return NULL;
	} else if (t == T_LZBRACE) {
		char buf2[128];
		t = gettoken(p, buf2, sizeof(buf2), 0);
		if (t != T_STRING)
			return NULL;

		t = gettoken(p, buf, sizeof(buf), 0);
		if (t != T_RZBRACE)
			return NULL;

		return new OPLeft(OPLeft::OBJ_REQ, OPLeft::OP_ATTRIB, buf2);
	}
	return NULL;
}

OPRight *Expression::right(const char **p)
{
	char buf[128];
	token_t t = gettoken(p, buf, sizeof(buf), 0);

	OPRight::val_type vt;
	if (t == T_NUM) {
		vt = OPRight::VAL_NUM;
	} else if (t == T_STRING) {
		vt = OPRight::VAL_STRING;
	} else if (t == T_REG) {
		vt = OPRight::VAL_REG;
	} else {
		return NULL;
	}

	return new OPRight(vt, buf);
}

Exp *Expression::expr2(const char **p, Exp *ex)
{
	char buf[64];
	token_t t = gettoken(p, buf, sizeof(buf), 0);
	if (t == T_EOL)
		return ex;

	if (t != T_OP_AND && t != T_OP_OR) {
		delete ex;
		return NULL;
	}

	Exp *ex2 = expr(p);
	if (ex2 == NULL) {
		delete ex;
		return NULL;
	}

	return expr2(p, new BoolExp(ex, t, ex2));
}

Exp * Expression::expr(const char **p)
{
	char buf[64];
	token_t t = gettoken(p, buf, sizeof(buf), 1);
	if (t == T_LBRACE) {
		gettoken(p, buf, sizeof(buf), 0);
		Exp *e = expr(p);
		if (!e)
			return NULL;

		t = gettoken(p, buf, sizeof(buf), 0);
		if (t != T_LBRACE) {
			delete e;
			return NULL;
		}
		return expr2(p, e);
	} else {
		OPLeft *pLeft = left(p);
		if (pLeft == NULL)
			return NULL;

		token_t t = gettoken(p, buf, sizeof(buf), 0);
		if (t <T_OP_EQ || t >T_OP_REG_NE) {
			delete pLeft;
			return NULL;
		}

		OPRight *pRight = right(p);
		if (pRight == NULL) {
			delete pLeft;
			return NULL;
		}

		return new Operation(pLeft, t, pRight);
	}
}

int Expression::Parse(const char *text)
{
	root = expr(&text);
	if (root == NULL)
		return -1;

	return 0;
}

bool Expression::Result()
{
	if (root)
		return root->Result();

	return false;
}
