#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds)
{
    constexpr long secondsPerMinute = 60;
    constexpr long secondsPerHour = secondsPerMinute * 60;
    int numHours = seconds / secondsPerHour;
    seconds -= numHours * secondsPerHour;
    int numMinutes = seconds / secondsPerMinute;
    seconds -= numMinutes * secondsPerMinute;
    // format the output
    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << numHours << ":" << std::setw(2) << numMinutes << ":"
               << std::setw(2) << seconds;
    return timeStream.str();
}
