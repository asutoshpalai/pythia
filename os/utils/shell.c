#include "shell.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
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

void paging_command() {
  char *command = strtok(NULL, ' ');

  if(command == NULL || strcmp(command, "help") == 0) {
    printf("Usage:\nentry: Print the page_table entry: \tentry <pde> <pte>\n" \
        "Numbers are in hex\n");
  }
  else if(strcmp(command, "entry") == 0) {
    char *s = strtok(NULL, ' ');
    int pde = atoi(s, 16);

    s = strtok(NULL, ' ');
    int pte = atoi(s, 16);

    show_page_entry(pde, pte);
  }
}

void *maddr;
void memory_command() {
  char *command = strtok(NULL, ' ');

  if(command == NULL || strcmp(command, "help") == 0) {
    printf("Usage:\nprint_pool: Print the memory pool linked list\n");
    printf("malloc [?size - in hex]: Execute malloc and print the address returned\n");
    printf("free [?addr - in hex]: Call free() with the last malloc address\n");
  }
  else if(strcmp(command, "print_pool") == 0) {
    print_memory_pool_list();
  }
  else if(strcmp(command, "malloc") == 0) {
    char *s = strtok(NULL, ' ');
    void * new;
    if (s == NULL)
      new = malloc(16);
    else
      new = malloc(atoi(s, 16));
    maddr = new;
    printf("Got memory address 0x%x\n", (int)new);
  }
  else if(strcmp(command, "free") == 0) {
    char *s = strtok(NULL, ' ');
    if (s == NULL)
      free(maddr);
    else
      free((void *)atoi(s, 16));
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

void set_command() {
  char *var, *sign, *value;
  var = strtok(NULL, ' ');
  sign = strtok(NULL, ' ');
  value = strtok(NULL, ' ');
  if(var == NULL) {
    printf("No arguments given\n");
    return;
  }
  if(sign == NULL || sign[0] != '=') {
    printf("Assignment sign error\n");
    return;
  }
  if(value == NULL) {
    printf("You forgot to provide the value\n");
    return;
  }

  if(strcmp(var, "prompt") == 0) {
    set_shell_prompt(value);
  }
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
    printf("Invalid command %s\nEnter help to list the available commands\n", command);
    return;
  }

  struct shell_command c = shell_commands[choice];

  void (*fun)() = c.function;
  fun();
}
void shell_init() {
  shell_active_commands = 0;

  install_command("help", "List the commands available", help_command);
  install_command("set", "Set a variable\nUsage: set <var> = <val>", set_command);
  install_command("echo", "print the tokens prvided\nUsage: echo <string>", echo_command);
  install_command("memory", "Commands related to memory\nUsage: memory <command>", memory_command);
  install_command("paging", "Commands related to paging", paging_command);
  install_command("", "Dummy command for empty queries", empty_command);
  shell_prompt = NULL;
  set_shell_prompt("Shell>");
}

void set_shell_prompt(char* p) {
  char *new_p = (char *)malloc(strlen(p) + 1);
  strcpy(new_p, p);
  if (shell_prompt)
    free(shell_prompt);
  shell_prompt = new_p;
}

void shell() {
  printf("%s ", shell_prompt);
  char command[200];
  gets(command);

  shell_execute_command(command);
}
