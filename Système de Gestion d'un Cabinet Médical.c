/*
 * =====================================================================================
 * Application de Gestion d'un Cabinet Médical
 * =====================================================================================
 * Description : Système complet de gestion médicale en C (Patients, Médecins, Consultations).
 * Structures   : Patient, Médecin, Consultation.
 * Données      : File d'attente (Queue FIFO) pour la gestion de la salle d'attente.
 * Persistance  : Fichiers binaires (.dat) pour la sauvegarde automatique des données.
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *                    DÉFINITION DES STRUCTURES
 * ============================================================ */

/**
 * @brief Structure représentant un Patient
 */
typedef struct
{
    int id;
    char nom[50];
    char prenom[50];
    char cin[20];
    int age;
    char telephone[15];
    char groupe_sanguin[5];
    char antecedents[200];
} Patient;

/**
 * @brief Structure représentant un Médecin
 */
typedef struct
{
    int id;
    char nom[50];
    char prenom[50];
    char specialite[50];
    char telephone[15];
} Medecin;

/**
 * @brief Structure représentant une Consultation médicale
 */
typedef struct
{
    int id;
    int id_patient;
    int id_medecin;
    char date[20];
    char heure[10];
    char diagnostic[300];
    char traitement[300];
    float prix;
} Consultation;

/* ============================================================
 *           FILE D'ATTENTE (QUEUE) - SALLE D'ATTENTE
 * ============================================================ */

/**
 * @brief Nœud de la liste chaînée représentant un patient en attente
 */
typedef struct Noeud
{
    int id_patient;
    struct Noeud *suivant;
} Noeud;

/**
 * @brief Structure de la file d'attente (FIFO)
 */
typedef struct
{
    Noeud *tete;   /* Premier patient à être servi */
    Noeud *queue;  /* Dernier patient arrivé */
    int taille;    /* Nombre total de patients en attente */
} FileAttente;

/**
 * @brief Initialise la file d'attente à vide
 */
void init_file(FileAttente *f)
{
    f->tete = NULL;
    f->queue = NULL;
    f->taille = 0;
}

/**
 * @brief Vérifie si la file d'attente est vide
 * @return 1 si vide, 0 sinon
 */
int file_vide(FileAttente *f)
{
    return f->tete == NULL;
}

/**
 * @brief Ajoute un patient à la fin de la file d'attente (Enfiler / Enqueue)
 */
void enfiler(FileAttente *f, int id_patient)
{
    Noeud *nouveau = (Noeud *)malloc(sizeof(Noeud));
    if (!nouveau)
    {
        printf("Erreur d'allocation memoire.\n");
        return;
    }
    nouveau->id_patient = id_patient;
    nouveau->suivant = NULL;

    if (file_vide(f))
    {
        f->tete = nouveau;
        f->queue = nouveau;
    }
    else
    {
        f->queue->suivant = nouveau;
        f->queue = nouveau;
    }
    f->taille++;
    printf("Patient ID %d ajoute a la salle d'attente.\n", id_patient);
}

/**
 * @brief Retire et renvoie le premier patient de la file (Défiler / Dequeue)
 * @return ID du patient retiré, ou -1 si la file est vide
 */
int defiler(FileAttente *f)
{
    if (file_vide(f))
    {
        printf("La salle d'attente est vide.\n");
        return -1;
    }
    Noeud *tmp = f->tete;
    int id_patient = tmp->id_patient;
    f->tete = f->tete->suivant;
    if (f->tete == NULL)
        f->queue = NULL;
    free(tmp);
    f->taille--;
    return id_patient;
}

/**
 * @brief Affiche la liste des patients actuellement en salle d'attente
 */
