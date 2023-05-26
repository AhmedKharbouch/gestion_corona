#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"

/*--------------------ahmed kharbouch 2ap g2 maarif------------------*/
patient Ajouter_patient(patient l, char cin1[25])
{
/*On crée un nouvel élément*/
element* nouveau = (element*)malloc(sizeof(element));
/*On assigne la valeur au nouvel élément*/
    strcpy(nouveau->CIN,cin1);
    printf("inserer le nom du patient:");
       fflush(stdin);
    scanf("%s",nouveau->nom);
    printf("inserer le prenom du patient:");
       fflush(stdin);
    scanf("%s",nouveau->prenom);
    printf("inserer le nom du boulvard:");
       fflush(stdin);
    scanf("%s",nouveau->adresse);
       fflush(stdin);
    printf("inserer l'age:");
    rewind(stdin);
    scanf("%d",&nouveau->age);
    do{
    printf("inserer le genre homme (h) ou femme (f):");
    rewind(stdin);
    scanf("%c",&nouveau->genre);
    }while(nouveau->genre!='h'&& nouveau->genre!='f');
   do{
    printf("inserer contamination local (1) non local (0):\n");
    rewind(stdin);
    scanf("%d",&nouveau->conta);
    }while(nouveau->conta!=1 && nouveau->conta!=0);
    do{
    printf("inserer l'etat normal (n) ou critique (c):");
    rewind(stdin);
    scanf("%c",&nouveau->etat);
    }while(nouveau->etat!='n' && nouveau->etat!='c');
/*On ajoute en fin, donc aucun élément ne va suivre*/
nouveau->suivant = NULL;
if(l == NULL)
{
/*Si la liste est vidée il suffit de renvoyer l'élément créé*/
return nouveau;
}
else
{
/*Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
indique que le dernier élément de la liste est relié au nouvel élément*/
element* tmp=l;
while(tmp->suivant != NULL)
{
tmp = tmp->suivant;
}
tmp->suivant = nouveau;
return l;
}
}



/* ***************************fonction d'affichage patient*********************************/
int affiche_patient(patient l,char cin3[25])
{
element* tmp=l;
int c=0;
while(tmp!=NULL){

  if (strcmp(tmp->CIN,cin3) == 0)
    {
  printf("CIN :%s\n",tmp->CIN);
   printf("\t");

    printf("NOM:%s\n",tmp->nom);
    printf("\t");

     printf("PRENOM:%s\n",tmp->prenom);
     printf("\t");

      printf("ADRESSE:%s\n",tmp->adresse);printf("\t");
       printf("AGE:%d",tmp->age);
       printf("\t");

        printf("ETAT:%c\n",tmp->etat);
        printf("\t");


         if(tmp->genre=='h')
         {
             printf("GENRE:HOMME\n");
          }
         else
            {
            printf("GENRE:FEMME\n");
            }
          if(tmp->conta==1)
          {
              printf("la contamination est local\n");
          }
          else
          {
              printf("la contamination n'est pas local\n");
          }
          c++;
}
     tmp=tmp->suivant;


}
return c;
}
/*-------------------------fonction remplissage maladie chronique--------------------*/

maladie Ajouter_maladie(maladie m,char cin2[25])
{
/*On crée un nouvel élément*/
element1* nouveau = (element1*)malloc(sizeof(element1));
/*On assigne la valeur au nouvel élément*/
    strcpy(nouveau->CIN,cin2);
    printf("inserer la maladie chronique: ");
    fflush(stdin);
    scanf("%s",nouveau->mc);


/*On ajoute en fin, donc aucun élément ne va suivre*/
nouveau->suivant = NULL;
if(m == NULL)
{
/*Si la liste est vidée il suffit de renvoyer l'élément créé*/
return nouveau;
}
else
{
/*Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
indique que le dernier élément de la liste est relié au nouvel élément*/
element1* tmp=m;
while(tmp->suivant != NULL)
{
tmp = tmp->suivant;
}
tmp->suivant = nouveau;
return m;
}
}

