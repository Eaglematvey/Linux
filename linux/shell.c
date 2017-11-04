#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int help(char **args);
int exit_shell(char **args);
int cur_time(char **args);
int execve_func(char **args);
int ncat(char **args);
int fileswap(char **args);
char *str_array[] = {
  "help",
  "curtime",
  "ncat",
"execve",
"fileswap",
  "exit"
};



int (*func_array[]) (char **) = {
  &help,
  &cur_time,
  &ncat,
  &execve_func,
  &fileswap,
  &exit_shell
};

char* shell_read_line(void)
{
  int bufsize = 1024;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;
  while (1) {
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;
  }
}

int help(char **args)
{
  printf("Extended shell");
  printf("\n");
  return 0;
}
int ncat(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, " expected argument to ncat");
  } else {
     long l = strtol(args[1], (char **)NULL, 10);
     while(1){
     char* line = shell_read_line();
     if(strcmp(line,"end")==0)
        break;
     for(long i =0;i<l;i++){
        printf("%s",line);
        printf("\n");
      }
    }
  }
  return 0;
}

int exit_shell(char **args)
{
  printf("Exitting.....\n");
  return 1;
}

int execve_func(char **args){
    //int my_pid;
    int my_pid =fork();
    if(my_pid==0){
     if(execve(args[1],&(args[1]),NULL)==-1){
       perror("Execve error");
       exit(EXIT_FAILURE);
     }
      exit(EXIT_SUCCESS);
    }
    wait(0);
    return 0;
}

int cur_time(char **args){
    system("date +'%T'");
    return 0;
}
int shell_launch(char **args,char* line,char **args2,char* line2)
{
  pid_t pid;
  int status;

  pid = fork();
  if(pid==0){
  	find_command(args,line);
        exit(EXIT_SUCCESS);
   }
  else {
        find_command(args2,line2);
   }
  wait(0);
  return 0;
}
int find_command(char** args,char* line){
for (int i = 0; i < sizeof(str_array) / sizeof(char *); i++) {
    if (strcmp(args[0], str_array[i]) == 0) {
      return (*func_array[i])(args);
    }
  }  
return -1;
}

char **shell_split_line(char *line)
{
  int bufsize = 64, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    tokens[position] = token;
    position++;

    token = strtok(NULL, " \t\r\n\a");
  }
  tokens[position] = NULL;
  return tokens;
}

int shell_execute(char* line,char **args)
{

  if (args[0] == NULL) {
    return 0;
  }
  if (strcmp(args[0], "parallel") == 0) {
        char* line2 = shell_read_line();
        char** args2 = shell_split_line(line2);
       return shell_launch(&(args[1]),&(line[9]),args2,line2);
  }else{
  	int command = find_command(args,line);
        if(command==-1)
           printf("Unknown command\n");
        return command;
  }    
  
}


int main(int argc, char **argv)
{
  char *line;
  char **args;
  int break_cycle = 0;
  //printf("%d",argc);
  if(argc>1){
      //char * line = NULL;
      size_t len = 0;
      size_t fp;
      ssize_t read;
      for(int i=1;i<argc;i++){
          //printf("%s\n",argv[i]);
          if(strcmp(argv[i],"-c")==0&&break_cycle==0)
             {
                break_cycle=1;
             }
           else if(break_cycle==1)
             {
                 printf("command from args:%s\n", argv[i]);
                 args = shell_split_line(argv[i]);
                 shell_execute(line,args);
                 free(args);
             } else{
                  fp = fopen(argv[i], "r");
		    if (fp == NULL)
			exit(EXIT_FAILURE);

                 while ((read = getline(&line, &len, fp)) != -1) {
		 printf("command from file:%s", line); 
                 args = shell_split_line(line);
                 shell_execute(line,args);
                 free(args);
	         }
                 close(fp);
                 
              }
           
      }
   }
   else while (break_cycle==0) {
    printf("$ ");
    line = shell_read_line();
    args = shell_split_line(line);
    if(shell_execute(line,args)==1)
       break_cycle=1;
    free(line);
    free(args);
  }

  return EXIT_SUCCESS;
}
