#include "shell.h"
#include "stdio.h"
#include "string.h"
#include "memory.h"

struct shell_command shell_commands[MAX_COMMANDS];

int shell_active_commands;

char *shell_prompt;

void help_command() {
  for(int i = 0; i < shell_active_commands; i++) {
    puts(shell_commands[i].command);
    puts("\t\t");
    puts(shell_commands[i].description);
    puts("\n");
  }
}

void *maddr;
void memory_command() {
  char *command = strtok(NULL, ' ');

  if(command == NULL || strcmp(command, "help") == 0) {
    printf("Usage:\nprint_pool: Print the memory pool linked list\n");
    printf("malloc: Execute malloc and print the address returned\n");
    printf("free: Call free() with the last malloc address\n");
  }
  else if(strcmp(command, "print_pool") == 0) {
    print_memory_pool_list();
  }
  else if(strcmp(command, "malloc") == 0) {
    void * new = malloc(16);
    maddr = new;
    printf("Got memory address 0x%x\n", (int)new);
  }
  else if(strcmp(command, "free") == 0) {
    free(maddr);
  }
}

void empty_command() {

}

void echo_command() {
  char *word;
  while((word = strtok(NULL, ' ')) != NULL) {
    puts(word);
    puts(" ");
  }
  puts("\n");
}

void install_command(char* command, char* desc, void (*function)()) {
  shell_commands[shell_active_commands].command = command;
  shell_commands[shell_active_commands].description = desc;
  shell_commands[shell_active_commands].function = function;
  shell_active_commands++;
}

int shell_find_command(char *command) {
  for(int i = 0; i < shell_active_commands; i++) {
    if(strcmp(command, shell_commands[i].command) == 0)
      return i;
  }
  return -1;
}

void shell_execute_command(char *full_command) {
  char *command = strtok(full_command, ' ');
  int choice = shell_find_command(command);

  if (choice == -1) {
    puts("Invalid command ");
    puts(command);
    puts("\n");
    return;
  }

  struct shell_command c = shell_commands[choice];

  void (*fun)() = c.function;
  fun();
}
void shell_init() {
  shell_active_commands = 0;

  install_command("help", "List the commands available", help_command);
  install_command("echo", "print the tokens prvided\nUsage: echo <string>", echo_command);
  install_command("memory", "Commands related to memory\nUsage: memory <command>", memory_command);
  install_command("", "Dummy command for empty queries", empty_command);
  shell_prompt = "Shell> ";
}

void set_shell_prompt(char* p) {
  shell_prompt = p;
}

void shell() {
  puts(shell_prompt);
  char command[200];
  gets(command);

  shell_execute_command(command);
}
