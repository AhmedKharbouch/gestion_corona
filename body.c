#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <time.h>
#endif
#include "head.h"

/*--------------------ahmed kharbouch 2ap g2 maarif------------------*/

/* ************************** fonctions d'affichage ************************** */

/* vrai si l'entrée vient d'un fichier (test automatique) et pas du clavier */
static int entree_redirigee(void)
{
#ifdef _WIN32
    return !_isatty(_fileno(stdin));
#else
    return !isatty(fileno(stdin));
#endif
}

/* nombre de caractères affichés d'une chaîne UTF-8 (é compte pour 1) */
static int largeur_utf8(const char *s)
{
    int n = 0;
    for(; *s; s++)
        if(((unsigned char)*s & 0xC0) != 0x80)
            n++;
    return n;
}

static void repeter(const char *motif, int n)
{
    while(n-- > 0)
        fputs(motif, stdout);
}

/* affiche s sur exactement w colonnes (coupe ou complète), suivi d'un espace */
static void colonne(const char *s, int w)
{
    const unsigned char *p = (const unsigned char *)s;
    int n = 0;
    while(*p && n < w)
    {
        const unsigned char *q = p + 1;
        while((*q & 0xC0) == 0x80)
            q++;
        fwrite(p, 1, q - p, stdout);
        p = q;
        n++;
    }
    repeter(" ", w - n + 1);
}

/* ************************** animations ************************** */
/* Mettre la variable d'environnement CORONA_SANS_ANIMATION=1 pour les désactiver. */
static int animations = 1;