void afficher_file(FileAttente *f)
{
    if (file_vide(f))
    {
        printf("  La salle d'attente est vide.\n");
        return;
    }
    Noeud *courant = f->tete;
    int pos = 1;
    printf("  Position | ID Patient\n");
    printf("  ---------+-----------\n");
    while (courant)
    {
        printf("     %2d    |    %d\n", pos++, courant->id_patient);
        courant = courant->suivant;
    }
    printf("  Total : %d patient(s) en attente.\n", f->taille);
}

/**
 * @brief Libère la mémoire de tous les éléments de la file d'attente
 */
void vider_file(FileAttente *f)
{
    while (!file_vide(f))
        defiler(f);
}

/* ============================================================
 *            GESTION DES FICHIERS BINAIRES (.DAT)
 * ============================================================ */

#define FICHIER_PATIENTS "patients.dat"
#define FICHIER_MEDECINS "medecins.dat"
#define FICHIER_CONSULTATIONS "consultations.dat"

/* --- Operations Fichiers : Patients --- */

int sauvegarder_patient(Patient *p)
{
    FILE *f = fopen(FICHIER_PATIENTS, "ab");
    if (!f)
        return 0;
    fwrite(p, sizeof(Patient), 1, f);
    fclose(f);
    return 1;
}

int charger_patients(Patient tableau[], int max)
{
    FILE *f = fopen(FICHIER_PATIENTS, "rb");
    if (!f)
        return 0;
    int n = 0;
    while (n < max && fread(&tableau[n], sizeof(Patient), 1, f) == 1)
        n++;
    fclose(f);
    return n;
}

int sauvegarder_tous_patients(Patient tableau[], int n)
{
    FILE *f = fopen(FICHIER_PATIENTS, "wb");
    if (!f)
        return 0;
    fwrite(tableau, sizeof(Patient), n, f);
    fclose(f);
    return 1;
}

/* --- Operations Fichiers : Medecins --- */

int sauvegarder_medecin(Medecin *m)
{
    FILE *f = fopen(FICHIER_MEDECINS, "ab");
    if (!f)
        return 0;
    fwrite(m, sizeof(Medecin), 1, f);
    fclose(f);
    return 1;
}

int charger_medecins(Medecin tableau[], int max)
{
    FILE *f = fopen(FICHIER_MEDECINS, "rb");
    if (!f)
        return 0;
    int n = 0;
    while (n < max && fread(&tableau[n], sizeof(Medecin), 1, f) == 1)
        n++;
    fclose(f);
    return n;
}

int sauvegarder_tous_medecins(Medecin tableau[], int n)
{
    FILE *f = fopen(FICHIER_MEDECINS, "wb");
    if (!f)
        return 0;
    fwrite(tableau, sizeof(Medecin), n, f);
    fclose(f);
    return 1;
}

/* --- Operations Fichiers : Consultations --- */

int sauvegarder_consultation(Consultation *c)
{
    FILE *f = fopen(FICHIER_CONSULTATIONS, "ab");
    if (!f)
        return 0;
    fwrite(c, sizeof(Consultation), 1, f);
    fclose(f);
    return 1;
}

int charger_consultations(Consultation tableau[], int max)
{
    FILE *f = fopen(FICHIER_CONSULTATIONS, "rb");
    if (!f)
        return 0;
    int n = 0;
    while (n < max && fread(&tableau[n], sizeof(Consultation), 1, f) == 1)
        n++;
    fclose(f);
    return n;
}

int sauvegarder_toutes_consultations(Consultation tableau[], int n)
{
    FILE *f = fopen(FICHIER_CONSULTATIONS, "wb");
    if (!f)
        return 0;
    fwrite(tableau, sizeof(Consultation), n, f);
    fclose(f);
    return 1;
}

/* ============================================================
 *                     FONCTIONS UTILITAIRES
 * ============================================================ */

/**
 * @brief Vide le flux d'entrée d'instructions (évite les bugs de saisie)
 */
void vider_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * @brief Affiche une ligne séparatrice dans la console
 */
void afficher_separateur()
{
    printf("============================================================\n");
}

