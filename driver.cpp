// Author: Avery VanAusdal
// Assignment Number: Lab 3
// File Name: driver.cpp
// Course/Section: COSC 2436 Section 3
// Date: 11/1/2018
// Instructor: Bernard Ku

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "LinkedStack.h"
#include "ArrayStack.h"
using namespace std;

/* 	Performs math operations based on given char operator
	@pre 	Operands are not evaluated.
	@post 	Operands have been evaluated using operator
	@param	op1 first operand
			op2 second operand
			op operator for eval type
	@return value of operands evaluated by operator */
int performOperation(int op1, int op2, char op)
{
	switch(op)
	{
		case '+': return op2+op1;
		case '-': return op2-op1;
		case '*': return op2*op1;
		case '/': if (op1 == 0) cout<<"Dividing by zero: "; return op2/op1;
	}
} //end performOperation

/*	Evaluates a postfix expression
	@post	If the expression is valid, output result, or if invalid, error type
	@param	expression is the expression string to be evaluated
			stack is a pointer to a StackInterface stack, such as LinkedStack or ArrayStack
					stack is used for holding operands in order until needed	*/
void postFixEvaluate(string expression, StackInterface<int> *stack)
{
	int size = expression.size();
	for (int i=0; i<size; i++)
	{
		char token = expression[i];
		if (isdigit(token)) stack->push(token - '0'); // - '0' converts char to correct int value
		else if (isalpha(token))
		{
			cout<<"Malformed expression - Invalid operand/character detected.\n";
			return;
		}
		else if (token=='+' || token=='-' || token=='*' || token=='/')
		{
			int v1, v2;
			if (stack->isEmpty())
			{
				cout<<"Malformed expression - Invalid Postfix detected.\n";
				return;
			}
			else
			{
				v1 = stack->peek();
				stack->pop();
				if (stack->isEmpty())
				{
					cout<<"Malformed expression - Invalid Postfix detected.\n";
					return;
				}
				else
				{
					v2 = stack->peek();
					stack->pop();
					stack->push(performOperation(v1, v2, token));
				}
			}
		}
		else if (token == ' '); //do nothing
		else
		{
			cout<<"Malformed expression - Invalid Operator detected.\n";
			return;
		}
	}
	cout<<"The value is: "<<stack->peek()<<endl;
	delete stack; //frees memory used by Stack from heap
} //end postFixEvaluate

/* Converts infix expressions to postfix expressions
	@pre An unconverted Infix expression
	@post Infix expression converted to Postfix
	@param inFix is a string that holds the Infix expression
	@return the resulting Postfix expression string		*/
string inFixToPostFix(string inFix)
{
	string result;
	LinkedStack<char> *stack = new LinkedStack<char>;
	int size = inFix.size();
	char tempToken;
	for (int i=0; i<size; i++)
	{
		char token = inFix[i];
		if (token == '#')
		{
			result.append("# ");
		}
		else if (token == '(')
		{
			stack->push('(');
		}
		else if (token=='+' || token=='-' || token=='*' || token=='/')
		{
			if (stack->isEmpty())
			{
				stack->push(token);
			}
			else
			{
				while (!stack->isEmpty())
				{
					tempToken = stack->peek();
					if (tempToken == '(')
					{
						break;
					}
					if (token == '*' || token == '/')
					{
						if (tempToken == '+' || tempToken == '-')
						{
							break;
						}
					}
					result += tempToken;
					stack->pop();
				}
				stack->push(token);
			}
		}
		else if (token == ')')
		{
			while (!stack->isEmpty())
			{
				tempToken = stack->peek();
				stack->pop();
				if (tempToken == '(')
				{
					break;
				}
				result += tempToken;
			}
		}
		else if (token != ' ') //as long as it's not a space
		{
			result += token;
		}
	}
	while (!stack->isEmpty())
	{
		result += stack->peek();
		stack->pop();
	}
	return result;
}

int main ()
{
	ifstream inFile;
	inFile.open("Postfix.txt");
	vector<string> expressionVector;
	string temp;
	while (!inFile.eof())
	{ //fills vector with lines from file
		getline(inFile, temp);
		expressionVector.push_back(temp);
	}
	inFile.close();
	int expressionCount = expressionVector.size();
	cout<<"LinkedStack Postfix Expression Evaluation: "<<endl;
	for (int i = 0; i < expressionCount; i++)
	{ //testing LinkedStack on file
		cout<<"PostFix Expression: "<<expressionVector[i]<<endl;
		postFixEvaluate(expressionVector[i], new LinkedStack<int>);
	}
	cout<<"\nArrayStack Postfix Expression Evaluation: "<<endl;
	for (int i = 0; i < expressionCount; i++)
	{ //testing ArrayStack on file
		cout<<"PostFix Expression: "<<expressionVector[i]<<endl;
		postFixEvaluate(expressionVector[i], new ArrayStack<int>);
	}

	inFile.open("Infix.txt");
	vector<string> inFixExpressionVector;
	while (!inFile.eof())
	{ //fills vector with lines from file
		getline(inFile, temp);
		inFixExpressionVector.push_back(temp);
	}
	inFile.close();
	expressionCount = inFixExpressionVector.size();
	cout<<"\nInfix to Postfix Expression and Calculation: \n";
	for (int i = 0; i < expressionCount; i++)
	{
		cout<<"Infix expression: "<<inFixExpressionVector[i]<<endl;
		temp = inFixToPostFix(inFixExpressionVector[i]);
		cout<<"Converted Postfix: "<<temp<<endl;
		postFixEvaluate(temp, new LinkedStack<int>);
	}
	
	cout<<"\nManual Entry:\n";
	while (true)
	{ //loop for user input Postfix Expressions
		string userInput = " ";
		cout<<"Enter a Postfix Expression (i.e. 2 3 4 + * ) or q to go to Infix entry:\n";
		getline(cin, userInput);
		if (userInput == "q")
		{ //sentinel exit value
			cout<<endl;
			break;
		}
		else postFixEvaluate(userInput, new LinkedStack<int>);
	}
	while (true)
	{
		string userInput = " ";
		cout<<"Enter an Infix Expression (i.e. 2 3 4 + * ) or q to quit:\n";
		getline(cin, userInput);
		if (userInput == "q")
		{ //sentinel exit value
			cout<<"Thank you, goodbye!";
			return 0;
		}
		else
		{
			string temp = inFixToPostFix(userInput);
			cout<<"Converted Postfix: "<<temp<<endl;
			postFixEvaluate(temp, new LinkedStack<int>);
		}
	}
}

