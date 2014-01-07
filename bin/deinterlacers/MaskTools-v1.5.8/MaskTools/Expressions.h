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

#ifndef __EXPR__
#define __EXPR__

class Function {

	enum FunctionType {
        SINUS, COSINUS, TANGENT, EXPONANTIAL,
		LOGARITHM, ARCSINUS, ARCCOSINUS, ARCTANGENT,
        ABSOLUTE, NONE
	};

	char *name;

	static double identity(double val);
	static FunctionType numFunction(const char *s);

public :

	Function(const char *s);
	~Function();
	static bool isFunction(const char *s);
	double (* process)(double val);
	int affiche(int pos, char *chaine);
};

class Operator {

	enum OperatorType {
		PLUS, MINUS, MULTIPLY, DIVIDE, 
		POWER, MODULO, SUPERIOR, INFERIOR, EQUAL,
		SUPEQUAL, INFEQUAL, NOTEQUAL,
		AND, OR, ANDN, XOR, NONE
	};


	char *name;
    
	static OperatorType numOperator(const char *s);

public :

	Operator(const char *s);
	~Operator();
	static bool isOperator(const char *c);
	double process(double lv, double rv);
	int affiche(int pos, char *chaine);
};

class TernaryOperator {

	enum OperatorType {
		IFTHENELSE, NONE
	};


	char *name;
    
	static OperatorType numOperator(const char *s);

public :

	TernaryOperator(const char *s);
	~TernaryOperator();
	static bool isTernaryOperator(const char *c);
	double process(double lv, double mv, double rv);
	int affiche(int pos, char *chaine);
};

class Expression {

	enum ExpressionType {
		VARIABLEX, VARIABLEY, OPERATOR, VALUE, FUNCTION, TERNOPERATOR
	};

	ExpressionType type;
	Operator *op;
	TernaryOperator *to;
	Function *fun;
	double val;

	Expression *left;
	Expression *right;
	Expression *parameter;


public :
	Expression(Operator *o, Expression *l, Expression *r);
	Expression(Function *f, Expression *p);
	Expression(TernaryOperator *o, Expression *p, Expression *l, Expression *r);
	Expression(double v);
	Expression(char *s);
	Expression(int n);
	~Expression();

	double process(double x, double y);
	static Expression *BuildExpression(const char *s, char separator);
	int affiche(int pos, char *chaine);
};

class ExpressionHeap {
	Expression **heap;
	int count;
	int maxSize;

public :
	ExpressionHeap(int ms);
	~ExpressionHeap();
	bool IsEmpty();
	void Push(Expression *e);
	Expression *Pop();
};


#endif