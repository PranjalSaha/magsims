#include "DateTime.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

DateTime::DateTime(){
    tp = parseDateTime("01 Jan 2000 00:00:00.000");
}

DateTime::DateTime(const std::string &time_str){
    tp = parseDateTime(time_str);
}

std::chrono::system_clock::time_point
DateTime::parseDateTime(const std::string &time_str){
    std::tm tm = {};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, "%d %b %Y %H:%M:%S");
    time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

std::string DateTime::toString() const{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    char buffer[100];
    std::strftime(
        buffer, 
        sizeof(buffer), 
        "%d %b %Y %H:%M:%S", 
        std::localtime(&t)
    );
    return buffer;
}

bool DateTime::operator<(const DateTime &other) const{
    return tp < other.tp;
}

bool DateTime::operator>(const DateTime &other) const{
    return tp > other.tp;
}

bool DateTime::operator==(const DateTime &other) const{
    return tp == other.tp;
}

double DateTime::operator-(const DateTime &other) const{
    return std::chrono::duration<double>(tp - other.tp).count();
}

DateTime DateTime::operator+(double seconds) const
{
    using namespace std::chrono;
    DateTime result = *this;
    result.tp += duration_cast<system_clock::duration>(
        duration<double>(seconds)
    );
    return result;
}

std::string DateTime::display() const{
    // Convert time_point to time_t
    std::time_t time_t_format = std::chrono::system_clock::to_time_t(tp);

    // Extract fractional seconds
    auto duration_since_epoch = tp.time_since_epoch();
    auto milliseconds = 
        std::chrono::duration_cast<std::chrono::milliseconds>(
            duration_since_epoch
        ) % 1000;

    // Convert to tm (broken-down time)
    std::tm tm_struct = *std::localtime(&time_t_format);

    // Month names
    static const char* months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << tm_struct.tm_mday << " "
        << months[tm_struct.tm_mon] << " "
        << (tm_struct.tm_year + 1900) << " "
        << std::setw(2) << tm_struct.tm_hour << ":"
        << std::setw(2) << tm_struct.tm_min << ":"
        << std::setw(2) << tm_struct.tm_sec << "."
        << std::setw(3) << milliseconds.count();

    return oss.str();
}
