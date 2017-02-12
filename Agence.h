#ifndef DEMANDERECEIVER_H
#define DEMANDERECEIVER_H
#include "common.h"


/**
 * Retourne liste hotel fct demande
 * @param ville
 * @param nbEtoile
 * @param typeChambre
 * @param retNbHotel
 * @param date
 * @return listeHotel
 *         NULL si bug
 */
Hotel *rechercherHotel(const char *ville,
        int nbEtoile,
        int typeChambre,
        int *retNbHotel,
        const char *date);


/**
 * Récupère les réservation d'un client 
 * @param clientName
 * @param nbResaFound
 * @return 
 */
Demande *getResaByClientName(const char* clientName, int *nbResaFound);


/**
 * Cette fonction permet d'enregistrer un hotel.
 * Son nom doit être unique
 * @param nomhotel
 * @param ville
 * @param nbEtoile
 * @param nbChambreSimple
 * @param prixChambreSimple
 * @param nbChambreDouble
 * @param prixChambreDouble
 * @param nbChambreTwin
 * @param prixChambreTwin
 * @return  -1 si l'ouverture du fichier des hotels échoue ou
 *          -2 si trop de chambre
 *          -3 nom hotel déja pris
 */
int createHotel(const char* nomhotel,
        const char* ville,
        int nbEtoile,
        int nbChambreSimple,
        int prixChambreSimple,
        int nbChambreDouble,
        int prixChambreDouble,
        int nbChambreTwin,
        int prixChambreTwin);

/**
 * Supprimer un hotel d'un fichier
 * @param nomHotel
 * @return -1 BAD
 *         -2 Hotel does not exist
 *          0 OK
 */
int deleteHotel(const char* nomHotel);

/**
 * Créer un réservation (par un client) pour un Hotel
 * @param hotelName
 * @param date
 * @param typeChambre
 * @param nomClient
 * @return -1 : Erreur allocation mémoire
 *         -4 plus de demande disponnible
 *         numéro de chambre
 */
int createReservation(const char* hotelName, const char* date, int typeChambre, const char* nomClient);



#endif 

/* DEMANDERECEIVER_H */

