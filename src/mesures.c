/**
 * @file 
 * @brief Util file to define all functions related to time measures.
 * @warning You should not modifie the current file.
 */

#include "mesures.h"

/**
 * @brief Structure used to measure time.
 **/
struct timeval compteur_start, compteur_stop;

void demarrer_chrono()
{
    gettimeofday(&compteur_start, NULL);
}

void arreter_chrono()
{
    gettimeofday(&compteur_stop, NULL);
}

long int temps_chrono()
{
    return (compteur_stop.tv_sec - compteur_start.tv_sec) * 1000000 + compteur_stop.tv_usec - compteur_start.tv_usec;
}