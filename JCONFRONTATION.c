/*
===================================================
SAE1.01 : 2025-2026

-Implémentation d'un besoin client
- Jeu de Confrontation

Lnaguage : C


Auteur : 
-   Ahmet BASBUNAR
-   Ava (Nicolas) OURY
-   Léane SCHMITTAG
===================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>


/*
 * Constantes déclarées
*/
#define NC_VMIN        1
#define NC_VMAX_MIN  500
#define NC_VMAX_MAX 1000
#define NC_MAX_ESSAIS  10

#define SUITE_COEF_MIN 1
#define SUITE_COEF_MAX 7
#define SUITE_MAX_ESSAIS 2

#define MM_TAILLE_CODE 4
#define MM_MAX_ESSAIS 10
#define MM_VOYELLES "AEIOUY"

#define SCOREFINALE 12

/* --------- Statistiques --------- */
typedef struct {
    int parties;
    int total_points;
} Stats;

/*---------- Tout les fonctions nécessaires ---------*/
// J1 : Nombre Caché
int jouer_nombre_cache(void);     /* renvoie de points de la partie */
// J2 : Suite Mystere
int jouer_suite_mystere(void);    /* renvoie de points de la partie */
// J3 : Mastermind
int jouer_mastermind(void);       /* renvoie de points de la partie */
// lire un entier simple (sans borne)
static int lire_entier_simple(const char *invite);
// lire un entier avec borne min..max
static int lire_entier_borne(const char *invite, int min, int max);
// calcule moyenne ou 12 si aucune partie jouée
static double moyenne(const Stats *s);
// affiche les moyennes des 3 jeux
static void afficher_moyennes(const Stats *j1, const Stats *j2, const Stats *j3);
// fonction principale
int main();
// mélangeur de voyelles
void melangeur_voyelles(char *voyelles, int n);





/* Tant que ce n'est pas un entier, on affiche une erreur et on redemande. */
static int lire_entier_simple(const char *invite) {
    int x, ch;
    for (;;) {
        printf("%s", invite);
        if (scanf("%d", &x) == 1) {
            while ((ch = getchar()) != '\n' && ch != EOF) {} /* vider la ligne */
            return x;
        } else {
            printf("Erreur : entre un entier.\n");
            while ((ch = getchar()) != '\n' && ch != EOF) {} /* vider la ligne */
        }
    }
}


/* Lecture bornée min..max, basée sur la lecture simple. */
static int lire_entier_borne(const char *invite, int min, int max) {
    for (;;) {
        int v = lire_entier_simple(invite);
        if (v < min || v > max) {
            printf("Hors bornes (%d..%d). Réessaie.\n", min, max);
            continue;
        }
        return v;
    }
}

static double moyenne(const Stats *s) {
    if (!s || s->parties == 0) return 12.0;
    return (double)s->total_points / (double)s->parties;
}

static void afficher_moyennes(const Stats *j1, const Stats *j2, const Stats *j3) {
    double m1 = moyenne(j1);
    double m2 = moyenne(j2);
    double m3 = moyenne(j3);
    double global = (m1 + m2 + m3) / 3.0;

    printf("\n===== Moyennes =====\n");
    printf("Nombre caché  : %.2f\n", m1);
    printf("Suite mystère : %.2f\n", m2);
    printf("Mastermind    : %.2f\n", m3);
    printf("Score global  : %.2f\n", global);
}


/*melangeur de voyelles*/
// methode de Fisher-Yates
void generer_code_voyelles(char *code) {
    const int n = 6; // nombre de voyelles possibles
    char voyelles_table[5];

    // Copier les voyelles possibles dans un tableau
    for (int i = 0; i < n; i++) {
        voyelles_table[i] = MM_VOYELLES[i];
    }

    // Mélanger la liste avec Fisher-Yates
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = voyelles_table[i];
        voyelles_table[i] = voyelles_table[j];
        voyelles_table[j] = temp;
    }

    // Prendre les premières MM_TAILLE_CODE voyelles
    for (int i = 0; i < MM_TAILLE_CODE; i++) {
        code[i] = voyelles_table[i];
    }
    code[MM_TAILLE_CODE] = '\0'; // Terminaison de la chaîne
}



