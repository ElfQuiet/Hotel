#include "main.h"

int app_exit_var;

int main(int argc, char** argv) {
    app_exit_var = 1;
    if (InitialisationAvecService(SERVER_PORT) == 0) {
        printf("[ERR]  Erreur d'initialisation du service web (port : %s)\n", SERVER_PORT);
        return (EXIT_FAILURE);
    } else {
        printf("[INFO] Initialisation du service web : Ok\n");
    }
    printf("[INFO] Use ctrl+c to stop and exit\n");
    printf("[INFO] Taille d'un Hotel : %d octet(s)\n", sizeof (Hotel));
    printf("[INFO] Attente d'une connexion\n");

    signal(SIGINT, app_exit);

    AttenteClient();

    while (app_exit_var) {
        ReadCmd();
    }
    return (EXIT_SUCCESS);
}

void app_exit(int value) {
    printf("[INFO] Terminaison client connexion\n");
    TerminaisonClient();
    printf("[INFO] Terminaison service web\n");
    Terminaison();
    app_exit_var = 0;
}

int ReadCmd() {
    char *rc = NULL;
    rc = Reception();
    if (rc) {
        //printf("J'ai recu: %s\n", rc);


/*
        if (Emission("Test de message serveur.\n") != 1) {
            printf("Erreur d'emission\n");
        }
*/
    } else {
        return -1;
    }

    return 0;
}