/**
 * @brief Génère le prochain identifiant unique basé sur l'ID maximal existant
 */
int prochain_id(int ids[], int n)
{
    int max = 0;
    for (int i = 0; i < n; i++)
        if (ids[i] > max)
            max = ids[i];
    return max + 1;
}

/* ============================================================
 *                 MODULE : GESTION DES PATIENTS
 * ============================================================ */

#define MAX_PATIENTS 200

Patient patients[MAX_PATIENTS];
int nb_patients = 0;

void afficher_patient(Patient *p)
{
    printf("  ID          : %d\n", p->id);
    printf("  Nom         : %s\n", p->nom);
    printf("  Prenom      : %s\n", p->prenom);
    printf("  CIN         : %s\n", p->cin);
    printf("  Age         : %d ans\n", p->age);
    printf("  Telephone   : %s\n", p->telephone);
    printf("  Groupe sang.: %s\n", p->groupe_sanguin);
    printf("  Antecedents : %s\n", p->antecedents);
}

void ajouter_patient()
{
    if (nb_patients >= MAX_PATIENTS)
    {
        printf("Capacite maximale atteinte.\n");
        return;
    }
    Patient p;
    int ids[MAX_PATIENTS];
    for (int i = 0; i < nb_patients; i++)
        ids[i] = patients[i].id;
    p.id = prochain_id(ids, nb_patients);

    printf("  Nom         : ");
    scanf("%49s", p.nom);
    printf("  Prenom      : ");
    scanf("%49s", p.prenom);
    printf("  CIN         : ");
    scanf("%19s", p.cin);
    printf("  Age         : ");
    scanf("%d", &p.age);
    vider_buffer();
    printf("  Telephone   : ");
    scanf("%14s", p.telephone);
    printf("  Groupe sang.: ");
    scanf("%4s", p.groupe_sanguin);
    vider_buffer();
    printf("  Antecedents : ");
    fgets(p.antecedents, sizeof(p.antecedents), stdin);
    p.antecedents[strcspn(p.antecedents, "\n")] = '\0';

    patients[nb_patients++] = p;
    sauvegarder_tous_patients(patients, nb_patients);
    printf("  Patient ajoute avec succes ! (ID = %d)\n", p.id);
}

void afficher_tous_patients()
{
    if (nb_patients == 0)
    {
        printf("  Aucun patient enregistre.\n");
        return;
    }
    for (int i = 0; i < nb_patients; i++)
    {
        printf("\n--- Patient %d ---\n", i + 1);
        afficher_patient(&patients[i]);
    }
}

int rechercher_patient_par_id(int id)
{
    for (int i = 0; i < nb_patients; i++)
        if (patients[i].id == id)
            return i;
    return -1;
}

int rechercher_patient_par_cin(char *cin)
{
    for (int i = 0; i < nb_patients; i++)
        if (strcmp(patients[i].cin, cin) == 0)
            return i;
    return -1;
}

void rechercher_patient()
{
    printf("  Recherche par : 1) ID   2) CIN\n  Choix : ");
    int choix;
    scanf("%d", &choix);
    vider_buffer();

    if (choix == 1)
    {
        int id;
        printf("  ID : ");
        scanf("%d", &id);
        vider_buffer();
        int idx = rechercher_patient_par_id(id);
        if (idx == -1)
            printf("  Patient introuvable.\n");
        else
        {
            printf("\n");
            afficher_patient(&patients[idx]);
        }
    }
    else
    {
        char cin[20];
        printf("  CIN : ");
        scanf("%19s", cin);
        vider_buffer();
        int idx = rechercher_patient_par_cin(cin);
        if (idx == -1)
            printf("  Patient introuvable.\n");
        else
        {
            printf("\n");
            afficher_patient(&patients[idx]);
        }
    }
}

