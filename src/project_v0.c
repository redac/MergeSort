/**
 * @file 
 * @brief Implementation of the V0 of the system project.
 * @warning You should not modifie the current file.
 */

#include "project_v0.h"

/**
 * @brief Maximum length (in character) for a file name.
 **/
#define PROJECT_FILENAME_MAX_SIZE 1024

/**
 * @brief Type of the sort algorithm used in this version.
 **/
//#define SORTALGO(nb_elem, values) SU_ISort(nb_elem, values)
//#define SORTALGO(nb_elem, values) SU_HSort(nb_elem, values)
#define SORTALGO(nb_elem, values) SU_QSort(nb_elem, values)

/**********************************/

void projectV0(const char * i_file, const char * o_file, unsigned long nb_split){

  /* Get number of line to sort */
  int nb_print = 0;
  unsigned long nb_lines = SU_getFileNbLine(i_file);
  unsigned long nb_lines_per_files = nb_lines/ (unsigned long) nb_split;
  fprintf(stderr,
	  "Projet1 version with %lu split of %lu lines\n",
	  nb_split,
	  nb_lines_per_files);

  /* 0 - Deal with case nb_split = 1 */
  if(nb_split < 2){
    int * values = NULL;
    unsigned long nb_elem = SU_loadFile(i_file, &values);

    SORTALGO(nb_elem, values);

    SU_saveFile(o_file, nb_elem, values);
    
    free(values);
    return;
  }

  /* 1 - Spit the source file */

  /* 1.1 - Create a vector of target filenames for the split */
  char ** filenames = (char**) malloc(sizeof(char*) * (size_t) nb_split);
  char ** filenames_sort = (char**) malloc(sizeof(char*) * (size_t) nb_split);
  unsigned long cpt = 0;
  for(cpt = 0; cpt < nb_split; ++cpt){
    filenames[cpt] = (char *) malloc(sizeof(char) * PROJECT_FILENAME_MAX_SIZE);
    nb_print = snprintf(filenames[cpt],
			PROJECT_FILENAME_MAX_SIZE,
			"/tmp/tmp_split_%d_%lu.txt",getpid(), cpt);
    if(nb_print >= PROJECT_FILENAME_MAX_SIZE){
      err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__ );
    }

    filenames_sort[cpt] = (char *) malloc(sizeof(char) * PROJECT_FILENAME_MAX_SIZE);
    nb_print = snprintf(filenames_sort[cpt],
			PROJECT_FILENAME_MAX_SIZE,
			"/tmp/tmp_split_%d_%lu.sort.txt",getpid(), cpt);
    if(nb_print >= PROJECT_FILENAME_MAX_SIZE){
      err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__ );
    }
  }

  /* 1.2 - Split the source file */
  SU_splitFile2(i_file,
		nb_lines_per_files,
		nb_split,
		(const char **) filenames
		);

  /* 2 - Sort each file */
  projectV0_sortFiles(nb_split, (const char **) filenames, (const char **) filenames_sort);

  /* 3 - Merge (two by two) */
  projectV0_combMerge(nb_split, (const char **) filenames_sort, (const char *) o_file);

  /* 4 - Clear */
  for(cpt = 0; cpt < nb_split; ++cpt){
    free(filenames[cpt]); // not needed :  clear in sort
    free(filenames_sort[cpt]);
  }

  free(filenames);
  free(filenames_sort);

}

void projectV0_sortFiles(unsigned long nb_split, const char ** filenames, const char ** filenames_sort){

  unsigned long cpt = 0;
  for(cpt = 0; cpt < nb_split; ++cpt){
    int * values = NULL;
    unsigned long nb_elem = SU_loadFile(filenames[cpt], &values);
    SU_removeFile(filenames[cpt]);
    fprintf(stderr, "Inner sort %lu: Array of %lu elem by %d\n", cpt, nb_elem, getpid());

    SORTALGO(nb_elem, values);

    SU_saveFile(filenames_sort[cpt], nb_elem, values);
    free(values);
  }
}

void projectV0_combMerge(unsigned long nb_split, const char ** filenames_sort, const char * o_file){

  int nb_print = 0;
  unsigned long cpt = 0;

  char previous_name [PROJECT_FILENAME_MAX_SIZE];
  nb_print = snprintf(previous_name,
		      PROJECT_FILENAME_MAX_SIZE,
		      "%s", filenames_sort[0]);
  if(nb_print >= PROJECT_FILENAME_MAX_SIZE){
    err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__ );
  }

  char current_name [PROJECT_FILENAME_MAX_SIZE];
  nb_print = snprintf(current_name,
		      PROJECT_FILENAME_MAX_SIZE,
		      "/tmp/tmp_split_%d_merge_%d.txt", getpid(), 0);
  if(nb_print >= PROJECT_FILENAME_MAX_SIZE){
    err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__ );
  }

  for(cpt = 1; cpt < nb_split - 1; ++cpt){

    fprintf(stderr, "Merge sort %lu : %s + %s -> %s \n",
	    cpt,
	    previous_name,
	    filenames_sort[cpt],
	    current_name);
    SU_mergeSortedFiles(previous_name,
			filenames_sort[cpt],
			current_name);
    SU_removeFile(previous_name);
    SU_removeFile(filenames_sort[cpt]);

    nb_print = snprintf(previous_name,
			PROJECT_FILENAME_MAX_SIZE,
			"%s", current_name);
    if(nb_print >= PROJECT_FILENAME_MAX_SIZE){
      err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__ );
    }

    nb_print = snprintf(current_name,
			PROJECT_FILENAME_MAX_SIZE,
			"/tmp/tmp_split_%d_merge_%lu.txt",getpid(), cpt);
    if(nb_print >= PROJECT_FILENAME_MAX_SIZE){
      err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__ );
    }
  }

  /* Last merge */
  fprintf(stderr, "Last merge sort : %s + %s -> %s \n",
	  previous_name,
	  filenames_sort[nb_split - 1],
	  o_file);
  SU_mergeSortedFiles(previous_name,
		      filenames_sort[nb_split - 1],
		      o_file);
  SU_removeFile(previous_name);
  SU_removeFile(filenames_sort[nb_split - 1]);

}
