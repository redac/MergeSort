/**
 * @file 
 * @brief Implementation by students of usefull function for the system project.
 * @todo Change the SU_removeFile to use exec instead of system.
 */

#include "system_utils.h"

/**
 * @brief Maximum length (in character) for a command line.
 **/
#define SU_MAXLINESIZE (1024 * 8)

/********************** File managment ************/

void SU_removeFile(const char *file)
{
  switch (fork())
  {
  case 0:
  { /* remove the file using exec */
    int resSys = execlp("rm", "rm", file, NULL);
    assert(resSys != -1);
    exit(EXIT_SUCCESS);
    break;
  }

  case -1:
  { /* fork failed */
    fprintf(stderr, "fork failed\n");
    exit(EXIT_FAILURE);
    break;
  }

  default:
  {
    wait(NULL); /* wait for the child process */
    break;
  }
  }

  /* 
  VERY Dirty tmp version 
  char buffer[SU_MAXLINESIZE];
  snprintf(buffer, SU_MAXLINESIZE, "rm %s",file);
  //fprintf(stderr, "%s\n", buffer);
  int resSys = system(buffer); 
  assert(resSys != -1); // TODO : change with err */
}
