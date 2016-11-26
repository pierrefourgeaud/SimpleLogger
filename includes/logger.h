// Copyright 2016 Pierre Fourgeaud

// Logging code initially based on Petru Marginean's design
// http://www.drdobbs.com/cpp/201804215?pgno=1

#ifndef PF_SIMPLELOGGER_H_
#define PF_SIMPLELOGGER_H_

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <time.h>

#include "./iloglistener.h"

/**
 * @brief The ELogLevel enum
 *
 * Represent the log levels available.
 */
enum class ELogLevel {
    Critical,
    Error,
    Warning,
    Info,
    Debug,
    Debug1,
    Debug2
};

#ifdef LOGGING_DISABLED
#define _LOG(level) {}
#else
#define _LOG(level)                         \
    if (level > SimpleLogger::ReportingLevel()) {}\
    else\
        SimpleLogger().Get(level)
#endif

#define LOGC  _LOG(ELogLevel::Critical)
#define LOGE  _LOG(ELogLevel::Error)
#define LOGW  _LOG(ELogLevel::Warning)
#define LOGI  _LOG(ELogLevel::Info)
#define LOGD  _LOG(ELogLevel::Debug)
#define LOGD1 _LOG(ELogLevel::Debug1)
#define LOGD2 _LOG(ELogLevel::Debug2)

// Utils to use in class declaration
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(TypeName&) = delete;          \
    void operator=(TypeName) = delete;

template <typename E>
constexpr typename std::underlying_type<E>::type to_i(E e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

// Forward declarations.
inline std::string NowTime();


/**
 * Logger class. Represents a log message.
 *
 * Represents a log message. At the destruction of the object,
 * all the log listeners will be notified.
 */
class SimpleLogger {
    // Forward declaration.
    class Notifier {
    public:
        Notifier(const std::string& iLog, ELogLevel iLevel)
            : m_Log(iLog)
            , m_Level(iLevel) {}

        void operator()(ILogListener* iListener) {
            iListener->Notify(m_Log, m_Level);
        }

    private:
        std::string m_Log;
        ELogLevel   m_Level;
    };

public:
    SimpleLogger() {}

    /**
     * Virtual destructor.
     *
     * Notifies all the listeners.
     */
    virtual ~SimpleLogger() { _NotifyListeners(); }

    /**
     * Called when logging.
     *
     * @param iLevel The Log Level for this message (optional, default to Info)
     *
     * @return An ostringstream prefiled with time and info.
     */
    std::ostringstream& Get(ELogLevel iLevel = ELogLevel::Info) {
        m_Os << NowTime();
        std::string level = SimpleLogger::ToString(iLevel);
        m_Os << " " << level << ":";
        if (level.size() < 6) {  m_Os << "\t"; }
        m_Os << " ";
        m_Level = iLevel;
        return m_Os;
    }

    /**
     * Used to add a log listener.
     *
     * @param iListener Pointer to the listener
     *
     * @return true if action successful, false otherwise.
     */
    static bool AddListener(ILogListener* iListener) {
        std::vector<ILogListener*>::iterator tmp =
            std::find(Listeners().begin(), Listeners().end(), iListener);

        // Return false if the listener is already in the vector. This is not expected
        // but there is nothing really wrong either.
        if (tmp != Listeners().end()) {
            return false;
        }

        Listeners().push_back(iListener);
        return true;
    }

    /**
     * Remove a log listener.
     *
     * @param iListener Pointer to the listener
     *
     * @return true if action successful, false otherwise.
     */
    static bool RemoveListener(ILogListener* iListener) {
        std::vector<ILogListener*>::iterator tmp =
            std::find(Listeners().begin(), Listeners().end(), iListener);

        // Return false if the listener could not be found
        if (tmp == Listeners().end()) {
            return false;
        } else {
            Listeners().erase(std::remove(Listeners().begin(), Listeners().end(), iListener));
            return true;
        }
    }

    /**
     * Static method that return a listeners. The first call to this
     * method will initialize the listeners container.
     *
     * @return A vector of listeners.
     */
    static inline std::vector<ILogListener*>& Listeners() {
        static std::vector<ILogListener*> listeners;
        return listeners;
    }

    /**
     * Return the log level. It also initialize it by default to Info.
     * You can modify the log level via this function since a reference is returned.
     *
     * ```.cpp
     * SimpleLogger::ReportingLevel() = ELogLevel::Debug;
     * ```
     *
     * @return A reference to the log level.
     */
    static inline ELogLevel& ReportingLevel() {
        static ELogLevel reportingLevel = ELogLevel::Info;
        return reportingLevel;
    }

    /**
     * Return a string that corresponding to the log level passed in argument.
     *
     * @param iLevel The log level you want print.
     *
     * @return A string containing the conversion from the log level.
     */
    static inline std::string ToString(ELogLevel iLevel) {
        static const char* const buffer[] = {
            "CRITICAL",
            "ERROR",
            "WARNING",
            "INFO",
            "DEBUG",
            "DEBUG1",
            "DEBUG2"};
        return buffer[to_i(iLevel)];
    }

    /**
     * Return the log level corresponding to the string passed in argument.
     * If the string doesn't correspond to any level, Info will be returned.
     *
     * @param iLevel The log level in string
     *
     * @return An enum class representing the log level.
     */
    static ELogLevel FromString(const std::string& iLevel) {
        if (iLevel == "DEBUG2") {
            return ELogLevel::Debug2;
        } else if (iLevel == "DEBUG1") {
            return ELogLevel::Debug1;
        } else if (iLevel == "DEBUG") {
            return ELogLevel::Debug;
        } else if (iLevel == "INFO") {
            return ELogLevel::Info;
        } else if (iLevel == "WARNING") {
            return ELogLevel::Warning;
        } else if (iLevel == "ERROR") {
            return ELogLevel::Error;
        } else if (iLevel == "CRITICAL") {
            return ELogLevel::Critical;
        }

        // If the log level is not found, we return the default one (Info)
        LOGW << "Unknown logging level '" << iLevel << "'. Using INFO level as default.";
        return ELogLevel::Info;
    }

protected:
    std::ostringstream m_Os;
    ELogLevel          m_Level = ELogLevel::Info;

    /**
     * Notify all listeners that a new message is available to be treated.
     *
     * @return true if there is listeners receiving the message, false otherwise.
     */
    bool _NotifyListeners() {
        std::string tmp = m_Os.str();
        std::for_each(Listeners().begin(), Listeners().end(), Notifier(tmp, m_Level));

        // Return false if there are no listeners in the vector
        return (Listeners().size() > 0);
    }

private:
    DISALLOW_COPY_AND_ASSIGN(SimpleLogger)
};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#else
#include <sys/time.h>
#endif // WIN32

inline std::string NowTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    char buffer[20];
    tm r = { 0 };
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    localtime_s(&r, &in_time_t);
	static DWORD first = GetTickCount();
	int tick = static_cast<int>(GetTickCount() - first) % 1000;
#else
    localtime_r(&in_time_t, &r);
    struct timeval tv;
    gettimeofday(&tv, 0);
    int tick =  static_cast<int>(tv.tv_usec) / 1000;
#endif // WIN32
    strftime(buffer, sizeof(buffer), "%F %T", &r);

    char result[100] = { 0 };
    std::snprintf(result, sizeof(result), "%s.%03d", buffer, tick);

    return result;
}

#endif // PF_SIMPLELOGGER_H_
