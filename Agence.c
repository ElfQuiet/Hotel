#include <string.h>
#include "Agence.h"
#include "main.h"

/* Definition of private function of Agance ----------------------------------*/


char* getPath(const char* filename);
/**
 * Write data to file
 * @param filename
 * @param toWrite
 * @param size
 * @param nbElement
 * @param mode
 * @return  0 if ok
 *         -1 if error occured
 */
int writeToFile(const char* filename, const void *toWrite, size_t size, int nbElement, const char *mode);
Hotel *getHotelByName(const char* hotelname);

/* Code implementation of function -------------------------------------------*/

Demande *getResaByClientName(const char* clientName, int *nbResaFound) {
    FILE * file;
    Hotel *hotel = malloc(sizeof (Hotel));
    Demande *demande = NULL;
    int ret, i, j, nbResa;
    
    nbResa = 0;

    /* Check memory allocation */
    if (!hotel) {
        dbg("Erreur : Allocation d'un Hotel impossible\n");
        return NULL;
    }

    file = fopen(getPath("hotel.bin"), "a+b");

    /* En cas d'erreur */
    if (!file) {
        dbg("Erreur : Impossible d'ouvrir le fichier hotel.bin\n");
        return NULL;
    }

    /* Lire structure */
    do {
        memset(hotel, 0, sizeof (Hotel));
        ret = fread(hotel, sizeof (Hotel), 1, file);
        if (ret == 1) {
            for (i = 0; i < hotel->nbChabmbre ; i++) {
                for (j = 0; j < hotel->chambre[i].nbDemande; j++) {
                    if (strcmp(hotel->chambre[i].demandeChambre[j].nomClient, clientName) == 0) {
                        dbg("  -> Date : %s , Hotel : %s, Chambre : %d\n", hotel->chambre[i].demandeChambre[j].date, hotel->nom, hotel->chambre[i].numero);
                        nbResa++;
                        demande = realloc(demande, sizeof(Demande) * nbResa);
                        if(demande == NULL) {
                            dbg("Erreur d'allocation mémoire Demande x %d", nbResa);
                            *nbResaFound = 0;
                            free(hotel);
                            return NULL;
                        }
                        memcpy(&demande[nbResa -1], &hotel->chambre[i].demandeChambre[j], sizeof(Demande));
                    }
                }
            }
        }
    } while (ret == 1);

    /* Aucune valeur renvoyé donc on free la mémoire et return NULL*/
    free(hotel);
    *nbResaFound = nbResa;
    return demande;


}

int createReservation(const char* hotelName, const char* date, int typeChambre, const char* nomClient) {
    Hotel *hdest;
    Demande *ddem;
    int i, j, result, chmNum, ret, isTaken, roomDispo, freeResa;

    hdest = getHotelByName(hotelName);

    if (!hdest) {
        dbg("Erreur d'allocation mémoire\n");
        return -1;
    }

    roomDispo = -45;
    for (i = 0; i < hdest->nbChabmbre; i++) {

        if (hdest->chambre[i].type == typeChambre) {

            isTaken = 0;
            freeResa = 0;

            for (j = 0; j < hdest->chambre[i].nbDemande; j++) {
                if (strcmp(hdest->chambre[i].demandeChambre[j].date, date) == 0) {
                    isTaken = 1;
                }

                if (strcmp(hdest->chambre[i].demandeChambre[j].date, "00/00/0000") == 0) {
                    freeResa = j;
                }
            }

            if ((isTaken == 0) && (freeResa > 0)) {

                roomDispo = i;
                i = hdest->nbChabmbre;
                dbg("Resa room : %d\n", roomDispo);

            }
        }
    }


    if (roomDispo > -1) {
        strcpy(hdest->chambre[roomDispo].demandeChambre[freeResa].hotelName, hdest->nom);
        strcpy(hdest->chambre[roomDispo].demandeChambre[freeResa].date, date);
        hdest->chambre[roomDispo].demandeChambre[freeResa].numeroChambre = roomDispo;
        strcpy(hdest->chambre[roomDispo].demandeChambre[freeResa].nomClient, nomClient);

        ret = deleteHotel(hdest->nom);
        if (ret != 0) {
            dbg("Erreur : Cannot delet Hotel : %s", hdest->nom);
            return -2;
        }

        ret = writeToFile("hotel.bin", hdest, sizeof (Hotel), 1, "a+b");
        if (ret != 0) {
            dbg("Erreur : Cannot add Hotel : %s", hdest->nom);
            return -3;
        }


        return roomDispo;
    } else {
        return -4;
    }
}