void modifier_patient()
{
    int id;
    printf("  ID du patient a modifier : ");
    scanf("%d", &id);
    vider_buffer();
    int idx = rechercher_patient_par_id(id);
    if (idx == -1)
    {
        printf("  Patient introuvable.\n");
        return;
    }

    Patient *p = &patients[idx];
    printf("  Nouveau nom [%s] : ", p->nom);
    char tmp[50];
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(p->nom, tmp);

    printf("  Nouveau prenom [%s] : ", p->prenom);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(p->prenom, tmp);

    printf("  Nouveau telephone [%s] : ", p->telephone);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(p->telephone, tmp);

    printf("  Nouveaux antecedents [%s] : ", p->antecedents);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(p->antecedents, tmp);

    sauvegarder_tous_patients(patients, nb_patients);
    printf("  Patient modifie avec succes.\n");
}

void supprimer_patient()
{
    int id;
    printf("  ID du patient a supprimer : ");
    scanf("%d", &id);
    vider_buffer();
    int idx = rechercher_patient_par_id(id);
    if (idx == -1)
    {
        printf("  Patient introuvable.\n");
        return;
    }

    for (int i = idx; i < nb_patients - 1; i++)
        patients[i] = patients[i + 1];
    nb_patients--;
    sauvegarder_tous_patients(patients, nb_patients);
    printf("  Patient supprime avec succes.\n");
}

/* ============================================================
 *                 MODULE : GESTION DES MÉDECINS
 * ============================================================ */

#define MAX_MEDECINS 50

Medecin medecins[MAX_MEDECINS];
int nb_medecins = 0;

void afficher_medecin(Medecin *m)
{
    printf("  ID          : %d\n", m->id);
    printf("  Nom         : %s\n", m->nom);
    printf("  Prenom      : %s\n", m->prenom);
    printf("  Specialite  : %s\n", m->specialite);
    printf("  Telephone   : %s\n", m->telephone);
}

void ajouter_medecin()
{
    if (nb_medecins >= MAX_MEDECINS)
    {
        printf("Capacite maximale atteinte.\n");
        return;
    }
    Medecin m;
    int ids[MAX_MEDECINS];
    for (int i = 0; i < nb_medecins; i++)
        ids[i] = medecins[i].id;
    m.id = prochain_id(ids, nb_medecins);

    printf("  Nom         : ");
    scanf("%49s", m.nom);
    printf("  Prenom      : ");
    scanf("%49s", m.prenom);
    vider_buffer();
    printf("  Specialite  : ");
    fgets(m.specialite, sizeof(m.specialite), stdin);
    m.specialite[strcspn(m.specialite, "\n")] = '\0';
    printf("  Telephone   : ");
    scanf("%14s", m.telephone);
    vider_buffer();

    medecins[nb_medecins++] = m;
    sauvegarder_tous_medecins(medecins, nb_medecins);
    printf("  Medecin ajoute avec succes ! (ID = %d)\n", m.id);
}

void afficher_tous_medecins()
{
    if (nb_medecins == 0)
    {
        printf("  Aucun medecin enregistre.\n");
        return;
    }
    for (int i = 0; i < nb_medecins; i++)
    {
        printf("\n--- Medecin %d ---\n", i + 1);
        afficher_medecin(&medecins[i]);
    }
}

int rechercher_medecin_par_id(int id)
{
    for (int i = 0; i < nb_medecins; i++)
        if (medecins[i].id == id)
            return i;
    return -1;
}

void modifier_medecin()
{
    int id;
    printf("  ID du medecin a modifier : ");
    scanf("%d", &id);
    vider_buffer();
    int idx = rechercher_medecin_par_id(id);
    if (idx == -1)
    {
        printf("  Medecin introuvable.\n");
        return;
    }

    Medecin *m = &medecins[idx];
    char tmp[50];

    printf("  Nouvelle specialite [%s] : ", m->specialite);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(m->specialite, tmp);

    printf("  Nouveau telephone [%s] : ", m->telephone);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(m->telephone, tmp);

    sauvegarder_tous_medecins(medecins, nb_medecins);
    printf("  Medecin modifie avec succes.\n");
}

