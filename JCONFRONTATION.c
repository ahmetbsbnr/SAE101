/*


lire_entier
- valeur : entier lu
- resultat : résultat de scanf


lire_entier_borne
- valeur : entier lu dans les bornes min..max
- min : borne minimale
- max : borne maximale

calculer_moyenne
- stats : type structure defini Stats
- * stats : pointeur vers stats type structure Stats

afficher_stats
- nc : pointeur vers stats du jeu : nombre caché
- suite : pointeur vers stats du jeu : suite mystère
- mm : pointeur vers stats du jeu : mastermind

jouer_nombre_cache
- retourne le score (nombre d'essais ou pénalité)

jouer_suite_mystere
- retourne le score (nombre d'essais ou pénalité)

jouer_mastermind
- retourne le score (nombre d'essais ou pénalité)

main
- point d'entrée du programme



*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// ===== DEFINES =====
#define NC_VMIN 1
#define NC_VMAX_MIN 500
#define NC_VMAX_MAX 1000
#define NC_MAX_ESSAIS 10

#define SUITE_COEF_MIN 1
#define SUITE_COEF_MAX 7
#define SUITE_MAX_ESSAIS 2

#define MM_TAILLE_CODE 4
#define MM_MAX_ESSAIS 10
#define MM_VOYELLES "AEIOUY"

#define SCORE_FINALE 12

// ===== STRUCT STATS =====
typedef struct {
    int parties;
    int total_points;
} Stats;

// ===== FONCTIONS UTILITAIRES =====











// Lire un entier 
int lire_entier(void) {
    int valeur;
    int resultat;
    char c;
    
    while (1) {
        resultat = scanf("%d", &valeur);
        if (resultat == 1) {
            // Vérifier qu'il n'y a rien après le nombre
            c = getchar();
            if (c == '\n') {
                return valeur;
            } else {
                // Il y a des caractères supplémentaires après le nombre
                while (c != '\n' && c != EOF) {
                    c = getchar();
                }
                printf("Entrée invalide. Veuillez entrer un entier : ");
            }
        } else {
            // scanf a échoué
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrée invalide. Veuillez entrer un entier : ");
        }
    }
}







// Lire un entier avec bornes
int lire_entier_borne(int min, int max) {
    int valeur;
    while (1) {
        valeur = lire_entier();
        if (valeur >= min && valeur <= max) {
            return valeur;
        }
        printf("Valeur hors limites [%d..%d]. Réessayez : ", min, max);
    }
}

// Calculer moyenne
double calculer_moyenne(Stats *stats) {
    if (stats->parties == 0) {
        return SCORE_FINALE;
    }
    return (double)stats->total_points / stats->parties;
}

// Afficher les statistiques
void afficher_stats(Stats *nc, Stats *suite, Stats *mm) {
    double moy_nc = calculer_moyenne(nc);
    double moy_suite = calculer_moyenne(suite);
    double moy_mm = calculer_moyenne(mm);
    double score_global = (moy_nc + moy_suite + moy_mm) / 3.0;

    printf("\n===== MOYENNES =====\n");
    printf("Nombre caché     : %.2f\n", moy_nc);
    printf("Suite mystère    : %.2f\n", moy_suite);
    printf("Mastermind       : %.2f\n", moy_mm);
    printf("Score global     : %.2f\n", score_global);
    printf("====================\n\n");
}

// ===== JEU 1 : NOMBRE CACHÉ =====







int jouer_nombre_cache(void) {
    int vmax = NC_VMAX_MIN + rand() % (NC_VMAX_MAX - NC_VMAX_MIN + 1);      // Valeur max aléatoire
    int nombre = NC_VMIN + rand() % vmax;                             // Nombre à deviner
    int essais = 0;                                                 // Compteur d'essais
    int proposition;                                       // Proposition du joueur

    printf("\n*** JEU 1 : NOMBRE CACHÉ ***\n");
    printf("Nombre à trouver entre 1 et %d. Vous avez %d essais.\n\n", vmax, NC_MAX_ESSAIS);   // affichage :  borne max et nb essais

    while (essais < NC_MAX_ESSAIS) {                    // boucle tant que nb essais n'est pas atteint        
        essais++;                                      // On incrémenter le compteur d'essais
        printf("Essai %d/%d : Entrez votre nombre : ", essais, NC_MAX_ESSAIS);  // on demande et lit la proposition
        proposition = lire_entier_borne(NC_VMIN, vmax);

        if (proposition == nombre) {                     // Si la proposition est correcte
            printf("Bravo ! Vous avez trouvé %d en %d essai(s).\n", nombre, essais);    // message + retour du nb d'essais
            return essais;
        } else if (proposition < nombre) {               // Si trop petit
            printf("C'est plus !\n");                     // message
        } else {                                        // Si trop grand
            printf("C'est moins !\n");                    // message
        }
    }

    printf("Dommage ! Le nombre était %d. Pénalité : %d points.\n", nombre, SCORE_FINALE);
    return SCORE_FINALE;
}








// ===== JEU 2 : SUITE MYSTÈRE =====

int jouer_suite_mystere(void) {
    int a = SUITE_COEF_MIN + rand() % (SUITE_COEF_MAX - SUITE_COEF_MIN + 1);
    int b = SUITE_COEF_MIN + rand() % (SUITE_COEF_MAX - SUITE_COEF_MIN + 1);
    int c = SUITE_COEF_MIN + rand() % (SUITE_COEF_MAX - SUITE_COEF_MIN + 1);

    int u0 = c;
    int u1 = a * u0 + b;
    int u2 = a * u1 + b;
    int u3_correct = a * u2 + b;

    printf("\n*** JEU 2 : SUITE MYSTÈRE ***\n");
    printf("Vous avez %d essai(s) pour trouver U3.\n\n", SUITE_MAX_ESSAIS);







    printf("[DEBUG] U3 = %d\n", u3_correct); // Ligne DEBUG






    printf("Suite : U0 = %d, U1 = %d, U2 = %d\n", u0, u1, u2);
    printf("Trouvez U3 : ");

    for (int essai = 1; essai <= SUITE_MAX_ESSAIS; essai++) {
        int proposition = lire_entier();

        if (proposition == u3_correct) {
            printf("Bravo ! U3 = %d trouvé en %d essai(s).\n", u3_correct, essai);
            return essai;
        } else if (essai < SUITE_MAX_ESSAIS) {
            if (proposition < u3_correct) {
                printf("C'est plus ! Réessayez : ");
            } else {
                printf("C'est moins ! Réessayez : ");
            }
        } else {
            printf("\n");
        }
    }

    printf("Dommage ! U3 = %d. Pénalité : %d points.\n", u3_correct, SCORE_FINALE);
    return SCORE_FINALE;
}

// ===== JEU 3 : MASTERMIND =====

int jouer_mastermind(void) {
    char voyelles[] = MM_VOYELLES;
    char code[MM_TAILLE_CODE + 1];
    int code_len = strlen(voyelles);

    // Générer 4 voyelles distinctes
    code[0] = voyelles[rand() % code_len];
    int ok;
    for (int i = 1; i < MM_TAILLE_CODE; i++) {
        do {
            code[i] = voyelles[rand() % code_len];
            ok = 1;
            for (int j = 0; j < i; j++) {
                if (code[i] == code[j]) {
                    ok = 0;
                    break;
                }
            }
        } while (!ok);
    }
    code[MM_TAILLE_CODE] = '\0';

    printf("\n*** JEU 3 : MASTERMIND ***\n");
    printf("Trouvez le code de 4 voyelles distinctes (A,E,I,O,U,Y).\n");
    printf("Vous avez %d essais.\n\n", MM_MAX_ESSAIS);

    for (int essai = 1; essai <= MM_MAX_ESSAIS; essai++) {
        printf("Essai %d/%d : Entrez 4 voyelles majuscules : ", essai, MM_MAX_ESSAIS);
        
        char proposition[MM_TAILLE_CODE + 1];
        scanf("%4s", proposition);
        while (getchar() != '\n');

        // Valider entrée
        int valide = 1;
        if (strlen(proposition) != MM_TAILLE_CODE) {
            valide = 0;
        }
        if (valide) {
            for (int i = 0; i < MM_TAILLE_CODE; i++) {
                proposition[i] = toupper(proposition[i]);
                if (strchr(MM_VOYELLES, proposition[i]) == NULL) {
                    valide = 0;
                    break;
                }
            }
        }
        if (!valide) {
            printf("Entrée invalide. Veuillez entrer 4 voyelles majuscules distinctes.\n");
            essai--;
            continue;
        }

        // Vérifier doublons
        int doublon = 0;
        for (int i = 0; i < MM_TAILLE_CODE; i++) {
            for (int j = i + 1; j < MM_TAILLE_CODE; j++) {
                if (proposition[i] == proposition[j]) {
                    doublon = 1;
                    break;
                }
            }
            if (doublon) break;
        }
        if (doublon) {
            printf("Les voyelles doivent être distinctes. Réessayez.\n");
            essai--;
            continue;
        }

        // Vérifier et compter
        int bien_places = 0;
        int mal_places = 0;

        for (int i = 0; i < MM_TAILLE_CODE; i++) {
            if (proposition[i] == code[i]) {
                bien_places++;
            } else if (strchr(code, proposition[i]) != NULL) {
                mal_places++;
            }
        }

        if (bien_places == MM_TAILLE_CODE) {
            printf("Bravo ! Code trouvé en %d essai(s).\n", essai);
            return essai;
        }

        printf("Bien placées : %d, Mal placées : %d\n", bien_places, mal_places);
    }

    printf("Dommage ! Le code était %s. Pénalité : %d points.\n", code, SCORE_FINALE);
    return SCORE_FINALE;
}

/* ===== MAIN ===== */

