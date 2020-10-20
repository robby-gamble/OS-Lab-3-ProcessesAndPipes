#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main(int argc, char **argv){
 
  int pipefd[2];
  int pipefd2[2];
  int pid;
  
  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};
  char *sort_args[] = {"sort", NULL,NULL};
  
  //creating a pipe
  pipe(pipefd);
  pipe(pipefd2);
  pid = fork();
  
  if (pid==0){ 
    //Cat out should be input for grep
    
    //This makes it so the output of cat will be input to grep
    dup2(pipefd[1],1);
    
    //Closing unused parts of pipes.Closing
    close(pipefd[0]);
    close(pipefd2[1]);
    close(pipefd2[0]);
    close(pipefd[1]);
    
    
    execvp(*cat_args, cat_args);
  }
  //If the parent pid > 0
  else{
    if(fork()==0){
      //Child is going to execute grep 
      dup2(pipefd[0],0); //This makes it so the output of grep will be the input to sort
      
      dup2(pipefd2[1],1); //Grep is between two pipes
      
      //closing unused parts of pipes
      close(pipefd2[0]);
      close(pipefd[1]);
      close(pipefd2[1]);
      close(pipefd[0]);
      
      execvp(*grep_args,grep_args);
    }
    
    else{
      //The parent 
      
      if(fork() ==0){
        //Child is going to execute sort
        dup2(pipefd2[0],0);
        
        //Closing unused parts of pipes
        close(pipefd2[1]);
        close(pipefd[1]);
        close(pipefd[0]);
        close(pipefd2[0]);
        
        execvp(*sort_args,sort_args);
      }
    }
  }
  
  close(pipefd2[1]);
  close(pipefd[1]);
  close(pipefd[0]);
  close(pipefd2[0]);
  
  //Waiting for the children processes.
  wait(NULL);
  wait(NULL);
  wait(NULL);
}