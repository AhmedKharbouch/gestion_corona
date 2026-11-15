#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head.h"
/*--------------------ahmed kharbouch 2ap g2 maarif------------------*/
/* Compilation : gcc main.c body.c -o output/main.exe */

/* message affiché une seule fois en haut du menu (chargement, erreur de sauvegarde...) */
static char annonce[200] = "";

/* nom du fichier sans le dossier, pour l'affichage */
static const char *nom_court(const char *chemin)
{
    const char *a = strrchr(chemin, '\\');
    const char *b = strrchr(chemin, '/');
    if(b > a)
        a = b;
    return a ? a + 1 : chemin;
}

/* menu principal avec un résumé des données en haut */
static void afficher_menu(patient l, personne p, hotel h)
{
    ui_titre("GESTION CORONA  ·  HÔPITAL");
    printf("\n   Patients " GRAS "%d" RESET "   " GRIS "·" RESET
           "   Contacts " GRAS "%d" RESET "   " GRIS "·" RESET
           "   Hôtels " GRAS "%d" RESET "   " GRIS "·" RESET
           "   En confinement " GRAS "%d" RESET "\n",
           compter_patients(l), compter_personnes(p), compter_hotels(h), compter_confines(p));
    printf("   " GRIS "Sauvegarde automatique dans %s" RESET "\n", nom_court(fichier_donnees()));
    if(annonce[0] != '\0')
    {
        printf("\n   %s" RESET "\n", annonce);
        annonce[0] = '\0';
    }

    ui_section("PATIENTS");
    ui_item(1, "Ajouter un patient");
    ui_item(2, "Afficher les patients");
    ui_item(5, "Modifier l'état d'un patient");
    ui_section("PERSONNES CONTACT");
    ui_item(7, "Afficher une personne");
    ui_item(9, "Modifier l'état d'une personne");
    ui_section("CONFINEMENT");
    ui_item(3, "Ajouter un hôtel");
    ui_item(4, "Afficher les hôtels");
    ui_item(6, "Transférer une personne en confinement");
    ui_item(8, "Afficher les personnes en confinement");
    printf("\n");
    ui_item(10, "Quitter");
    printf("\n");
    ui_champ("Votre choix");
}

/* lit un entier entre min et max avec un message d'aide */
static int demander(const char *label, int min, int max, const char *aide)
{
    int x;
    for(;;)
    {
        ui_champ(label);
        x = lire_entier();
        if(x >= min && x <= max)
            return x;
        ui_invalide(aide);
    }
}

