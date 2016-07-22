// Copyright 2016 Airtame

#ifndef MANAGEDEVICE_LOGGING_BUFFERLOGGER_H_
#define MANAGEDEVICE_LOGGING_BUFFERLOGGER_H_

#include <string>
#include "./iloglistener.h"

class BufferLogger : public ILogListener {
public:
    BufferLogger() {}
    virtual ~BufferLogger() {}

    std::string GetBuffer(bool iClearBuffer = true) {
        std::string buffer = m_Buffer.str();

        if (iClearBuffer) {
            ClearBuffer();
        }

        return buffer;
    }

    void ClearBuffer() {
        m_Buffer.str("");
        m_Buffer.clear();
    }

    virtual void Notify(const std::string& iMsg, ELogLevel iLevel) {
        m_Buffer << iMsg;
    }

private:
    std::ostringstream m_Buffer;
};

#endif  // MANAGEDEVICE_LOGGING_BUFFERLOGGER_H_
