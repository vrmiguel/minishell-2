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
