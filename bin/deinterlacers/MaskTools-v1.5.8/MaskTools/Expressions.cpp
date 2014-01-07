// YV12LUT - LookUp Table transform for YV12
// YV12LUT (C) 2003 Kurosu (kurosu@inforezo.org)

// Based on "Levels" function from AviSynth software

// See legal notice in Interface.cpp and Readme.txt for more information

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .

#include "Expressions.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Function::FunctionType Function::numFunction(const char *s)
{
	if ( strcmp("sin",s) == 0 )
		return SINUS;
	if ( strcmp("cos",s) == 0 )
		return COSINUS;
	if ( strcmp("tan",s) == 0 )
		return TANGENT;
	if ( strcmp("exp",s) == 0 )
		return EXPONANTIAL;
	if ( strcmp("log",s) == 0 )
		return LOGARITHM;
	if ( strcmp("asin",s) == 0 )
		return ARCSINUS;
	if ( strcmp("acos",s) == 0 )
		return ARCCOSINUS;
	if ( strcmp("atan",s) == 0 )
		return ARCTANGENT;
	if ( strcmp("abs",s) == 0 )
		return ABSOLUTE;

	return NONE;
}

double Function::identity(double val)
{
	return val;
}

Function::Function(const char *s)
{
	switch(numFunction(s))
	{
		case SINUS : process = sin; break;
		case COSINUS : process = cos; break;
		case TANGENT : process = tan; break;
		case EXPONANTIAL : process = exp; break;
		case LOGARITHM : process = log; break;
		case ARCSINUS : process = asin; break;
		case ARCCOSINUS : process = acos; break;
		case ARCTANGENT : process = atan; break;
		case ABSOLUTE : process = fabs; break;
		default : process = Function::identity;
	}

    name = new char[strlen(s)+1];
	strcpy(name,s);
}

Function::~Function()
{
	delete[] name;
}

bool Function::isFunction(const char *s)
{
	return ( numFunction(s) != NONE );
}

Operator::OperatorType Operator::numOperator(const char *c)
{
	if ( strcmp("+",c) == 0 )
		return PLUS;
	if ( strcmp("-",c) == 0 )
		return MINUS;
	if ( strcmp("*",c) == 0 )
		return MULTIPLY;
	if ( strcmp("/",c) == 0 )
		return DIVIDE;
	if ( strcmp("^",c) == 0 )
		return POWER;
	if ( strcmp("%",c) == 0 )
		return MODULO;
	if ( strcmp(">",c) == 0 )
		return SUPERIOR;
	if ( strcmp("<",c) == 0 )
		return INFERIOR;
	if ( strcmp("=",c) == 0 )
		return EQUAL;
	if ( strcmp(">=",c) == 0 )
		return SUPEQUAL;
	if ( strcmp("<=",c) == 0 )
		return INFEQUAL;
	if ( strcmp("!=",c) == 0 )
		return NOTEQUAL;
	if ( strcmp("&",c) == 0 )
		return AND;
	if ( strcmp("|",c) == 0 )
		return OR;
	if ( strcmp("!&",c) == 0 )
		return ANDN;
	if ( strcmp("°",c) == 0 )
		return XOR;

	return NONE;
}

Operator::Operator(const char *s)
{
	name = new char[strlen(s)+1];
	strcpy(name,s);
}

Operator::~Operator()
{
	delete[] name;
}

bool Operator::isOperator(const char *c)
{
	return ( numOperator(c) != NONE );
}

