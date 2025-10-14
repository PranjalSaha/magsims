#ifndef DEBUG_H
#define DEBUG_H

#include <string>

class CallStackTracker
{
public:
    static void push(const std::string &func);
    static void pop();
    static std::string trace();
};

class CallTracer
{
public:
    CallTracer(const std::string &func);
    ~CallTracer();
};

// Use this macro at the beginning of a function to trace it
#define TRACE_CALL CallTracer tracer(__func__)

#endif // DEBUG_H

