#ifndef BUFFER_H
#define BUFFER_H

#include "utilities.h"
/*
#include "fiume.h"
#include "rana.h"
#include "avvia.h"
#include "coccodrilli.h"
#include "campo.h"
#include "punteggio.h"
#include "menu.h"
*/

extern Buffer b;

Thread leggi(pthread_mutex_t* mutex);
void scrivi(pthread_mutex_t* mutex, Thread boh);

#endif 