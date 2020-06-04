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

using std::cin;
using std::cout;

int main(int argc, char ** argv)
{
    cout << "miniSHELL 2 -- github.com/vrmiguel/minishell2\n";
    CLIInputs(argc, argv);          // Process command-line arguments
    Prompt prompt;
    SignalHandler sighandler;
    for(;;)
    {
        if (exit_program) return 0; // Tests for SIGINT/SIGHUP
        string line;
        prompt.print();
        std::getline(cin, line);

        if (exit_program) return 0; // Tests for "quit"
        if (cin.eof())
        {
            cout << "\nEOF found. Exiting.\n";
            return 0;
        }
        if(line.empty())
            continue;
        prompt.parse(line);
        prompt.run();
    }
    return 0;
}
