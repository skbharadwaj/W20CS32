#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>

using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result);
void toPostFix(string infix, string& postfix);
bool isOperator(char c);
bool operand(char c);
bool precedence(char op1, char op2);
int valueInMapAt(const Map& m, char key);
int returnValue(const Map& m, string infix);
char closest(string infix, int i, bool which);

int main () {
            char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
            int  vals[] = {  3,  -9,   6,   2,   4,   1  };
            Map m;
            for (int k = 0; vars[k] != '#'; k++)
                m.insert(vars[k], vals[k]);
            string pf;
            int answer;
            assert(evaluate("a+ e", m, pf, answer) == 0  &&
                                    pf == "ae+"  &&  answer == -6);
            answer = 999;
            assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
              // unary operators not allowed:
            assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
            assert(evaluate("a*b", m, pf, answer) == 2  &&
                                    pf == "ab*"  &&  answer == 999);
            assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                                    pf == "yoau-*+"  &&  answer == -1);
            answer = 999;
            assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                                    pf == "oyy-/"  &&  answer == 999);
            assert(evaluate(" a  ", m, pf, answer) == 0  &&
                                    pf == "a"  &&  answer == 3);
            assert(evaluate("((a))", m, pf, answer) == 0  &&
                                    pf == "a"  &&  answer == 3);
            cout << "Passed all tests" << endl;
}

int returnValue(const Map& m, string infix) {
	for(int i = 0;i < infix.size();i++) {
		if((int)(infix[i]) >= 65 && (int)(infix[i]) <= 90) return 1;
	}
	int open = 0;
	int close = 0;
	for(int i = 0;i < infix.size();i++) {
		if(infix[i] == '(') open++;
		if(infix[i] == ')') close++;
	}
	if(open != close) return 1;
	if(infix.size() == 0) return 1;
	for(int i = 0;i < infix.size();i++) 
		if(operand(infix[i]) && !m.contains(infix[i])) return 2;
	if(infix.size() == 1 && !operand(infix[0])) return 1;
	char first = closest(infix, -1, true);
	int indexFirst = 0;
	for(int i = 0;i < infix.size();i++)	
		if(first == infix[i]) { indexFirst = i; break; }
	char second = closest(infix, indexFirst, true);
	if(isOperator(first) || first == ')' || (second == '(' && operand(first)) || second == ')') return 1;

	for(int i = 1;i < infix.size();i++) {
		char ch = infix[i];
		char before = closest(infix, i, false);
		char after = closest(infix, i, true);
		if(operand(ch)) {
			if(operand(before) || operand(after)) return 1;
			else if(before == ')' || after == '(') return 1;
		}
		else if(isOperator(ch)) {
			if(isOperator(before) || isOperator(after)) return 1;
			else if(before == '(' || after == ')') return 1;
		}
	}
	char end;
	int index = 0;
	for(int i = 0;i < infix.size();i++)
		if(infix[i] != ' ') { end = infix[i]; index = i; }
	char secondLast = closest(infix, index, false);

	if(isOperator(end) || end == '(') return 1;
	if((secondLast == ')' && operand(end)) || secondLast == '(') return 1;
	if(end == '(') return 1;
	return 0;
}

char closest(string infix, int i, bool which) {
	if(which) {
		for(int j = i+1;j < infix.size();j++) if(infix[j] != ' ') return infix[j];
	}
	else { 
		for(int j = i-1;j > - 1;j--) if(infix[j] != ' ') return infix[j];
	}
	return '$';
}

int valueInMapAt(const Map& m, char key) {
	ValueType v;
	m.get(key, v);
	return v;
}

int evaluate(string infix, const Map& values, string& postfix, int& result) {
	toPostFix(infix, postfix);
	stack<int> operands; 
	int rv = returnValue(values, infix);
	if(rv == 1 || rv == 2) return rv;

	int counter = 0;	
	for(int i = 0;i < postfix.size();i++) {
		if(operand(postfix[i])) counter++;
		else if(isOperator(postfix[i])) {
			counter -= 2;
			if(counter < 0) return 1;
			counter++;
		}
	}
	if(counter != 1) return 1;


    for(int i = 0;i < postfix.size();i++) {
        int op1;
        int op2;
        if(operand(postfix[i])) operands.push(valueInMapAt(values, postfix[i]));
        else {
            op1 = operands.top();
            operands.pop();
            op2 = operands.top();
            operands.pop();
            switch(postfix[i]) {
                case '+':
                    operands.push(op1 + op2);
                    break;
                case '-':
                    operands.push(op2 - op1);
                    break;
                case '*':
                    operands.push(op1*op2);
                    break;
                case '/':
					if(op1 == 0) return 3;
                    operands.push(op2/op1);
                    break;
            }
        }
    }
    result = operands.top();
	if(operands.size() != 1) return 1;
	return 0;
}

void toPostFix(string infix, string& postfix) {
	postfix = "";
	stack<char> operators;
	for(int i = 0;i < infix.size();i++) {
		char c = infix[i];
		if(operand(c)) postfix += c;
		else if(c == '(') operators.push(c);
		else if(c == ')') {
			while(operators.top() != '(') {
				postfix += operators.top();
				operators.pop();
			}
			operators.pop();
		}
		else if(isOperator(c)) {
			while(!operators.empty() && operators.top() != '(' && precedence(c, operators.top())) {
				postfix += operators.top();
				operators.pop();
			}
			operators.push(c);
		}
	}
	while(!operators.empty()) {
		postfix += operators.top();
		operators.pop();
	}
}


bool isOperator(char c) {
    return c == '+' || c == '-' || c == '/' || c == '*';
}

bool operand(char c) { 
	return !isOperator(c) && c != '(' && c != ')' && c != ' ' && c != '$';
}

bool precedence(char op1, char op2) {
	return (op1 == op2) || (op1 == '+') || (op1 == '-') || (op1 == '*' && op2 == '/') || (op1 == '/' && op1 == '*');
}







