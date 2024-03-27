#include "logging.h"

#include <chrono>
#include <mutex>
#include <sstream>

#include "termcolor.h"

namespace logging
{

bool gSilentLog = false;
LogLevel gMinLogLevel = LogLevel::Debugging;

std::string LevelToString(LogLevel logLevel)
{
  std::stringstream strStream;
  strStream << termcolor::colorize;
  switch (logLevel)
  {
    case LogLevel::Error:
      strStream << termcolor::red << "E" << termcolor::reset;
      break;
    case LogLevel::Warning:
      strStream << termcolor::yellow << "W" << termcolor::reset;
      break;
    case LogLevel::Info:
      strStream << termcolor::green << "I" << termcolor::reset;
      break;
    case LogLevel::Debugging:
      strStream << termcolor::cyan << "D" << termcolor::reset;
      break;
    case LogLevel::Trace:
      strStream << "T";
      break;
    default:
      strStream << termcolor::magenta << "U" << termcolor::reset;
      break;
  }
  return strStream.str();
}

std::string TimeToString(struct timespec ts)
{
  const struct tm* time = localtime(&ts.tv_sec);
  return Format("%02d/%02d/%04d %2d:%02d:%02d.%06ld", time->tm_mday, time->tm_mon + 1, time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, ts.tv_nsec);
}

void Print(struct timespec ts, LogLevel level, const std::string& filename, const uint32_t& line, const std::string& message)
{
  const std::string msg = Format("%s [%s] %s:%u: %s\n", TimeToString(ts).c_str(), LevelToString(level).c_str(), filename.c_str(), line, message.c_str());
  if (level == LogLevel::Error)
  {
    fprintf(stderr, "%s", msg.data());
    fflush(stderr);
  }
  else
  {
    fprintf(stdout, "%s", msg.data());
    fflush(stdout);
  }
}

void Log(LogLevel level, const std::string& filename, const uint32_t& line, const std::string& message)
{
  static std::mutex mPrintfMutex;
  if (level > gMinLogLevel || gSilentLog)
    return;

  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  // Get filename minus the extension
  const uint32_t idx = filename.find_last_of("/\\") + 1;
  const uint32_t size = filename.size() - filename.find_last_of(".") + 1;
  const std::string shortFilename = filename.substr(idx, size);

  std::lock_guard<std::mutex> lock(mPrintfMutex);
  Print(ts, level, shortFilename, line, message);
}

}