void supprimer_medecin()
{
    int id;
    printf("  ID du medecin a supprimer : ");
    scanf("%d", &id);
    vider_buffer();
    int idx = rechercher_medecin_par_id(id);
    if (idx == -1)
    {
        printf("  Medecin introuvable.\n");
        return;
    }

    for (int i = idx; i < nb_medecins - 1; i++)
        medecins[i] = medecins[i + 1];
    nb_medecins--;
    sauvegarder_tous_medecins(medecins, nb_medecins);
    printf("  Medecin supprime avec succes.\n");
}

/* ============================================================
 *              MODULE : GESTION DES CONSULTATIONS
 * ============================================================ */

#define MAX_CONSULTATIONS 500

Consultation consultations[MAX_CONSULTATIONS];
int nb_consultations = 0;

void afficher_consultation(Consultation *c)
{
    printf("  ID             : %d\n", c->id);
    printf("  ID Patient     : %d\n", c->id_patient);
    printf("  ID Medecin     : %d\n", c->id_medecin);
    printf("  Date           : %s\n", c->date);
    printf("  Heure          : %s\n", c->heure);
    printf("  Diagnostic     : %s\n", c->diagnostic);
    printf("  Traitement     : %s\n", c->traitement);
    printf("  Prix           : %.2f MAD\n", c->prix);
}

void ajouter_consultation()
{
    if (nb_consultations >= MAX_CONSULTATIONS)
    {
        printf("Capacite maximale atteinte.\n");
        return;
    }
    if (nb_patients == 0)
    {
        printf("  Aucun patient enregistre.\n");
        return;
    }
    if (nb_medecins == 0)
    {
        printf("  Aucun medecin enregistre.\n");
        return;
    }

    Consultation c;
    int ids[MAX_CONSULTATIONS];
    for (int i = 0; i < nb_consultations; i++)
        ids[i] = consultations[i].id;
    c.id = prochain_id(ids, nb_consultations);

    printf("  ID Patient  : ");
    scanf("%d", &c.id_patient);
    vider_buffer();
    if (rechercher_patient_par_id(c.id_patient) == -1)
    {
        printf("  Patient introuvable.\n");
        return;
    }
    printf("  ID Medecin  : ");
    scanf("%d", &c.id_medecin);
    vider_buffer();
    if (rechercher_medecin_par_id(c.id_medecin) == -1)
    {
        printf("  Medecin introuvable.\n");
        return;
    }
    printf("  Date (jj/mm/aaaa) : ");
    scanf("%19s", c.date);
    vider_buffer();
    printf("  Heure (hh:mm)     : ");
    scanf("%9s", c.heure);
    vider_buffer();
    printf("  Diagnostic        : ");
    fgets(c.diagnostic, sizeof(c.diagnostic), stdin);
    c.diagnostic[strcspn(c.diagnostic, "\n")] = '\0';
    printf("  Traitement        : ");
    fgets(c.traitement, sizeof(c.traitement), stdin);
    c.traitement[strcspn(c.traitement, "\n")] = '\0';
    printf("  Prix (MAD)        : ");
    scanf("%f", &c.prix);
    vider_buffer();

    consultations[nb_consultations++] = c;
    sauvegarder_toutes_consultations(consultations, nb_consultations);
    printf("  Consultation ajoutee avec succes ! (ID = %d)\n", c.id);
}

void afficher_toutes_consultations()
{
    if (nb_consultations == 0)
    {
        printf("  Aucune consultation enregistree.\n");
        return;
    }
    for (int i = 0; i < nb_consultations; i++)
    {
        printf("\n--- Consultation %d ---\n", i + 1);
        afficher_consultation(&consultations[i]);
    }
}

