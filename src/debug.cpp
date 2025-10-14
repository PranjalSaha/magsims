#include "Debug.h"
#include <vector>
#include <string>
#include <sstream>
#include <mutex>

namespace
{
    std::vector<std::string> call_stack;
    std::mutex call_stack_mutex;
}

void CallStackTracker::push(const std::string &func)
{
    std::lock_guard<std::mutex> lock(call_stack_mutex);
    call_stack.push_back(func);
}

void CallStackTracker::pop()
{
    std::lock_guard<std::mutex> lock(call_stack_mutex);
    if (!call_stack.empty())
    {
        call_stack.pop_back();
    }
}

std::string CallStackTracker::trace()
{
    std::lock_guard<std::mutex> lock(call_stack_mutex);
    std::ostringstream oss;
    for (const auto &func : call_stack)
    {
        oss << func << " -> ";
    }
    oss << "[EXCEPTION]";
    return oss.str();
}

CallTracer::CallTracer(const std::string &func)
{
    CallStackTracker::push(func);
}

CallTracer::~CallTracer()
{
    CallStackTracker::pop();
}
