#include <iostream>
#include <string>
#include <windows.h>
#include "math.hpp"

using namespace std;

string input;
Math math;
double n;
Math::itr_range rng;
std::map<const char*, double, Math::compcl>::iterator itr;

int main()
{
    cout << "Console calculator."
	#ifdef DEBUG
	<< "(PID: " << getpid() << ")"
	#endif
    << "\r\nType expression and press Enter.\r\nType '.' to exit\r\n\
Type '?' to list all variables\r\n>";
    while(true)
    {
        getline(cin, input);
        if(input[0] == '.')
            break;
        else if(input[0] == '?')
        {
            rng = math.list_vars();
            for(itr = rng.begin; itr != rng.end; itr++)
                cout << itr->first << "=" << itr->second << endl;
            cout << ">";
        }
        else
        {
            n = math(input.c_str());
            if(!n && math.was_error())
                cout << "Error while counting:\r\n\t" << math.error_message();
            else
            {
                cout << n;
            }
            cout << "\r\n>";
        }
    }

    return 0;
}
