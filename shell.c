#include "list.h"
#include "shelly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    char** token = malloc(sizeof(char*));
    token[0] = strtok(command,s);
    history = list_remove(history, command);
    history = list_insert_head(history, command);


    //creates an array of char* for my different tokens
    int i = 0;
    while(token[i] != NULL){
      i++;
      token=realloc(token,(i+1)*sizeof(char*));
      token[i]=strtok(NULL,s);
    }
    
    //checks exit condition
    if(strcmp(token[0], "exit")==0){
      exit = 1;
    }//terminate immediately

   
      printf("%s\n", token[0]);//marker
     


      //checks recall
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
	 }//while
	}//else
      }//checks recall

      //checks history
      if(strcmp(token[0], "history")==0){
	if(token[1] != NULL){
	  int temp = atoi(token[1]);
	  //get how many items specified
	}else{
	  //history to the 10th spot if possible.
	}
      }

        //checks CD
      if(strcmp(token[0], "cd")==0){
	if(token[1] != NULL){
	  if(strncmp(token[1], "~", 1)){
	      char* path = getenv("HOME");
	      char* printed_path = token[1];
	      printed_path++;
	      
	      char* dir = malloc((strlen(path)+strlen(printed_path)+1)*sizeof(char*));
	      strcat(dir, path);
	      strcat(dir, printed_path);
	      chdir(dir);

	  }else{
	    chdir(token[1]);
	  }
	 
	}else{
	  printf("Please specify a path");
	}
      }



      //start the execvp stuff...
    
  }//while(exit==0)


  printf("this is where we exit\n");
  list_print(history);
  list_destroy(history);
  return 0;
}//main
