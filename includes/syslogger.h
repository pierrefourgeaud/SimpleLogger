#ifndef RK_SYSLOGGER_H_
#define RK_SYSLOGGER_H_

#include <iostream>
#include <string>
#include <syslog.h>

#include "./iloglistener.h"
#include "./logger.h"

/**
 * @brief The SysLogger class
 *
 * Logger that will write the ouput to the <a href=https://linux.die.net/man/3/syslog>syslog</a>
 */
class SysLogger : public ILogListener {
public:
    /**
     * @brief A constructor, takes one parameter, to specify application's identity
     *
     * @param iIdentity application's identity (used by syslog)
     */
    explicit SysLogger(const std::string& identity) {
        openlog(identity.c_str(), LOG_PID | LOG_NDELAY, LOG_DAEMON);
    }

    /**
     * Virtual destructor
     */
    virtual ~SysLogger() {
        closelog();
    }

    /**
     * @brief Notify Mandatory method to implement for every log listener.
     *        Will be called everytime there is a new message to ouput it to the syslog.
     *
     *        if iLevel Critical or Error, output to cerr, otherwise to cout
     *
     * @param iLog The message
     * @param iLevel The log level
     */
    virtual void Notify(const std::string& iMsg, ELogLevel iLevel) {
        syslog(_ELevelToSysLevel(iLevel), "%s", iMsg.c_str());
    }

private:
    /**
     * @brief Converts an internal @ELogLevel to the one defined by syslog
     *
     * @param iLevel The log level
     */
    int _ELevelToSysLevel(ELogLevel iLevel) {
        switch (iLevel) {
        case ELogLevel::Critical:
            return LOG_CRIT;
        case ELogLevel::Error:
            return LOG_ERR;
        case ELogLevel::Warning:
            return LOG_WARNING;
        case ELogLevel::Info:
            return LOG_INFO;
        case ELogLevel::Debug:
        case ELogLevel::Debug1:
        case ELogLevel::Debug2:
        default:
            return LOG_DEBUG;
        }
    }
};

#endif // RK_SYSLOGGER_H_
