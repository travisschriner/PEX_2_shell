#include "list.h"
#include "shelly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

  node* history = NULL;
  char command[128];
  const char s[2] = " ";

  
  int exit = 0;
  while(exit == 0){
    printf("PEX_Shell: $ ");
    //parses command, checks the history,
    //then places in the history
    fgets(command, 128, stdin);
    //kills '/n'
    command[strlen(command)-1]='\0';

    //TODO: figure out how to not store recall...
    if(strncmp(command, "recall", 6)){
    history = list_remove(history, command);
    history = list_insert_head(history, command);
    }
    char** token = malloc(sizeof(char*));
    token[0] = strtok(command,s);

    //creates an array of char* for my different tokens
    int i = 0;
    while(token[i] != NULL){
      i++;
      token=realloc(token,(i+1)*sizeof(char*));
      token[i]=strtok(NULL,s);
    }
    
    //------------------------
    //exit 
    //------------------------
    if(strcmp(token[0], "exit")==0){
      exit = 1;
      list_destroy(history);
      free(token);
      return 0;
    }//terminate immediately

   
    //------------------------
    //recall
    //------------------------

    if(strcmp(token[0], "recall")==0){
  
	if(token[1] == NULL){
	  printf("Please specify a number to recall");
	}else{

	  int temp = atoi(token[1]);
	  //recall this command 
	  char* call = list_get(history, temp);
	  free(token);
	  token = malloc(sizeof(char*));
	  token[0] = strtok(call,s);
	  int i = 0;
	  while(token[i] != NULL){
	    i++;
	    token=realloc(token,(i+1)*sizeof(char*));
	    token[i]=strtok(NULL,s);
	 }
	}
      }


      //------------------------
      //history
      //*tested and works*
      //------------------------
      if(strcmp(token[0], "history")==0){
	if(token[1] != NULL){ //checks for history x
	  int temp = atoi(token[1]); //turns ASCII to int
	  list_printx(history, temp);
	}else{
	  list_print10(history);	   
	}
      }
  


    //------------------------
    //CD
    //------------------------
     else if(strcmp(token[0], "cd")==0){
       printf("entered CD\n");
	if(token[1] != NULL){
	  printf("entered loop!\n");
	  if(strncmp(token[1], "~", 1)==0){ //checks for home
	    printf("checks for ~\n");
	    char* path = getenv("HOME"); //gets my path to ~
	    printf("Home Path: %c\n", *path);
	    char* printed_path = token[1]; //creates var for user's inputted path
	    printf("user path: %c\n", *printed_path);
	    printed_path++;  //advances to the next char (removes ~)  
	    printf("edited user path: %c\n",*printed_path);
	    char* dir = malloc((strlen(path)+strlen(printed_path)+1)*sizeof(char*)); //allocs proper size mem for full path
	    strcat(dir, path); //adds path from root to ~
	    printf("dir: %c\n", *dir);
	    strcat(dir, printed_path); //adds path from ~ to dir
	    printf("dir: %c\n", *dir);
	    chdir(dir); //chdir to new and proper path!
	  }else{
	    printf("~ failed\n");
	    chdir(token[1]); //should work, right?
	  }	 
	}else{
	  printf("Please specify a path\n");
	}
      }



      //got this from:
      //http://stackoverflow.com/questions/19099663/how-to-correctly-use-fork-exec-wait
     else{
       //pid_t parent = getpid();
      pid_t pid = fork();

      if(pid == -1){
	printf("Your fork bent...");
      }else if(pid>0){
	int status;
	waitpid(pid, &status,0);
      }else{
	execvp(token[0], token);
	_exit(EXIT_FAILURE);
      }
      //start the execvp stuff...
      //execvp(char*, char*[token])
     }
    
  }//while(exit==0)

  return 0;
}//main
