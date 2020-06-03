#include "Headers/opsys.h"

OpSys OS;       // Global OpSys variable. Should be accessible to all modules.
bool sigint = false;

//template <std::size_t N>
//int execvps(const char* file, const char* const (&argv)[N])
//{
//    cerr << "Here\n";
//    assert((N > 0) && (argv[N - 1] == nullptr));
//    return execvp(file, const_cast<char* const*>(argv));
//}


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

short int OpSys::simple_command(vector<string> tokens)
{
    pid_t pid; // valor para teste pai/filho

//    std::vector<char*> argv(tokens.size() + 1);

//    std::transform(tokens.begin(), tokens.end(), argv.begin(),
//                   [](std::string arg) {
//                       return &(arg)[0];
//                   });

    string command = "";
    for (unsigned int i = 1; i<tokens.size(); i++)
    {
        command += tokens[i-1];
    }
    string bincmd = "/bin/" + command;
    int status;
    pid = fork();
    if (pid == 0)
    {
//        execvp(argv[0], argv.data());
        execlp(bincmd.c_str(), command.c_str(), NULL);
        cerr << tokens[0] << ": comando não encontrado\n";
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
