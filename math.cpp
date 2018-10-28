#include "math.hpp"

#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

const char *Math::err_msgs[5] = {
    "Broken balance brackets",
    "Unexpected end of expression",
    "Unexpected operator",
    "Overflow",
    "Unknown symbol"
};

void skipSpaces(const char *&str)
{
    while(isspace(*str))
        str++;
}

int isoperator(int ch)
{
    return (bool)strchr("()!^*/%+-=", ch);
}

unsigned long long fact(long long a)
{
    if(a < 0)
        throw Math::ERR_OVER;

    return a > 1 ? fact(a - 1) * a : 1;
}

void Math::next_token()
{
    skipSpaces(exp);
    //Проверка на конец выражения
    if(exp[0] == 0)
    {
        tok_type = NONE;
        return;
    }

    //Проверка типа
    if(isdigit(exp[0]))     //Число
    {
        tok_type = NUM;

        int i = 0;
        //Копирование числа
        while(isdigit(exp[0]) || exp[0] == '.')
        {
            token[i] = *exp++;
            i++;
        }
        token[i] = 0;
    }
    else if(isalpha(exp[0]))    //Переменная
    {
        tok_type = VAR;

        int i = 0;
        //Копирование переменной
        while(isalpha(exp[0]))
        {
            token[i] = *exp++;
            i++;
        }
        token[i] = 0;
    }
    else if(isoperator(exp[0])) //Оператор
    {
        *token = *exp++;
        tok_type = OP;
    }
    else
        throw ERR_UNKNOWN;
}

double Math::operator()(const char *exp)
{
    this->exp = exp;
    double result = 0;
    this->_was_error = false;

    try
    {
        eval_exp5(result);
    }
    catch(ERRORS ec)
    {
        _was_error = true;
        _error_code = ec;
        result = 0;
    }
    return result;
}

void Math::eval_exp5(double &result)    // =
{
    next_token();
    eval_exp4(result);

    if(tok_type == OP &&
        token[0] == '=')
    {
        char *tmp = new char[strlen(lastVar) + 1];
        strcpy(tmp, lastVar);
        next_token();
        eval_exp4(result);
        if(result)
            vars[tmp] = result;
        else
            vars.erase(tmp);
    }
}

void Math::eval_exp4(double &result)    // + -
{
    eval_exp3(result);

    char op = token[0];
    while(tok_type == OP &&
        (op == '+' ||
        op == '-'))
    {
        next_token();
        double tmp = 0;
        eval_exp3(tmp);
        switch(op)
        {
        case '+':
            result += tmp;
            break;
        case '-':
            result -= tmp;
            break;
        }
        op = token[0];
    }
}

void Math::eval_exp3(double &result)    // * / %
{
    eval_exp2(result);

    char op = token[0];
    while(tok_type == OP &&
        (op == '*' ||
        op == '/' ||
        op == '%'))
    {
        next_token();
        double tmp = 0;
        eval_exp2(tmp);
        switch(op)
        {
        case '*':
            result *= tmp;
            break;
        case '/':
            result /= tmp;
            break;
        case '%':
            result = (long long)floor(result) % (long long)floor(tmp);
            break;
        }
        op = token[0];
    }
}

void Math::eval_exp2(double &result)    // + - Унарные
{
    char op = '+';

    if(tok_type == OP
        && (token[0] == '+'
        || token[0] == '-'))    //Есть подходящий оператор
    {
        op = token[0];
        next_token();
    }
    
    eval_exp1(result);

    switch(op)
    {
    case '+':
        break;
    case '-':
        result = -result;
        break;
    }
}

void Math::eval_exp1(double &result)    // ^ !
{
    eval_exp0(result);

    char op = token[0];
    while(tok_type == OP &&
        (op == '^'
        || op == '!')
        )
    {
        next_token();
        double tmp = 0;
        switch(op)
        {
        case '^':
            eval_exp1(tmp);
            result = pow(result, tmp);
            break;
        case '!':
            try
            {
                result = fact( abs(floor(result)) );    // = |_result_|!
            }
            catch(...)
            {
                throw ERR_OVER;
            }
            break;
        }
        op = token[0];
    }
}

void Math::eval_exp0(double &result)    // ( )
{
    if(tok_type == OP && *token == '(')
    {
        eval_exp5(result);
        if(!(tok_type == OP &&
            token[0] == ')'))
            throw ERR_BAL;
        else
            next_token();
    }
    else
        atom(result);
}

void Math::atom(double &result)
{
    if(tok_type == OP)
        throw ERR_OP;
    if(tok_type == NONE)
        throw ERR_END;
    
    if(tok_type == VAR)
    {
        strcpy(lastVar, token);

        try
        {
            result = vars.at(token);
        }
        catch(...)
        {
            result = 0;
        }
        
    }
    else
    {
        result = atof(token);
    }
    next_token();
}
