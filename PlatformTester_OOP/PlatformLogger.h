#ifndef PLATFORM_LOGGER_H_
#define PLATFORM_LOGGER_H_

#if defined(QT_PLATFORM)
#include <QtGlobal>
#endif

#if defined(QT_VERSION)
#include <QTextEdit>
#else 
    #include <iostream>
    #include <stdio.h>
    #include <stdarg.h>
#endif

#include <string>

namespace geeds {
namespace gdpd_gp {

enum class LogType
{
    Error,
    Warning,
    Info
};


class Logger{
public:
    Logger(){}
    ~Logger() {}
    #if defined(QT_VERSION)
    Logger(QTextEdit* logObject):_loggerObject(logObject){ }
#endif
    void log (LogType _logType, const char* format...) const;

private:
    void _log_qt(LogType _logType, const std::string& _msg) const;
    void _log_console(LogType _logType, const char* format...) const;
#if defined(QT_VERSION)
    QTextEdit* _loggerObject;
#endif
};

}  // namespace gdpd_gp
}  // namespace geeds

#endif
