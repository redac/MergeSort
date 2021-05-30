#ifndef MESURES_H
#define MESURES_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>     /* time */
#include <unistd.h>   /* Unix std */
#include <sys/time.h> /* gettimeofdate */

void demarrer_chrono();
void arreter_chrono();
long int temps_chrono();

#endif