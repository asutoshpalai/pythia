#ifndef SHELL_H
#define SHELL_H

#define MAX_COMMANDS 10

struct shell_command {
  char* command;
  char* description;
  void* function;
};

void shell_init();

void shell();

void set_shell_prompt(char *prompt);

void shell_install_command(char* command, char* desc, void (*function)());

#endif