int main(void) {
    srand((unsigned)time(NULL));

    Stats stat_nc = {0, 0};
    Stats stat_suite = {0, 0};
    Stats stat_mm = {0, 0};

    int choix;
    int score;

    printf("===== JEUX DE CONFRONTATION =====\n\n");

    while (1) {
        printf("Menu principal :\n");
        printf("1 - Nombre caché\n");
        printf("2 - Suite mystère\n");
        printf("3 - Mastermind\n");
        printf("4 - Afficher les moyennes\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");

        choix = lire_entier();

        switch (choix) {
            case 1:
                score = jouer_nombre_cache();
                stat_nc.parties++;
                stat_nc.total_points += score;
                afficher_stats(&stat_nc, &stat_suite, &stat_mm);
                break;

            case 2:
                score = jouer_suite_mystere();
                stat_suite.parties++;
                stat_suite.total_points += score;
                afficher_stats(&stat_nc, &stat_suite, &stat_mm);
                break;

            case 3:
                score = jouer_mastermind();
                stat_mm.parties++;
                stat_mm.total_points += score;
                afficher_stats(&stat_nc, &stat_suite, &stat_mm);
                break;

            case 4:
                afficher_stats(&stat_nc, &stat_suite, &stat_mm);
                break;

            case 0:
                printf("\nStatistiques finales :\n");
                afficher_stats(&stat_nc, &stat_suite, &stat_mm);
                printf("Au revoir !\n");
                return 0;

            default:
                printf("Choix invalide. Réessayez.\n\n");
        }
    }

    return 0;
}
