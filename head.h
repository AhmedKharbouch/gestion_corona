#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

/*--------------------ahmed kharbouch 2ap g2 maarif------------------*/
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
/*declaration de la structure maladie*/
struct element2
{
char CIN[25];
char nom[20];
    char prenom[25];
    char adresse[25];
    int age;
    char etat;
    char genre;
    int conta;
    int j;
    int m;
    int annee;
     int j1;
    int m1;
    int annee1;
    char nomh[25];
    char CINP[25];

struct element2 *suivant;
};
/*-Définition du type element2-*/
typedef struct element2 element2;
/*-Définition du type personne-*/
typedef element2* personne;

/*declaration de la structure personne*/
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

patient Ajouter_patient(patient l,char cin1[25]);
int affiche_patient(patient l,char cin3[25]);
maladie Ajouter_maladie(maladie m,char cin2[25]);
int affiche_maladie(maladie m,char cin3[25]);
personne Ajouter_personne(personne p,char cin2[25],char *co);
int affiche_personne(personne p,char cin3[25]);
patient supprimer_patient(patient l,char *co);
maladie supprimer_maladie(maladie m,char *co);
hotel Ajouter_hotel(hotel h);
int affiche_hotel(hotel h);
int affiche_confinement(personne p);
hotel nombre_dispo(personne p,hotel h);
int affiche_pmaladie(maladie m,char cin2[25]);
int affiche_personneseul(personne p,char cin2[25]);
personne transfere_personne123(personne p,hotel h, char cin5[25]);
int affiche_ageetgenre(patient l,char g,int a);
personne supprimer_personne(personne p,char *co);
personne modification_etatperso(personne p,char cin3[25]);

#endif // HEAD_H_INCLUDED







