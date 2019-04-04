#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
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
void greet(){
  char greet[] =  "Wilkommen bei der funkingen Funk_shell, tippen sie help für Übersicht\n";
  char *current = strtok(greet, " ");
  for ( ;current != NULL; current= strtok(NULL, " ")){
    funkyfunk();
    printf("%s ", current);
  }

}
//liest die vom User eingegebene Zeile
#define READLINE_BUFSIZE 1024 //Definiere Buffer in größe von 1024
char *read_line(void){
  int bufsize = READLINE_BUFSIZE;
  int position = 0; //Position beim einlesen
  char *buffer = malloc(sizeof(char) * bufsize); //1024 * größe unseren eingegeben string
  int c; //Zähler

  if (!buffer){
    fprintf(stderr, "Funkiger Speicherzuordungsfehler \n");//fprintf zum schreiben von Informationen in Dateien
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
      perror("Funk_shell");
    }
    exit(EXIT_FAILURE);
  } else if(pid < 0) {
    perror(" Funk_shell");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}
//eingebaute befehle-----------------------------------------
int shell_help(char **args);
int shell_cd(char **args);
int shell_exit(char **args);
int shell_funk(char **args);
//-----------------------------------------------------------
//Strings die zu den Befehlen gehören
char *built_in_str[] = {
  "help",
  "cd",
  "exit",
  "funk"
};
int (*built_in_f[]) (char **) = {
  &shell_help,
  &shell_cd,
  &shell_exit,
  &shell_funk
};
int funk_num_builtins(){
  return sizeof(built_in_str)/sizeof(char *);
}
//ruft die funkigen Funktionen
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
int shell_cd(char **args) {

}
int shell_exit(char **args){

}
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
    printf("Funk_shell>> ");
    reset();
    bold_red();
    line = read_line();
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
  printf("Danke für ihre Aufmerksamkeit, aufwiedersehen\n");
  return EXIT_SUCCESS;
}