void consultations_par_patient()
{
    int id;
    printf("  ID patient : ");
    scanf("%d", &id);
    vider_buffer();
    int trouve = 0;
    for (int i = 0; i < nb_consultations; i++)
    {
        if (consultations[i].id_patient == id)
        {
            printf("\n");
            afficher_consultation(&consultations[i]);
            trouve = 1;
        }
    }
    if (!trouve)
        printf("  Aucune consultation pour ce patient.\n");
}

void modifier_consultation()
{
    int id;
    printf("  ID de la consultation a modifier : ");
    scanf("%d", &id);
    vider_buffer();
    int idx = -1;
    for (int i = 0; i < nb_consultations; i++)
        if (consultations[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        printf("  Consultation introuvable.\n");
        return;
    }

    Consultation *c = &consultations[idx];
    char tmp[300];

    printf("  Nouveau diagnostic [%s] : ", c->diagnostic);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(c->diagnostic, tmp);

    printf("  Nouveau traitement [%s] : ", c->traitement);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        strcpy(c->traitement, tmp);

    printf("  Nouveau prix [%.2f] : ", c->prix);
    fgets(tmp, sizeof(tmp), stdin);
    tmp[strcspn(tmp, "\n")] = '\0';
    if (strlen(tmp) > 0)
        c->prix = atof(tmp);

    sauvegarder_toutes_consultations(consultations, nb_consultations);
    printf("  Consultation modifiee avec succes.\n");
}

void supprimer_consultation()
{
    int id;
    printf("  ID de la consultation a supprimer : ");
    scanf("%d", &id);
    vider_buffer();
    int idx = -1;
    for (int i = 0; i < nb_consultations; i++)
        if (consultations[i].id == id)
        {
            idx = i;
            break;
        }
    if (idx == -1)
    {
        printf("  Consultation introuvable.\n");
        return;
    }

    for (int i = idx; i < nb_consultations - 1; i++)
        consultations[i] = consultations[i + 1];
    nb_consultations--;
    sauvegarder_toutes_consultations(consultations, nb_consultations);
    printf("  Consultation supprimee avec succes.\n");
}

/* ============================================================
 *                 MODULE : SALLE D'ATTENTE (FILE)
 * ============================================================ */

FileAttente salle_attente;

void menu_salle_attente()
{
    int choix;
    do
    {
        afficher_separateur();
        printf("     SALLE D'ATTENTE (FILE)\n");
        afficher_separateur();
        printf("  1. Ajouter un patient a la file\n");
        printf("  2. Appeler le prochain patient\n");
        printf("  3. Afficher la file d'attente\n");
        printf("  0. Retour\n");
        afficher_separateur();
        printf("  Choix : ");
        scanf("%d", &choix);
        vider_buffer();

        switch (choix)
        {
        case 1:
        {
            int id;
            printf("  ID du patient : ");
            scanf("%d", &id);
            vider_buffer();
            if (rechercher_patient_par_id(id) == -1)
                printf("  Patient introuvable.\n");
            else
                enfiler(&salle_attente, id);
            break;
        }
        case 2:
        {
            int id = defiler(&salle_attente);
            if (id != -1)
            {
                int idx = rechercher_patient_par_id(id);
                printf("  Prochain patient appele :\n");
                if (idx != -1)
                    afficher_patient(&patients[idx]);
                else
                    printf("  ID : %d\n", id);
            }
            break;
        }
        case 3:
            afficher_file(&salle_attente);
            break;
        case 0:
            break;
        default:
            printf("  Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ============================================================
 *                    MENUS ET NAVIGATION
 * ============================================================ */

void menu_patients()
{
    int choix;
    do
    {
        afficher_separateur();
        printf("     GESTION DES PATIENTS\n");
        afficher_separateur();
        printf("  1. Ajouter un patient\n");
        printf("  2. Afficher tous les patients\n");
        printf("  3. Rechercher un patient\n");
        printf("  4. Modifier un patient\n");
        printf("  5. Supprimer un patient\n");
        printf("  0. Retour\n");
        afficher_separateur();
        printf("  Choix : ");
        scanf("%d", &choix);
        vider_buffer();
        switch (choix)
        {
        case 1:
            ajouter_patient();
            break;
        case 2:
            afficher_tous_patients();
            break;
        case 3:
            rechercher_patient();
            break;
        case 4:
            modifier_patient();
            break;
        case 5:
            supprimer_patient();
            break;
        case 0:
            break;
        default:
            printf("  Choix invalide.\n");
        }
    } while (choix != 0);
}

void menu_medecins()
{
    int choix;
    do
    {
        afficher_separateur();
        printf("     GESTION DES MEDECINS\n");
        afficher_separateur();
        printf("  1. Ajouter un medecin\n");
        printf("  2. Afficher tous les medecins\n");
        printf("  3. Modifier un medecin\n");
        printf("  4. Supprimer un medecin\n");
        printf("  0. Retour\n");
        afficher_separateur();
        printf("  Choix : ");
        scanf("%d", &choix);
        vider_buffer();
        switch (choix)
        {
        case 1:
            ajouter_medecin();
            break;
        case 2:
            afficher_tous_medecins();
            break;
        case 3:
            modifier_medecin();
            break;
        case 4:
            supprimer_medecin();
            break;
        case 0:
            break;
        default:
            printf("  Choix invalide.\n");
        }
    } while (choix != 0);
}

void menu_consultations()
{
    int choix;
    do
    {
        afficher_separateur();
        printf("     GESTION DES CONSULTATIONS\n");
        afficher_separateur();
        printf("  1. Ajouter une consultation\n");
        printf("  2. Afficher toutes les consultations\n");
        printf("  3. Consultations par patient\n");
        printf("  4. Modifier une consultation\n");
        printf("  5. Supprimer une consultation\n");
        printf("  0. Retour\n");
        afficher_separateur();
        printf("  Choix : ");
        scanf("%d", &choix);
        vider_buffer();
        switch (choix)
        {
        case 1:
            ajouter_consultation();
            break;
        case 2:
            afficher_toutes_consultations();
            break;
        case 3:
            consultations_par_patient();
            break;
        case 4:
            modifier_consultation();
            break;
        case 5:
            supprimer_consultation();
            break;
        case 0:
            break;
        default:
            printf("  Choix invalide.\n");
        }
    } while (choix != 0);
}

/* ============================================================
 *                    FONCTION PRINCIPALE (MAIN)
 * ============================================================ */

int main()
{
    /* Chargement initial des donnees depuis les fichiers binaires */
    nb_patients = charger_patients(patients, MAX_PATIENTS);
    nb_medecins = charger_medecins(medecins, MAX_MEDECINS);
    nb_consultations = charger_consultations(consultations, MAX_CONSULTATIONS);
    init_file(&salle_attente);

    int choix;
    do
    {
        afficher_separateur();
        printf("   CABINET MEDICAL - MENU PRINCIPAL\n");
        afficher_separateur();
        printf("  1. Gestion des Patients\n");
        printf("  2. Gestion des Medecins\n");
        printf("  3. Gestion des Consultations\n");
        printf("  4. Salle d'attente (File)\n");
        printf("  0. Quitter\n");
        afficher_separateur();
        printf("  Choix : ");
        scanf("%d", &choix);
        vider_buffer();

        switch (choix)
        {
        case 1:
            menu_patients();
            break;
        case 2:
            menu_medecins();
            break;
        case 3:
            menu_consultations();
            break;
        case 4:
            menu_salle_attente();
            break;
        case 0:
            /* Nettoyage de la mémoire dynamique avant de quitter */
            vider_file(&salle_attente);
            printf("  Au revoir !\n");
            break;
        default:
            printf("  Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