/* 
    * Fonction principale
*/
int main(void) {
    srand((unsigned)time(NULL));

    Stats S_nb = {0,0};  // Nombre caché
    Stats S_sm = {0,0};  // Suite mystère
    Stats S_mm = {0,0};  // Mastermind

    int choix = -1;

    while (choix != 0) {
        printf("\n=== MENU ===\n");
        printf("1 - Nombre caché\n");
        printf("2 - Suite mystère\n");
        printf("3 - Mastermind\n");
        printf("4 - Afficher les moyennes\n");
        printf("0 - Quitter\n");

        choix = lire_entier_borne("Votre choix : ", 0, 4);

        int points = 0;
        if (choix == 1) {
            points = jouer_nombre_cache();
            S_nb.parties++;
            S_nb.total_points += points;
            afficher_moyennes(&S_nb, &S_sm, &S_mm);
        }
        else if (choix == 2) {
            points = jouer_suite_mystere();
            S_sm.parties++;
            S_sm.total_points += points;
            afficher_moyennes(&S_nb, &S_sm, &S_mm);
        }
        else if (choix == 3) {
            points = jouer_mastermind();
            S_mm.parties++;
            S_mm.total_points += points;
            afficher_moyennes(&S_nb, &S_sm, &S_mm);
        }
        else if (choix == 4) {
            afficher_moyennes(&S_nb, &S_sm, &S_mm);
        }
    }

    printf("Au revoir !\n");
    return 0;
}

/*
 ========================================================================
 ==========================>>>> JEU 1,2,3 <<<<<==========================
 ========================================================================
*/

/* ================== Implémentation JEU 1 ================== */
/* Jeu Nombre Caché */
int jouer_nombre_cache(void) {
    // 1. Générer VMAX aléatoirement dans [500..1000] 
    int VMAX = (rand() % (NC_VMAX_MAX - NC_VMAX_MIN + 1)) + NC_VMAX_MIN;
    
    // 2. Générer le nombre secret N dans [1..VMAX]
    int secret = (rand() % (VMAX - NC_VMIN + 1)) + NC_VMIN;
    
    int essai_user;

    printf("======================================\n");
    printf("\n--- Bienvenue au Nombre Caché ! ---\n");
    printf("J'ai choisi un nombre entre %d et %d.\n", NC_VMIN, VMAX);
    printf("Vous avez %d essais pour le trouver.\n", NC_MAX_ESSAIS);

    // 3. Boucle des essais
    for (int essai = 1; essai <= NC_MAX_ESSAIS; essai++) {

        /* Afficher l'invite simplement puis lire */
        printf("Essai %d/%d : ", essai, NC_MAX_ESSAIS);
        essai_user = lire_entier_borne("", NC_VMIN, VMAX);

        if (essai_user == secret) {
            printf("\nBravo ! Vous avez trouvé en %d essai(s) !\n", essai);
            return essai; // Le score est le nombre d'essais
        }
        else if (essai_user < secret) {
            printf("C'est plus !\n");
        }
        else {
            printf("C'est moins !\n");
        }
    }

    // 5. On sort de la boucle, le joueur a perdu
    printf("\nDommage ! Vous avez épuisé vos %d essais.\n", NC_MAX_ESSAIS);
    printf("Le nombre secret était : %d\n", secret);
    
    return SCOREFINALE; // Score de pénalité
}


/* ================== Implémentation JEU 2 ================== */
/* Jeu Suite Mystère */
/*===================*/
int jouer_suite_mystere(void) {
    // a, b, c ∈ [1..7]
    int a = (rand() % (SUITE_COEF_MAX - SUITE_COEF_MIN + 1)) + SUITE_COEF_MIN;
    int b = (rand() % (SUITE_COEF_MAX - SUITE_COEF_MIN + 1)) + SUITE_COEF_MIN;
    int c = (rand() % (SUITE_COEF_MAX - SUITE_COEF_MIN + 1)) + SUITE_COEF_MIN;

    // U0=c ; U1=a*U0+b ; U2=a*U1+b ; U3=a*U2+b
    int U0 = c;
    int U1 = a * U0 + b;
    int U2 = a * U1 + b;
    int U3 = a * U2 + b;   // valeur à deviner

    printf("\n--- Suite mystère ---\n");
    printf("La suite est définie par: U0 = c, et U(n) = a*U(n-1) + b\n");
    printf("Avec a=%d, b=%d, c=%d\n", a, b, c);
    printf("Voici les trois premiers termes: %d, %d, %d\n", U0, U1, U2);
    printf("Trouve U3 ! (tu as %d essai(s))\n", SUITE_MAX_ESSAIS);

    for (int essai = 1; essai <= SUITE_MAX_ESSAIS; ++essai) {
        printf("Essai %d/%d : ", essai, SUITE_MAX_ESSAIS);
        int prop = lire_entier_simple("");

        if (prop == U3) {
            printf("Bravo ! U3 = %d. Trouvé en %d essai(s).\n", U3, essai);
            return essai; // score = nb d’essais utilisés
        } else if (prop < U3) {
            printf("C’est plus !\n");
        } else {
            printf("C’est moins !\n");
        }
    }

    printf("Raté ! La bonne réponse était U3 = %d.\n", U3);
    return SCOREFINALE; // pénalité
}




