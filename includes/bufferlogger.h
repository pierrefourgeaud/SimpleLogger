// Copyright 2016 Pierre Fourgeaud

#ifndef PF_BUFFERLOGGER_H_
#define PF_BUFFERLOGGER_H_

#include <string>
#include "./iloglistener.h"

/**
 * @brief The BufferLogger class
 *
 * Logger that will write the log in a buffer (stringstream) and
 * that allows the programmer to use the output
 */
class BufferLogger : public ILogListener {
public:
    BufferLogger() {}

    /**
     * Virtual destructor
     */
    virtual ~BufferLogger() {}

    /**
     * @brief GetBuffer Used to get the current buffer as a string. If the
     *                  argument clear buffer is true, the buffer will be emptied.
     *
     * @param iClearBuffer If true, the buffer will be emptied (optional, defaulted to true)
     *
     * @return The buffer as a string
     */
    std::string GetBuffer(bool iClearBuffer = true) {
        std::string buffer = m_Buffer.str();

        if (iClearBuffer) {
            ClearBuffer();
        }

        return buffer;
    }

    /**
     * @brief ClearBuffer Empty the buffer
     */
    void ClearBuffer() {
        m_Buffer.str("");
        m_Buffer.clear();
    }

    /**
     * @brief Notify Mandatory method to implement for every log listener.
     *        Will be called everytime there is a new message to add the message
     *        to the current buffer.
     *
     * @param iLog The message
     * @param iLevel The log level
     */
    virtual void Notify(const std::string& iMsg, ELogLevel iLevel) {
        m_Buffer << iMsg;
    }

private:
    std::ostringstream m_Buffer;
};

#endif  // PF_BUFFERLOGGER_H_