int main(void)
{
    patient l = NULL;
    maladie m = NULL;
    personne p = NULL;
    hotel h = NULL;
    char cin[25];
    char co1[25];
    char g;
    int n1, n2, n3, i, t, a, avant;
    int choix, choix1;
    int charges, erreurs;

    ui_init();
    charges = charger_donnees(&l, &m, &p, &h, &erreurs);
    if(charges > 0)
        snprintf(annonce, sizeof annonce, VERT "✓ Données de la dernière session rechargées (%d élément(s))." , charges);
    if(erreurs > 0)
        snprintf(annonce + strlen(annonce), sizeof annonce - strlen(annonce),
                 RESET "\n   " JAUNE "i %d ligne(s) illisible(s) ignorée(s) dans le fichier.", erreurs);
    do
    {
        ui_transition();
        afficher_menu(l, p, h);
        choix = lire_entier();
        ui_transition();
        switch(choix)
        {
        case 1:/*----------------------ajout d'un patient------------------------*/
            ui_titre("NOUVEAU PATIENT");
            ui_section("IDENTITÉ");
            for(;;)
            {
                ui_champ("CIN          ");
                lire_cin(cin, sizeof cin);
                if(!patient_existe(l, cin))
                    break;
                ui_invalide("un patient avec cette CIN existe déjà");
            }
            avant = compter_patients(l);
            l = Ajouter_patient(l, cin);
            if(compter_patients(l) == avant)
                break;      /* plus de memoire */

            ui_section("MALADIES CHRONIQUES");
            n1 = demander("Combien ? (0 à 3)", 0, 3, "entre 0 et 3");
            for(i = 1; i <= n1; i++)
                m = Ajouter_maladie(m, cin);

            ui_section("PERSONNES EN CONTACT");
            n2 = demander("Combien ?", 0, 50, "entre 0 et 50");
            for(i = 1; i <= n2; i++)
            {
                ui_section("CONTACT %d / %d", i, n2);
                p = Ajouter_personne(p, cin, co1);
                if(co1[0] == '\0')
                    break;          /* plus de memoire */
                n3 = demander("Maladies chroniques (0 à 3)", 0, 3, "entre 0 et 3");
                for(t = 1; t <= n3; t++)
                    m = Ajouter_maladie(m, co1);
            }
            ui_ok("Patient %s enregistré : %d maladie(s), %d contact(s).", cin, n1, n2);
            break;

        case 2: /*----------------------affichage------------------------*/
            do
            {
                ui_transition();
                ui_titre("AFFICHAGE DES PATIENTS");
                printf("\n");
                ui_item(1, "Rechercher un patient par CIN");
                ui_item(2, "Filtrer par âge et genre");
                ui_item(3, "Retour au menu principal");
                printf("\n");
                ui_champ("Votre choix");
                choix1 = lire_entier();
                ui_transition();
                switch(choix1)
                {
                case 1:
                    ui_titre("FICHE PATIENT");
                    printf("\n");
                    ui_champ("CIN du patient");
                    lire_cin(cin, sizeof cin);
                    if(affiche_patient(l, cin) == 0)
                        ui_erreur("Aucun patient avec la CIN %s.", cin);
                    else
                    {
                        affiche_maladie(m, cin);
                        affiche_personne(p, cin);
                    }
                    ui_pause();
                    break;
                case 2:
                    ui_titre("PATIENTS PAR ÂGE ET GENRE");
                    printf("\n");
                    a = demander("Âge        ", 0, 130, "entre 0 et 130");
                    do{
                        ui_champ("Genre (h/f)");
                        g = lire_caractere();
                        if(g != 'h' && g != 'f')
                            ui_invalide("tapez h ou f");
                    }while(g != 'h' && g != 'f');
                    if(affiche_ageetgenre(l, g, a) == 0)
                        ui_info("Aucun patient de %d ans (%s).", a, g == 'h' ? "homme" : "femme");
                    ui_pause();
                    break;
                case 3:
                    break;
                default:
                    ui_erreur("Choix invalide : tapez 1, 2 ou 3.");
                    ui_pause();
                    break;
                }
            }while(choix1 != 3);
            break;

        case 3:/*----------------------ajout d'un hotel------------------------*/
            ui_titre("NOUVEL HÔTEL");
            printf("\n");
            h = Ajouter_hotel(h);
            break;

        case 4:/*----------------------affichage des hotels------------------------*/
            ui_titre("HÔTELS DE CONFINEMENT");
            h = nombre_dispo(p, h);
            if(affiche_hotel(h) == 0)
                ui_info("Aucun hôtel enregistré. Utilisez le choix 3 pour en ajouter.");
            break;

        case 5:/*----------------------modification etat patient------------------------*/
            ui_titre("MODIFIER L'ÉTAT D'UN PATIENT");
            printf("\n");
            ui_champ("CIN du patient");
            lire_cin(cin, sizeof cin);
            printf("\n");
            l = modification_etat(l, cin);
            /* les patients gueris ou morts sont supprimes avec leurs maladies */
            avant = compter_patients(l);
            l = supprimer_patient(l, &m);
            if(compter_patients(l) < avant)
                ui_info("Patient guéri ou décédé : sa fiche et ses maladies ont été retirées.");
            break;

        case 6:/*----------------------transfert en confinement------------------------*/
            ui_titre("TRANSFERT EN CONFINEMENT");
            h = nombre_dispo(p, h);
            if(h == NULL)
            {
                ui_erreur("Aucun hôtel enregistré. Ajoutez-en un avec le choix 3.");
                break;
            }
            if(affiche_transferables(p, h) == 0)
            {
                if(p == NULL)
                    ui_info("Aucune personne contact enregistrée. Les contacts se saisissent "
                            "avec le patient (choix 1, « Personnes en contact »).");
                else
                    ui_info("Toutes les personnes contact sont déjà en confinement.");
                break;
            }
            ui_champ("CIN de la personne");
            lire_cin(cin, sizeof cin);
            if(!personne_existe(p, cin) && patient_existe(l, cin))
            {
                ui_erreur("%s est la CIN d'un patient, pas d'une personne contact.", cin);
                printf("    " GRIS "Seules les personnes du tableau vont en confinement." RESET "\n");
            }
            else
                p = transfere_personne123(p, h, cin);
            break;

        case 7:/*----------------------affichage d'une personne------------------------*/
            ui_titre("FICHE PERSONNE CONTACT");
            printf("\n");
            ui_champ("CIN de la personne");
            lire_cin(cin, sizeof cin);
            if(affiche_personneseul(p, cin) == 0)
                ui_erreur("Aucune personne contact avec la CIN %s.", cin);
            else
                affiche_maladie(m, cin);
            break;

        case 8:/*----------------------personnes en confinement------------------------*/
            ui_titre("PERSONNES EN CONFINEMENT");
            if(affiche_confinement(p) == 0)
                ui_info("Aucune personne en confinement.");
            break;

        case 9:/*----------------------modification etat personne------------------------*/
            ui_titre("MODIFIER L'ÉTAT D'UNE PERSONNE");
            printf("\n");
            ui_champ("CIN de la personne");
            lire_cin(cin, sizeof cin);
            printf("\n");
            p = modification_etatperso(p, cin);
            /* les personnes gueries ou mortes sont supprimees avec leurs maladies */
            avant = compter_personnes(p);
            p = supprimer_personne(p, &m);
            if(compter_personnes(p) < avant)
                ui_info("Personne guérie ou décédée : sa fiche et ses maladies ont été retirées.");
            break;

        case 10:
            ui_titre("À BIENTÔT");
            if(sauvegarder_donnees(l, m, p, h))
                ui_ok("Données sauvegardées dans %s.", fichier_donnees());
            else
                ui_erreur("Impossible d'écrire %s.", fichier_donnees());
            ui_ok("Merci d'avoir utilisé Gestion Corona.");
            printf("\n");
            break;

        default:
            ui_erreur("Choix invalide : tapez un nombre entre 1 et 10.");
            break;
        }
        /* sauvegarde automatique après chaque modification */
        if(choix == 1 || choix == 3 || choix == 5 || choix == 6 || choix == 9)
        {
            if(!sauvegarder_donnees(l, m, p, h))
                snprintf(annonce, sizeof annonce, ROUGE "× Impossible d'écrire %s : les dernières "
                         "modifications ne sont pas sauvegardées.", nom_court(fichier_donnees()));
        }
        if(choix != 2 && choix != 10)
            ui_pause();
    }while(choix != 10);

    return 0;
}
