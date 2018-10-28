#pragma once

#include <map>
#include <cstring>

class Math
{
public:
    enum ERRORS { ERR_BAL, ERR_END, ERR_OP, ERR_OVER, ERR_UNKNOWN };

    class compcl
    {
    public:
        bool operator()(const char *a, const char *b) const
        { return strcmp(a, b) < 0; }
    };

private:
    enum { NONE=0, NUM, OP, VAR };

    static const char *err_msgs[];
    
    char token[80];
    char tok_type;
    const char *exp;
    
    std::map<const char*, double, compcl> vars;
    char lastVar[80];

    bool _was_error :1;
    int  _error_code:7;

    void next_token();

    void eval_exp5(double &result); // =
    void eval_exp4(double &result); // + -
    void eval_exp3(double &result); // * / %
    void eval_exp2(double &result); // + - Унарные
    void eval_exp1(double &result); // ^ !
    void eval_exp0(double &result); // ( )
    void atom(double &result);
public:
    double operator()(const char *exp);

    bool was_error() { return _was_error; }
    const char *error_message() { return err_msgs[_error_code]; }

    struct itr_range
    {
        std::map<const char*, double, compcl>::iterator begin;
        std::map<const char*, double, compcl>::iterator end;
    };

    itr_range list_vars()
    {
        return {
            vars.begin(),
            vars.end()
        };
    }
};
