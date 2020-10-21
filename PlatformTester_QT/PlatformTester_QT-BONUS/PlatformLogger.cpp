#include "PlatformLogger.h"

namespace geeds {
namespace gdpd_gp {

void Logger::log(LogType _logType, const char* format...) const{
#if defined(QT_VERSION)
    char strBuffer[1024];
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(strBuffer, sizeof strBuffer, format, argptr);
    va_end(argptr);

    std::string formatedStr(strBuffer);
    _log_qt(_logType, formatedStr);
#else 
    //_log_console(_logType, format); //segmentation fault
    va_list argptr;
    switch (_logType) {
    case LogType::Warning:
    case LogType::Info:
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        fprintf(stdout, "\n");
        break;
    case LogType::Error:
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
        fprintf(stderr, "\n");
        break;
    default:
        break;
    }
#endif
}

//TODO: make ptr for QtObject, and make dynamic cast for the passed logger
void Logger::_log_qt(LogType _logType, const std::string& _msg) const { 
#if defined(QT_VERSION)
    switch (_logType) {
    case LogType::Error:
        _loggerObject->setTextColor(Qt::red);
        _loggerObject->insertPlainText("[ERROR]\t");
        _loggerObject->insertPlainText(QString::fromStdString(_msg) + "\n");
        break;
    case LogType::Warning:
        _loggerObject->setTextColor(Qt::green);
        _loggerObject->insertPlainText("[WARNING]\t");
        _loggerObject->insertPlainText(QString::fromStdString(_msg) + "\n");
        break;
    case LogType::Info:
        _loggerObject->setTextColor(Qt::blue);
        _loggerObject->insertPlainText("[INFO]\t");
        _loggerObject->insertPlainText(QString::fromStdString(_msg) + "\n");
        break;
    default:
        break;
    }
#endif 


}
//This function is not used, as the va_list gets segmentation fault if the stack is args are passed again
void Logger::_log_console(LogType _logType, const char* format...) const {
    va_list argptr;
    switch (_logType) {
    case LogType::Warning:
    case LogType::Info:	
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        fprintf(stdout, "\n");
        break;
    case LogType::Error:
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);      
    	va_end(argptr);
        fprintf(stderr, "\n");
        break;
    default:
        break;
    }

}


}  // namespace gdpd_gp
}  // namespace geeds
