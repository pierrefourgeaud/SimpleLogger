// Copyright 2016 Pierre Fourgeaud

#ifndef PF_ILOGLISTENER_H_
#define PF_ILOGLISTENER_H_

#include <string>

// Forward declarations.
enum class ELogLevel;


/**
 * @brief The ILogListener class
 *
 * Define an interface to implement when creating a new LogListener.
 * SimpleLogger provide 3 default listeners: Buffer, File, Console.
 *
 * Implement this class if you want to create your own listener.
 */
class ILogListener {
public:
    /**
     * Virtual destructor.
     */
    virtual ~ILogListener() {}

    /**
     * Pure virtual method to implement when implementing this interface.
     * Method called when getting notified.
     *
     * @params iLog The string representing the messag to log
     * @params iLevel Log level of this message
     */
    virtual void Notify(const std::string& iLog, ELogLevel iLevel) = 0;
};

#endif // PF_ILOGLISTENER_H_