/*----------------------affichage maladie-------------------*/
int affiche_maladie(maladie m,char cin3[25])
{
element1* tmp=m;
int c=0;

while(tmp!=NULL){

if(strcmp(tmp->CIN,cin3) == 0)
{
printf("MALADIE:%s\n",tmp->mc);
printf("\t");
   c++;
}
     tmp=tmp->suivant;

}
return c;
}

/*----------------insertion personne-----------------------*/
personne Ajouter_personne(personne p,char cin2[25],char *co)
{
/*On crée un nouvel élément*/
element2* nouveau = (element2*)malloc(sizeof(element2));
/*On assigne la valeur au nouvel élément*/
    printf("inserer la cin de la personne:");
       fflush(stdin);
    scanf("%s",nouveau->CINP);
    strcpy(co,nouveau->CINP);
    strcpy(nouveau->CIN,cin2);
    strcpy(nouveau->nomh,"none");
    printf("inserer le nom de la personne:");
       fflush(stdin);
    scanf("%s",nouveau->nom);
    printf("inserer le prenom de la personne:");
       fflush(stdin);
    scanf("%s",nouveau->prenom);
    printf("inserer le nom du boulvard:");
       fflush(stdin);
    scanf("%s",nouveau->adresse);
       fflush(stdin);
    printf("inserer l'age:");
    rewind(stdin);
    scanf("%d",&nouveau->age);
    do{
    printf("inserer le genre homme (h) ou femme (f):");
    rewind(stdin);
    scanf("%c",&nouveau->genre);
    }while(nouveau->genre!='h'&& nouveau->genre!='f');
   do{
    printf("inserer contamination local (1) non local (0):\n");
    rewind(stdin);
    scanf("%d",&nouveau->conta);
    }while(nouveau->conta!=1 && nouveau->conta!=0);
    do{
    printf("inserer l'etat normal (n) ou critique (c):");
    rewind(stdin);
    scanf("%c",&nouveau->etat);
    }while(nouveau->etat!='n' && nouveau->etat!='c');
/*On ajoute en fin, donc aucun élément ne va suivre*/
nouveau->suivant = NULL;
if(p == NULL)
{
/*Si la liste est vidée il suffit de renvoyer l'élément créé*/
return nouveau;
}
else
{
/*Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
indique que le dernier élément de la liste est relié au nouvel élément*/
element2* tmp=p;
while(tmp->suivant != NULL)
{
tmp = tmp->suivant;
}
tmp->suivant = nouveau;
return p;
}
}
/*--------------------------affichage personnes-------------------*/
int affiche_personne(personne p,char cin3[25])
{
element2* tmp=p;
int c=0;
while(tmp!=NULL){

  if (strcmp(tmp->CIN,cin3) == 0)
    {
  printf("NOM:%s\n",tmp->nom);
    printf("\t");

     printf("PRENOM:%s\n",tmp->prenom);
     printf("\t");

      printf("ADRESSE:%s\n",tmp->adresse);printf("\t");
       printf("AGE:%d",tmp->age);
       printf("\t");

        printf("ETAT:%c\n",tmp->etat);
        printf("\t");

        printf("HOTEL:%s\n",tmp->nomh);
        printf("\t");
        printf("\n");
          c++;
}
     tmp=tmp->suivant;


}
return c;
}

/*-----------------------------modification etat du patient-----------------*/

patient modification_etat(patient l,char cin3[25])
{
element* tmp=l;
int c=0;
while(tmp!=NULL){

  if (strcmp(tmp->CIN,cin3) == 0)
    {
 do{
    printf("inserer l'etat normal (n) , critique (c) , gerie (g) , mort (m):");
    rewind(stdin);
    scanf("%c",&tmp->etat);
    }while(tmp->etat!='n' && tmp->etat!='c' && tmp->etat!='g'&& tmp->etat!='m');
    c++;
    break;
}
     tmp=tmp->suivant;


}
if(c==0)
{
    printf("PAS DE PATIENT AVEC LA CIN: %s",cin3);
    printf("\n");
}
else
  {
  printf("MODIFICATION REUSSIE");
  printf("\n");
  }
return l;
}