double Operator::process(double lv, double rv)
{
	switch (numOperator(name))
	{
		case PLUS : return lv + rv; break;
		case MINUS : return lv - rv; break;
		case MULTIPLY : return lv * rv; break;
		case DIVIDE : return lv / rv; break;
		case POWER : return pow(lv, rv); break;
		case MODULO : return ((int)lv) % ((int)rv); break;
		case SUPERIOR : return ( lv > rv ) ? 1 : -1; break;
		case INFERIOR : return ( lv < rv ) ? 1 : -1; break;
		case EQUAL : return ( lv == rv ) ? 1 : -1; break;
		case NOTEQUAL : return ( lv != rv ) ? 1 : -1; break;
		case SUPEQUAL : return ( lv >= rv ) ? 1 : -1; break;
		case INFEQUAL : return ( lv <= rv ) ? 1 : -1; break;
		case AND : return ( lv < 0 ) ? -1 : ( rv < 0 ) ? -1 : 1; break;
		case OR : return ( lv > 0 ) ? 1 : ( rv > 0 ) ? 1 : -1; break;
		case ANDN : return ( lv < 0 ) ? -1 : ( rv < 0 ) ? 1 : -1; break;
		case XOR : return ( lv < 0 ) ? (( rv > 0 ) ? 1 : -1) : (( rv < 0 ) ? 1 : -1); break;
		default : return 0;
	}
}

TernaryOperator::TernaryOperator(const char *s)
{
	name = new char[strlen(s)+1];
	strcpy(name,s);
}

TernaryOperator::~TernaryOperator()
{
	delete[] name;
}

bool TernaryOperator::isTernaryOperator(const char *c)
{
	return ( numOperator(c) != NONE );
}

TernaryOperator::OperatorType TernaryOperator::numOperator(const char *c)
{
	if ( strcmp("?",c) == 0 )
		return IFTHENELSE;
	return NONE;
}

double TernaryOperator::process(double lv, double mv, double rv)
{
	switch(numOperator(name))
	{
	case IFTHENELSE : return (lv > 0) ? mv : rv; break;
	default : return 0;
	}
}

Expression::Expression(Operator *o, Expression *l, Expression *r)
{
	type = OPERATOR;
	op = o;
	left = l;
	right = r;
	parameter = NULL;
	fun = NULL;
	to = NULL;
	val = 0;
}

Expression::Expression(TernaryOperator *o, Expression *p, Expression *l, Expression *r)
{
	type = TERNOPERATOR;
	op = NULL;
	left = l;
	right = r;
	parameter = p;
	fun = NULL;
	to = o;
	val = 0;
}

Expression::Expression(Function *f, Expression *p)
{
	type = FUNCTION;
	fun = f;
	parameter = p;
	op = NULL;
	left = NULL;
	right = NULL;
	to = NULL;
	val = 0;
}

Expression::Expression(const double v)
{
	type = VALUE;
	val = v;
	op = NULL;
	left = NULL;
	right = NULL;
	fun = NULL;
	to = NULL;
	parameter = NULL;
}

Expression *Expression::BuildExpression(const char *s, const char separator)
{
	int separatorCount = 0;
	int length = strlen(s);
	char *separastring = new char[2];
	int i, count;
	
	separastring[0] = separator;
	separastring[1] = 0;

	for ( i = 0; i < length; i++ )
		separatorCount += (s[i] == separator) ? 1 : 0;

	char **array = new char*[separatorCount+1];
	const char *sp = s;

	for ( count = 0; count < separatorCount; count++ )
	{
		int end = strcspn(sp,separastring);
		array[count] = new char[end+1];
		for ( i = 0; i < end; i++ )
			array[count][i] = sp[i];
		array[count][end] = 0;

		sp = sp + end + strspn(sp+end,separastring);
	}
	int end = strlen(sp);
	array[separatorCount] = new char[end+1];
	for ( i = 0; i < end; i++ )
		array[separatorCount][i] = sp[i];
	array[separatorCount][end] = 0;

	ExpressionHeap * heap = new ExpressionHeap(1000);
	Expression *l, *r, *p;
	Expression *e;
	Operator *o;
	TernaryOperator *to;
	Function *f;

	for ( count = 0; count < separatorCount + 1; count++ )
	{
		if ( Function::isFunction(array[count]) )
		{
			r = heap->Pop();
			f = new Function(array[count]);
			heap->Push(new Expression(f,r));
		}
		else if ( Operator::isOperator(array[count]) )
		{
			r = heap->Pop();
			l = heap->Pop();
			o = new Operator(array[count]);
			heap->Push(new Expression(o,l,r));
		}
		else if ( TernaryOperator::isTernaryOperator(array[count]) )
		{
			r = heap->Pop();
			l = heap->Pop();
			p = heap->Pop();
			to = new TernaryOperator(array[count]);
			heap->Push(new Expression(to,p,l,r));
		}
		else if ( array[count][0] == 'x' )
		{
			heap->Push(new Expression(1));
		}
		else if ( array[count][0] == 'y' )
		{
			heap->Push(new Expression(2));
		}
		else 
		{
			heap->Push(new Expression(atof(array[count])));
		}
	}

	for ( i = 0; i < separatorCount + 1; i++ )
        delete[] array[i];

	delete[] array;
	delete[] separastring;

	e = heap->Pop();

	delete heap;

	return e;
}

