#ifndef OPSYS_H
#define OPSYS_H

#include "input.h"
#include <csignal>          // struct sigaction, sigaction, SIGINT, SIGHUP
#include <iostream>         // std::cin, std::cout, printf, fprintf
extern bool exit_program;

#define WRITE_END 1
#define READ_END  0

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
    vector<vector<string>> history;
public:
    string username;        // Username
    string hostname;        // User's hostname
    string cwd;             // Current working directory
    OpSys();
    void  change_dir(vector<string>);
    void  show_history();
    short simple_command(vector<string>);
    short piped_command (vector<string>, int);
    friend class Prompt;
};

extern OpSys OS;

#endif // OPSYS_H