/*---------------------------suppression du patient--------------------*/

patient supprimer_patient(patient l,char *co)
{
if(l == NULL)
return NULL;
/*Si l'élément en cours de traitement est de valeur x, il doit être supprimé*/
if(l->etat == 'g' || l->etat == 'm')
{
    strcpy(co,l->CIN);
/*On le supprime en prenant soin de mémoriser l'adresse de l'élément suivant*/
element* tmp = l->suivant;
free(l);
/* L'élément ayant été supprimé, la liste commencera à l'élément suivant
pointant sur une liste qui ne contient plus aucun élément ayant la valeur
recherchée */
tmp=supprimer_patient(tmp,co);
return tmp;
}
else
{
/* Si l'élément en cours de traitement ne doit pas être supprimé, alors la liste
finale commencera par cet élément et suivra une liste ne contenant plus
d'élément ayant la valeur recherchée*/
l->suivant = supprimer_patient(l->suivant,co);
return l;
}
}

/*------------------------suppression des maladie chronique---------------*/

maladie supprimer_maladie(maladie m,char *co)
{
if(m == NULL)
return NULL;
/*Si l'élément en cours de traitement est de valeur x, il doit être supprimé*/
if(strcmp(m->CIN,co) == 0)
{
/*On le supprime en prenant soin de mémoriser l'adresse de l'élément suivant*/
element1* tmp = m->suivant;
free(m);
/* L'élément ayant été supprimé, la liste commencera à l'élément suivant
pointant sur une liste qui ne contient plus aucun élément ayant la valeur
recherchée */
tmp=supprimer_maladie(tmp,co);
return tmp;
}
else
{
/* Si l'élément en cours de traitement ne doit pas être supprimé, alors la liste
finale commencera par cet élément et suivra une liste ne contenant plus
d'élément ayant la valeur recherchée*/
m->suivant = supprimer_maladie(m->suivant,co);
return m;
}
}

/*-------------------------ajout d'un hotel---------------------*/
hotel Ajouter_hotel(hotel h)
{
/*On crée un nouvel élément*/
element3* nouveau = (element3*)malloc(sizeof(element3));
/*On assigne la valeur au nouvel élément*/

    printf("inserer le nom de l'hotel: ");
    fflush(stdin);
    scanf("%s",nouveau->nom);
    printf("inserer l'adresse de l'hotel: ");
    fflush(stdin);
    scanf("%s",nouveau->adresse);
    printf("inserer le nombre de lits max: ");
    scanf("%d",&nouveau->nbmax);
     printf("\n");printf("\n");
           nouveau->nbr=0;

/*On ajoute en fin, donc aucun élément ne va suivre*/
nouveau->suivant = NULL;
if(h == NULL)
{
/*Si la liste est vidée il suffit de renvoyer l'élément créé*/
return nouveau;
}
else
{
/*Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
indique que le dernier élément de la liste est relié au nouvel élément*/
element3* tmp=h;
while(tmp->suivant != NULL)
{
tmp = tmp->suivant;
}
tmp->suivant = nouveau;
return h;
}
}
/*-------------------------AFFICHER HOTEL---------------------*/

