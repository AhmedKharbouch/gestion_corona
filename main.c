#include <stdio.h>
#include <stdlib.h>
#include "body.c"
/*--------------------ahmed kharbouch 2ap g2 maarif------------------*/
int main()
{
patient l=NULL;
maladie m=NULL;
personne p=NULL;
hotel h=NULL;
char cin[25];
char cin5[25];
char cin6[25];
char cin3[25];
char co[25];
char co1[25];
char co2[25];
char g;
int n1,i,c,c1,c3,choix,c4,n3,t,c5,c6,c7,choix1,v1,a;
int n2;

/*------------------ veuillez ignorer cette partie merci-------------*/

/*--------------INSERTION PATIENTS MALADIES ET PERSONNES---------------


printf("inserer la cin:");
fflush(stdin);
scanf("%s",cin);
do{
printf("le nombre de maladie chronique ne depasse pas 3:");
scanf("%d",&n1);
}while(n1<0 || n1>3);
printf("le nombre de personne en contact avec le patient:");
scanf("%d",&n2);
l=Ajouter_patient(l,cin);
if(n1!=0)
{
 for(i=1;i<=n1;i++)
 {
  m=Ajouter_maladie(m,cin);
 }
if(n2!=0)
{
for(i=1;i<=n2;i++)
 {
  p=Ajouter_personne(p,cin);
 }
}
}

/*--------------------------FIN DE LINSERTION-------------------------*/

/*------------------affichage patient maladie et personne-------------

printf("INSERER LA CIN DU PATIENT A AFFICHER:");
fflush(stdin);
scanf("%s",cin);
c=affiche_patient(l,cin);
c1=affiche_maladie(m,cin);
c3=affiche_personne(p,cin);
if(c==0)
{
    printf("PAS DE PATIENT AVEC CETTE CIN:%s",cin);
    printf("\n");
}
else
    {
    if(c1==0)
    {
      printf("PAS DE MALADIE CHRONIQUE POUR CE PATIENT");
      printf("\n");
    }
    else
    {}
        if(c3==0)
    {
      printf("PAS DE PERSONNES EN CONTACT AVEC CE PATIENT");
      printf("\n");
    }
    else
        {}
    }
/*--------------INSERTION PATIENTS MALADIES ET PERSONNES---------------*
    printf("inserer la cin:");
fflush(stdin);
scanf("%s",cin);
do{
printf("le nombre de maladie chronique ne depasse pas 3:");
scanf("%d",&n1);
}while(n1<0 || n1>3);
printf("le nombre de personne en contact avec le patient:");
scanf("%d",&n2);
l=Ajouter_patient(l,cin);
if(n1!=0)
{
 for(i=1;i<=n1;i++)
 {
  m=Ajouter_maladie(m,cin);
 }
if(n2!=0)
{
for(i=1;i<=n2;i++)
 {
  p=Ajouter_personne(p,cin);
 }
}
}

/*------------------affichage patient maladie et personne-------------*
printf("INSERER LA CIN DU PATIENT A AFFICHER:");
fflush(stdin);
scanf("%s",cin);
c=affiche_patient(l,cin);do{
printf("le nombre de maladie chronique ne depasse pas 3:");
scanf("%d",&n1);
}while(n1<0 || n1>3);
c1=affiche_maladie(m,cin);
c3=affiche_personne(p,cin);
if(c==0)
{
    printf("PAS DE PATIENT AVEC CETTE CIN:%s",cin);
    printf("\n");
}
else
    {
    if(c1==0)
    {
      printf("PAS DE MALADIE CHRONIQUE POUR CE PATIENT");
      printf("\n");
    }
    else
    {}
        if(c3==0)
    {
      printf("PAS DE PERSONNES EN CONTACT AVEC CE PATIENT");
      printf("\n");
    }
    else
        {}
    }
    /*-------------modification et suppression de l'etat du patient-------------*
printf("INSERER LA CIN DU PATIENT A MODIFIER:");
scanf("%s",cin);
l=modification_etat(l,cin);
affiche_patient(l,cin);
l=supprimer_patient(l,co);
m=supprimer_maladie(m,co);

-------------------------------------FIN-------------------------*/

/*----------------------------------ajout des hotels-----------------------
h=Ajouter_hotel(h);
*/

do
{

printf(" ____________MENU DE GESTION_____________\n");
printf("|         1.AJOUTER UN PATIENT           |\n");
printf("|         2.AFFICHER PATIENT             |\n");
printf("|         3.AJOUTER UN HOTEL             |\n");
printf("|         4.AFFICHER HOTEL               |\n");
printf("|         5.MODIFICATION ETAT PATIENT    |\n");
printf("|         6.TRANSFERE CONFINEMENT        |\n");
printf("|         7.AFFICHER PERSONNE            |\n");
printf("|         8.AFFICHER CONFINEMENT         |\n");
printf("|         9.MODIFICATION ETAT PERSONNE   |\n");
printf("|______________10.QUITTER________________|\n");
printf("\n");
printf("    Entrer un choix : ") ;
scanf("%d",&choix);
system("cls");
switch(choix)
{
case 1:/*----------------------la case 1------------------------*/
printf("inserer la cin:");
fflush(stdin);
scanf("%s",cin);
do{
printf("le nombre de maladie chronique ne depasse pas 3:");
scanf("%d",&n1);
}while(n1<0 || n1>3);
printf("le nombre de personne en contact avec le patient:");
scanf("%d",&n2);
l=Ajouter_patient(l,cin);
if(n1!=0)
{
 for(i=1;i<=n1;i++)
 {
  m=Ajouter_maladie(m,cin);
 }
 }
if(n2!=0)
{
for(i=1;i<=n2;i++)
 {
  p=Ajouter_personne(p,cin,co1);
  do{
printf("le nombre de maladie chronique ne depasse pas 3:");
scanf("%d",&n3);
}while(n3<0 || n3>3);
  if(n3!=0)
  {
      for(t=1;t<=n3;t++)
 {
  m=Ajouter_maladie(m,co1);
 }
  }

 }
}

break;
case 2: /*----------------------la case 2------------------------*/
do
{

printf(" ____________MENU D'AFFICHAGE___________  \n");
printf("|       1.AFFICHER SELON CIN            |\n");
printf("|       2.AFFICHAGE SELON AGE ET GENRE  |\n");
printf("|______________3.QUITTER________________|\n");
printf("\n");
printf("    Entrer un choix : ") ;
scanf("%d",&choix1);
system("cls");
switch(choix1)
{
case 1:
    printf("INSERER LA CIN DU PATIENT A AFFICHER:");
fflush(stdin);
scanf("%s",cin);
c=affiche_patient(l,cin);
c1=affiche_maladie(m,cin);
c3=affiche_personne(p,cin);
if(c==0)
{
    printf("PAS DE PATIENT AVEC CETTE CIN:%s",cin);
    printf("\n");
}
else
    {
    if(c1==0)
    {
      printf("PAS DE MALADIE CHRONIQUE POUR CE PATIENT");
      printf("\n");
    }
    else
    {}
        if(c3==0)
    {
      printf("PAS DE PERSONNES EN CONTACT AVEC CE PATIENT");
      printf("\n");
    }
    else
        {}
    }

break;
case 2:
    printf("INSERER L'AGE: ");
    scanf("%d",&a);
    do{
    printf("INSERER LE GENRE HOMME (h) ET FEMME (f)");
    rewind(stdin);
    scanf("%c",&g);
    }while(g!='h' && g!='f');
    v1=affiche_ageetgenre(l,g,a);
    if(v1==0)
    {
      printf("AUCUN PATIENT AVEC L'AGE:%d ET LE GENRE:%c",a,g);
      printf("\n");
    }
    break;
case 3:/*----------------------la case 8------------------------*/
    printf("\n");
         break;

default:/*----------------------DEFAULT------------------------*/
     printf("\n CHOISIR ENTRE 1 ET 2\n");
     printf("\n");
  break;
}

}
while(choix1!=3);
   break;
case 3:/*----------------------la case 3------------------------*/
 h=Ajouter_hotel(h);
 break;
case 4:/*----------------------la case 4------------------------*/
    h=nombre_dispo(p,h);
    c4=affiche_hotel(h);
    if(c4==0)
{
    printf("PAS D'HOTEL DISPONIBLE");
    printf("\n");

}
 break;
case 5:/*----------------------la case 4------------------------*/
    printf("INSERER LA CIN DU PATIENT A MODIFIER:");
scanf("%s",cin);
l=modification_etat(l,cin);
affiche_patient(l,cin);
l=supprimer_patient(l,co);
m=supprimer_maladie(m,co);
break;
case 6:/*----------------------la case 6------------------------*/
    h=nombre_dispo(p,h);
    printf("INSERER LA CIN DE LA PERSONNE A TRANSFERER:");
    scanf("%s",cin5);
    p=transfere_personne123(p,h,cin5);
    break;

case 7:/*----------------------------------------------*/
       printf("INSERER LA CIN DU PERSONNE A AFFICHER:");
       fflush(stdin);
       scanf("%s",cin);
  c6=affiche_personneseul(p,cin);
  c7=affiche_pmaladie(m,cin);
  if(c6==0)
  {
      printf("PAS DE PERSONNE AVEC LE CIN:%s",cin);
      printf("\n");
  }
  else{
    if(c7==0)
    {
        printf("PAS DE MALADIE POUR CETTE PERSONNE");
         printf("\n");
    }

  }
   break;

case 8:/*----------------------la case 8------------------------*/

   c5=affiche_confinement(p);
   if(c5==0)
   {
     printf("AUCUNE PERSONNE EN CONFINEMENT");
   }
         break;

case 9:/*----------------------la case 9------------------------*/
       printf("INSERER LA CIN DU PATIENT A MODIFIER:");
scanf("%s",cin3);
p=modification_etatperso(p,cin3);
affiche_personneseul(p,cin3);
p=supprimer_personne(p,co);
m=supprimer_maladie(m,co);
         break;

case 10:/*----------------------la case 10------------------------*/
    printf("------------------------MERCI---------------------");
    printf("\n");
         break;

default:/*----------------------DEFAULT------------------------*/
     printf("\n les choix entre 1 et 10\n");
     printf("\n");
  break;
}

}
while(choix!=10);


return 0;
}

/*----------------------------------------------------------------------*/








