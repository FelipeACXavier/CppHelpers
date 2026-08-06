#include "logging.h"

#include <mutex>
#include <sstream>

#include "termcolor.h"

namespace logging
{

bool gSilentLog = false;
LogLevel gMinLogLevel = LogLevel::Debugging;

#ifdef LOGGING_USE_SOURCE
std::string gSourceName = "";
std::function<void(std::chrono::system_clock::time_point ts, logging::LogLevel level, const std::string& source, const std::string& filename, const uint32_t& line, const std::string& message)> gLogToStream = nullptr;
#else
std::function<void(std::chrono::system_clock::time_point ts, logging::LogLevel level, const std::string& filename, const uint32_t& line, const std::string& message)> gLogToStream = nullptr;
#endif

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

std::tm ToLocalTm(std::time_t now)
{
  std::tm tm{};
#if defined(_WIN32)
  localtime_s(&tm, &now);
#else
  localtime_r(&now, &tm);
#endif
  return tm;
}

std::string TimeToString(std::chrono::system_clock::time_point now)
{
  auto secs = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now - secs).count();
  auto t = std::chrono::system_clock::to_time_t(secs);
  std::tm tm = ToLocalTm(t);

  return Format(
      "{:02}/{:02}/{:04} {:02}:{:02}:{:02}.{:09}",
      tm.tm_mday, tm.tm_mon + 1,
      tm.tm_year + 1900, tm.tm_hour, tm.tm_min,
      tm.tm_sec, static_cast<long long>(micros));
}

void Print(std::chrono::system_clock::time_point now, LogLevel level, const std::string& filename,
           const uint32_t& line, const std::string& message)
{
  const std::string msg = Format("{} [{}] {}:{}: {}\n", TimeToString(now), LevelToString(level), filename, line, message);
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
  if (level > gMinLogLevel)
    return;

  auto now = std::chrono::system_clock::now();

  // Get filename minus the extension
  const uint32_t idx = filename.find_last_of('/') + 1;
  const uint32_t size = filename.find_last_of('.') - idx;
  const std::string shortFilename = filename.substr(idx, size);

#ifdef LOGGING_USE_SOURCE
  if (gLogToStream)
    gLogToStream(now, level, gSourceName, shortFilename, line, message);
#else
  if (gLogToStream)
    gLogToStream(now, level, shortFilename, line, message);
#endif

  if (!gSilentLog)
  {
    std::lock_guard<std::mutex> lock(mPrintfMutex);
    Print(now, level, shortFilename, line, message);
  }
}

}  // namespace logging