/* pause en millisecondes (minuterie haute précision sous Windows) */
static void dormir(int ms)
{
    fflush(stdout);
#ifdef _WIN32
    static HANDLE minuterie = NULL;
    LARGE_INTEGER d;
    if(minuterie == NULL)
        minuterie = CreateWaitableTimerExW(NULL, NULL, 0x00000002 /* HIGH_RESOLUTION */, TIMER_ALL_ACCESS);
    if(minuterie != NULL)
    {
        d.QuadPart = -(LONGLONG)ms * 10000;
        if(SetWaitableTimer(minuterie, &d, 0, NULL, NULL, FALSE))
        {
            WaitForSingleObject(minuterie, INFINITE);
            return;
        }
    }
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

static void cacher_curseur(void)  { printf("\033[?25l"); }
static void montrer_curseur(void) { printf("\033[?25h"); fflush(stdout); }

/* taille visible de la console (colonnes, lignes) */
static void taille_console(int *w, int *h)
{
    *w = 80;
    *h = 25;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
    {
        *w = info.srWindow.Right - info.srWindow.Left + 1;
        *h = info.srWindow.Bottom - info.srWindow.Top + 1;
    }
#endif
}

/* petite pause entre deux lignes : le contenu apparaît en cascade */
void ui_cascade(void)
{
    if(animations)
        dormir(14);
}

/* active les couleurs et l'UTF-8 dans la console Windows */
void ui_init(void)
{
    const char *sans = getenv("CORONA_SANS_ANIMATION");
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if(GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | 0x0004); /* ENABLE_VIRTUAL_TERMINAL_PROCESSING */
    SetConsoleTitleA("Gestion Corona");
#endif
    if(sans != NULL && sans[0] != '\0' && sans[0] != '0')
        animations = 0;
    atexit(montrer_curseur);
}

void ui_effacer(void)
{
    printf("\033[H\033[2J\033[3J");
}

/* transition entre deux pages : une bande dégradée balaie l'écran de haut en bas */
void ui_transition(void)
{
    static const char *bande[3] = { "▓", "▒", "░" };   /* du haut (traîne) vers le bas (front) */
    static const char *couleur[3] = { BLEU, CYAN, CYAN };
    int w, h, r, i, k;
    if(!animations)
    {
        ui_effacer();
        return;
    }
    taille_console(&w, &h);
    if(h > 40)
        h = 40;
    w--;                       /* évite le retour à la ligne automatique */
    cacher_curseur();
    for(r = 1; r <= h + 3; r += 2)
    {
        for(i = 0; i < 3; i++)
        {
            int ligne = r - 2 + i;
            if(ligne < 1 || ligne > h)
                continue;
            printf("\033[%d;1H%s", ligne, couleur[i]);
            for(k = 0; k < w; k++)
                fputs(bande[i], stdout);
        }
        for(i = 3; i <= 4; i++)        /* derrière la bande : écran vide */
            if(r - i >= 1 && r - i <= h)
                printf("\033[%d;1H\033[2K", r - i);
        printf(RESET);
        dormir(9);
    }
    ui_effacer();
    montrer_curseur();
}

/* écrit les k premiers caractères (UTF-8) de s */
static void ecrire_debut(const char *s, int k)
{
    const unsigned char *p = (const unsigned char *)s;
    while(*p && k > 0)
    {
        const unsigned char *q = p + 1;
        while((*q & 0xC0) == 0x80)
            q++;
        fwrite(p, 1, q - p, stdout);
        p = q;
        k--;
    }
}

/* dessine le cadre du titre sur 3 lignes : largeur w, k caractères du titre visibles */
static void dessiner_cadre(int w, const char *titre, int k)
{
    int l = largeur_utf8(titre);
    int marge = (LARGEUR - w) / 2;
    int g = (w - 2 - l) / 2;
    int d = w - 2 - l - g;
    printf("\r\033[2K ");
    repeter(" ", marge);
    printf(CYAN "╔");
    repeter("═", w - 2);
    printf("╗\n\r\033[2K ");
    repeter(" ", marge);
    printf("║" RESET GRAS);
    if(w - 2 >= l)
    {
        repeter(" ", g);
        ecrire_debut(titre, k);
        repeter(" ", l - k + d);
    }
    else
        repeter(" ", w - 2);
    printf(RESET CYAN "║\n\r\033[2K ");
    repeter(" ", marge);
    printf("╚");
    repeter("═", w - 2);
    printf("╝" RESET "\n");
}

/* grand titre encadré et centré ; animé : le cadre s'ouvre puis le titre s'écrit */
void ui_titre(const char *titre)
{
    int l = largeur_utf8(titre);
    int w, k;
    printf("\n");
    if(!animations)
    {
        dessiner_cadre(LARGEUR, titre, l);
        return;
    }
    cacher_curseur();
    dessiner_cadre(4, titre, 0);
    for(w = 4 + 10; w < LARGEUR; w += 10)
    {
        dormir(12);
        printf("\033[3A");
        dessiner_cadre(w, titre, 0);
    }
    for(k = 0; k <= l; k += 2)
    {
        dormir(10);
        printf("\033[3A");
        dessiner_cadre(LARGEUR, titre, k);
    }
    printf("\033[3A");
    dessiner_cadre(LARGEUR, titre, l);
    montrer_curseur();
}

/* petit titre de section avec un trait */
void ui_section(const char *fmt, ...)
{
    char texte[100];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(texte, sizeof texte, fmt, ap);
    va_end(ap);
    ui_cascade();
    printf("\n " GRAS BLEU "%s " RESET GRIS, texte);
    repeter("─", LARGEUR - largeur_utf8(texte) - 1);
    printf(RESET "\n");
}

void ui_message(const char *couleur, const char *symbole, const char *fmt, ...)
{
    va_list ap;
    printf("\n  %s%s ", couleur, symbole);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf(RESET "\n");
}

/* invite de saisie ; le texte tapé s'affiche en jaune */
void ui_champ(const char *label)
{
    printf("  " CYAN "›" RESET " %s : " JAUNE, label);
}

void ui_invalide(const char *aide)
{
    printf(ROUGE "    × %s" RESET "\n", aide);
}

void ui_pause(void)
{
    int ch;
    printf("\n  " GRIS "Appuyez sur Entrée pour continuer..." RESET);
    while((ch = getchar()) != '\n')
        if(ch == EOF)
            exit(0);
}

/* une ligne de menu : numéro en couleur + texte */
void ui_item(int numero, const char *texte)
{
    ui_cascade();
    printf("    " GRAS CYAN "%2d" RESET "  %s\n", numero, texte);
}

/* libellé gris aligné pour les fiches */
static void libelle(const char *l)
{
    ui_cascade();
    printf("   " GRIS);
    colonne(l, 15);
    printf(RESET);
}

static const char *etat_nom(char e)
{
    switch(e)
    {
    case 'n': return "Normal";
    case 'c': return "Critique";
    case 'g': return "Guéri";
    case 'm': return "Décédé";
    }
    return "?";
}

static const char *etat_couleur(char e)
{
    switch(e)
    {
    case 'n': return VERT;
    case 'c': return ROUGE;
    case 'g': return CYAN;
    }
    return GRIS;
}

/* état en couleur sur w colonnes */
static void etat_colonne(char e, int w)
{
    printf("%s", etat_couleur(e));
    colonne(etat_nom(e), w);
    printf(RESET);
}

static void nom_complet(char *dest, size_t taille, const char *nom, const char *prenom)
{
    snprintf(dest, taille, "%s %s", nom, prenom);
}

/* ************************** fonctions de saisie ************************** */
/* Elles remplacent fflush(stdin) / rewind(stdin), qui ne sont pas du C standard,
   et évitent les débordements de tableaux et les boucles infinies. */

/* jette le reste de la ligne tapée par l'utilisateur */
void vider_buffer(void)
{
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}

/* lit une ligne (espaces autorisés) sans dépasser la taille du tableau */
void lire_chaine(char *s, int taille)
{
    size_t n;
    for(;;)
    {
        if(fgets(s, taille, stdin) == NULL)
        {
            printf(RESET "\n");
            exit(0);
        }
        n = strlen(s);
        if(n > 0 && s[n-1] == '\n')
            s[--n] = '\0';
        else
            vider_buffer();          /* saisie trop longue : on coupe */
        while(n > 0 && (s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t'))
            s[--n] = '\0';         /* espaces en fin de saisie */
        if(n > 0 && (s[0] == ' ' || s[0] == '\t'))
        {
            size_t d = strspn(s, " \t");   /* espaces en début de saisie */
            memmove(s, s + d, n - d + 1);
            n -= d;
        }
        if(n > 0)
            break;
        if(entree_redirigee())
            printf("\n");
        printf(ROUGE "    × champ obligatoire, recommencez : " JAUNE);
    }
    for(char *c = s; *c; c++)       /* '|' est réservé au fichier de sauvegarde */
        if(*c == '|')
            *c = '/';
    if(entree_redirigee())
        printf("%s\n", s);
    printf(RESET);
}

/* lit une CIN : sans espaces autour et en majuscules, pour que ab123 et AB123 soient la même */
void lire_cin(char *s, int taille)
{
    lire_chaine(s, taille);
    for(; *s; s++)
        if(*s >= 'a' && *s <= 'z')
            *s = (char)(*s - 'a' + 'A');
}

/* lit un entier ; redemande tant que la saisie n'est pas un nombre */
int lire_entier(void)
{
    int x, r;
    while((r = scanf("%d", &x)) != 1)
    {
        if(r == EOF)
        {
            printf(RESET "\n");
            exit(0);
        }
        vider_buffer();
        if(entree_redirigee())
            printf("\n");
        printf(ROUGE "    × entrez un nombre : " JAUNE);
    }
    vider_buffer();
    if(entree_redirigee())
        printf("%d\n", x);
    printf(RESET);
    return x;
}

/* lit un caractère en ignorant les espaces et retours à la ligne */
char lire_caractere(void)
{
    int ch;
    do
    {
        ch = getchar();
        if(ch == EOF)
        {
            printf(RESET "\n");
            exit(0);
        }
    }while(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
    vider_buffer();
    if(entree_redirigee())
        printf("%c\n", ch);
    printf(RESET);
    return (char)ch;
}

/* lit un entier compris entre min et max */
static int lire_entre(const char *label, int min, int max, const char *aide)
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

/* lit un caractère parmi ceux de "permis" */
static char lire_choix(const char *label, const char *permis, const char *aide)
{
    char c;
    for(;;)
    {
        ui_champ(label);
        c = lire_caractere();
        if(strchr(permis, c) != NULL)
            return c;
        ui_invalide(aide);
    }
}

/* lit une date valide (jour 1-31, mois 1-12, annee >= 2020) */
static void lire_date(const char *titre, int *j, int *m, int *a)
{
    printf("   " GRAS "%s" RESET "\n", titre);
    *j = lire_entre("Jour ", 1, 31, "le jour doit être entre 1 et 31");
    *m = lire_entre("Mois ", 1, 12, "le mois doit être entre 1 et 12");
    *a = lire_entre("Année", 2020, 2100, "l'année doit être 2020 ou après");
}

/* saisie commune aux patients et aux personnes contact */
static void lire_identite(char *nom, int tnom, char *prenom, int tprenom, char *adresse, int tadresse,
                          int *age, char *genre, int *conta, char *etat)
{
    ui_champ("Nom          ");
    lire_chaine(nom, tnom);
    ui_champ("Prénom       ");
    lire_chaine(prenom, tprenom);
    ui_champ("Boulevard    ");
    lire_chaine(adresse, tadresse);
    *age   = lire_entre("Âge          ", 0, 130, "l'âge doit être entre 0 et 130");
    *genre = lire_choix("Genre (h/f)  ", "hf", "tapez h pour homme ou f pour femme");
    *conta = lire_entre("Contamination locale ? (1 = oui, 0 = non)", 0, 1, "tapez 1 ou 0");
    *etat  = lire_choix("État (n = normal, c = critique)", "nc", "tapez n ou c");
}

/* ************************** compteurs ************************** */
int compter_patients(patient l)
{
    int n = 0;
    for(; l != NULL; l = l->suivant)
        n++;
    return n;
}

int compter_personnes(personne p)
{
    int n = 0;
    for(; p != NULL; p = p->suivant)
        n++;
    return n;
}

int compter_hotels(hotel h)
{
    int n = 0;
    for(; h != NULL; h = h->suivant)
        n++;
    return n;
}

int compter_confines(personne p)
{
    int n = 0;
    for(; p != NULL; p = p->suivant)
        if(strcmp(p->nomh, "none") != 0)
            n++;
    return n;
}

int patient_existe(patient l, char cin[25])
{
    for(; l != NULL; l = l->suivant)
        if(strcmp(l->CIN, cin) == 0)
            return 1;
    return 0;
}

/* ***************************ajout patient********************************* */
patient Ajouter_patient(patient l, char cin1[25])
{
    /*On crée un nouvel élément*/
    element* nouveau = (element*)malloc(sizeof(element));
    if(nouveau == NULL)
    {
        ui_erreur("Mémoire insuffisante.");
        return l;
    }
    /*On assigne la valeur au nouvel élément*/
    strcpy(nouveau->CIN, cin1);
    lire_identite(nouveau->nom, sizeof nouveau->nom, nouveau->prenom, sizeof nouveau->prenom,
                  nouveau->adresse, sizeof nouveau->adresse,
                  &nouveau->age, &nouveau->genre, &nouveau->conta, &nouveau->etat);

    /*On ajoute en fin, donc aucun élément ne va suivre*/
    nouveau->suivant = NULL;
    if(l == NULL)
        return nouveau;   /*Si la liste est vide il suffit de renvoyer l'élément créé*/

    /*Sinon, on parcourt la liste et on relie le dernier élément au nouveau*/
    element* tmp = l;
    while(tmp->suivant != NULL)
        tmp = tmp->suivant;
    tmp->suivant = nouveau;
    return l;
}

/* ***************************fiche d'un patient********************* */
int affiche_patient(patient l, char cin3[25])
{
    element* tmp = l;
    char nc[50];
    int c = 0;
    while(tmp != NULL)
    {
        if(strcmp(tmp->CIN, cin3) == 0)
        {
            ui_section("PATIENT  %s", tmp->CIN);
            nom_complet(nc, sizeof nc, tmp->nom, tmp->prenom);
            libelle("Nom complet");   printf(GRAS "%s" RESET "\n", nc);
            libelle("Adresse");       printf("%s\n", tmp->adresse);
            libelle("Âge");           printf("%d ans\n", tmp->age);
            libelle("Genre");         printf("%s\n", tmp->genre == 'h' ? "Homme" : "Femme");
            libelle("État");          etat_colonne(tmp->etat, 10); printf("\n");
            libelle("Contamination"); printf("%s\n", tmp->conta == 1 ? "Locale" : "Non locale");
            c++;
        }
        tmp = tmp->suivant;
    }
    return c;
}

/*-------------------------fonction remplissage maladie chronique--------------------*/
maladie Ajouter_maladie(maladie m, char cin2[25])
{
    element1* nouveau = (element1*)malloc(sizeof(element1));
    if(nouveau == NULL)
    {
        ui_erreur("Mémoire insuffisante.");
        return m;
    }
    strcpy(nouveau->CIN, cin2);
    ui_champ("Maladie chronique");
    lire_chaine(nouveau->mc, sizeof nouveau->mc);

    nouveau->suivant = NULL;
    if(m == NULL)
        return nouveau;

    element1* tmp = m;
    while(tmp->suivant != NULL)
        tmp = tmp->suivant;
    tmp->suivant = nouveau;
    return m;
}

/*----------------------ligne "Maladies" d'une fiche (patient ou personne)-------------------*/
int affiche_maladie(maladie m, char cin3[25])
{
    element1* tmp = m;
    int c = 0;
    libelle("Maladies");
    while(tmp != NULL)
    {
        if(strcmp(tmp->CIN, cin3) == 0)
        {
            printf("%s" JAUNE "%s" RESET, c > 0 ? ", " : "", tmp->mc);
            c++;
        }
        tmp = tmp->suivant;
    }
    if(c == 0)
        printf(GRIS "aucune" RESET);
    printf("\n");
    return c;
}

/*----------------insertion personne-----------------------*/
/* co reçoit la CIN de la personne ajoutée, pour lui associer ses maladies */
personne Ajouter_personne(personne p, char cin2[25], char *co)
{
    element2* nouveau = (element2*)malloc(sizeof(element2));
    if(nouveau == NULL)
    {
        ui_erreur("Mémoire insuffisante.");
        co[0] = '\0';
        return p;
    }
    ui_champ("CIN          ");
    lire_cin(nouveau->CINP, sizeof nouveau->CINP);
    strcpy(co, nouveau->CINP);
    strcpy(nouveau->CIN, cin2);
    strcpy(nouveau->nomh, "none");
    nouveau->j = nouveau->m = nouveau->annee = 0;
    nouveau->j1 = nouveau->m1 = nouveau->annee1 = 0;
    lire_identite(nouveau->nom, sizeof nouveau->nom, nouveau->prenom, sizeof nouveau->prenom,
                  nouveau->adresse, sizeof nouveau->adresse,
                  &nouveau->age, &nouveau->genre, &nouveau->conta, &nouveau->etat);

    nouveau->suivant = NULL;
    if(p == NULL)
        return nouveau;

    element2* tmp = p;
    while(tmp->suivant != NULL)
        tmp = tmp->suivant;
    tmp->suivant = nouveau;
    return p;
}

/*--------------------------tableau des personnes en contact avec un patient-------------------*/
int affiche_personne(personne p, char cin3[25])
{
    element2* tmp;
    char nc[50];
    int c = 0;
    for(tmp = p; tmp != NULL; tmp = tmp->suivant)
        if(strcmp(tmp->CIN, cin3) == 0)
            c++;

    ui_section("PERSONNES EN CONTACT  (%d)", c);
    if(c == 0)
    {
        printf("   " GRIS "aucune personne en contact" RESET "\n");
        return 0;
    }
    printf("   " GRIS);
    colonne("CIN", 10); colonne("Nom complet", 22); colonne("Âge", 4); colonne("État", 9); colonne("Hôtel", 14);
    printf(RESET "\n");
    for(tmp = p; tmp != NULL; tmp = tmp->suivant)
    {
        if(strcmp(tmp->CIN, cin3) == 0)
        {
            char age[8];
            nom_complet(nc, sizeof nc, tmp->nom, tmp->prenom);
            snprintf(age, sizeof age, "%d", tmp->age);
            ui_cascade();
            printf("   ");
            colonne(tmp->CINP, 10); colonne(nc, 22); colonne(age, 4); etat_colonne(tmp->etat, 9);
            if(strcmp(tmp->nomh, "none") == 0)
                printf(GRIS "—" RESET);
            else
                printf("%s", tmp->nomh);
            printf("\n");
        }
    }
    return c;
}

/*-----------------------------modification etat du patient-----------------*/
patient modification_etat(patient l, char cin3[25])
{
    element* tmp = l;
    while(tmp != NULL)
    {
        if(strcmp(tmp->CIN, cin3) == 0)
        {
            printf("   Patient : " GRAS "%s %s" RESET "  (état actuel : ", tmp->nom, tmp->prenom);
            printf("%s%s" RESET ")\n\n", etat_couleur(tmp->etat), etat_nom(tmp->etat));
            tmp->etat = lire_choix("Nouvel état (n = normal, c = critique, g = guéri, m = décédé)",
                                   "ncgm", "tapez n, c, g ou m");
            ui_ok("État mis à jour : %s.", etat_nom(tmp->etat));
            return l;
        }
        tmp = tmp->suivant;
    }
    ui_erreur("Aucun patient avec la CIN %s.", cin3);
    return l;
}

/*---------------------------suppression des patients gueris ou morts--------------------*/
/* Supprime chaque patient d'état 'g' ou 'm' ET ses maladies chroniques. */
patient supprimer_patient(patient l, maladie *m)
{
    if(l == NULL)
        return NULL;
    if(l->etat == 'g' || l->etat == 'm')
    {
        element* tmp = l->suivant;
        *m = supprimer_maladie(*m, l->CIN);
        free(l);
        return supprimer_patient(tmp, m);
    }
    l->suivant = supprimer_patient(l->suivant, m);
    return l;
}

/*------------------------suppression des maladies chroniques d'une CIN---------------*/
maladie supprimer_maladie(maladie m, char *cin)
{
    if(m == NULL)
        return NULL;
    if(strcmp(m->CIN, cin) == 0)
    {
        element1* tmp = m->suivant;
        free(m);
        return supprimer_maladie(tmp, cin);
    }
    m->suivant = supprimer_maladie(m->suivant, cin);
    return m;
}

/*-------------------------ajout d'un hotel---------------------*/
hotel Ajouter_hotel(hotel h)
{
    element3* nouveau = (element3*)malloc(sizeof(element3));
    if(nouveau == NULL)
    {
        ui_erreur("Mémoire insuffisante.");
        return h;
    }
    ui_champ("Nom de l'hôtel    ");
    lire_chaine(nouveau->nom, sizeof nouveau->nom);
    ui_champ("Adresse (boulevard)");
    lire_chaine(nouveau->adresse, sizeof nouveau->adresse);
    nouveau->nbmax = lire_entre("Nombre de lits     ", 1, 100000, "au moins 1 lit");
    nouveau->nbr = 0;
    ui_ok("Hôtel « %s » ajouté avec %d lit(s).", nouveau->nom, nouveau->nbmax);

    nouveau->suivant = NULL;
    if(h == NULL)
        return nouveau;

    element3* tmp = h;
    while(tmp->suivant != NULL)
        tmp = tmp->suivant;
    tmp->suivant = nouveau;
    return h;
}

/*-------------------------tableau des hotels avec jauge d'occupation---------------------*/
int affiche_hotel(hotel h)
{
    element3* tmp;
    int c = 0;
    const int jauge = 12;
    if(h == NULL)
        return 0;
    printf("\n   " GRIS);
    colonne("Hôtel", 16); colonne("Adresse", 14); colonne("Occupation", jauge + 6); colonne("Libres", 6);
    printf(RESET "\n   " GRIS);
    repeter("─", LARGEUR - 3);
    printf(RESET "\n");
    for(tmp = h; tmp != NULL; tmp = tmp->suivant)
    {
        int plein = tmp->nbr * jauge / tmp->nbmax;
        const char *coul = tmp->nbr >= tmp->nbmax ? ROUGE : (tmp->nbr * 2 >= tmp->nbmax ? JAUNE : VERT);
        char occ[16];
        if(tmp->nbr > 0 && plein == 0)
            plein = 1;
        ui_cascade();
        printf("   " GRAS);
        colonne(tmp->nom, 16);
        printf(RESET);
        colonne(tmp->adresse, 14);
        printf("%s", coul);
        repeter("█", plein);
        printf(GRIS);
        repeter("░", jauge - plein);
        snprintf(occ, sizeof occ, " %d/%d", tmp->nbr, tmp->nbmax);
        printf(RESET);
        colonne(occ, 6);
        printf("%s%d" RESET "\n", coul, tmp->nbmax - tmp->nbr);
        c++;
    }
    return c;
}

/*----------------------tableau des personnes en confinement-----------------------*/
int affiche_confinement(personne p)
{
    element2* tmp;
    char nc[50], d1[12], d2[12];
    int c = compter_confines(p);
    if(c == 0)
        return 0;
    printf("\n   " GRIS);
    colonne("CIN", 8); colonne("Nom complet", 18); colonne("Hôtel", 13); colonne("Début", 10); colonne("Fin", 10);
    printf(RESET "\n   " GRIS);
    repeter("─", LARGEUR - 3);
    printf(RESET "\n");
    for(tmp = p; tmp != NULL; tmp = tmp->suivant)
    {
        if(strcmp(tmp->nomh, "none") != 0)
        {
            nom_complet(nc, sizeof nc, tmp->nom, tmp->prenom);
            snprintf(d1, sizeof d1, "%02d/%02d/%04d", tmp->j, tmp->m, tmp->annee);
            snprintf(d2, sizeof d2, "%02d/%02d/%04d", tmp->j1, tmp->m1, tmp->annee1);
            ui_cascade();
            printf("   ");
            colonne(tmp->CINP, 8);
            printf(GRAS); colonne(nc, 18); printf(RESET);
            printf(CYAN); colonne(tmp->nomh, 13); printf(RESET);
            colonne(d1, 10); colonne(d2, 10);
            printf("\n");
        }
    }
    printf("\n   " GRIS "%d personne(s) en confinement" RESET "\n", c);
    return c;
}

/*--------------------------------nombre de places occupées dans chaque hotel------------------------*/
hotel nombre_dispo(personne p, hotel h)
{
    element2* tmp;
    element3* tmp1;
    int c;
    for(tmp1 = h; tmp1 != NULL; tmp1 = tmp1->suivant)
    {
        c = 0;
        for(tmp = p; tmp != NULL; tmp = tmp->suivant)
        {
            if(strcmp(tmp->nomh, tmp1->nom) == 0)
                c++;
        }
        tmp1->nbr = c;
    }
    return h;
}

/*---------------------------fiche d'une personne contact--------------------------*/
int affiche_personneseul(personne p, char cin2[25])
{
    element2* tmp = p;
    char nc[50];
    int c = 0;
    while(tmp != NULL)
    {
        if(strcmp(tmp->CINP, cin2) == 0)
        {
            ui_section("PERSONNE CONTACT  %s", tmp->CINP);
            nom_complet(nc, sizeof nc, tmp->nom, tmp->prenom);
            libelle("Nom complet");  printf(GRAS "%s" RESET "\n", nc);
            libelle("Contact de");   printf("patient %s\n", tmp->CIN);
            libelle("Adresse");      printf("%s\n", tmp->adresse);
            libelle("Âge");          printf("%d ans\n", tmp->age);
            libelle("Genre");        printf("%s\n", tmp->genre == 'h' ? "Homme" : "Femme");
            libelle("État");         etat_colonne(tmp->etat, 10); printf("\n");
            libelle("Confinement");
            if(strcmp(tmp->nomh, "none") == 0)
                printf(GRIS "non confiné(e)" RESET "\n");
            else
                printf(CYAN "%s" RESET "  du %02d/%02d/%04d au %02d/%02d/%04d\n", tmp->nomh,
                       tmp->j, tmp->m, tmp->annee, tmp->j1, tmp->m1, tmp->annee1);
            c++;
        }
        tmp = tmp->suivant;
    }
    return c;
}

/* compare deux textes sans tenir compte des majuscules (Maarif = maarif) */
static int meme_texte(const char *a, const char *b)
{
    for(; *a && *b; a++, b++)
    {
        char x = (*a >= 'A' && *a <= 'Z') ? (char)(*a - 'A' + 'a') : *a;
        char y = (*b >= 'A' && *b <= 'Z') ? (char)(*b - 'A' + 'a') : *b;
        if(x != y)
            return 0;
    }
    return *a == *b;
}

/* premier hôtel à cette adresse qui a encore un lit libre, ou NULL */
static element3* hotel_disponible(hotel h, const char *adresse)
{
    for(; h != NULL; h = h->suivant)
        if(meme_texte(h->adresse, adresse) && h->nbr < h->nbmax)
            return h;
    return NULL;
}

int personne_existe(personne p, char cin[25])
{
    for(; p != NULL; p = p->suivant)
        if(strcmp(p->CINP, cin) == 0)
            return 1;
    return 0;
}

/* tableau des personnes contact pas encore confinées, avec l'hôtel qui peut les accueillir */
int affiche_transferables(personne p, hotel h)
{
    element2* tmp;
    element3* dispo;
    char nc[50];
    int c = 0;
    for(tmp = p; tmp != NULL; tmp = tmp->suivant)
        if(strcmp(tmp->nomh, "none") == 0)
            c++;
    if(c == 0)
        return 0;

    ui_section("PERSONNES CONTACT À TRANSFÉRER  (%d)", c);
    printf("   " GRIS);
    colonne("CIN", 8); colonne("Nom complet", 18); colonne("Boulevard", 14); colonne("Hôtel disponible", 16);
    printf(RESET "\n");
    for(tmp = p; tmp != NULL; tmp = tmp->suivant)
    {
        if(strcmp(tmp->nomh, "none") == 0)
        {
            ui_cascade();
            nom_complet(nc, sizeof nc, tmp->nom, tmp->prenom);
            printf("   " GRAS);
            colonne(tmp->CINP, 8);
            printf(RESET);
            colonne(nc, 18);
            colonne(tmp->adresse, 14);
            dispo = hotel_disponible(h, tmp->adresse);
            if(dispo != NULL)
                printf(VERT "%s" RESET "\n", dispo->nom);
            else
                printf(ROUGE "aucun à cette adresse" RESET "\n");
        }
    }
    printf("\n");
    return c;
}

/*----------------------------transfert d'une personne en confinement------------------------*/
/* Place la personne dans le premier hotel de la même adresse qui a encore de la place.
   Appeler nombre_dispo avant pour que les compteurs nbr soient à jour. */
personne transfere_personne123(personne p, hotel h, char cin5[25])
{
    element2* tmp;
    element3* tmp1;

    if(h == NULL)
    {
        ui_erreur("Aucun hôtel enregistré. Ajoutez-en un avec le choix 3.");
        return p;
    }

    /* recherche de la personne */
    for(tmp = p; tmp != NULL; tmp = tmp->suivant)
    {
        if(strcmp(tmp->CINP, cin5) == 0)
            break;
    }
    if(tmp == NULL)
    {
        ui_erreur("Aucune personne contact avec la CIN %s.", cin5);
        return p;
    }
    if(strcmp(tmp->nomh, "none") != 0)
    {
        ui_info("%s %s est déjà en confinement à l'hôtel %s.", tmp->nom, tmp->prenom, tmp->nomh);
        return p;
    }

    /* recherche d'un hotel à la même adresse avec une place libre */
    tmp1 = hotel_disponible(h, tmp->adresse);
    if(tmp1 == NULL)
    {
        ui_erreur("Aucun hôtel avec des lits libres au boulevard « %s ».", tmp->adresse);
        printf("    " GRIS "L'hôtel doit avoir la même adresse que la personne (voir le choix 4)." RESET "\n");
        return p;
    }

    printf("\n   " GRAS "%s %s" RESET "  →  " CYAN "%s" RESET "  (%d/%d lits occupés)\n\n",
           tmp->nom, tmp->prenom, tmp1->nom, tmp1->nbr, tmp1->nbmax);
    lire_date("Début du confinement", &tmp->j, &tmp->m, &tmp->annee);
    printf("\n");
    for(;;)
    {
        lire_date("Fin du confinement", &tmp->j1, &tmp->m1, &tmp->annee1);
        if(tmp->annee1*10000 + tmp->m1*100 + tmp->j1 >= tmp->annee*10000 + tmp->m*100 + tmp->j)
            break;
        ui_invalide("la date de fin doit être après la date de début");
    }

    strcpy(tmp->nomh, tmp1->nom);
    tmp1->nbr++;
    ui_ok("%s %s est transféré(e) à l'hôtel %s (%d/%d lits).", tmp->nom, tmp->prenom,
          tmp1->nom, tmp1->nbr, tmp1->nbmax);
    return p;
}

/*---------------------------------tableau des patients selon age et genre-------------------*/
int affiche_ageetgenre(patient l, char g, int a)
{
    element* tmp;
    char nc[50];
    int c = 0;
    for(tmp = l; tmp != NULL; tmp = tmp->suivant)
        if(tmp->genre == g && tmp->age == a)
            c++;
    if(c == 0)
        return 0;

    ui_section("%d PATIENT(S) — %s DE %d ANS", c, g == 'h' ? "HOMMES" : "FEMMES", a);
    printf("   " GRIS);
    colonne("CIN", 10); colonne("Nom complet", 22); colonne("Adresse", 18); colonne("État", 9);
    printf(RESET "\n");
    for(tmp = l; tmp != NULL; tmp = tmp->suivant)
    {
        if(tmp->genre == g && tmp->age == a)
        {
            nom_complet(nc, sizeof nc, tmp->nom, tmp->prenom);
            ui_cascade();
            printf("   ");
            colonne(tmp->CIN, 10); colonne(nc, 22); colonne(tmp->adresse, 18); etat_colonne(tmp->etat, 9);
            printf("\n");
        }
    }
    return c;
}

/*---------------------------suppression des personnes gueries ou mortes----------------------*/
/* Supprime chaque personne d'état 'g' ou 'm' ET ses maladies chroniques. */
personne supprimer_personne(personne p, maladie *m)
{
    if(p == NULL)
        return NULL;
    if(p->etat == 'g' || p->etat == 'm')
    {
        element2* tmp = p->suivant;
        *m = supprimer_maladie(*m, p->CINP);
        free(p);
        return supprimer_personne(tmp, m);
    }
    p->suivant = supprimer_personne(p->suivant, m);
    return p;
}

/*-----------------------------modification etat d'une personne-----------------*/
personne modification_etatperso(personne p, char cin3[25])
{
    element2* tmp = p;
    while(tmp != NULL)
    {
        if(strcmp(tmp->CINP, cin3) == 0)
        {
            printf("   Personne : " GRAS "%s %s" RESET "  (état actuel : ", tmp->nom, tmp->prenom);
            printf("%s%s" RESET ")\n\n", etat_couleur(tmp->etat), etat_nom(tmp->etat));
            tmp->etat = lire_choix("Nouvel état (n = normal, c = critique, g = guéri, m = décédé)",
                                   "ncgm", "tapez n, c, g ou m");
            ui_ok("État mis à jour : %s.", etat_nom(tmp->etat));
            return p;
        }
        tmp = tmp->suivant;
    }
    ui_erreur("Aucune personne contact avec la CIN %s.", cin3);
    return p;
}

/* ************************** sauvegarde des données ************************** */
/* Toutes les listes sont enregistrées dans un fichier texte, une ligne par élément :
     H|nom|adresse|places occupées|places max                      (hôtel)
     P|CIN|nom|prénom|adresse|âge|état|genre|contamination          (patient)
     M|CIN|maladie                                                  (maladie chronique)
     C|CIN patient|nom|prénom|adresse|âge|état|genre|contamination|
       jour|mois|année|jour fin|mois fin|année fin|hôtel|CIN personne (personne contact)
   Le fichier est placé à côté de main.exe. La variable d'environnement CORONA_FICHIER
   permet d'utiliser un autre fichier (pratique pour les tests). */

#define NOM_FICHIER "donnees_corona.txt"

/* chemin complet du fichier de sauvegarde */
const char *fichier_donnees(void)
{
    static char chemin[1024] = "";
    const char *force;
    if(chemin[0] != '\0')
        return chemin;
    force = getenv("CORONA_FICHIER");
    if(force != NULL && force[0] != '\0')
    {
        snprintf(chemin, sizeof chemin, "%s", force);
        return chemin;
    }
#ifdef _WIN32
    {
        DWORD n = GetModuleFileNameA(NULL, chemin, sizeof chemin);
        char *fin = (n > 0 && n < sizeof chemin) ? strrchr(chemin, '\\') : NULL;
        if(fin != NULL && (size_t)(fin - chemin) + 1 + sizeof NOM_FICHIER <= sizeof chemin)
        {
            strcpy(fin + 1, NOM_FICHIER);
            return chemin;
        }
    }
#endif
    snprintf(chemin, sizeof chemin, "%s", NOM_FICHIER);
    return chemin;
}

/* coupe une ligne sur les '|' ; renvoie le nombre de champs */
static int decouper(char *ligne, char *champs[], int max)
{
    int n = 0;
    char *c = ligne;
    champs[n++] = c;
    for(; *c; c++)
    {
        if(*c == '|')
        {
            if(n == max)
                return -1;
            *c = '\0';
            champs[n++] = c + 1;
        }
    }
    return n;
}

static void copier(char *dest, size_t taille, const char *src)
{
    strncpy(dest, src, taille - 1);
    dest[taille - 1] = '\0';
}

/* Recharge les listes depuis le fichier.
   Renvoie le nombre d'éléments chargés, ou -1 s'il n'y a pas encore de fichier.
   *erreurs reçoit le nombre de lignes illisibles (ignorées). */
int charger_donnees(patient *l, maladie *m, personne *p, hotel *h, int *erreurs)
{
    FILE *f = fopen(fichier_donnees(), "r");
    char ligne[600];
    char *c[20];
    int n, total = 0;
    element *dl = NULL;      /* derniers éléments de chaque liste : ajout en fin rapide */
    element1 *dm = NULL;
    element2 *dp = NULL;
    element3 *dh = NULL;

    *erreurs = 0;
    if(f == NULL)
        return -1;
    while(fgets(ligne, sizeof ligne, f) != NULL)
    {
        ligne[strcspn(ligne, "\r\n")] = '\0';
        if(ligne[0] == '\0' || ligne[0] == '#')
            continue;
        n = decouper(ligne, c, 20);

        if(n == 5 && strcmp(c[0], "H") == 0)
        {
            element3 *e = (element3*)malloc(sizeof(element3));
            if(e == NULL) break;
            copier(e->nom, sizeof e->nom, c[1]);
            copier(e->adresse, sizeof e->adresse, c[2]);
            e->nbr = atoi(c[3]);
            e->nbmax = atoi(c[4]);
            e->suivant = NULL;
            if(dh) dh->suivant = e; else *h = e;
            dh = e;
        }
        else if(n == 9 && strcmp(c[0], "P") == 0)
        {
            element *e = (element*)malloc(sizeof(element));
            if(e == NULL) break;
            copier(e->CIN, sizeof e->CIN, c[1]);
            copier(e->nom, sizeof e->nom, c[2]);
            copier(e->prenom, sizeof e->prenom, c[3]);
            copier(e->adresse, sizeof e->adresse, c[4]);
            e->age = atoi(c[5]);
            e->etat = c[6][0];
            e->genre = c[7][0];
            e->conta = atoi(c[8]);
            e->suivant = NULL;
            if(dl) dl->suivant = e; else *l = e;
            dl = e;
        }
        else if(n == 3 && strcmp(c[0], "M") == 0)
        {
            element1 *e = (element1*)malloc(sizeof(element1));
            if(e == NULL) break;
            copier(e->CIN, sizeof e->CIN, c[1]);
            copier(e->mc, sizeof e->mc, c[2]);
            e->suivant = NULL;
            if(dm) dm->suivant = e; else *m = e;
            dm = e;
        }
        else if(n == 17 && strcmp(c[0], "C") == 0)
        {
            element2 *e = (element2*)malloc(sizeof(element2));
            if(e == NULL) break;
            copier(e->CIN, sizeof e->CIN, c[1]);
            copier(e->nom, sizeof e->nom, c[2]);
            copier(e->prenom, sizeof e->prenom, c[3]);
            copier(e->adresse, sizeof e->adresse, c[4]);
            e->age = atoi(c[5]);
            e->etat = c[6][0];
            e->genre = c[7][0];
            e->conta = atoi(c[8]);
            e->j = atoi(c[9]);   e->m = atoi(c[10]);  e->annee = atoi(c[11]);
            e->j1 = atoi(c[12]); e->m1 = atoi(c[13]); e->annee1 = atoi(c[14]);
            copier(e->nomh, sizeof e->nomh, c[15]);
            copier(e->CINP, sizeof e->CINP, c[16]);
            e->suivant = NULL;
            if(dp) dp->suivant = e; else *p = e;
            dp = e;
        }
        else
        {
            (*erreurs)++;
            continue;
        }
        total++;
    }
    fclose(f);
    nombre_dispo(*p, *h);    /* recalcule les places occupées */
    return total;
}

/* Écrit toutes les listes dans le fichier. Renvoie 1 si tout s'est bien passé.
   On écrit d'abord dans un fichier temporaire, puis on remplace l'ancien :
   si le programme est coupé pendant l'écriture, l'ancienne sauvegarde reste intacte. */
int sauvegarder_donnees(patient l, maladie m, personne p, hotel h)
{
    char tmp[1100];
    FILE *f;
    const char *chemin = fichier_donnees();
    snprintf(tmp, sizeof tmp, "%s.tmp", chemin);
    f = fopen(tmp, "w");
    if(f == NULL)
        return 0;
    fprintf(f, "# Gestion Corona - sauvegarde automatique\n");
    for(; h != NULL; h = h->suivant)
        fprintf(f, "H|%s|%s|%d|%d\n", h->nom, h->adresse, h->nbr, h->nbmax);
    for(; l != NULL; l = l->suivant)
        fprintf(f, "P|%s|%s|%s|%s|%d|%c|%c|%d\n", l->CIN, l->nom, l->prenom, l->adresse,
                l->age, l->etat, l->genre, l->conta);
    for(; m != NULL; m = m->suivant)
        fprintf(f, "M|%s|%s\n", m->CIN, m->mc);
    for(; p != NULL; p = p->suivant)
        fprintf(f, "C|%s|%s|%s|%s|%d|%c|%c|%d|%d|%d|%d|%d|%d|%d|%s|%s\n", p->CIN, p->nom, p->prenom,
                p->adresse, p->age, p->etat, p->genre, p->conta, p->j, p->m, p->annee,
                p->j1, p->m1, p->annee1, p->nomh, p->CINP);
    if(ferror(f) | fclose(f))
    {
        remove(tmp);
        return 0;
    }
#ifdef _WIN32
    if(!MoveFileExA(tmp, chemin, MOVEFILE_REPLACE_EXISTING))
#else
    if(rename(tmp, chemin) != 0)
#endif
    {
        remove(tmp);
        return 0;
    }
    return 1;
}
