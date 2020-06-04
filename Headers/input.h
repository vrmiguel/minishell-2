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
