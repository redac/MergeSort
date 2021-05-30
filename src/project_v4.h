/**
 * @file
 * @brief V4 of the system project.
 */

#ifndef PROJECT_V4_H
#define PROJECT_V4_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>   /* time */
#include <unistd.h> /* Unix std */

#include "system_utils.h"

/**
 * @brief Function to sort a file using an external sort and nb_split subfiles.
 * @param[in] i_file Name of the file to process.
 * @param[in] o_file Name of the output sorted file.
 * @param[in] nb_split Number of subfiles to use for the external sort.
 * @note With parallelism of the fusion of two sub-files "en cascade" instead of one.
 **/
void projectV4(const char *i_file, const char *o_file, unsigned long nb_split);

/**
 * @brief Function to sort a temporary subfile and remove it.
 * @param[in] nb_split Index of the subfile in the array of files.
 * @param[in] filenames Array of file to sort names.
 * @param[in] filenames_sort Array of sorted file names.
 **/
void projectV4_sortFiles(unsigned long nb_split,
                         const char **filenames,
                         const char **filenames_sort);

/**
 * @brief Function to sort-merge a list of sorted subfiles using parallelization.
 * @param[in] nb_split Index of the subfile in the array of files.
 * @param[in] filenames_sort Array of sorted file names.
 * @param[in] o_file Nome of the output file where sorted data are written.
 * @note It work in stream. Files are not fully loaded in memory.
 **/
void projectV4_combMerge(unsigned long nb_split,
                         const char **filenames_sort,
                         const char *o_file);

/**
 * @brief Function to sort-merge a list of sorted subfiles using a tournament tree.
 * @param[in] nb_split Index of the subfile in the array of files.
 * @param[in] filenames_sort Array of sorted file names.
 * @param[in] o_file Nome of the output file where sorted data are written.
 * @note It work in stream. Files are not fully loaded in memory.
 **/
void projectV4_treeMerge(unsigned long nb_split,
                         const char **filenames_sort,
                         const char *o_file);

#endif
