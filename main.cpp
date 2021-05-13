/**
 * license: GNU GPLv3
 * See file 'LICENSE' for full text
 */

#include <iostream>
#include <string>
#include "math.hpp"

using namespace std;

string input;           // Буффер для ввода
Math math;              // Интерпретатор
double n;               // Результат
Math::itr_range rng;    // Это была плохая идея. Уберу потом
std::map<const char*, double, Math::compcl>::iterator itr;  // Тоже костыль

int main()
{
    // Вывод краткой информации
    cout << "Console calculator.";
    cout<< "\r\nWorking under GNU GPLv3 license. There is NO WARANTY of absolutly stable working.\r\n";
    cout<< "See file 'LICENSE' in github repositoryto read full text of license";
    cout<< "\r\nType expression and press Enter.\r\nType '.' to exit\r\n\
Type '?' to list all variables\r\n>";

    while(true) // Основной цикл
    {
        getline(cin, input);    // Ввод текста в буфер(1 строка)
        if(input[0] == '.')     // Если введена точка, выходим из цикла
            break;
        else if(input[0] == '?')// Если знак вопроса, выводим все переменные
        {
            rng = math.list_vars(); // Костыль. Будет удалён
            for(itr = rng.begin; itr != rng.end; itr++)
                cout << itr->first << "=" << itr->second << endl;
            cout << ">";    // Приглашение ко вводу
        }
        else
        {
            n = math(input.c_str());    // Вычисляем результат. функция 'operator()' в классе
            if(!n && math.was_error())  // Если результат = 0, проверяем ошибки
                cout << "Error while counting:\r\n\t" << math.error_message();  // Выводим текст ошибки
            else
            {   // Если нет ошибок, выводим результат
                cout << n;
            }
            cout << "\r\n>";    // Приглашение ко вводу
        }
    }
    // Завершение выз ошибок
    return 0;
}
