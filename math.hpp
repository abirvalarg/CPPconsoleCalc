/**
 * license: GNU GPLv3
 * See file 'LICENSE' for full text
 */

#pragma once

#include <map>
#include <cstring>

class Math  // Интерпретатор
{
public:
    // Коды ошибок
    enum ERRORS { ERR_BAL, ERR_END, ERR_OP, ERR_OVER, ERR_UNKNOWN };

    // Клас для сравнения имён переменных
    // Используется в стандартном классе std::map
    class compcl
    {
    public:
        bool operator()(const char *a, const char *b) const
        { return strcmp(a, b) < 0; }
    };

private:
    // Типы токенов
    enum { NONE=0, NUM, OP, VAR };

    // Массив строк с описаниеь ошибок
    static const char *err_msgs[];
    
    char token[80];     // Текст токена
    char tok_type;      // Тип токена(описаны выше)
    const char *exp;    // Указатель на строку с выражением
    
    // Для переменных
    // тут нужен класс 'compcl'
    std::map<const char*, double, compcl> vars;
    char lastVar[80];   // Имя последней переменной

    bool _was_error :1; // Флаг наличия ошибки (1 бит)
    int  _error_code:7; // Код ошибки (7 бит)

    void next_token();  // Обработка следующего токена

    // Функции для обработки операторов
    void eval_exp5(double &result); // =
    void eval_exp4(double &result); // + -
    void eval_exp3(double &result); // * / %
    void eval_exp2(double &result); // + - Унарные
    void eval_exp1(double &result); // ^ !
    void eval_exp0(double &result); // ( )
    void atom(double &result);      // Получает число или переменную
public:
    double operator()(const char *exp); // Эта функция запускает обработку

    // Следующие 2 функции работают быстрее,
    // т.к. их код подставляется вместо вызова
    bool was_error() { return _was_error; } // Возвращает флаг ошибки
                                            // Возвращает код ошибки
    const char *error_message() { return err_msgs[_error_code]; }

    struct itr_range    // Костыль
    {
        std::map<const char*, double, compcl>::iterator begin;
        std::map<const char*, double, compcl>::iterator end;
    };

    itr_range list_vars()   // Тоже костыль
    {
        return {
            vars.begin(),
            vars.end()
        };
    }
};
