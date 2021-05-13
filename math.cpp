/**
 * license: GNU GPLv3
 * See file 'LICENSE' for full text
 */

#include "math.hpp"

#include <cctype>
#include <cstdlib>
#include <cmath>

using namespace std;

// Тексты ошибок
const char *Math::err_msgs[5] = {
    "Broken balance brackets",
    "Unexpected end of expression",
    "Unexpected operator",
    "Overflow",
    "Unknown symbol"
};

// Пропускает пробелы
void skipSpaces(const char *&str)
{
    while(isspace(*str))    // Пока силвол - пробел...
        str++;              // Перемещаем указатель вперёд
}

// Если ch - оператор, возвращает true(!= 0), иначе false(0)
int isoperator(int ch)
{
    return (bool)strchr("()!^*/%+-=", ch);
}

// Вычисляет факториал числа
unsigned long long fact(long long a)
{
    if(a < 0)
        throw Math::ERR_OVER;   // Выброс ошибки

    return a > 1 ? fact(a - 1) * a : 1;
}

void Math::next_token()
{
    /**
     * Пропуск пробелов
     * 
     * Знак '&' означает, что передаётся ссылка на переменную.
     * Это значит, что функция будет использовать её как обычную переменную,
     * но при её изменении, меняется и значение переменной, откуда взялось начение.
     * Это значит, что после вызова этой функции, значение переменной 'exp' изменится,
     * если она указывает на пробел(условие функции)
     */
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
        tok_type = NUM;     // Тип токена - число

        int i = 0;
        //Копирование числа
        while(isdigit(exp[0]) || exp[0] == '.')
        {   // Берёт символ, на который указыват 'exp' и перемещает указатель вперёд
            token[i] = *exp++;
            i++;
        }
        token[i] = 0;
    }
    else if(isalpha(exp[0]))    //Переменная
    {
        tok_type = VAR;     // Тип токена - переменная

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
        *token = *exp++;    // Все операторы имеют длинну 1 символ
        tok_type = OP;
    }
    else
        throw ERR_UNKNOWN;  // Неизвестный символ
}

double Math::operator()(const char *exp)
{
    this->exp = exp;    // Копирование указателя
    double result = 0;  // предустановка результата
    this->_was_error = false;   // Обнуление флага ошибки

    try // Исключения позволяют легко выбрасывать ошибки из любого уровня вызова
    {
        eval_exp5(result);  // Обработка переменных (=)
    }
    catch(ERRORS ec)    // Перехватываем исключение в виде числа
    {
        _was_error = true;
        _error_code = ec;
        result = 0;
    }
    return result;  // Возврат результата
}

void Math::eval_exp5(double &result)    // =
{
    next_token();   // Следующий токен
    eval_exp4(result);  // обработка сложения (+-)

    if(tok_type == OP &&    // Если след. оператор - '='
        token[0] == '=')
    {
        // Копирование имяни переменной
        char *tmp = new char[strlen(lastVar) + 1];  // FIXME Возможно, тут утечка памяти
        strcpy(tmp, lastVar);

        // Получение значения
        next_token();
        eval_exp4(result);

        // Запись результата
        if(result)
            vars[tmp] = result;
        else
            vars.erase(tmp);    // Если 0, стераем переменную, что бы не мешалась
    }
}

void Math::eval_exp4(double &result)    // + -
{
    eval_exp3(result);  // Обработка умножения (*/%)

    char op = token[0]; // Копирование оператора
    while(tok_type == OP &&
        (op == '+' ||
        op == '-'))     // Если оператор - '+' или '-'
    {
        // Второе слагаемое
        next_token();
        double tmp = 0;
        eval_exp3(tmp);

        switch(op)
        {
        case '+':   // Сложение
            result += tmp;
            break;
        case '-':   // Вычитание
            result -= tmp;
            break;
        }
        op = token[0];  // Следующий токен, для выражени типа 'a + b + c'
    }
}

void Math::eval_exp3(double &result)    // * / %
{
    eval_exp2(result);  // Обработка унарных операторов (+-) для выражени типа '-a' или '+a'

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
        case '*':   // Умножение
            result *= tmp;
            break;
        case '/':   // Деление
            result /= tmp;
            break;
        case '%':   // Остаток от деление
            result = (long long)floor(result) % (long long)floor(tmp);
            break;
        }
        op = token[0];
    }
}

void Math::eval_exp2(double &result)    // + - Унарные
{
    char op = '+';  // Оператор по умолчанию, если не указан

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
    case '+':   // Положительное число
        break;
    case '-':   // Отрицателньое
        result = -result;
        break;
    }
}

void Math::eval_exp1(double &result)    // ^ !
{
    eval_exp0(result);  // Обработка скобок

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
        case '^':   // Возведение в степень
            eval_exp1(tmp);
            result = pow(result, tmp);
            break;
        case '!':   // Факториал
            try // Возможно переполнение стэка
            {
                result = fact( abs(floor(result)) );    // = |_result_|!
            }
            catch(...)  // Перехват всех исключений
            {
                throw ERR_OVER; // Выюрос ошибки
            }
            break;
        }
        op = token[0];
    }
}

void Math::eval_exp0(double &result)    // ( )
{
    if(tok_type == OP && *token == '(') // Если есть открывающаяся скобка
    {
        eval_exp5(result);      // Обработка выражения в скобках
        if(!(tok_type == OP &&  // Проверка, что скобки закравются
            token[0] == ')'))
            throw ERR_BAL;      // Если нет, выбрасываем ошибку
        else
            next_token();
    }
    else
        atom(result);   // Если нет скобок, обрабатываем числа/переменные
}

void Math::atom(double &result)
{
    // Проверка, что дальше число или переменная
    if(tok_type == OP)
        throw ERR_OP;
    if(tok_type == NONE)
        throw ERR_END;
    
    if(tok_type == VAR)
    {
        // Копирование переменной
        strcpy(lastVar, token);

        try
        {
            result = vars.at(token);
        }
        catch(...)  // Если такой нет, то берём 0
        {
            result = 0;
        }
        
    }
    else
    {
        result = atof(token);   // Делаем число из строки
    }
    next_token();
}
