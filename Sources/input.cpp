#include "Headers/opsys.h"

bool is_verbose  = false;
bool cwd_changed = false;

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
                is_verbose = false;
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
    cout << "PROMPT " << last_dir << '\n';
}

Prompt::Prompt()
{
    get_last_dir();
}

void Prompt::print()
{
    if (cwd_changed)
    {
        get_last_dir();
        cwd_changed = false;
    }
    cout << BLUE_ANSI << OS.username << "@" << OS.hostname << ":" << RED_ANSI << last_dir << BLUE_ANSI << "$ " << RESET_ANSI;
}

void Prompt::parse(string input)
{
    if(!tokens.empty())
        tokens.clear();
    istringstream iss (input);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
    for(unsigned short int i = 0; i < tokens.size(); i++)
        cout << tokens[i] << ' ';
    cout << '\n';
}

short int Prompt::run()
{
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
    } else
    {
        cout << "Wait.\n";
        return 1;
    }
}