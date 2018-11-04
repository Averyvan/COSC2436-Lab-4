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
	{
		postFixEvaluate(expressionVector[i], new LinkedStack<int>);
	}
	cout<<"\nArrayStack Postfix Expression Evaluation: "<<endl;
	for (int i = 0; i < expressionCount; i++)
	{
		postFixEvaluate(expressionVector[i], new ArrayStack<int>);
	}
	cout<<"\nManual Entry:\n";
	while (true)
	{
		string userInput = " ";
		cout<<"Enter a Postfix Expression (i.e. 2 3 4 + * ) or \"q\" to quit:\n";
		getline(cin, userInput);
		if (userInput == "q")
		{
			cout<<"Thank you, goodbye!";
			return 0;
		}
		else postFixEvaluate(userInput, new LinkedStack<int>);
	}
}

/* Sample output:
LinkedStack Postfix Expression Evaluation:
The value is: 473
The value is: -1
The value is: 3
The value is: 3
The value is: 14
The value is: 3
The value is: 14
Malformed expression - Invalid Operator detected.
Malformed expression - Invalid Postfix detected.
Malformed expression - Invalid operand/character detected.
Malformed expression - Invalid Postfix detected.
Malformed expression - Invalid Operator detected.

ArrayStack Postfix Expression Evaluation:
The value is: 473
The value is: -1
The value is: 3
The value is: 3
The value is: 14
The value is: 3
The value is: 14
Malformed expression - Invalid Operator detected.
Malformed expression - Invalid Postfix detected.
Malformed expression - Invalid operand/character detected.
Malformed expression - Invalid Postfix detected.
Malformed expression - Invalid Operator detected.

Manual Entry:
Enter a Postfix Expression (i.e. 2 3 4 + * ) or "q" to quit:
234+*
The value is: 14
Enter a Postfix Expression (i.e. 2 3 4 + * ) or "q" to quit:
72%
Malformed expression - Invalid Operator detected.
Enter a Postfix Expression (i.e. 2 3 4 + * ) or "q" to quit:
45*2/
The value is: 10
Enter a Postfix Expression (i.e. 2 3 4 + * ) or "q" to quit:
q
Thank you, goodbye!
--------------------------------
Process exited after 118.4 seconds with return value 0
Press any key to continue . . .
*/