int affiche_hotel(hotel h)
{
element3* tmp=h;
int c=0;
while(tmp!=NULL){

     printf("NOM:%s\n",tmp->nom);
     printf("\t");
     printf("|ADRESSE:%s\n",tmp->adresse);
     printf("\t");
     printf("PLACE MAX:%d\n",tmp->nbmax);
     printf("\t");
     printf("PLACE REMPLIE:%d\n",tmp->nbr);
     printf("\t");
     printf("\n");

          c++;

     tmp=tmp->suivant;


}
return c;
}
/*----------------------personne en confinement-----------------------*/
int affiche_confinement(personne p)
{
element2* tmp=p;
int c=0;
while(tmp!=NULL){
if(strcmp(tmp->nomh,"none") != 0)
{
     printf("CIN:%s\n",tmp->CINP);
     printf("\t");
     printf("NOM:%s\n",tmp->nom);
     printf("\t");
     printf("PRENOM:%s\n",tmp->adresse);
     printf("\t");
     printf("HOTEL:%s\n",tmp->nomh);
     printf("\t");
     printf("DATE DEBUT:%d/%d/%d\n",tmp->j,tmp->m,tmp->annee);
     printf("\t");
     printf("DATE FIN:%d/%d/%d\n",tmp->j1,tmp->m1,tmp->annee1);
     printf("\t");
     printf("\n");

          c++;
}


     tmp=tmp->suivant;


}
return c;
}
/*--------------------------------nombre dispo dans un hotel------------------------*/
hotel nombre_dispo(personne p,hotel h)
{
element2* tmp=p;
element3* tmp1=h;
                      int c=0;
for(tmp1=h;tmp1!=NULL;tmp1=tmp1->suivant){
      c=0;
     for(tmp=p;tmp!=NULL;tmp=tmp->suivant)
     {
      if(strcmp(tmp->nomh,tmp1->nom) == 0)
      {
          c++;
      }

     }
    tmp1->nbr=c;

}
return h;
}
/*----------------------------afficher maladie chronique pour personne----------------------*/
int affiche_pmaladie(maladie m,char cin2[25])
{
element1* tmp=m;
int c=0;

while(tmp!=NULL){

if(strcmp(tmp->CIN,cin2) == 0)
{
printf("MALADIE:%s\n",tmp->mc);
printf("\t");
   c++;
}
     tmp=tmp->suivant;

}
return c;
}

/*---------------------------afficher personne seule--------------------------*/
int affiche_personneseul(personne p,char cin2[25])
{
 element2* tmp=p;
 int c=0;
 while(tmp!=NULL){

  if (strcmp(tmp->CINP,cin2) == 0)
    {
  printf("NOM:%s\n",tmp->nom);
    printf("\t");

     printf("PRENOM:%s\n",tmp->prenom);
     printf("\t");

      printf("ADRESSE:%s\n",tmp->adresse);printf("\t");
       printf("AGE:%d",tmp->age);
       printf("\t");

        printf("ETAT:%c\n",tmp->etat);
        printf("\t");

        printf("HOTEL:%s\n",tmp->nomh);
        printf("\t");
        printf("\n");
          c++;
}
     tmp=tmp->suivant;


}
return c;
}

/*----------------------------affichage patient par age et genre--------------*/
/*----------------------------confinement personne22------------------------*/

