#ifndef OPSYS_H
#define OPSYS_H

#include "input.h"
extern bool sigint;

/*!
 * \class SignalHandler
 * \brief Handles SIGHUP and SIGINT signals.
 */
class SignalHandler
{
    struct sigaction signal_action;
public:
    /*!
     * \brief Constructor for SignalHandler. Handles SIGHUP and SIGINT signals.
     */
    SignalHandler();
};

class OpSys
{
    string get_cwd();
public:
    string username;        // Username
    string hostname;        // User's hostname
    string cwd;             // Current working directory
    OpSys();
    void change_dir(vector<string>);
};

extern OpSys OS;

#endif // OPSYS_H
