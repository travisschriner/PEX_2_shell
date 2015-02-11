#include "shelly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void history(node* history){
  int i=11;
  while(i>0){
    char* temp = list_get(history,i);
    printf("%d: %s\n",i,temp);
    i--;
   
  }

}


void history_x(node* history, int length){

}

void recall(int pos){

}

void cd(char* directory){

}


