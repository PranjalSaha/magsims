#include <string>

#ifndef SHELL_H
#define SHELL_H

int command(const std::string &command);
int run_python_script(const std::string &script);
int venv_activate();
int venv_deactivate();

#endif
