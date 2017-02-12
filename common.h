#ifndef STRUCT_H
#define STRUCT_H

#define DEBUG
#ifdef DEBUG
#define dbg(MESSAGE,args...) { \
  const char *A[] = {MESSAGE}; \
  printf("[DEBUG] %s -> %s(line %d)",__FILE__, __func__,__LINE__); fflush(stdout);\
  if(sizeof(A) > 0) {\
    printf(": "); \
    printf(*A,##args); \
  } else {\
    printf("\n"); \
  }\
}
#else
#define dbg(MESSAGE,args...) ;
#endif


#include <errno.h>

#define MAX_HOTEL       100
#define MAX_CHAMBRE     100
#define MAX_DEMANDE     100
#define MAX_CHARP_SIZE  60                 /* 99 caratères max + '\0' */

/* Chemin d'emplacement des fichier binnaires, doit se terminer par "/" */
#define DIR_FILE        "/home/maxime/"

#define TYPE_SIMPLE     1
#define TYPE_DOUBLE     2
#define TYPE_TWIN       3

#define SERVER_PORT     "1235"

typedef struct Demande_s {
    char hotelName[MAX_CHARP_SIZE];
    char nomClient[MAX_CHARP_SIZE];
    char date[11];
    int numeroChambre;
} Demande;

typedef struct Chambre_s {
    int numero;
    int prix;
    int type;
    int nbDemande;
    Demande demandeChambre[MAX_CHAMBRE];
} Chambre;

typedef struct Hotel_s {
    char nom[MAX_CHARP_SIZE];
    int nbEtoile;
    int nbChabmbre;
    Chambre chambre[MAX_CHAMBRE];
    char ville[MAX_CHARP_SIZE];
} Hotel;

#endif /* STRUCT_H */