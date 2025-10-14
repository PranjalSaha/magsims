#include "Shell.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int command(const string &command) {
    cout << "Running shell command:" << endl;
    cout << command << endl;

    int result = system(command.c_str());
    if (result != 0) {
        cerr << "Error: command failed with code "
             << result << endl;
        return result + 10;
    }
    return 0;
}

string get_python_exec(){
    string cmd;
#ifdef _WIN32
    cmd = "venv\\Scripts\\python.exe";  // Windows
#else
    cmd = "../venv/bin/python3";         // Linux/macOS
#endif
    return cmd;
}

int run_python_script(const string &script){
    string python_exec = get_python_exec();
    string cmd = python_exec + " " + script;
    return command(cmd);
}

string detect_shell_command() {
#ifdef _WIN32
    char *comspec = getenv("ComSpec");
    char *pspath  = getenv("PSModulePath");

    if (pspath != nullptr) {
        // Likely PowerShell
        return "venv\\Scripts\\Activate.ps1";
    } else if (comspec != nullptr) {
        // Default: Command Prompt
        return "venv\\Scripts\\activate.bat";
    } else {
        return "echo Unknown Windows shell";
    }
#else
    char *shell = getenv("SHELL");
    if (shell == nullptr) {
        return "echo Could not detect shell";
    }
    string sh(shell);

    if (sh.find("bash") != string::npos ||
        sh.find("zsh") != string::npos ||
        sh.find("sh") != string::npos) {
        return "source venv/bin/activate";
    } else if (sh.find("fish") != string::npos) {
        return "source venv/bin/activate.fish";
    } else if (sh.find("csh") != string::npos ||
               sh.find("tcsh") != string::npos) {
        return "source venv/bin/activate.csh";
    } else {
        return "echo Unknown shell: " + sh;
    }
#endif
}

int venv_activate(){
    string cmd = detect_shell_command();
    return command(cmd);
}

int venv_deactivate(){
    return command("deactivate");
}
