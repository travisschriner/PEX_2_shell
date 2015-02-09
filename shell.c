#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  //  node* history = NULL;
  char command[128];
  const char s[2] = " ";
  char *token;
  
  int exit = 0;
  while(exit == 0){
    printf("PEX_Shell: $ ");
    
    fgets(command, 128, stdin);

    token = strtok(command,s);
    
    //checks exit condition
    if(strcmp(token, "exit\n")==0){
      exit = 1;
    }

    while(token != NULL){
      printf("%s\n", token);


      token = strtok(NULL,s);
    }
    
    
    // list_insert_head(history, command);
    //list_print(history);


  }



  return 0;
}
