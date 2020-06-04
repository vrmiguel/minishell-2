/*
 * miniSHELL-2
 * https://github.com/vrmiguel/minishell-2
 *
 * Copyright (c) 2020 Vinícius R. Miguel <vinicius.miguel at unifesp.br>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
    string home_dir;            // User's $HOME directory
    OpSys();
    void  change_dir(vector<string>);
    void  show_history();
    short simple_command(vector<string>);
    short piped_command (vector<string>, int);
    friend class Prompt;
};

extern OpSys OS;

#endif // OPSYS_H
