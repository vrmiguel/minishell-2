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

#include "Headers/opsys.h"
#include <regex>            // std::regex and std::regex_replace
#include <termios.h>

bool is_verbose  = false;
bool cwd_changed = false;

#include <unistd.h>         // fork, execvp
#include <pwd.h>            // struct password, getpwuid_r
#include <sstream>          // isstreams, osstreams
#include <algorithm>        // copy
#include <iterator>         // istream_iterator, iterator, back_inserter

using std::istringstream;
using std::copy;
using std::istream_iterator;
using std::ostream_iterator;
using std::cin;
using std::cout;

CLIInputs::CLIInputs(int argc, char **argv)
{
    if(argc > 1)
    {
        for(short unsigned int i = 1; i < argc; i++)
        {
            string s_argv = argv[i];
            if (!s_argv.compare("-h") || !s_argv.compare("--help"))
            {
                printf("%-20s\tObtain help.\n", "-h, --help");
                printf("%-20s\tShows additional execution data.\n", "-v, --verbose");
                exit(0);
            }
            if (!s_argv.compare("-v") || !s_argv.compare("--verbose"))
            {
                printf("Executing in verbose mode.\n");
                is_verbose = true;
            }
        }
    }
}

void Prompt::get_last_dir()
{
    string last_dir = OS.cwd;
    string home_dir = getenv("HOME");
    last_dir = std::regex_replace(last_dir, std::regex(home_dir), "~");     // Change HOME to ~ on the prompt line.
    this->last_dir = last_dir;
}

Prompt::Prompt()
{
//    struct termios t;
//    tcgetattr(STDIN_FILENO, &t);
//    t.c_lflag &= ~ICANON;
//    tcsetattr(STDIN_FILENO, TCSANOW, &t); // WIP for arrow keys support

    get_last_dir();  // Process cwd for exibition on prompt
}

void Prompt::print()
{
    if (cwd_changed)
    {
        get_last_dir();
        cwd_changed = false;
    }
    cout << BLUE_ANSI << OS.username << "@" << OS.hostname << ":" << RED_ANSI << last_dir << RESET_ANSI << "$ ";
}

void Prompt::parse(string input)
{
    if(!tokens.empty())
        tokens.clear();
    istringstream iss (input);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

    if(is_verbose)
    {
        cout << "Parsed: [";
        for(unsigned short int i = 0; i < tokens.size()-1; i++)
            cout << "\"" << tokens[i] << "\", ";
        cout << "\"" << tokens.back() << "\"]\n";
    }
}

short int Prompt::run()
{
    string pipe = "|";

        // Counts occurrences of a pipe character in the given command
    int pipe_count = std::count(tokens.begin(), tokens.end(), pipe);

    OS.history.push_back(tokens);   // Save the current command on OS.history

    if(pipe_count)
    {       // If there were pipes on the command, run OS.piped_command();
        return OS.piped_command(tokens, pipe_count);
    }
    if(!tokens[0].compare("cd"))
    {
        OS.change_dir(tokens);
        return cwd_changed;
    } else if (!tokens[0].compare("pwd"))
    {
        cout << OS.cwd << '\n';
        return 1;
    } else if (!tokens[0].compare("help"))
    {
        cout << "Insert help text here.\n";
        return 1;
    } else if (!tokens[0].compare("history"))
    {
        OS.show_history();
        return 1;
    }
    else if (!tokens[0].compare("quit"))
    {
        std::cerr << "Exiting.\n";
        exit_program = true;
        return 1;
    }
    else
    {
        return OS.simple_command(tokens);
    }
}