Hotel *rechercherHotel(const char *ville,
        int nbEtoile,
        int typeChambre,
        int *retNbHotel,
        const char *date) {

    FILE *file;
    Hotel *hotel = malloc(sizeof (Hotel));
    Hotel *retHotel = malloc(sizeof (Hotel) * MAX_HOTEL);
    Demande *demande = malloc(sizeof (Demande));
    int ret;
    int nbHotelFinal = 0;

    if (!demande) {
        dbg("Erreur d'allocation mémoire d'une réservatoion\n");
        goto err;
    }

    if (!hotel) {
        dbg("Erreur : Allocation d'un Hotel impossible\n");
        goto err;
    }

    if (!retHotel) {
        dbg("Erreur allocation mémoire de %s Hotels, total size : %d octets", MAX_HOTEL, sizeof (Hotel) * MAX_HOTEL);
        goto err;
    }

    /* Open hotel.bin in reading mode */
    file = fopen(getPath("hotel.bin"), "a+b");
    /* En cas d'erreur */

    if (!file) {
        dbg("Erreur : Impossible d'ouvrir le fichier hotel.bin\n");
        goto err;
    }

    /* Lire structure */
    int compteurChambre;
    int compteurDemande;
    int l_retNbHotel = 0;
    int ok;
    do {
        /* Pour tout les Hotels */
        memset(hotel, 0, sizeof (Hotel));
        ret = fread(hotel, sizeof (Hotel), 1, file);
        if (ret == 1) {
            if ((strcmp(hotel->ville, ville) == 0)&&(hotel->nbEtoile == nbEtoile)) {
                ok = 1;
                /* Pour toute les chambre d'un Hotel */
                for (compteurChambre = 0; compteurChambre < hotel->nbChabmbre; compteurChambre++) {

                    /* Pour toute les demandes d'un chambre */
                    for (compteurDemande = 0; compteurDemande < hotel->chambre[compteurChambre].nbDemande; compteurDemande++) {
                        if (hotel->chambre[compteurChambre].type == typeChambre) {
                            if (strcmp(hotel->chambre[compteurChambre].demandeChambre[compteurDemande].date, date) == 0) {
                                ok = 0;
                            }
                        }
                    }
                    if (ok) {
                        memcpy(&retHotel[l_retNbHotel], hotel, sizeof (Hotel));
                        l_retNbHotel++;
                        compteurChambre = hotel->nbChabmbre;
                    }

                }

            }
        }
    } while (ret == 1);

    /* Libérer la mémoire de hotel */
    if (hotel) {
        free(hotel);
    }

    *retNbHotel = l_retNbHotel;
    return retHotel;

out:
    if (demande) {
        free(demande);
    }

    if (hotel) {
        free(hotel);
    }

    return hotel;

err:
    if (hotel) {
        free(hotel);
    }

    if (demande) {
        free(demande);
    }

    if (retHotel) {
        free(retHotel);
    }

    return NULL;
}

int createHotel(const char* nomhotel,
        const char* ville,
        int nbEtoile,
        int nbChambreSimple,
        int prixChambreSimple,
        int nbChambreDouble,
        int prixChambreDouble,
        int nbChambreTwin,
        int prixChambreTwin) {

    Hotel *host;

    /* Verifier le nombre de chambre ne dépasse pas la limite du proramme */
    if ((nbChambreDouble + nbChambreSimple + nbChambreTwin) > MAX_CHAMBRE) {
        return -2;
    }

    /* Verifier qu'aucun autre Hôtel n'est enregister avec le même nom */
    host = getHotelByName(nomhotel);
    if (host) {
        free(host);
        return -3;
    }

    Hotel monHotel;

    /* Attribution des données de l'Hôtel */
    strcpy(monHotel.nom, nomhotel);
    strcpy(monHotel.ville, ville);
    monHotel.nbEtoile = nbEtoile;
    monHotel.nbChabmbre = nbChambreDouble + nbChambreSimple + nbChambreTwin;

    /* Attribution des données des chambres */
    int i, j, k;
    i = -1;
    //for (i = 0; i < monHotel.nbChabmbre; i++) {
    /* Creation des chambre double */
    for (j = 0; j < nbChambreDouble; j++) {
        monHotel.chambre[++i].numero = j;
        monHotel.chambre[i].prix = prixChambreDouble;
        monHotel.chambre[i].type = TYPE_DOUBLE;
        monHotel.chambre[i].nbDemande = MAX_DEMANDE;
        memset(monHotel.chambre[i].demandeChambre, 0, sizeof (Demande) * MAX_DEMANDE);
        for (k = 0; k < MAX_DEMANDE; k++) {
            strcpy(monHotel.chambre[i].demandeChambre[k].date, "00/00/0000");
        }

    }
    /* Creation des chambre simple */
    for (j = 0; j < nbChambreSimple; j++) {
        monHotel.chambre[++i].numero = j;
        monHotel.chambre[i].prix = prixChambreSimple;
        monHotel.chambre[i].type = TYPE_SIMPLE;
        monHotel.chambre[i].nbDemande = MAX_DEMANDE;
        memset(monHotel.chambre[i].demandeChambre, 0, sizeof (Demande) * MAX_DEMANDE);
        for (k = 0; k < MAX_DEMANDE; k++) {
            strcpy(monHotel.chambre[i].demandeChambre[k].date, "00/00/0000");
        }
    }
    /* Creation des chambre twin */
    for (j = 0; j < nbChambreTwin; j++) {
        monHotel.chambre[++i].numero = j;
        monHotel.chambre[i].prix = prixChambreTwin;
        monHotel.chambre[i].type = TYPE_TWIN;
        monHotel.chambre[i].nbDemande = MAX_DEMANDE;
        memset(monHotel.chambre[i].demandeChambre, 0, sizeof (Demande) * MAX_DEMANDE);
        for (k = 0; k < MAX_DEMANDE; k++) {
            strcpy(monHotel.chambre[i].demandeChambre[k].date, "00/00/0000");
        }
    }
    //}

    /* Ecrire l'Hotel dans le fichier hotel.bin*/
    int ret;
    ret = writeToFile("hotel.bin", &monHotel, sizeof (monHotel), 1, "a+b");

    return ret;
}

