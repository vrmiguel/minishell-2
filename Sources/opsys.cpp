#include "Headers/opsys.h"

#include <sys/param.h>      // MAXPATHLEN
#include <sys/wait.h>       // waitpid
#include <unistd.h>         // getcwd, fork, chdir, geuid, gethostname, execvp
#include <pwd.h>            // struct passwd, getpwuid_r

using std::cin;
using std::cout;
using std::cerr;

OpSys OS;       // Global OpSys variable. Should be accessible to all modules.
bool exit_program = false;

void signal_handler(int s)
{
    if (s==SIGINT)
    {
        cout << "\nSIGINT (Ctrl+C) received. Code " << s << ". Exiting.\n";
        exit_program = true;  // We'll do this instead of an exit(0) in order to allow for cleanup.
    }

    if (s==SIGHUP)
    {
        cout << "\nSIGHUP received. Code " << s << ". Exiting.\n";
        exit_program = true;  // We'll do this instead of an exit(0) in order to allow for cleanup.
    }
}

SignalHandler::SignalHandler()
{
    signal_action.sa_handler = signal_handler;
    sigemptyset(&signal_action.sa_mask);
    signal_action.sa_flags = 0;
    sigaction(SIGINT, &signal_action, NULL);
}

string OpSys::get_cwd()
{
    char temp[MAXPATHLEN];
    return getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("");
}

void OpSys::change_dir(vector<string> command)
{
    if (command.size() == 1 || !command[1].compare("~") || !command[1].compare("$HOME"))
    {           // User wants to cd into $HOME/~.
        chdir(getenv("HOME"));
        this->cwd = get_cwd();
        cwd_changed = true;
        return;
    }

    string cmd;

    for(unsigned short int i = 1; i < command.size(); i++)      // TODO: unneeded?
        cmd += command[i];

    if (chdir(cmd.c_str()))
    {
                // Couldn't get to change dirs
        cerr << "minish2: cd: " << cmd << ": Arquivo ou diretório não encontrado.\n";
    }
    else
    {
                // chdir worked. and tell Prompt to act accordingly.
        this->cwd = get_cwd();      // Update the OpSys OS's current cwd
        cwd_changed = true;         // Signals a cwd change to Prompt.
    }
}

vector<const char*> make_argv(vector<string>const& in)
{
    vector<const char*> out;
    out.reserve(in.size() + 1);
    for (const auto& s : in)
        out.push_back(s.data());
    out.push_back(nullptr);
    out.shrink_to_fit();
    return out;
}

short OpSys::piped_command(vector<string> tokens, int pipe_count)
{
    if(is_verbose)
    {
        cerr << "Running piped_command with ";
        cerr << "tokens: [";
        for(unsigned short int k = 0; k < tokens.size()-1; k++)
            cerr << "\"" << tokens[k] << "\", ";
        cerr << "\"" << tokens.back() << "\"]\n";
    }

    int n_commands = pipe_count + 1;
    int file_descriptor_array[10][2];
    unsigned short i, j=0;
    string pipe_str = "|";

    for(i=0; i < n_commands; i++)
    {
        if (n_commands > 10)
        {               // This is an arbitrary decision. Allowing for more pipes is easily possible.
            cerr << "More pipes than supported.\n";
            return -1;
        }
        vector<string> aux_cmd;
        for(;j<tokens.size();)
        {
            if (!tokens[j].compare(pipe_str))
            {               //  We've hit a pipe character.
                j++;        //  Skip a position for the next iteration.
                break;
            }
            else
            {               /*  Current token is not a pipe character
                             *  so let's add it to our auxiliary command vector */
                aux_cmd.push_back(tokens[j++]);
            }
        }

            // Creating pipe
        if(i!=n_commands-1)
        {
              if(pipe(file_descriptor_array[i])<0)
              {
                  cerr << "Error on pipe initialization.\n";
                  return -1;
              }
        }
            // Forking
        pid_t pid = fork();
        if(pid==0) // Primeiro processo filho
        {
            if(i!=n_commands-1)
            {
                dup2(file_descriptor_array[i][WRITE_END],STDOUT_FILENO);
                close(file_descriptor_array[i][READ_END]);
                close(file_descriptor_array[i][WRITE_END]);
            }
            if(i!=0)
            {
                dup2(file_descriptor_array[i-1][READ_END],STDIN_FILENO);
                close(file_descriptor_array[i-1][WRITE_END]);
                close(file_descriptor_array[i-1][READ_END]);
            }
            if(is_verbose)
            {
                cerr << "piped_command is going to execvp aux_cmd: [";
                for(unsigned short int k = 0; k < aux_cmd.size()-1; k++)
                    cerr << "\"" << aux_cmd[k] << "\", ";
                cerr << "\"" << aux_cmd.back() << "\"]\n";
            }
            execvp(aux_cmd[0].c_str(), const_cast<char* const *>(make_argv(aux_cmd).data()));
            cerr << aux_cmd[0] << ": Comando não encontrado.\n";
            return -1;
        }
        else {
            if(i!=0)
            {
                close(file_descriptor_array[i - 1][READ_END]);
                close(file_descriptor_array[i - 1][WRITE_END]);
            }
        }
    }
    for(i=0; i<n_commands; i++)
          wait(NULL);
    return 1;
}

void OpSys::show_history()              // TODO: save (and read) history on a file? use Bourne Again's history??
{
    unsigned short i = 0, j;
    for(vector<string> command : history)
    {
        string command_str = "";
        for(j=0; j<command.size()-1; j++)
        {
            command_str += command[j] + ' ';
        }
        command_str += command.back();
        printf("%-20hu\t%s\n", i++,  command_str.c_str());       // You may think this is unoptimized... which would actually be quite true.
    }
    fflush(stdout);
}

short OpSys::simple_command(vector<string> tokens)
{
    int status;
    if(is_verbose)
    {
        cerr << "execvp(\"" << tokens[0] << "\",\"";
        for(unsigned int i = 0; i < tokens.size()-1; i++)
            cerr << tokens[i] << ' ';
        cerr << tokens.back();
        cerr << "\");\n";
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(tokens[0].c_str(), const_cast<char* const *>(make_argv(tokens).data()));
        cerr << tokens[0] << ": command not found.\n";
        return -1;        // TODO: exit with cleanup
    }
    else if (pid < 0)
    {
        printf("Couldn't fork a process.");
        return 0;
    }
    waitpid(-1, &status, WUNTRACED);
    return 1;
}

OpSys::OpSys()
{
    cwd = get_cwd();
    if(cwd.empty())
        cerr << RED_ANSI << "Could not get the current working directory.\n" << RESET_ANSI;

    uid_t uid = geteuid();             // Gets the effective ID of the user that started miniSHELL
    struct passwd pwent;
    struct passwd *pwent_ptr;
    char buffer[1024];

        // Looks for the UDI on the password databank and saves the result on pwent
    getpwuid_r(uid, &pwent, buffer, sizeof buffer, &pwent_ptr);
    username = pwent.pw_name;   // Saves username

    char hostname[64];
    gethostname(hostname, 64);
    this->hostname = hostname;
}
