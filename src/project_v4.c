/**
 * @file 
 * @brief Implementation of the V3 of the system project.
 * @warning You should not modify the current file.
 */

#include "project_v4.h"
#include "mesures.h"

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
void projectV4(const char *i_file, const char *o_file, unsigned long nb_split)
{
    long int temps_cumul;
    long int usec;
    long int msec;
    long int sec;

    /* Get number of line to sort */
    demarrer_chrono();
    int nb_print = 0;
    unsigned long nb_lines = SU_getFileNbLine(i_file);
    unsigned long nb_lines_per_files = nb_lines / (unsigned long)nb_split;
    fprintf(stderr,
            "Projet4 version with %lu split of %lu lines\n",
            nb_split,
            nb_lines_per_files);

    /* 0 - Deal with case nb_split = 1 */
    if (nb_split < 2)
    {
        int *values = NULL;
        unsigned long nb_elem = SU_loadFile(i_file, &values);

        SORTALGO(nb_elem, values);

        SU_saveFile(o_file, nb_elem, values);

        free(values);
        return;
    }

    /* 1 - Split the source file */

    /* 1.1 - Create a vector of target filenames for the split */
    char **filenames = (char **)malloc(sizeof(char *) * (size_t)nb_split);
    char **filenames_sort = (char **)malloc(sizeof(char *) * (size_t)nb_split);
    unsigned long cpt = 0;
    for (cpt = 0; cpt < nb_split; ++cpt)
    {
        filenames[cpt] = (char *)malloc(sizeof(char) * PROJECT_FILENAME_MAX_SIZE);
        nb_print = snprintf(filenames[cpt],
                            PROJECT_FILENAME_MAX_SIZE,
                            "/tmp/tmp_split_%d_%lu.txt", getpid(), cpt);
        if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
        {
            err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
        }

        filenames_sort[cpt] = (char *)malloc(sizeof(char) * PROJECT_FILENAME_MAX_SIZE);
        nb_print = snprintf(filenames_sort[cpt],
                            PROJECT_FILENAME_MAX_SIZE,
                            "/tmp/tmp_split_%d_%lu.sort.txt", getpid(), cpt);
        if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
        {
            err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
        }
    }

    /* 1.2 - Split the source file */
    SU_splitFile2(i_file,
                  nb_lines_per_files,
                  nb_split,
                  (const char **)filenames);
    arreter_chrono();
    temps_cumul = temps_chrono();
    usec = temps_cumul % 1000;
    msec = (temps_cumul / 1000) % 1000;
    sec = temps_cumul / 1000000;
    printf("Split time : (s:ms:us) %ld:%ld:%ld\n", sec, msec, usec);

    /* 2 - Sort each file in parallel */
    demarrer_chrono();
    projectV4_sortFiles(nb_split, (const char **)filenames, (const char **)filenames_sort);
    arreter_chrono();
    temps_cumul = temps_chrono();
    usec = temps_cumul % 1000;
    msec = (temps_cumul / 1000) % 1000;
    sec = temps_cumul / 1000000;

    printf("Sort time : (s:ms:us) %ld:%ld:%ld\n", sec, msec, usec);

    /* 3 - Merge (two by two) using a tournament tree */
    demarrer_chrono();
    projectV4_treeMerge(nb_split, (const char **)filenames_sort, (const char *)o_file);
    arreter_chrono();
    temps_cumul = temps_chrono();
    usec = temps_cumul % 1000;
    msec = (temps_cumul / 1000) % 1000;
    sec = temps_cumul / 1000000;

    printf("Merge time : (s:ms:us) %ld:%ld:%ld\n", sec, msec, usec);

    /* 4 - Clear */
    for (cpt = 0; cpt < nb_split; ++cpt)
    {
        free(filenames[cpt]); // not needed :  clear in sort
        free(filenames_sort[cpt]);
    }

    free(filenames);
    free(filenames_sort);
}