personne transfere_personne123(personne p,hotel h, char cin5[25])
{
element2* tmp=p;
element3* tmp1=h;

int c;
int s=0;
 c=tmp1->nbr;
for(tmp=p;tmp!=NULL;tmp=tmp->suivant){

    if(strcmp(tmp->CINP,cin5) == 0)
     {

    for(tmp1=h;tmp1!=NULL;tmp1=tmp1->suivant)
 {

  if(strcmp(tmp->adresse,tmp1->adresse) == 0 && strcmp(tmp->nomh,"none")==0 && c <=(tmp1->nbmax) )
  {
      strcpy(tmp->nomh,tmp1->nom);
    printf("COMPLETER LES INFOS DE MR %s %s ",tmp->nom,tmp->prenom);
    printf("\n");
    printf("inserer la date debut de confinement: ");
    do{
    printf("inserer le jour: ");
    rewind(stdin);
    scanf("%d",&tmp->j);
    }while(tmp->j<1 && tmp->j>31);

      do{
    printf("inserer le mois: ");
    rewind(stdin);
    scanf("%d",&tmp->m);
    }while(tmp->m<1 && tmp->m>12);

    do{
    printf("inserer l'annee: ");
    rewind(stdin);
    scanf("%d",&tmp->annee);
    }while(tmp->annee<2020);
     printf("\n");
     printf("inserer la date fin de confinement: ");
     printf("\n");
        do{
    printf("inserer le jour: ");
    rewind(stdin);
    scanf("%d",&tmp->j);
    }while(tmp->j1<1 && tmp->j1>31);

      do{
    printf("inserer le mois: ");
    rewind(stdin);
    scanf("%d",&tmp->m1);
    }while(tmp->m1<1 && tmp->m1>12);

    do{
    printf("inserer l'annee: ");
    rewind(stdin);
    scanf("%d",&tmp->annee1);
    }while(tmp->annee1<2020);
     printf("\n");
     c++;
     s++;
     break;
  }


}
}
if(s!=0)
{
    break;
}
if(s!=0)
{
 printf("TRANSFERE REUSSIE");
    break;
}

}
return p;
}


/*---------------------------------affichage par age et genre-------------------*/
 int affiche_ageetgenre(patient l,char g,int a)
{
 element* tmp=l;
 int c=0;
 while(tmp!=NULL){

  if (tmp->genre==g && tmp->age==a)
    {
  printf("NOM:%s\n",tmp->nom);
    printf("\t");

     printf("PRENOM:%s\n",tmp->prenom);
     printf("\t");

      printf("ADRESSE:%s\n",tmp->adresse);printf("\t");
       printf("AGE:%d",tmp->age);
       printf("\t");

        printf("ETAT:%c\n",tmp->etat);
        printf("\t");

        printf("AGE:%d\n",tmp->age);
        printf("\t");

         if(tmp->genre=='h')
         {
             printf("GENRE:HOMME\n");
          }
         else
            {
            printf("GENRE:FEMME\n");
            }
          c++;
}
     tmp=tmp->suivant;


}
return c;
}
/*---------------------------suppresion de la personne----------------------*/
personne supprimer_personne(personne p,char *co)
{
if(p == NULL)
return NULL;
/*Si l'élément en cours de traitement est de valeur x, il doit être supprimé*/
if(p->etat == 'g' || p->etat == 'm')
{
    strcpy(co,p->CINP);
/*On le supprime en prenant soin de mémoriser l'adresse de l'élément suivant*/
element2* tmp = p->suivant;
free(p);
/* L'élément ayant été supprimé, la liste commencera à l'élément suivant
pointant sur une liste qui ne contient plus aucun élément ayant la valeur
recherchée */
tmp=supprimer_personne(tmp,co);
return tmp;
}
else
{
/* Si l'élément en cours de traitement ne doit pas être supprimé, alors la liste
finale commencera par cet élément et suivra une liste ne contenant plus
d'élément ayant la valeur recherchée*/
p->suivant = supprimer_personne(p->suivant,co);
return p;
}
}

/*-----------------------------modification etat du personne-----------------*/

personne modification_etatperso(personne p,char cin3[25])
{
element2* tmp=p;
int c=0;
while(tmp!=NULL){

  if (strcmp(tmp->CINP,cin3) == 0)
    {
 do{
    printf("inserer l'etat normal (n) , critique (c) , gerie (g) , mort (m):");
    rewind(stdin);
    scanf("%c",&tmp->etat);
    }while(tmp->etat!='n' && tmp->etat!='c' && tmp->etat!='g'&& tmp->etat!='m');
    c++;
    break;
}
     tmp=tmp->suivant;


}
if(c==0)
{
    printf("PAS DE PATIENT AVEC LA CIN: %s",cin3);
    printf("\n");
}
else
  {
  printf("MODIFICATION REUSSIE");
  printf("\n");
  }
return p;
}
