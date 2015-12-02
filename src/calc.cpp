#include <iostream>
#include <string>

#include "math.h"
#include "strlib.h"
#include "stackshpp.h"
#include "vectorshpp.h"
#include "console.h"

using namespace std;

/**
 * This program is an advanced calculator that accepts an
 * equation and displays the result. The program accepts and
 * processes the following mathematical operators:
 * '+', '-', '*', '/', '^', 'sin', 'cos', 'sqrt', 'tan'.
 * To use the ('sin', 'cos', 'sqrt', 'tan'), you need to
 * call desired function and write the value in parentheses.
 * Example: sin(25)
 * Fractional numbers must be entered using the '.'
 *
 * Example of writing the equation: -19+(sin(-0.5))*((7^4)/5)+sqrt(4)
 *
 */

// function prototypes
VectorSHPP<string> polishInvertedRecord(string equation);
double getResult(VectorSHPP<string> & records);
int opPreced(char ch);
bool isNumber(char ch);
bool isOperator(char ch);
bool isFunction(string func);

/**
 * The main method of the program, which prompts the user for the
 * equation to be solved, and displays the result on the screen.
 */
int main() {
    while(true){
        string equation;
        cout << "Enter your equation: ";
        cin >> equation;
        equation = toLowerCase(equation);

        VectorSHPP<string> polishRecord =  polishInvertedRecord(equation);
        double res = getResult(polishRecord);

        cout << "Result: " << res << endl;
    }
    return 0;
}

/**
 * Method: polishInvertedRecord
 * Usage: VectorSHPP<string> polishRecord = polishInvertedRecord(string equation)
 * ______________________________________________________________________________
 *
 * The method accepts a string entered by the user, and allows it puts priority
 * actions. Using an algorithm sorting station. Returns a vector of strings, where
 * string is decomposed by the algorithm reverse Polish notation.
 *
 * @param equation - expression entered by the user
 * @return - vector of strings
 */
VectorSHPP<string> polishInvertedRecord(string equation){
    StackSHPP<string> stack;
    string output("");
    VectorSHPP<string> res;
    int length = equation.length();
    for (int i = 0; i < length; ++i){
        // Checking whether an incoming character part number
        if (isNumber(equation[i]) || (equation[i] == '-' && i == 0) || (i > 0 && equation[i-1] == '(' && equation[i] == '-')){
            // check whether a character is, the latest in a number of
            if(i + 1 == length || isOperator(equation[i+1]) || equation[i+1] == '(' || equation[i+1] == ')'){
                output = output + equation[i];
                res.add(output);
                output = "";
            } else {
                output = output + equation[i];
            }
            // Check whether the incoming part of the function symbol
        } else if (equation[i] >= 'a' && equation[i] <= 'z'){
            // check whether a character is, the latest in a function
            if(equation[i+1] == '('){
                output = output + equation[i];
                stack.push(output);
                output = "";
            } else {
                output = output + equation[i];
            }
        }
        else if (equation[i] == '(') {
            stack.push("(");
        }
        else if (equation[i] == ')') {
            while (stack.peek() != "(") {
                output = output + stack.pop();
                res.add(output);
                output = "";
            }
            stack.pop();
        }
        else if (isOperator(equation[i])) {
            while (!stack.isEmpty() && (opPreced(stack.peek()[0]) >= opPreced(equation[i]))) {
                output = output + stack.pop();
                res.add(output);
                output = "";
            }
            stack.push(charToString(equation[i]));
        } else {
            throw "Error incoming data";
        }
    }
    // Output characters that are left in the stack
    int stackSize = stack.size();
    if (stackSize > 0) {
        for (int i = 0; i < stackSize - 1; ++i) {
            output = output + stack.pop();
            res.add(output);
            output = "";
        }
        output = output + stack.pop();
        res.add(output);
    }
    return res;
}

/**
 * Method: opPreced
 * Usage: int priority = opPreced(char ch)
 * __________________________________________
 *
 * Sets priority of symbols
 *
 * @param ch - symbol
 * @return - priority symbol
 */
int opPreced(char ch) {
    if(ch >= 'a' && ch <= 'z'){
        return 4;
    } else if (ch == '^'){
        return 3;
    } else if (ch == '*' || ch == '/'){
        return 2;
    } else if (ch == '+' || ch == '-'){
        return 1;
    } else {
        // in the case of an incoming character '('
        return 0;
    }

}

/**
 * Method: isNumber
 * Usage: if(isNumber(char ch))
 * ______________________________________________________
 *
 * Checks whether a character is a number and returns
 * a Boolean answer. Point is counted as part of a number
 *
 * @param ch - character
 * @return - true if character is number
 */
bool isNumber(char ch) {
    return (('0' <= ch) && (ch <= '9')) || (ch == '.');
}

/**
 * Method: isOperator
 * Usage: if(isOperator(char ch))
 * ______________________________________________________
 *
 * Checks whether a character is a operator and returns
 * a Boolean answer.
 *
 * @param ch - character
 * @return - true if character is operator
 */
bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^' || (ch >= 'a' && ch <= 'z'));
}

/**
 * Method: isFunction
 * Usage: if(isFunction(string func))
 * ______________________________________________________
 *
 * Checks whether a string is a function and returns
 * a Boolean answer.
 *
 * @param func - string
 * @return - true if string is function
 */
bool isFunction(string func) {
    return (func == "sin" || func == "cos" || func == "sqrt" || func == "tan");
}

/**
 * Method: getResult
 * Usage: double result = getResult(VectorSHPP<string> & records)
 * ____________________________________________________________
 *
 * This function takes each element of the vector values and
 * places numbers in the stack, if the value is an operator or
 * function then removed numbers from stack and made mathematical equations
 * and places back in stack. So is continued until vector will not empty and
 * in the stack remains only a single number. It will be result.
 *
 * @param records - Vector values, sorted by Polish writeback algorithm
 * @return - Result of the solution of equation
 */
double getResult(VectorSHPP<string> & records){
    StackSHPP<double> stack;

    for (int i = 0; i < records.size(); i++){
        double res;
        string element = records[i];

        if (isNumber(element[0]) || isNumber(element[1])){
            stack.push(stringToDouble(element));
        } else if (isFunction(element)){
            double operand = stack.pop();

            if(element == "sin"){
                res = sin(operand);
                stack.push(res);
            } else if (element == "cos"){
                res = cos(operand);
                stack.push(res);
            } else if (element == "sqrt"){
                res = sqrt(operand);
                stack.push(res);
            } else if (element == "tan"){
                res = tan(operand);
                stack.push(res);
            }

        } else if (isOperator(element[0])){
            double firstOperand = stack.pop();
            double secondOperand = stack.pop();
            switch (element[0]){
            case '+':
                res = secondOperand + firstOperand;
                break;
            case '-':
                res = secondOperand - firstOperand;
                break;
            case '*':
                res = secondOperand * firstOperand;
                break;
            case '/':
                res = secondOperand / firstOperand;
                break;
            case '^':
                res = pow(secondOperand, firstOperand);
                break;
            }
            stack.push(res);
        }
    }

    // in the stack is only one value - the result
    double res = stack.pop();
    return res;
}
