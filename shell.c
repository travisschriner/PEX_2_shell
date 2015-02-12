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
  char* call = malloc(sizeof(char*));//fix this with strlen and stuff..

  
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
      free(call);
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
	  //recall this command and allocate proper size mem
	  
	  strcpy(call, list_get(history, temp));
	  //adds things to the list
	  history = list_remove(history, call);
	  history = list_insert_head(history, call);	  
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
    //*tested and works*
    //------------------------
     else if(strcmp(token[0], "cd")==0){
	if(token[1] != NULL){
	  if(strncmp(token[1], "~", 1)==0){ //checks for home
	    char* path = getenv("HOME"); //gets my path to ~
	    char* printed_path = token[1]; //creates var for user's inputted path
	    printed_path++;  //advances to the next char (removes ~)
	    char* dir = malloc((strlen(path)+strlen(printed_path)+1)*sizeof(char*)); //allocs proper size mem for full path
	    strcat(dir, path); //adds path from root to ~
	    strcat(dir, printed_path); //adds path from ~ to dir
	    chdir(dir); //chdir to new and proper path!
	    free(dir);
	  }else{
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
     }
    
       
  }//while(exit==0)

  return 0;
}//main
