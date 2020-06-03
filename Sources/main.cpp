#include "Headers/opsys.h"

int main(int argc, char ** argv)
{
    cout << "miniSHELL 2 -- github.com/vrmiguel/minishell2\n";
    CLIInputs(argc, argv);          // Process command-line arguments
    Prompt prompt;
    SignalHandler sighandler;
    for(;;)
    {
        string line;
        prompt.print();
        std::getline(cin, line);
        if (sigint)
            return 0;
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
