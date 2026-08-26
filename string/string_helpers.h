#pragma once

#include <stdint.h>

#include <format>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <tuple>
#include <utility>
#ifdef LOGGING_USE_QT
#include <QString>
#include <QStringView>
#endif
#endif

#define WHITESPACE_CHARS " \t\n\r\f\v"

void ToUpperCase(std::string& str, uint32_t startPos = 0);
void ToLowerCase(std::string& str, uint32_t startPos = 0);

std::string ToUpperCase(const std::string& str, uint32_t startPos, uint32_t endPos);
std::string ToLowerCase(const std::string& str, uint32_t startPos, uint32_t endPos);

std::string Trim(std::string s, const char* t = WHITESPACE_CHARS);
std::string LTrim(std::string s, const char* t = WHITESPACE_CHARS);
std::string RTrim(std::string s, const char* t = WHITESPACE_CHARS);

std::vector<std::string> Split(const std::string& str, char c);
std::vector<std::string> Keys(const std::map<std::string, uint32_t>& v);
std::map<std::string, std::string> Split(const std::string& str, char c1, char c2);

std::string VectorToString(const std::vector<uint32_t>& values);

inline std::string Format(std::string_view fmt)
{
  return std::string(fmt);
}

// Since templates are instantiated where they are used, the compiler needs
// the full definition in each translation unit.
#ifdef __EMSCRIPTEN__

#ifdef LOGGING_USE_QT

inline std::string FormatValue(const QString& value)
{
  return value.toStdString();
}

inline std::string FormatValue(QStringView value)
{
  return value.toString().toStdString();
}

template <typename T>
  requires(!std::same_as<std::remove_cvref_t<T>, QString> &&
           !std::same_as<std::remove_cvref_t<T>, QStringView>)
decltype(auto) FormatValue(T&& value)
{
  return std::forward<T>(value);
}

#else  // LOGGING_USE_QT

template <typename T>
decltype(auto) FormatValue(T&& value)
{
  return std::forward<T>(value);
}

#endif  // LOGGING_USE_QT

template <typename... Args>
  requires(sizeof...(Args) > 0)
std::string Format(std::string_view fmt, Args&&... args)
{
  auto converted = std::tuple{
      FormatValue(std::forward<Args>(args))...};

  return std::apply(
      [&](auto&... values) {
        return std::vformat(fmt, std::make_format_args(values...));
      },
      converted);
}

#else  // __EMSCRIPTEN__

template <typename... Args>
  requires(sizeof...(Args) > 0)
std::string Format(std::string_view fmt, Args&&... args)
{
  return std::vformat(fmt, std::make_format_args(args...));
}

#endif
