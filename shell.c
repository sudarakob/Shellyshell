#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
// Ruft die Funktionen zum lesen, erkennen von verschiedenen Argumenten und ausführen der commands auf
//brennan.io/2015/01/16/write-a-shell-in-c/
//liest die vom User eingegebene Zeile
#define READLINE_BUFSIZE 1024 //Definiere Buffer in größe von 1024
char *read_line(void){
  int bufsize = READLINE_BUFSIZE;
  int position = 0; //Position beim einlesen
  char *buffer = malloc(sizeof(char) * bufsize); //1024 * größe unseren eingegeben string
  int c; //Zähler

  if (!buffer){
    fprintf(stderr, " Speicherzuordungsfehler \n");//fprintf zum schreiben von Informationen in Dateien
    exit(EXIT_FAILURE);
  }

  while(1){
    //lesen eines einzelnen zeichens
    c = getchar();

    //wenn EOF erreicht wird oder ein zeilenumbruch, wird dieser durch 0 ersetzt
    if (c == EOF || c == '\n'){
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++; //erhöhe position um 1

    if (position >= bufsize) {
      bufsize += READLINE_BUFSIZE;
      buffer[position] = c;
    }
  }
}
/*zerstückelt die vom User eingegebene Zeile in einzelne Argumente
im großen und ganzen wie read_line aufgebaut*/
#define TOKEN_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"
char **split_line(char *line){
  int position = 0, bufsize = TOKEN_BUFSIZE;
  char **tokens = malloc(sizeof(char*) * bufsize);
  char *token;
  if(!tokens) {
    fprintf(stderr," Speicherzuordungsfehler\n");
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
        fprintf(stderr," Speicherzuordungsfehler\n");
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
      perror("FOO_shell:");
    }
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    perror(" FOO_shell:");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
int execute(char **args) {
  if(args[0] == NULL) {
    return 1;
  }
  return shell_launch(args);
}
//eingebaute befehle-----------------------------------------
int shell_help(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
//-----------------------------------------------------------
//Strings die zu den Befehlen gehören
char *built_in_str[] = {
  "help",
  "cd",
  "exit"
};
//ruft die 

void shell_loop(void){
  char *line;
  char **args;
  int status;

  do{
    printf("FOO_shell>> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}
int main(int argc, char *argv[]){
  printf("Wilkommen bei der FOO_shell, tippen sie help für Übersicht\n");  
  shell_loop();
  printf("Danke für ihre Aufmerksamkeit, aufwiedersehen\n");
  return EXIT_SUCCESS;
}
