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

#ifndef INPUT_H
#define INPUT_H

#include <string>           // strings
#include <vector>           // vectors

using std::string;
using std::vector;

#define RED_ANSI   "\x1b[31m" // ANSI escape code for red
#define BLUE_ANSI  "\x1b[34m" // ANSI escape code for blue
#define RESET_ANSI "\x1b[0m"  // ANSI escape code to reset stdout's color
#define KEY_UP    72          // ASCII code for arrow key up
#define KEY_LEFT  75          // ASCII code for arrow key left
#define KEY_RIGHT 77          // ASCII code for arrow key right
#define KEY_DOWN  80          // ASCII code for arrow key down

extern bool is_verbose;
extern bool cwd_changed;

class CLIInputs
{
public:
    /*!
     * \brief Constructor for CLIInputs.
     * \param argc Command-line argument count.
     * \param argv Array of strings that representing the command-line argument given.
     */
    CLIInputs(int argc, char ** argv);
};

class Prompt
{
    string last_dir;
    vector<string> tokens;
public:
    void get_last_dir();
    void print();
    short int run();
    void parse(string input);
    Prompt();
};

#endif // INPUT_H
