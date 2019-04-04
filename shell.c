#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "bread.h"
//brennan.io/2015/01/16/write-a-shell-in-c/ (Quelle)

//Farben
void red() {printf("\033[0;31m");}
void bold_red() {printf("\033[1;31m");}
void green() {printf("\033[0;32m");}
void bold_green() {printf("\033[1;32m");}
void yellow() {printf("\033[0;33m");}
void bold_yellow() {printf("\033[01;33m");}
void blue() {printf("\033[0;34m");}
void bold_blue() {printf("\033[1;34m");}
void magenta() {printf("\033[0;35m");}
void bold_magenta() {printf("\033[1;35m");}
void cyan() {printf("\033[0;36m");}
void bold_cyan() {printf("\033[1;36m");}
void reset() {printf("\033[0m");}

//zufallsfarben
void funkyfunk() {
  switch(rand() % 11){
    case 0: red(); break;
    case 1: bold_red(); break;
    case 2: green(); break;
    case 3: bold_green(); break;
    case 4: yellow(); break;
    case 5: bold_yellow(); break;
    case 6: blue(); break;
    case 7: bold_blue(); break;
    case 8: magenta(); break;
    case 9: bold_magenta(); break;
    case 10: cyan(); break;
    case 11: bold_cyan(); break;
  }
}

//opening
void greet(){
  char greet[] = "Welcome to the funky Funk_shell, type help for overview!\n";
  char *current = strtok(greet, " ");
  for ( ;current != NULL; current= strtok(NULL, " ")){
    funkyfunk();
    printf("%s ", current);
  }
}
void fake(){
  char fake[] = "You thought I would let you escape, you unfunky son of a bitch?!\n";
  char *current = strtok(fake, " ");
  for ( ;current != NULL; current= strtok(NULL, " ")){
    funkyfunk();
    printf("%s ", current);
  }
}
void greet2(){
  char greet2[] = "See you soon, you funky Funk!\n";
  char *current = strtok(greet2, " ");
  for ( ;current != NULL; current= strtok(NULL, " ")){
    funkyfunk();
    printf("%s ", current);
  }
}
//TODO: token parser
/*zerstückelt die vom User eingegebene Zeile in einzelne Argumente
im großen und ganzen wie read_line aufgebaut*/
#define TOKEN_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n"
char **split_line(char *line){
  int position = 0, bufsize = TOKEN_BUFSIZE;
  char **tokens = malloc(sizeof(char*) * bufsize);
  char *token;
  if(!tokens) {
    fprintf(stderr,"Funkiger Speicherzuordungsfehler\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(line, TOKEN_DELIM);
  while(token != NULL) {
    tokens[position] = token;
    position++;
    if(position >= bufsize) {
      bufsize += TOKEN_BUFSIZE;
      tokens = realloc(tokens, sizeof(char*) * bufsize);
      if(!tokens) {
        fprintf(stderr,"Funkiger Speicherzuordungsfehler\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, TOKEN_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int shell_launch(char **args) {
  pid_t pid, wpid;
  int status;
  pid = fork();
  if(pid == 0) {
    if(execvp(args[0], args) == -1) {
      perror("not very funky");
    }
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    perror("not very funky");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

//eingebaute befehle-----------------------------------------
int shell_help(char **args);
int shell_cd(int argc,char **args);
int shell_funkyexit(char **args);
int shell_funk(char **args);
int shell_genocide(char **args);
int shell_exit(char **args);
//-----------------------------------------------------------

//Strings die zu den Befehlen gehören
char *built_in_str[] = {
  "help",
  "cd",
  "funkyexit",
  "funk",
  "genocide",
  "exit"
};

int (*built_in_f[]) (char **) = {
  &shell_help,
  &shell_cd,
  &shell_funkyexit,
  &shell_funk,
  &shell_genocide,
  &shell_exit
};

int funk_num_builtins(){
  return sizeof(built_in_str)/sizeof(char *);
}
int shell_genocide(char **args){
  system("shutdown -h now");
  return 0;
}
//hilfe
int shell_help(char **args) {
  bold_red();
  printf("\nDie Funk_shell beinhaltet folgende eingebaute Befehle:\n\n");
  bold_yellow();
  printf("    help: lässt diese funkige Anzeige erscheinen.\n\n");
  printf("    cd: wechselt das Verzeichnis, suche nach deinen Dateien mit noch mehr Funk!\n\n");
  printf("    exit: schließt die Shell, nicht sehr funky...\n\n");
  reset();
  return 1;
}
//wechselt das verzeichnis
int shell_cd(int argc, char **args) {
  if (argc < 2){
    args[1] = getenv("HOME");
  }
  if(chdir(args[1]) != 0) {
    bold_red();
    perror("not very funky");
  }
  return 1;
}
//verlässt die shell
int shell_funkyexit(char **args){
  return 0;

}

int shell_exit(char **args){
  fake();
  return 1;
}
//easteregg
int shell_funk(char **args) {
  while(1) {
    funkyfunk();
    printf("Funk ");
  }
  return 1;
}

int execute(char **args) {

  if(args[0] == NULL) {
    return 1;
  }
  int i;
  for (i=0; i < funk_num_builtins(); i++){
    if(strcmp(args[0], built_in_str[i]) == 0){
      return (*built_in_f[i])(args);
    }
  }
  return shell_launch(args);
}
//haupt Funk
void Funk_loop(void){
  char *line;
  char **args;
  int status;

  do{
    funkyfunk();
    line = bread_line("Funk_shell>>\001\033[1;31m\002 ");
    args = split_line(line);
    status = execute(args);
    free(line);
    free(args);
  } while (status);
}

int main(int argc, char *argv[]){
  srand((unsigned) time(NULL));
  greet();
  Funk_loop();
  greet2();
  return EXIT_SUCCESS;
}