void projectV4_sortFiles(unsigned long nb_split, const char **filenames, const char **filenames_sort)
{

    unsigned long cpt = 0;
    for (cpt = 0; cpt < nb_split; ++cpt)
    {
        switch (fork()) /* create nb_split processes, each process will sort a sub-file */
        {
        case 0:
        {
            int *values = NULL;
            unsigned long nb_elem = SU_loadFile(filenames[cpt], &values);
            SU_removeFile(filenames[cpt]);
            fprintf(stderr, "Inner sort %lu: Array of %lu elem by %d\n", cpt, nb_elem, getpid());

            SORTALGO(nb_elem, values);

            SU_saveFile(filenames_sort[cpt], nb_elem, values);
            free(values);
            exit(EXIT_SUCCESS);
            break;
        }

        case -1:
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        default:
            break;
        }
    }
    for (cpt = 0; cpt < nb_split; ++cpt)
    {
        wait(NULL); /* wait for all child processes to finish. */
    }
}
void projectV4_combMerge(unsigned long nb_split, const char **filenames_sort, const char *o_file)
{

    int nb_print = 0;
    unsigned long cpt = 0;
    char previous_name[PROJECT_FILENAME_MAX_SIZE];
    switch (fork()) /* create a child process, which will merge half the sub-files */
    {
    case 0: /* child process code: merge half the sub-files */
    {
        nb_print = snprintf(previous_name,
                            PROJECT_FILENAME_MAX_SIZE,
                            "%s", filenames_sort[0]);
        if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
        {
            err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
        }

        char current_name[PROJECT_FILENAME_MAX_SIZE];
        nb_print = snprintf(current_name,
                            PROJECT_FILENAME_MAX_SIZE,
                            "/tmp/tmp_split_%d_merge_%d.txt", getpid(), 0);
        if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
        {
            err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
        }
        for (cpt = 1; cpt < (nb_split / 2 - 1); ++cpt)
        {
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
            if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
            {
                err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
            }

            nb_print = snprintf(current_name,
                                PROJECT_FILENAME_MAX_SIZE,
                                "/tmp/tmp_split_%d_merge_%lu.txt", getpid(), cpt);
            if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
            {
                err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
            }
        }
        /* Last merge of the child process*/
        fprintf(stderr, "Last merge sort of the child process : %s + %s -> %s \n",
                previous_name,
                filenames_sort[(nb_split / 2) - 1],
                "/tmp/tmp_split_merge_fils.txt");
        SU_mergeSortedFiles(previous_name,
                            filenames_sort[(nb_split / 2) - 1],
                            "/tmp/tmp_split_merge_fils.txt");
        SU_removeFile(previous_name);
        SU_removeFile(filenames_sort[(nb_split / 2) - 1]);
        exit(EXIT_SUCCESS);
        break;
    }

    default: /* parent process code: merge the other half */
    {
        nb_print = snprintf(previous_name,
                            PROJECT_FILENAME_MAX_SIZE,
                            "%s", filenames_sort[(nb_split / 2)]);
        if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
        {
            err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
        }

        char current_name[PROJECT_FILENAME_MAX_SIZE];
        nb_print = snprintf(current_name,
                            PROJECT_FILENAME_MAX_SIZE,
                            "/tmp/tmp_split_%d_merge_%d.txt", getpid(), 0);
        if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
        {
            err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
        }
        for (cpt = (nb_split / 2) + 1; cpt < nb_split - 1; ++cpt)
        {
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
            if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
            {
                err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
            }

            nb_print = snprintf(current_name,
                                PROJECT_FILENAME_MAX_SIZE,
                                "/tmp/tmp_split_%d_merge_%lu.txt", getpid(), cpt);
            if (nb_print >= PROJECT_FILENAME_MAX_SIZE)
            {
                err(1, "Out of buffer (%s:%d)", __FILE__, __LINE__);
            }
        }
        /* Last merge of the parent process */
        fprintf(stderr, "Last merge sort of the parent process: %s + %s -> %s \n",
                previous_name,
                filenames_sort[nb_split - 1],
                "/tmp/tmp_split_merge_pere.txt");
        SU_mergeSortedFiles(previous_name,
                            filenames_sort[nb_split - 1],
                            "/tmp/tmp_split_merge_pere.txt");
        SU_removeFile(previous_name);
        SU_removeFile(filenames_sort[nb_split - 1]);
    }
    }
    wait(NULL); /* Wait for the child process */

    /* Last merge of the parent and child files */
    fprintf(stderr, "Last merge sort : %s + %s -> %s \n",
            "/tmp/tmp_split_merge_fils.txt",
            "/tmp/tmp_split_merge_pere.txt",
            o_file);
    SU_mergeSortedFiles(
        "/tmp/tmp_split_merge_fils.txt",
        "/tmp/tmp_split_merge_pere.txt",
        o_file);
    SU_removeFile("/tmp/tmp_split_merge_fils.txt");
    SU_removeFile("/tmp/tmp_split_merge_pere.txt");
}

void projectV4_treeMerge(unsigned long nb_split, const char **filenames_sort, const char *o_file)
{
    pid_t p1;
    pid_t p2;
    char child_name[PROJECT_FILENAME_MAX_SIZE];
    char parent_name[PROJECT_FILENAME_MAX_SIZE];

    if (nb_split < 9) /* Base Case: the merge can be done using one process */
    {
        projectV4_combMerge(nb_split, filenames_sort, o_file); /* Use the v2 sort-merge algorithm (using parallelization) */
    }
    else
    {
        p1 = fork();
        switch (p1)
        {
        case 0:
        {
            snprintf(child_name,
                     PROJECT_FILENAME_MAX_SIZE,
                     "/tmp/tmp_split_%d.txt", getpid());
            projectV4_treeMerge((nb_split + 1) / 2, filenames_sort, child_name);
            exit(EXIT_SUCCESS);
            break;
        }

        case -1:
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
            break;
        }

        default:
        {
            p2 = fork();
            switch (p2)
            {
            case 0:
            {
                snprintf(parent_name,
                         PROJECT_FILENAME_MAX_SIZE,
                         "/tmp/tmp_split_%d.txt", getpid());
                projectV4_treeMerge((nb_split) / 2, filenames_sort + ((nb_split + 1) / 2), parent_name);
                exit(EXIT_SUCCESS);
                break;
            }

            case -1:
            {
                perror("fork failed");
                exit(EXIT_FAILURE);
                break;
            }
            }
            break;
        }
        }
        /* wait for all the children */
        wait(NULL);
        wait(NULL);
        snprintf(parent_name,
                 PROJECT_FILENAME_MAX_SIZE,
                 "/tmp/tmp_split_%d.txt", p1);
        snprintf(child_name,
                 PROJECT_FILENAME_MAX_SIZE,
                 "/tmp/tmp_split_%d.txt", p2);

        /* Last merge */
        SU_mergeSortedFiles(
            child_name,
            parent_name,
            o_file);
        SU_removeFile(child_name);
        SU_removeFile(parent_name);
    }
}