/**
 * @file 
 * @brief Implementation by students of usefull function for the system project.
 * @todo Change the SU_removeFile to use exec instead of system.
 */


#include "system_utils.h"

/**
 * @brief Maximum length (in character) for a command line.
 **/
#define SU_MAXLINESIZE (1024*8) 

/********************** File managment ************/

void SU_removeFile(const char * file){
  int status; 
  pid_t PID = fork();
  if( PID == -1){
    // Fork failed.
    exit(EXIT_FAILURE);
  }
  else if ( PID == 0 )
  {
    // Remove the file.
    execl("usr/bin/bash", "bash", "-c", strncat("rm", file, SU_MAXLINESIZE), (char *) NULL);
  }
  else
  {
    waitpid(PID, &status, 0);
  }
  /* 
  VERY Dirty tmp version 
  char buffer[SU_MAXLINESIZE];
  snprintf(buffer, SU_MAXLINESIZE, "rm %s",file);
  //fprintf(stderr, "%s\n", buffer);
  int resSys = system(buffer); 
  assert(resSys != -1); // TODO : change with err */
}
