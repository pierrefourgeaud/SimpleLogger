# SimpleLogger
Single header only logger library for C++. Focused on performance, simplicity and extensibility.

### Table of Contents
<pre>
<a href="#introduction">Getting Started</a>
    <a href="#example">Example</a>
<a href="#usage">Usage</a>
    <a href="#log-levels">Log Levels</a>
    <a href="#loggers-available">Loggers available</a>
        <a href="#consolelogger">ConsoleLogger</a>
        <a href="#filelogger">FileLogger</a>
        <a href="#bufferlogger">BufferLogger</a>
    <a href="#implement-your-own-logger">Implement your own logger</a>
<a href="#todos">TODOs</a>
<a href="#contributing">Contributing</a>
    <a href="#found-an-issue-">Found an issue ?</a>
    <a href="#pull-requests">Pull requests</a>
</pre>

# Getting Started

Either add the project as a submodule or download it.

Then include the header you need :) As simple as that...

### Example

Let's log to console for example:

```cpp
#include "logger.h"
#include "consolelogger.h"

int main() {
  ILogListener* consoleLogger = new ConsoleLogger;
  Logger::AddListener(consoleLogger);

  LOGI << "As simple as that :)" << std::endl;

  Logger::RemoveListener(consoleLogger);
  delete consoleLogger;
}
```

If you execute this sample programme you will see in the console :

```
$ ./MySimpleLog
16:39:35.394 INFO: As simple as that :)
```

# Usage

You have different log levels and different way to log, here we will see in details how to customize all of that.

## Log Levels

| Level    | Macro   | String   | Description                                        |
|----------|---------|----------|----------------------------------------------------|
| Critical | `LOGC`  | CRITICAL | Critical error happened (program will not recover) |
| Error    | `LOGE`  | ERROR    | Error happened, program tries to recover           |
| Warning  | `LOGW`  | WARNING  | Something unusual happened (or unexpected)         |
| Info     | `LOGI`  | INFO     | Simple log message (default)                       |
| Debug    | `LOGD`  | DEBUG    | Debugging message                                  |
| Debug 1  | `LOGD1` | DEBUG1   | More debugging message (might impact performance)  |
| Debug 2  | `LOGD2` | DEBUG2   | More debugging message (will impact performance)   |

If you selected a Log Level info (which is the default one), all the more important messages will be logged (Critical, Error, Warning) and the Info ones. This goes for every log level. Debug 2 is the least important, Critical is the most.

**_Why do we have more than one Debug log level ?_**

_In some program, performance is a must. In order to not impact performance but still be able to log, you might want to have different kind of Debug messages. That is, when I started this project, I was using it in a small homemade Game Engine, where performance is critical. I could use Debug2 in the Game loop will using just Debug in the startup of the Engine, and Debug1 in the loading of the assets._

## Loggers available

SimpleLogger provide 3 default loggers: ConsoleLogger, FileLogger and BufferLogger.

### ConsoleLogger

Logger responsible for logging the messages in the ..... console. The example earlier shows how to use it.

If the Level of the message printed is `Critical` or `Error`, the message will be printed on the error output, otherwise every other message will be printed on the standard output.

### FileLogger

This one will simply log the messages in a file.

```cpp
#include <string>
#include "logger.h"
#include "filelogger.h"

static const std::string g_FilePath = "/home/me/cool.log";

int main() {
  ILogListener* fileLogger = new FileLogger(g_FilePath);
  Logger::AddListener(fileLogger);

  LOGI << "As simple as that :)" << std::endl;

  Logger::RemoveListener(fileLogger);
  delete fileLogger;
}
```

As you can expect:

```
$ ./MySimpleLog
$ cat /home/me/cool.log
16:39:35.394 INFO: As simple as that :)
```

### BufferLogger

The less obvious of the 3, it has for responsability to put all the messages inside a buffer. Then periodically you can get the messages. Useful if you want to send the messages over the network, on a graphical user interface of if you prefer to limitate the number of IO in a file or on the console.

The usage is the same as the others:

```cpp
#include <iostream>
#include "logger.h"
#include "bufferlogger.h"

int main() {
  ILogListener* bufferLogger = new BufferLogger;
  Logger::AddListener(bufferLogger);

  LOGI << "As simple as that :)" << std::endl;
  LOGI << "Isn't it ?" << std::endl;

  std::cout << "Waiting to print the logs..." << std::endl;

  std::cout << bufferLogger.GetBuffer() << std::endl;

  Logger::RemoveListener(bufferLogger);
  delete bufferLogger;
}
```

Ouput:

```
$ ./MySimpleLog
Waiting to print the logs...
16:39:35.394 INFO: As simple as that :)
16:39:35.394 INFO: Isn't it ?
```

## Implement your own logger

With SimpleLogger, you can implement your own logger.

Our use case will be to implement a logger over the network.

To implement your own logger you have to implement the `ILogListener` interface:

```cpp
class ILogListener {
public:
    virtual ~ILogListener() {}
    virtual void Notify(const std::string& iLog, ELogLevel iLevel) = 0;
};
```

networklogger.h:

```cpp

// some includes...

// The socket API used here is just made up and simplified for the example

class NetworkLogger : public ILogListener {
public:
    NetworkLogger(const std::string& iServerURI) {
        m_MySocket = connect(iServerURI.c_str());
    }

    ~NetworkLogger() {
        if (m_MySocket) {
            disconnect(m_MySocket);
            m_MySocket = nullptr;
        }
    }

    // Method called everytime a new message is logged
    virtual void Notify(const std::string& iMsg, ELogLevel iLevel) {
        send(m_MySocket, iMsg.c_str());
    }

private:
    Socket_t* m_MySocket = nullptr;
};
```

And now, just use it !

```cpp
#include "logger.h"
#include "networklogger.h"

int main() {
  ILogListener* networkLogger = new NetworkLogger("http://mylog.mycompany.com:1234");
  Logger::AddListener(networkLogger);

  LOGI << "As simple as that :)" << std::endl;

  Logger::RemoveListener(networkLogger);
  delete networkLogger;
}
```

On the server you should simply receive: `16:39:35.394 INFO: As simple as that :)`;

# TODOs

- Allow user to format the log (today it is always `datetime [LogLevel]: message`
- Add tests
- Allow users to define their own log levels
- Assign colors for the terminal depending on the level

# Contributing

## Found an issue ?

If you find a bug in the source code or a mistake in the documentation, you can help by submitting an issue [here](https://github.com/pierrefourgeaud/SimpleLogger/issues).

## Pull requests

Before you submit your pull request consider the following guidelines:

- Search GitHub for an open or closed Pull Request that relates to your submission. You don't want to duplicate effort.

- Make your changes in a new git branch:

  ```
  $ git checkout -b my-fix-branch master
  ```

- Create your patch

- Commit your changes using a descriptive commit message

- Push your branch to GitHub:

  ```
  $ git push origin my-fix-branch
  ```

- In GitHub, send a pull request to `SimpleLogger:master`.

- If we suggest changes then:

  - Make the required updates.

  - Re-run the test suite to ensure tests are still passing.

  - If necessary, rebase your branch and force push to your GitHub repository (this will update your Pull Request):

    ```
    $ git rebase master -i
    $ git push origin my-fix-branch -f
    ```

That's it! Thank you for your contribution!
