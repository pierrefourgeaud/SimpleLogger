// Copyright 2016 Pierre Fourgeaud

#ifndef PF_CONSOLELOGGER_H_
#define PF_CONSOLELOGGER_H_

#include <iostream>
#include <string>
#include "./iloglistener.h"
#include "./Logger.h"

/**
 * @brief The FileLogger class
 *
 * Logger that will write the ouput to the console (cerr or cout depending on log level)
 */
class ConsoleLogger : public ILogListener {
public:
    ConsoleLogger() {}

    /**
     * Virtual destructor
     */
    virtual ~ConsoleLogger() {}

    /**
     * @brief Notify Mandatory method to implement for every log listener.
     *        Will be called everytime there is a new message to ouput it to the console.
     *
     *        if iLevel Critical or Error, output to cerr, otherwise to cout
     *
     * @param iLog The message
     * @param iLevel The log level
     */
    virtual void Notify(const std::string& iMsg, ELogLevel iLevel) {
        if (iLevel <= ELogLevel::Error) {
            std::cerr << iMsg;
        } else {
            std::cout << iMsg;
        }
    }
};

#endif // PF_CONSOLELOGGER_H_
