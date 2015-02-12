#include "list.h"
#include "shelly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> //for fork/execvp/pipe 
#include <sys/wait.h>  //for fork/execvp

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void) {

  node* history = NULL; //history linked list
  char command[128];
  const char s[2] = " "; //white space deliminator
  char* call = malloc(sizeof(char)*(1+strlen(command)));//fix this with strlen and stuff..
  // int fd[2];
  //pid_t pipe;
  


  
  int exit = 0;
  while(exit == 0){
    printf("PEX_Shell: $ ");
    //parses command, checks the history,
    //then places in the history
    fgets(command, 128, stdin);
    //kills '/n'
    command[strlen(command)-1]='\0';
  
    if(strncmp(command, "recall", 6)){
    history = list_remove(history, command);
    history = list_insert_head(history, command);
    }
    char** token = malloc(sizeof(char)*(1+strlen(command)));
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
    //*tested and works*
    //------------------------
    if(strcmp(token[0], "recall")==0){
  
	if(token[1] == NULL){
	  printf("Please specify a number to recall\n");
	}else{
	  int temp = atoi(token[1]);
	  strcpy(call, list_get(history, temp));
	  history = list_remove(history, call); //adds things to the history list
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


      //------------------------------------------------------------
      //piping...
      //this is a dismal failure...
      //------------------------------------------------------------
      
      //     else if(strcmp(token[1], "|")==0){
      // if(pipe(fd) == -1){
      //	 fprintf(stderr, "Pipe bent...");
      //}
      // pipe = fork();
      //if(pipe>0){
      //	 close(fd[READ_END]);

      // }
      //}
      
      //-------------------------------------------------------------
      //fork
      //*tested and works*
      //stack overflow gave a similar example of using fork properly.
      //it explained how fork worked, and combined with the man,
      //I was able to understand how to get the thing to wait and
      //to kill it in the event of something bad occuring. 
      //-------------------------------------------------------------
     else{
      pid_t pid = fork(); //create a new process
      if(pid == -1){ //fork failed to occure
	printf("Your fork bent...\n");
      }else if(pid>0){
	int status;
	waitpid(pid, &status,0); //status shows how the child died
      }else{
	execvp(token[0], token);
	_exit(EXIT_FAILURE); //this kills the child...in the event the child doesnt kill itself
      }
     }
 }//while(exit==0)
  return 0;
}//main
