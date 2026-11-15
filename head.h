#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

/*--------------------ahmed kharbouch 2ap g2 maarif------------------*/

/*declaration de la structure patient*/
struct element
{
    char CIN[25];
    char nom[20];
    char prenom[25];
    char adresse[25];
    int age;
    char etat;
    char genre;
    int conta;
    struct element *suivant;
};
/*-Définition du type element-*/
typedef struct element element;
/*-Définition du type liste-*/
typedef element* patient;

/*declaration de la structure maladie*/
struct element1
{
    char CIN[25];
    char mc[25];
    struct element1 *suivant;
};
/*-Définition du type element1-*/
typedef struct element1 element1;
/*-Définition du type maladie-*/
typedef element1* maladie;

/*declaration de la structure personne (contact d'un patient)*/
struct element2
{
    char CIN[25];      /* CIN du patient avec qui la personne a été en contact */
    char nom[20];
    char prenom[25];
    char adresse[25];
    int age;
    char etat;
    char genre;
    int conta;
    int j;             /* date début de confinement */
    int m;
    int annee;
    int j1;            /* date fin de confinement */
    int m1;
    int annee1;
    char nomh[25];     /* hotel de confinement, "none" si aucun */
    char CINP[25];     /* CIN de la personne */
    struct element2 *suivant;
};
/*-Définition du type element2-*/
typedef struct element2 element2;
/*-Définition du type personne-*/
typedef element2* personne;

/*declaration de la structure hotel*/
struct element3
{
    char nom[20];
    char adresse[25];
    int nbr;
    int nbmax;
    struct element3 *suivant;
};
/*-Définition du type element3-*/
typedef struct element3 element3;
/*-Définition du type hotel-*/
typedef element3* hotel;

/*-------------------couleurs de la console (codes ANSI)------------------*/
#define RESET  "\033[0m"
#define GRAS   "\033[1m"
#define CYAN   "\033[96m"
#define BLEU   "\033[94m"
#define VERT   "\033[92m"
#define ROUGE  "\033[91m"
#define JAUNE  "\033[93m"
#define GRIS   "\033[90m"

#define LARGEUR 64   /* largeur des cadres et des séparateurs */

/*-------------------fonctions d'affichage------------------*/
void ui_init(void);
void ui_effacer(void);
void ui_transition(void);
void ui_cascade(void);
void ui_titre(const char *titre);
void ui_section(const char *fmt, ...);
void ui_message(const char *couleur, const char *symbole, const char *fmt, ...);
void ui_champ(const char *label);
void ui_invalide(const char *aide);
void ui_pause(void);
void ui_item(int numero, const char *texte);
#define ui_ok(...)     ui_message(VERT,  "✓", __VA_ARGS__)
#define ui_erreur(...) ui_message(ROUGE, "×", __VA_ARGS__)
#define ui_info(...)   ui_message(JAUNE, "i", __VA_ARGS__)

/*-------------------fonctions de saisie------------------*/
void vider_buffer(void);
void lire_chaine(char *s, int taille);
void lire_cin(char *s, int taille);
int lire_entier(void);
char lire_caractere(void);

/*-------------------compteurs------------------*/
int compter_patients(patient l);
int compter_personnes(personne p);
int compter_hotels(hotel h);
int compter_confines(personne p);
int patient_existe(patient l, char cin[25]);
int personne_existe(personne p, char cin[25]);

/*-------------------fonctions de gestion-----------------*/
patient Ajouter_patient(patient l, char cin1[25]);
int affiche_patient(patient l, char cin3[25]);
patient modification_etat(patient l, char cin3[25]);
patient supprimer_patient(patient l, maladie *m);
int affiche_ageetgenre(patient l, char g, int a);

maladie Ajouter_maladie(maladie m, char cin2[25]);
int affiche_maladie(maladie m, char cin3[25]);
maladie supprimer_maladie(maladie m, char *cin);

personne Ajouter_personne(personne p, char cin2[25], char *co);
int affiche_personne(personne p, char cin3[25]);
int affiche_personneseul(personne p, char cin2[25]);
personne modification_etatperso(personne p, char cin3[25]);
personne supprimer_personne(personne p, maladie *m);
int affiche_confinement(personne p);
personne transfere_personne123(personne p, hotel h, char cin5[25]);
int affiche_transferables(personne p, hotel h);

hotel Ajouter_hotel(hotel h);
int affiche_hotel(hotel h);
hotel nombre_dispo(personne p, hotel h);

/*-------------------sauvegarde dans un fichier-----------------*/
const char *fichier_donnees(void);
int charger_donnees(patient *l, maladie *m, personne *p, hotel *h, int *erreurs);
int sauvegarder_donnees(patient l, maladie m, personne p, hotel h);

#endif // HEAD_H_INCLUDED