Expression::Expression(int n)
{
	type = (n == 2) ? VARIABLEY : VARIABLEX;
	left = NULL;
	right = NULL;
	parameter = NULL;
	op = NULL;
	fun = NULL;
	to = NULL;
	val = -1;
}

Expression::~Expression()
{
	switch (type)
	{
	case FUNCTION : delete fun; delete parameter; break;
	case OPERATOR : delete op; delete right; delete left; break;
	case TERNOPERATOR : delete to; delete parameter; delete left; delete right; break;
	}
}

double Expression::process(double x, double y)
{
	switch(type)
	{
        case OPERATOR : return op->process(left->process(x,y),right->process(x,y));
		case TERNOPERATOR : return to->process(parameter->process(x,y), left->process(x,y), right->process(x,y));
		case FUNCTION : return fun->process(parameter->process(x,y));
		case VALUE : return val;
		case VARIABLEY : return y;
		default : return x;
	}
}

ExpressionHeap::ExpressionHeap(int ms)
{
	maxSize = ms;
	heap = new Expression*[maxSize];
	for ( int i = 0; i < maxSize; i++ )
		heap[i] = NULL;
	count = 0;
}

ExpressionHeap::~ExpressionHeap()
{
	delete[] heap;
}

bool ExpressionHeap::IsEmpty() { return (count == 0); }

void ExpressionHeap::Push(Expression *e)
{
	if (count < maxSize)
	{
		heap[count] = e;
		count++;
	}
}

Expression *ExpressionHeap::Pop()
{
	if (!IsEmpty())
	{
		count--;
	}
	Expression *e = heap[count];
	heap[count] = NULL;
	return e;
}

int Function::affiche(int pos, char *chaine)
{
	strcpy(chaine+pos,name);
	chaine[pos+strlen(name)] = ' ';
	return pos + strlen(name) + 1;
}

int Operator::affiche(int pos, char *chaine)
{
	strcpy(chaine+pos,name);
	chaine[pos+strlen(name)] = ' ';
	return pos + strlen(name) + 1;
}

int TernaryOperator::affiche(int pos, char *chaine)
{
	strcpy(chaine+pos,name);
	chaine[pos+strlen(name)] = ' ';
	return pos + strlen(name) + 1;
}

int Expression::affiche(int pos, char *chaine)
{
	int pos2, pos3;
	switch(type)
	{
	case FUNCTION : pos2 = fun->affiche(pos,chaine); return parameter->affiche(pos2,chaine); break;
	case OPERATOR : pos2 = op->affiche(pos,chaine); pos3 = left->affiche(pos2,chaine); return right->affiche(pos3,chaine); break;
	case VALUE :    return pos + sprintf(chaine+pos,"%f ", val);
	default : chaine[pos] = 'X'; chaine[pos+1] = ' '; return pos + 2; break;
	}
}