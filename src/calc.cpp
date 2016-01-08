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
 */

// function prototypes
VectorSHPP<string> polishInvertedRecord(string equation);
double getResult(VectorSHPP<string> & records);
int operatorPriority(char ch);
bool isNumber(char ch);
bool isOperator(char ch);
bool isFunction(string func);

/**
 * The main function of the program, which prompts the user for the
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
 * Function: polishInvertedRecord
 * Usage: VectorSHPP<string> polishRecord = polishInvertedRecord(string equation)
 * ______________________________________________________________________________
 *
 * Function accepts a string entered by the user, and allows it puts priority
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
    for (int i = 0; i < equation.length(); i++){
        if (isNumber(equation[i]) || (equation[i] == '-' && i == 0) || (i > 0 && equation[i-1] == '(' && equation[i] == '-')){ // Checking whether an incoming character part number
            if(i == equation.length() - 1 || !isNumber(equation[i + 1])){ // check whether a character is, the latest in a number of
                output += equation[i];
                res.add(output);
                output = "";
            } else {
                output += equation[i];
            }
        } else if (equation[i] >= 'a' && equation[i] <= 'z'){ // Check whether the incoming part of the function symbol
            if(equation[i+1] == '('){ // check whether a character is, the latest in a function
                output += equation[i];
                stack.push(output);
                output = "";
            } else {
                output += equation[i];
            }
        } else if (equation[i] == '(') {
            stack.push("(");
        } else if (equation[i] == ')') {
            while (stack.peek() != "(") {
                res.add(stack.pop());
            }
            stack.pop();
        } else if (isOperator(equation[i])) {
            while (!stack.isEmpty() && (operatorPriority(stack.peek()[0]) >= operatorPriority(equation[i]))) {
                res.add(stack.pop());
            }
            stack.push(charToString(equation[i]));
        } else {
            cout << "Error incoming data" << endl;
            break;
        }
    }
    // Takes out remaining values from the stack
    while(!stack.isEmpty()){
        res.add(stack.pop());
    }
    return res;
}

/**
 * Function: operatorPriority
 * Usage: int priority = operatorPriority(char ch)
 * __________________________________________
 *
 * Sets priority of symbols
 *
 * @param ch - symbol
 * @return - priority symbol
 */
int operatorPriority(char ch) {
    int res = 0;
    if(ch >= 'a' && ch <= 'z'){
        res = 4;
    } else if (ch == '^'){
        res = 3;
    } else if (ch == '*' || ch == '/'){
        res = 2;
    } else if (ch == '+' || ch == '-'){
        res = 1;
    }
    return res;
}

/**
 * Function: isNumber
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
    if(('0' <= ch && ch <= '9') || (ch == '.')){
        return true;
    }
    return false;
}

/**
 * Function: isOperator
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
    if((ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^' || (ch >= 'a' && ch <= 'z'))){
        return true;
    }
    return false;
}

/**
 * Function: isFunction
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
    for(int i = 0; i < func.size(); i++){
        if(func[i] < 'a' || func[i] > 'z'){
            return false;
        }
    }
    return true;
}

/**
 * Function: getResult
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
            } else if (element == "cos"){
                res = cos(operand);
            } else if (element == "sqrt"){
                res = sqrt(operand);
            } else if (element == "tan"){
                res = tan(operand);
            } else {
                cout << "Error: unknown operator" << endl;
                exit(1);
            }
            stack.push(res);

        } else if (isOperator(element[0])){
            double firstOperand = stack.pop();
            double secondOperand = stack.pop();
            if(element[0] == '+'){
                res = secondOperand + firstOperand;
            } else if (element[0] == '-'){
                res = secondOperand - firstOperand;
            } else if (element[0] == '*'){
                res = secondOperand * firstOperand;
            } else if (element[0] == '/'){
                res = secondOperand / firstOperand;
            } else if (element[0] == '^'){
                res = pow(secondOperand, firstOperand);
            }
            stack.push(res);
        } else {
            cout << "Incorrect data entered" << endl;
            exit(1);
        }
    }
    // in the stack is only one value - the result
    return stack.pop();
}
