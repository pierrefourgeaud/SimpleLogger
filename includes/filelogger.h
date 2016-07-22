// Copyright 2016 Pierre Fourgeaud

#ifndef PF_FILELOGGER_H_
#define PF_FILELOGGER_H_

#include <iostream>
#include <fstream>
#include <string>

#include "./iloglistener.h"

const std::string DEFAULT_FILEPATH = "./log.txt";

/**
 * @brief The FileLogger class
 *
 * Logger that will write the ouput in a file.
 */
class FileLogger : public ILogListener {
public:
    explicit FileLogger(const std::string& iFileName = DEFAULT_FILEPATH)
        : m_FileName(iFileName)
        , m_IsCreated(false) {

        _OpenFile();
    }

    /**
     * Virtual Destructor
     *
     * Have the responsability to close the file if it is still open.
     */
    virtual ~FileLogger() {
        if (m_File.is_open()) {
            m_File.close();
        }
    }

    /**
     * @brief Notify Mandatory method to implement for every log listener.
     *        Will be called everytime there is a new message.
     *
     * @param iLog The message
     * @param iLevel The log level
     */
    void Notify(const std::string& iLog, ELogLevel iLevel) {
        if (m_File.is_open()) {
            m_File << iLog;
            m_File.flush();
        }
    }

    /**
     * @brief GetFileName Return the filename.
     *
     * @return Return the filename as a string.
     */
    inline std::string GetFileName() const {
        return m_FileName;
    }

    /**
     * @brief ChangeFile Allow to change the file we log to during the execution
     *        (useful to keep a certain size for every log files)
     *
     * @param iFileName The path to the new file
     *
     * @return true if change successful, false otherwise
     */
    bool ChangeFile(const std::string& iFileName) {
        if (m_File.is_open()) {
            m_File.close();
        }

        m_FileName = iFileName;

        _OpenFile();
        return m_IsCreated;
    }

private:
    std::string m_FileName;
    std::ofstream m_File;
    bool m_IsCreated;

    /**
     * @brief _OpenFile Private function that create (or open and empty) the file.
     */
    void _OpenFile() {
        m_File.open(m_FileName);

        if (m_File.is_open()) {
            m_IsCreated = true;
        }
    }
};

#endif // PF_FILELOGGER_H_
