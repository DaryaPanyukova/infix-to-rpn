#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm> 
#include <map> 
#include <cmath> 

int index = 0;

std::map <std::string, int> operations = { {"^", 1 }, {"n", 1}, {"s", 1}, {"*", 3},
                                    {"/", 3}, {"+", 4}, {"-", 4}, {"_", 1} };
std::vector <char> possiible_symbols = {'i', 'n', '-', '*', 'o','s', 'c', '/', '+', '^', '(', ')' };
std::string str;

std::string Read() {
    std::string num = "";
    while (index < str.size()) {
        if (str[index] < '0' || str[index] > '9') {
            return num;
        }
        num += str[index];
        ++index;
    }
    return num;
}

double Calculate(std::stack <std::string>& line) {
    std::stack <double> result;
    while (!line.empty()) {
        std::string tmp = line.top();
        line.pop();
        if (operations.count(tmp) == 1) {
            if (tmp == "_" || tmp == "n" || tmp == "s") {
                double number = result.top();
                result.pop();
                if (tmp == "_") {
                    result.push(-number);
                } else if (tmp == "n") {
                    result.push(sin(number));
                    line.pop();
                } else if (tmp == "s") {
                    result.push(cos(number));
                } 
            } else {
                double number1 = result.top();
                result.pop();
                double number2 = result.top();
                result.pop();
                if (tmp == "+") {
                    result.push(number1 + number2);
                } else if (tmp == "-") {
                    result.push(number2 - number1);
                } else if (tmp == "*") {
                    result.push(number1 * number2);
                } else if (tmp == "/") {
                    result.push(number2 / number1);
                } else if (tmp == "^") {
                    result.push(pow(number2, number1));
                }
            }
        } else {
            result.push(atoi(tmp.c_str()));
        }
    }
    return result.top();
}


void PrintPoliz(std::stack <std::string>& result) {
    std::stack <std::string> tmp;
    std::stack <std::string> tmp2;
    while (!result.empty()) {
        tmp.push(result.top());
        result.pop();
    }
    tmp2 = tmp;
    while (!tmp.empty()) { 
        if (tmp.top() == "n") {
            std::cout << "sin" << ' ';
            tmp.pop();
            tmp.pop();
        } else if (tmp.top() == "s") {
            std::cout << "cos" << ' ';
            tmp.pop();
        } else {
            std::cout << tmp.top() << ' ';
            tmp.pop();
        }
    }
    std::cout << '\n';
    std::cout << Calculate(tmp2);
}

bool DoSomethingWithBraces(std::string& element, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {
    if (element[0] == '(') { // 2
        buffer.push(element);
        return false;
    } else if (element[0] == ')') { // 4
        while (!buffer.empty() && buffer.top() != "(") {
            std::string tmp = buffer.top();
            buffer.pop();
            result.push(tmp);
        }
        if (!buffer.empty() && buffer.top() == "(") {
            buffer.pop();
            return false;
        } else {
            return true;
        }
    }
}

void Punkt3a(std::string& operation, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {
    while (!buffer.empty() && buffer.top() != "(" &&
        operations.count(buffer.top()) != 0 &&
        operations[buffer.top()] <= operations[operation]) {
        result.push(buffer.top());
        buffer.pop();
    }
    buffer.push(operation);
}

void Punkt3b(std::string& operation, std::stack <std::string>& buffer,
    std::stack <std::string>& result) {
    std::string x = buffer.top();
    if (operation == "^" || operation == "n"
        || operation == "s" || operation == "_") {
        if (buffer.top() == "^" || buffer.top() == "n"
            || buffer.top() == "s" || buffer.top() == "_") {
            buffer.push(operation);
        }
    } else {
        while (!buffer.empty() &&
            operations[buffer.top()] == operations[operation]) {
            result.push(buffer.top());
            buffer.pop();
        }
        buffer.push(operation);
    }

}

void DoSomethingWithOperation(std::string& operation,
    std::stack <std::string>& buffer,
    std::stack <std::string>& result) {
    if (operations[buffer.top()] < operations[operation]) {
        Punkt3a(operation, buffer, result);
    } else
        Punkt3b(operation, buffer, result);
}

/*унарный после (, операции*/
/*бинарный после ), после числа*/
bool CheckString() {
    for (int i = 1; i < str.size(); ++i) {
        if (find(possiible_symbols.begin(),
            possiible_symbols.end(), str[i]) == possiible_symbols.end()) {
            if (str[i] > '9' || str[i] < '0') {
                std::cout << "Unknown symbol '" << str[i];
                return false;
            }
        }
        if (str[i] == '-') {
            if (str[i - 1] == '(' || str[i - 1] == '+'
                || str[i - 1] == 'n' || str[i - 1] == '/' || str[i - 1] == '-'
                || str[i - 1] == '_' || str[i - 1] == 's' || str[i - 1] == '^'
                || str[i - 1] == '*') {
                str[i] = '_';
            }
        }
        if (str[i] == '(' || (str[i] >= '0' && str[i] <= '9')
            || str[i] == 's' || str[i] == '_' || str[i] == 'c') {
            if (str[i - 1] != '(' && !(str[i - 1] == '(' || str[i - 1] == '+'
                || str[i - 1] == 'n' || str[i - 1] == '/' || str[i - 1] == '-'
                || str[i - 1] == '_' || str[i - 1] == 's' || str[i - 1] == '^'
                || str[i - 1] == 'o' || str[i - 1] == '*') && (str[i-1] < '0' || str[i-1] > '9')) {
                std::cout << "Error";
                return false;
            }
        }
        if (str[i] == '+' || str[i] == '/' || str[i] == '-'
            || str[i] == '^' || str[i] == '*' || str[i] == ')') {
            if ((str[i-1] < '0' || str[i-1] > '9' ) && str[i-1] != ')') {
                std::cout << "Error";
                return false;
            }
        }
    }
    return true;
}

int main()
{
    for (;;) {
        index = 0;
        std::stack <std::string> result;
        std::stack <std::string> buffer;
        bool error = false;
        getline(std::cin, str);
        str = "(" + str + ")";
        if (CheckString()) {
            while (index < str.size()) {
                std::string element = Read();
                if (element == "") {
                    element = str[index];
                    ++index;
                    if (element[0] == '(' || element[0] == ')') {
                        if (DoSomethingWithBraces(element, buffer, result)) {
                            error = true;
                            std::cout << "Error";
                        }
                    } else if (operations.count(element) != 0 && !error) {
                        DoSomethingWithOperation(element, buffer, result);
                    }
                } else {
                    result.push(element);
                }
            }
            if (!error) {
                if (buffer.empty()) {
                    PrintPoliz(result);
                } else {
                    std::cout << "Error";
                }
            }
        }
        std::cout << '\n' << '\n';
    }
}

