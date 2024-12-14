#include <iostream>
#include <stack>
#include <vector>
#include <math.h>

using namespace std;

string whitespace_delete(string&expression, string whitespace = " ", string voidness = "")
{
    while(expression.find(whitespace) != string::npos){
        unsigned long start = expression.find(whitespace);
        expression.erase(start, 1);
        expression.insert(start, voidness);
    }
    return expression;
}

int op_tor_priority(char op_tor){
    if(op_tor == '-' or op_tor == '+') return 1;
    if(op_tor == '*' or op_tor == '/') return 2;
    return 0;
}

void rPe_in_complex_func_argument(string& expression, vector<string>& rPe, size_t& count){
    stack<char> op_tor_in_func;
    int count_open_brace = 0;
    while(count < expression.length()){
        if(isdigit(expression[count]) or expression[count] == '.'){// чтение числа из строки!
            string num; // число в виде строки
            while(isdigit(expression[count]) or expression[count] == '.'){
                num = num + expression[count];
                count++;
            }
            rPe.push_back(num);// заносим операнд(число) в выходную строку
        } else if(op_tor_priority(expression[count]) > 0){ // чтение операторов из строки!
            while(!op_tor_in_func.empty() and (op_tor_priority(op_tor_in_func.top()) >= op_tor_priority(expression[count]))){
                rPe.push_back(string(1, op_tor_in_func.top()));
                op_tor_in_func.pop();
            }
            op_tor_in_func.push(expression[count]); // текущий оператор
            count++;
        } else if(expression[count] == '('){
            op_tor_in_func.push(expression[count]);
            count++;
            count_open_brace++;
        } else if(expression[count] == ')'){
            while(op_tor_in_func.top() != '('){ // до открывающейся скобки
                rPe.push_back(string(1, op_tor_in_func.top()));
                op_tor_in_func.pop(); // удаляем из стека
            }
            count_open_brace -= 1;
            op_tor_in_func.pop();
            count++;
            if(count_open_brace == 0)
            {
                break;
            }
        }
    }
}

vector<string> change_to_reverse_Polish_entry(string&expression)
{
    stack<char> op_tor;
    vector<string> rPe;
    size_t count = 0;
    while(count < expression.length()){
        if(isdigit(expression[count]) or expression[count] == '.'){ // чтение числа из строки!
            string num; // число в виде строки
            while(isdigit(expression[count]) or expression[count] == '.')
            {
                num = num + expression[count];
                count++;
            }
            rPe.push_back(num);// заносим операнд(число) в выходную строку
        } else if(op_tor_priority(expression[count]) > 0){ // чтение операторов из строки!
            while(!op_tor.empty() and (op_tor_priority(op_tor.top()) >= op_tor_priority(expression[count]))){ // пока операторы стека большего приоритета
                rPe.push_back(string(1, op_tor.top()));
                op_tor.pop(); // удаляем их из стека
            }
            op_tor.push(expression[count]); // заносим текущий оператор
            count++;
        } else if(expression[count] == '('){
            op_tor.push(expression[count]);
            count++;
        } else if(expression[count] == ')'){
            while(op_tor.top() != '('){
                rPe.push_back(string(1, op_tor.top()));
                op_tor.pop();
            }
            op_tor.pop(); // удаляем открывающуюся скобку
            count++;
        } else if(expression.substr(count, 3) == "sin")
        {
            count += 3;
            rPe_in_complex_func_argument(expression, rPe, count);
            rPe.push_back("sin");
        } else if(expression.substr(count, 3) == "cos")
        {
            count += 3;
            rPe_in_complex_func_argument(expression, rPe, count);
            rPe.push_back("cos");
        } else if(expression.substr(count, 2) == "tg")
        {
            count += 2;
            rPe_in_complex_func_argument(expression, rPe, count);
            rPe.push_back("tg");
        } else if(expression.substr(count, 3) == "ctg")
        {
            count += 3;
            rPe_in_complex_func_argument(expression, rPe, count);
            rPe.push_back("ctg");
        } else if(expression.substr(count, 3) == "exp")
        {
            count += 3;
            rPe_in_complex_func_argument(expression, rPe, count);
            rPe.push_back("exp");
        } else count++; // если ничего из вышеперечисленного
    }
    
    while(!op_tor.empty()){
        rPe.push_back(string(1, op_tor.top()));
        op_tor.pop();
    }
    return rPe;
}

double rpe_calculation(vector<string>& rPe)
{
    stack<double> op_und;
    for(int cur = 0; cur < rPe.size(); ++cur)
    {
        if(isdigit(rPe[cur][0]))
        {
            op_und.push(stod(rPe[cur]));
        } else if(rPe[cur] == "+")
        {
            double second = op_und.top(); op_und.pop();
            double first = op_und.top(); op_und.pop();
            op_und.push(first + second);
        } else if(rPe[cur] == "-")
        {
            double second = op_und.top(); op_und.pop();
            double first = op_und.top(); op_und.pop();
            op_und.push(first - second);
        } else if(rPe[cur] == "*")
        {
            double second = op_und.top(); op_und.pop();
            double first = op_und.top(); op_und.pop();
            op_und.push(first * second);
        } else if(rPe[cur] == "/")
        {
            double second = op_und.top(); op_und.pop();
            double first = op_und.top(); op_und.pop();
            if (second != 0) {op_und.push(first / second);}
            else {
                cerr << "Ошибка деления на ноль" << endl;
                exit(1);
            }
        } else if(rPe[cur] == "sin"){
            double arg = op_und.top();
            op_und.pop();
            op_und.push(sin(arg));
        } else if(rPe[cur] == "cos"){
            double arg = op_und.top();
            op_und.pop();
            op_und.push(cos(arg));
        } else if(rPe[cur] == "tg"){
            double arg = op_und.top();
            op_und.pop();
            op_und.push(tan(arg));
        } else if(rPe[cur] == "ctg"){
            double arg = op_und.top();
            op_und.pop();
            op_und.push(1/tan(arg));
        } else if(rPe[cur] == "exp"){
            double arg = op_und.top();
            op_und.pop();
            op_und.push(exp(arg));
        }
    }
    return op_und.top();
}

int main()
{
    string expression;
    getline(cin, expression);
    expression = whitespace_delete(expression);
    vector<string> rPe = change_to_reverse_Polish_entry(expression);
    cout << rpe_calculation(rPe) << endl;
}
