// Copyright 2016 Airtame

#ifndef MANAGEDEVICE_LOGGING_ILOGLISTENER_H_
#define MANAGEDEVICE_LOGGING_ILOGLISTENER_H_

#include <string>

// Forward declarations.
enum class ELogLevel;

class ILogListener {
public:
    virtual ~ILogListener() {}
    virtual void Notify(const std::string& iLog, ELogLevel iLevel) = 0;
};

#endif  // MANAGEDEVICE_LOGGING_ILOGLISTENER_H_