int deleteHotel(const char* nomHotel) {
    FILE *file;
    Hotel *hotel = malloc(sizeof (Hotel));
    Hotel *mesHotels = malloc(sizeof (Hotel) * MAX_HOTEL);
    int ret;

    if (!mesHotels) {
        dbg("Erreur d'allocation mémoire pour mesHotels de taille %d octet(s)\n", sizeof (Hotel) * MAX_HOTEL);
        return -1;
    }

    if (!hotel) {
        dbg("Erreur : Allocation d'un Hotel impossible\n");
        return -1;
    }

    file = fopen(getPath("hotel.bin"), "a+b");

    /* En cas d'erreur */
    if (!file) {
        dbg("Erreur : Impossible d'ouvrir le fichier hotel.bin\n");
        return -1;
    }

    /* Lire structure */
    int isHotelForThisName = 0;
    int compteur = 0;
    do {
        memset(hotel, 0, sizeof (Hotel));
        ret = fread(hotel, sizeof (Hotel), 1, file);
        if (ret == 1) {
            if (strcmp(hotel->nom, nomHotel) != 0) {
                memcpy(&mesHotels[compteur], hotel, sizeof (Hotel));
                compteur++;
            } else {
                isHotelForThisName = 1;
            }
        }
    } while (ret == 1);

    free(hotel);
    fclose(file);

    if (!isHotelForThisName) {
        return -2;
    }

    /* Réécriture des Hotel dans un fichier w+ va troquer le fichier à 0 octets */
    ret = writeToFile("hotel.bin", mesHotels, sizeof (Hotel), compteur, "w+b");

    if (mesHotels) {
        free(mesHotels);
    }

    return ret;
}

int writeToFile(const char* filename, const void *toWrite, size_t size, int nbElement, const char *mode) {
    FILE * file;
    int ret;
    file = fopen(getPath(filename), mode);

    /* En cas d'erreur */
    if (!file) {
        dbg("Cannot open %s\n", getPath(filename));
        return -1;
    }

    /* Ecrire la structrue */
    ret = fwrite(toWrite, size, nbElement, file);
    //dbg("Write %d elements, base elements size : %d octet(s)\n", ret, size);
    fclose(file);

    if (ret < nbElement) {
        return -1;
    }

    return 0;
}

Hotel *getHotelByName(const char* hotelname) {
    FILE * file;
    Hotel *hotel = malloc(sizeof (Hotel));
    int ret;

    /* Check memory allocation */
    if (!hotel) {
        dbg("Erreur : Allocation d'un Hotel impossible\n");
        return NULL;
    }

    file = fopen(getPath("hotel.bin"), "a+b");

    /* En cas d'erreur */
    if (!file) {
        dbg("Erreur : Impossible d'ouvrir le fichier hotel.bin\n");
        return NULL;
    }

    /* Lire structure */
    do {
        memset(hotel, 0, sizeof (Hotel));
        ret = fread(hotel, sizeof (Hotel), 1, file);
        if (ret == 1) {
            if (strcmp(hotel->nom, hotelname) == 0) {
                return hotel;
            }
        }
    } while (ret == 1);

    /* Aucune valeur renvoyé donc on free la mémoire et return NULL*/
    free(hotel);
    return NULL;
}

char* getPath(const char* filename) {
    char* path;
    path = malloc(sizeof (char)*100);

    if (!path) {
        return NULL;
    }

    memset(path, '\0', sizeof (char)*100);
    sprintf(path, "%s%s", DIR_FILE, filename);
    return path;
}