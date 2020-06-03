#include "Headers/opsys.h"

OpSys OS;       // Global OpSys variable. Should be accessible to all modules.
bool sigint = false;

void signal_handler(int s)
{
    if (s==SIGINT)
    {
        cout << "\nSIGINT (Ctrl+C) received. Code " << s << ". Exiting.\n";
        sigint = true;  // We'll do this instead of an exit(0) in order to allow for cleanup.
    }

    if (s==SIGHUP)
    {
        cout << "\nSIGHUP received. Code " << s << ". Exiting.\n";
        sigint = true;  // We'll do this instead of an exit(0) in order to allow for cleanup.
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
    string cmd = "";
    for(unsigned short int i = 1; i < command.size(); i++)
    {
        cmd += command[i];
    }
    if (chdir(cmd.c_str()))
    {
        cerr << "miniShell: cd: " << cmd << ": Arquivo ou diretório não encontrado\n";
    }
    else {
        get_cwd();
        cwd_changed = true;
    }
}

std::vector<const char*> make_argv( std::vector<std::string>const& in )
{
  std::vector<const char*> out;
  out.reserve( in.size() + 1 );

  for (const auto& s : in) {
    out.push_back( s.data() );
  }
  out.push_back(NULL);
  out.shrink_to_fit();

  return out;
}

short int OpSys::simple_command(vector<string> tokens)
{
    pid_t pid;

    int status;
    if(is_verbose)
    {
        cerr << "execvp(\"" << tokens[0] << "\",\"";
        for(unsigned int i = 0; i < tokens.size()-1; i++)
            cerr << tokens[i] << ' ';
        cerr << tokens.back();
        cerr << "\");\n";
    }


    pid = fork();
    if (pid == 0)
    {
        //printf("execlp(%s, %s, NULL);", bincmd.c_str(), command.c_str());
        execvp(tokens[0].c_str(), const_cast<char* const *>(make_argv(tokens).data()));
        cerr << tokens[0] << ": comando não encontrado\n";
        exit(0);        // TODO: exit with cleanup
    }
    else if (pid < 0)
    {
        printf("Erro ao produzir fork.");
        return 0;
    }
    waitpid(-1, &status, WUNTRACED);
    return 1; // sucesso
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