//	Sample output:
//		
//	LinkedStack Postfix Expression Evaluation:
//	PostFix Expression: 345*+67*8+9*+
//	The value is: 473
//	PostFix Expression: 1234*+5/-
//	The value is: -1
//	PostFix Expression: 12+43-*
//	The value is: 3
//	PostFix Expression: 48+65-*32-22+*/
//	The value is: 3
//	PostFix Expression: 234+*
//	The value is: 14
//	PostFix Expression: 72/
//	The value is: 3
//	PostFix Expression: 512+4*+3-
//	The value is: 14
//	PostFix Expression: 72%
//	Malformed expression - Invalid Operator detected.
//	PostFix Expression: 7*22+
//	Malformed expression - Invalid Postfix detected.
//	PostFix Expression: A2/
//	Malformed expression - Invalid operand/character detected.
//	PostFix Expression: 72//
//	Malformed expression - Invalid Postfix detected.
//	PostFix Expression: (23-
//	Malformed expression - Invalid Operator detected.
//	
//	ArrayStack Postfix Expression Evaluation:
//	PostFix Expression: 345*+67*8+9*+
//	The value is: 473
//	PostFix Expression: 1234*+5/-
//	The value is: -1
//	PostFix Expression: 12+43-*
//	The value is: 3
//	PostFix Expression: 48+65-*32-22+*/
//	The value is: 3
//	PostFix Expression: 234+*
//	The value is: 14
//	PostFix Expression: 72/
//	The value is: 3
//	PostFix Expression: 512+4*+3-
//	The value is: 14
//	PostFix Expression: 72%
//	Malformed expression - Invalid Operator detected.
//	PostFix Expression: 7*22+
//	Malformed expression - Invalid Postfix detected.
//	PostFix Expression: A2/
//	Malformed expression - Invalid operand/character detected.
//	PostFix Expression: 72//
//	Malformed expression - Invalid Postfix detected.
//	PostFix Expression: (23-
//	Malformed expression - Invalid Operator detected.
//	
//	Infix to Postfix Expression and Calculation:
//	Infix expression: 3 + 4 * 5 + ( 6 * 7 + 8 ) * 9
//	Converted Postfix: 345*+67*8+9*+
//	The value is: 473
//	Infix expression: 1 - (2 + 3 * 4)/5
//	Converted Postfix: 1234*+5/-
//	The value is: -1
//	Infix expression: (1+2)*(4-3)
//	Converted Postfix: 12+43-*
//	The value is: 3
//	Infix expression: (4+8)*(6-5)/((3-2)*(2+2))
//	Converted Postfix: 48+65-*32-22+*/
//	The value is: 3
//	
//	Manual Entry:
//	Enter a Postfix Expression (i.e. 2 3 4 + * ) or q to go to Infix entry:
//	234+*
//	The value is: 14
//	Enter a Postfix Expression (i.e. 2 3 4 + * ) or q to go to Infix entry:
//	7 2%
//	Malformed expression - Invalid Operator detected.
//	Enter a Postfix Expression (i.e. 2 3 4 + * ) or q to go to Infix entry:
//	45*2/
//	The value is: 10
//	Enter a Postfix Expression (i.e. 2 3 4 + * ) or q to go to Infix entry:
//	q
//	Enter an Infix Expression (i.e. 2 3 4 + * ) or q to quit:
//	4+5*6
//	Converted Postfix: 456*+
//	The value is: 34
//	Enter an Infix Expression (i.e. 2 3 4 + * ) or q to quit:
//	(2+6)/(3-1)
//	Converted Postfix: 26+31-/
//	The value is: 4
//	Enter an Infix Expression (i.e. 2 3 4 + * ) or q to quit:
//	2*
//	Converted Postfix: 2*
//	Malformed expression - Invalid Postfix detected.
//	Enter an Infix Expression (i.e. 2 3 4 + * ) or q to quit:
//	q
//	Thank you, goodbye!
//	--------------------------------
//	Process exited after 135 seconds with return value 0
//	Press any key to continue . . .