/* ================== Implémentation JEU 3 ================== */
/* Jeu Mastermind (voyelles distinctes) */
/*======================================*/

int jouer_mastermind(void) {
    const char *voyelles = MM_VOYELLES; // "AEIOUY"
    char secret[MM_TAILLE_CODE + 1];
    int i;

    // Générer un code de 4 voyelles distinctes parmi 6
    int util[6] = {0,0,0,0,0,0};    
    for (i = 0; i < MM_TAILLE_CODE; ++i) {
        int idx;
        do {
            idx = rand() % 6;       
        } while (util[idx]);
        util[idx] = 1;
        secret[i] = voyelles[idx];
    }
    secret[MM_TAILLE_CODE] = '\0';

    printf("\n--- Mastermind ---\n");
    printf("Devine un code de %d voyelles parmi {%s}\n", MM_TAILLE_CODE, voyelles);
    printf("Tu as %d essais.\n", MM_MAX_ESSAIS);
    printf("Les voyelles ne se répètent pas.\n");



    //test
    printf("[DEBUG] Code secret : %s\n", secret);





    for (int essai = 1; essai <= MM_MAX_ESSAIS; ++essai) {
        char proposition[64];
        printf("Essai %d/%d : ", essai, MM_MAX_ESSAIS);
        scanf("%63s", proposition);
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

        // Vérification de la longueur du saisie utilisateur
        if ((int)strlen(proposition) != MM_TAILLE_CODE) {
            printf("Il faut exactement %d lettres.\n", MM_TAILLE_CODE);
            essai--; continue; 
        }
        

        // Uppercase
        for (i = 0; i < MM_TAILLE_CODE; ++i) {
            proposition[i] = toupper((unsigned char)proposition[i]);
        }

        // Vérification alphabet + unicité
        int voyelles_presentes[6] = {0,0,0,0,0,0}; // <-- 6
        int input_ok = 1;
        for (i = 0; i < MM_TAILLE_CODE; ++i) {
            int trouve = 0;
            for (int k = 0; k < 6; ++k) {          // <-- 6
                if (proposition[i] == voyelles[k]) {
                    if (voyelles_presentes[k]) {
                        printf("Code invalide : lettres sans doublon.\n");
                        input_ok = 0;
                    } else {
                        voyelles_presentes[k] = 1;
                    }
                    trouve = 1;
                    break;
                }
            }
            if (!trouve) {
                printf("Code invalide : utiliser uniquement {%s}.\n", voyelles);
                input_ok = 0;
            }
        }

        // si erreur, on redemande
        if (!input_ok) { essai--; continue; } 

        // Gagné ?
        if (strcmp(proposition, secret) == 0) {
            printf("Bravo ! Code %s trouvé en %d essais.\n", secret, essai);
            return essai;
        }

        // Calcul bien/mal placées
        int bien = 0, mal = 0;
        int cSecret[6] = {0}, cProp[6] = {0};      

        for (i = 0; i < MM_TAILLE_CODE; ++i) {
            if (proposition[i] == secret[i]) {
                bien++;
            } else {
                for (int k = 0; k < 6; ++k) {      
                    if (secret[i] == voyelles[k])   cSecret[k]++;
                    if (proposition[i] == voyelles[k]) cProp[k]++;
                }
            }
        }
        for (int k = 0; k < 6; ++k) {              
            mal += (cSecret[k] < cProp[k]) ? cSecret[k] : cProp[k];
        }

        printf("Bien placées: %d, Mal placées: %d\n", bien, mal);
    }

    printf("Dommage ! Le code était : %s\n", secret);
    return SCOREFINALE;
}
