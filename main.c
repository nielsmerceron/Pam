#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//pour exécuter taper :
//1. gcc -Wall main.c -o main -lm
//2. ./main 1 4 50 choixpeauMagique.csv

//---------------------[fonction pour la vérification des arguments + extraction du fichier.csv]---------------------

//fonction de vérification des arguments passés au programme
int verifarg(int nbarg, char *arg[])
{
    if (nbarg > 5 || nbarg < 5)
    {
        fprintf(stderr, " \n -argument non valide \n -exemple : ./main header(int) nbcluster(int) nbobjets(int) dossierdata(String) \n\n");
        return (0);
    }

    if (atoi(arg[1]) > 1)
    {
        fprintf(stderr, "le premier argument ne peux prendre que 0 (n'a pas d'header) ou 1(a un header) \n");
        return (0);
    }

    if (strstr(arg[4], ".csv") == NULL)
    {
        fprintf(stderr, "\n -le dossier passer en argument n'est pas au bon format, il doit être au format .csv \n\n");
        return (0);
    }

    int i;
    for (i = 0; i < strlen(arg[2]) && i < strlen(arg[3]); i++)
    {
        if (isdigit(arg[2][i]) == 0 || isdigit(arg[3][i]) == 0)
        {
            fprintf(stderr, " - \n nbcluster ou nbobjets n'est pas un chiffre/nombre \n\n ");
            return (0);
        }
    }

    if (atoi(arg[2]) > atoi(arg[3]))
    {
        fprintf(stderr, "\n -le nombre de cluster est trop grand \n -veuillez mettre un nombre de cluster plus petit que le nombre d'objet a clusteriser\n\n");
        return (0);
    }

    return (1);
}

//fonction servant a savoir le nb de donné quantitative et qualitative dans le fichier
int *nbdonnequaliquanti(char *dossier, int header)
{
    int vf;
    char buf[100];
    int i;
    int quanti = 0;
    int quali = 0;
    int *tempo = malloc(sizeof(int) * 2);

    if (tempo == NULL)
    {
        fprintf(stderr, "mauvaise allocation de compte pour quanti quali \n");
        return (NULL);
    }

    FILE *fichi = fopen(dossier, "r");
    if (fichi == NULL)
    {
        fprintf(stderr, "ne peut pas ouvir le fichier\n");
        return (NULL);
    }
    if (header == 1)
    {
        fgets(buf, 100, fichi);
        fgets(buf, 100, fichi);
        char *strtoken = strtok(buf, ";");
        while (strtoken != NULL)
        {
            for (i = 0; i < strlen(strtoken); i++)
            {
                if (isdigit(strtoken[i]) == 0)
                {
                    vf = 1;
                    break;
                }
                else
                {
                    vf = 0;
                }
            }
            if (vf == 0)
            {
                quanti += 1;
            }
            else
            {
                quali += 1;
            }

            strtoken = strtok(NULL, ";");
        }
    }
    else if (header == 0)
    {
        fgets(buf, 100, fichi);
        char *strtoken = strtok(buf, ";");
        while (strtoken != NULL)
        {
            for (i = 0; i < strlen(strtoken); i++)
            {
                if (isdigit(strtoken[i]) == 0)
                {
                    vf = 1;
                    break;
                }
                else
                {
                    vf = 0;
                }
            }
            if (vf == 0)
            {
                quanti += 1;
            }
            else
            {
                quali += 1;
            }

            strtoken = strtok(NULL, ";");
        }
    }

    if (fclose(fichi) != 0)
    {
        fprintf(stderr, "mauvaise fermeture du fichier\n");
        return (NULL);
    }
    tempo[0] = quali;
    tempo[1] = quanti;
    return (tempo);
}

//structure pour accueillir les données
struct donne
{
    char **qualitatif;
    int *quantitatif;
};
typedef struct donne Donne;

//fonction d'initialisation de la structure accueillant les données
Donne *newDonne(int taillequali, int taillequanti)
{
    Donne *new = malloc(sizeof(Donne));
    if (new == NULL)
    {
        fprintf(stderr, "mauvaise allocation de la structure donne\n");
        return (NULL);
    }
    new->qualitatif = malloc(sizeof(char *) * taillequali);
    if (new->qualitatif == NULL)
    {
        fprintf(stderr, "mauvaise allocation des données qualitatif\n");
        return (NULL);
    }
    new->quantitatif = malloc(sizeof(int) * taillequanti);
    if (new->quantitatif == NULL)
    {
        fprintf(stderr, "mauvaise allocation des données qualitatif\n");
        return (NULL);
    }
    for (int i = 0; i < taillequanti; i++)
    {
        new->quantitatif[i] = 0;
    }

    return (new);
}

//fonction d'initialisation d'un tableau de référence de structure de donné
Donne **extractdonne(char *dossier, int nbobjet, int header, int quali, int quanti)
{

    char buf[100];
    int i = 0;
    int compquali = 0;
    int compquanti = 0;
    int vf = 0;

    FILE *fichi = fopen(dossier, "r");
    if (fichi == NULL)
    {
        fprintf(stderr, "ne peut pas ouvir le fichier\n");
        return (NULL);
    }

    Donne **tableaudonnne = malloc(sizeof(Donne *) * nbobjet);
    if (tableaudonnne == NULL)
    {
        fprintf(stderr, "mauvaise allocation du tableau de donné\n");
        return (NULL);
    }

    i = 0;
    while (fgets(buf, 100, fichi) != NULL)
    {
        compquanti = 0;
        compquali = 0;

        if (i != header - 1)
        {
            Donne *dontempo = newDonne(quali, quanti);
            char *strtoken = strtok(buf, ";");
            while (strtoken != NULL)
            {
                for (int j = 0; j < strlen(strtoken); j++)
                {
                    if (isdigit(strtoken[j]) == 0)
                    {
                        vf = 1;
                        break;
                    }
                    else
                    {
                        vf = 0;
                    }
                }
                if (vf == 1 && compquali != quali)
                {
                    char *temp = malloc(sizeof(char) * 40);
                    if (temp == NULL)
                    {
                        fprintf(stderr, "mauvaise allocation d'une donnée quantitative \n");
                        return (NULL);
                    }
                    strcpy(temp, strtoken);
                    dontempo->qualitatif[compquali] = temp;
                    compquali += 1;
                }
                else if (vf == 0 && compquanti != quanti)
                {
                    dontempo->quantitatif[compquanti] = atoi(strtoken);
                    compquanti += 1;
                }
                strtoken = strtok(NULL, ";");
            }
            tableaudonnne[i - 1] = dontempo;
        }
        i += 1;
    }
    if (fclose(fichi) != 0)
    {
        fprintf(stderr, "mauvaise fermeture du fichier\n");
        return (NULL);
    }

    return (tableaudonnne);
}

// fonction de libération de la mémoire pour les données
void clear_DONNE(Donne **tableaudonne, int nbdonne, int nbquali)
{

    for (int i = 0; i < nbdonne; i++)
    {
        for (int j = 0; j < nbquali; j++)
        {
            free(tableaudonne[i]->qualitatif[j]);
        }
        free(tableaudonne[i]->qualitatif);
        free(tableaudonne[i]->quantitatif);
        free(tableaudonne[i]);
    }
    free(tableaudonne);
}

//fonction d'affichage de donnée pour vérification
void affdonne(Donne **tableau, int nb)
{
    int i;
    for (i = 0; i < nb; i++)
    {
        fprintf(stderr, "%s %d %d %d %d %s \n", tableau[i]->qualitatif[0], tableau[i]->quantitatif[0], tableau[i]->quantitatif[1], tableau[i]->quantitatif[2], tableau[i]->quantitatif[3], tableau[i]->qualitatif[1]);
    }
}

//---------------------[fonction pour Pam]---------------------

struct cluster
{
    int representatif;
    int coupartition;
    int nbmembre;
    int *membre;
};
typedef struct cluster Cluster;

//fonction d'aléatoire + donne une valeur dans l'interval voulu (et fait le café)
int alea2(int inf, int sup, int rando)
{
    srand(rando * time(NULL));

    int irandom;
    irandom = inf + (rand() % (sup - inf + 1));

    return (irandom);
}

//fonction selectionnant n objet représentatif pour les clusters
int *selectionaleaclust(int nbobjet, int nbcluster)
{
    int *alea = malloc(sizeof(int) * nbcluster);
    if (alea == NULL)
    {
        fprintf(stderr, "mauvaise allocation pour la récupération des valeur aléatoire");
        return (NULL);
    }
    int boule = 0;
    int tempo = 0;
    int i = 1;
    alea[0] = alea2(0, nbobjet, rand());
    while (i < nbcluster)
    {
        tempo = alea2(0, nbobjet, i);
        for (int j = 0; j < i; j++)
        {
            if (alea[j] == tempo)
            {
                boule = 0;
                break;
            }
            else
            {
                boule = 1;
            }
        }
        if (boule == 1)
        {
            alea[i] = tempo;
            i += 1;
        }
    }
    return (alea);
}

//fonction calculant la distance de manathan entre deux données
int distance_manathan(Donne *a, Donne *b, int nbquanti)
{
    int distmana = 0;

    for (int i = 0; i < nbquanti; i++)
    {
        distmana = distmana + abs(a->quantitatif[i] - b->quantitatif[i]);
    }
    return (distmana);
}

//fonctions effectuant la matrice de manathan
int **mat_manathan(Donne **tableau, int *indicecluster, int nbcluster, int nbobjet, int nbquanti)
{
    int **matmana = malloc(sizeof(int *) * (nbcluster + 2));
    if (matmana == NULL)
    {
        fprintf(stderr, "erreur d'allocation de la matrice de manathan\n");
        return (NULL);
    }
    int *distmanamin = malloc(sizeof(int) * nbobjet);
    int *sommetmini = malloc(sizeof(int) * nbobjet);
    if (distmanamin == NULL || sommetmini == NULL)
    {
        fprintf(stderr, "mauvaise allocation lors de la création de la matrice de manathan");
        return (NULL);
    }

    for (int i = 0; i < nbcluster; i++)
    {
        int *distmana = malloc(sizeof(int) * nbobjet);
        if (distmana == NULL)
        {
            fprintf(stderr, "problème d'allocation d'une colonne de la matrice de manathan");
            return (NULL);
        }
        for (int j = 0; j < nbobjet; j++)
        {
            distmana[j] = distance_manathan(tableau[indicecluster[i]], tableau[j], nbquanti);
            if(i==0){
                distmanamin[j]=distmana[j];
                sommetmini[j]=i;
            }
            else if(distmanamin[j]>distmana[j]){
                distmanamin[j]=distmana[j];
                sommetmini[j]=i;
            }
        }
        matmana[i] = distmana;
    }
    matmana[nbcluster] = sommetmini;
    matmana[nbcluster + 1] = distmanamin;
    return (matmana);
}

//fonction qui clean la matrice de Manathan
void cleanMatman(int **matmana, int nbcluster)
{
    for (int i = 0; i < (nbcluster + 2); i++)
    {
        free(matmana[i]);
    }
    free(matmana);
}

//fonction d'affichage dee la matrices de manathan
void affmana(int **matmana, int nbcluster, int nbdonne)
{
    for (int j = 0; j < nbdonne; j++)
    {
        fprintf(stderr, " %d %d %d %d   %d %d\n", matmana[0][j], matmana[1][j], matmana[2][j], matmana[3][j], matmana[4][j], matmana[5][j]);
    }
}

//fonction d'allocation d'un cluster
Cluster *newCluster()
{
    Cluster *clust = malloc(sizeof(Cluster));
    if (clust == NULL)
    {
        fprintf(stderr, "mauvaise allocation d'un cluster");
        return (NULL);
    }

    return (clust);
}

//fonction d'initialisation/regroupement des clusters
Cluster **initClustertab(int **matmanathan, int *indicecluster, int nbcluster, int nbdonne)
{
    Cluster **cluste = malloc(sizeof(Cluster *) * nbcluster);
    if (cluste == NULL)
    {
        fprintf(stderr, "l'allocation du tableau de cluster a échoué\n");
        return (NULL);
    }
    int buf[51];
    for (int i = 0; i < nbcluster; i++)
    {
        Cluster *clust = newCluster();
        clust->representatif = indicecluster[i];
        clust->coupartition = 0;
        clust->nbmembre = 0;
        for (int j = 0; j < nbdonne; j++)
        {
            if (matmanathan[nbcluster][j] == i)
            {
                buf[clust->nbmembre] = j;
                clust->nbmembre += 1;
                clust->coupartition += matmanathan[(nbcluster + 1)][j];
            }
        }
        int *membre = malloc(sizeof(int) * clust->nbmembre);
        if (membre == NULL)
        {
            fprintf(stderr, "mauvaise allocation lors des associations de cluster");
            return (NULL);
        }
        for (int i = 0; i < clust->nbmembre; i++)
        {
            membre[i] = buf[i];
        }
        clust->membre = membre;
        cluste[i] = clust;
    }

    return (cluste);
}

//fonction d'affichage de l'ensemble des cluster
void affcluster(Cluster **tableau, int nbclust,Donne ** table)
{
    int partitot = 0;
    for (int i = 0; i < nbclust; i++)
    {
        fprintf(stderr, "rep: %s \n membre: ", table[tableau[i]->representatif]->qualitatif[0]);
        for (int j = 0; j < tableau[i]->nbmembre; j++)
        {
            fprintf(stderr, " %s,", table[tableau[i]->membre[j]]->qualitatif[0]);
        }
        fprintf(stderr, "   parti:%d \n\n", tableau[i]->coupartition);
        partitot += tableau[i]->coupartition;

    }
    fprintf(stderr, "--- coût de partition global: %d --- \n", partitot);
}

//fonction donnant le coup total de partitionnement
int coupatglobal(Cluster **clust, int nbcluster)
{
    int somme = 0;
    for (int i = 0; i < nbcluster; i++)
    {
        somme += clust[i]->coupartition;
    }
    return (somme);
}

//fonction de nettoyage des clusters
void cleancluster(Cluster **tableauclust, int nbcluster)
{
    for (int i = 0; i < nbcluster; i++)
    {
        free(tableauclust[i]->membre);
        free(tableauclust[i]);
    }
    free(tableauclust);
}

//fonction effectuant Pam
Cluster **Pam(Donne **tableau, int nbdonne, int nbcluster, int *quantiquali)
{
    int vrai = 1;
    int *alea = selectionaleaclust((nbdonne - 1), nbcluster);
    fprintf(stderr," indice sélectionné: ");
    for(int w=0;w<nbcluster;w++){
        fprintf(stderr," %d,",alea[w]);
    }
    fprintf(stderr,"\n");

    int **matricemana = mat_manathan(tableau, alea, nbcluster, nbdonne, quantiquali[1]);

    Cluster **clust = initClustertab(matricemana, alea, nbcluster, nbdonne);
    int E = coupatglobal(clust, nbcluster);
    fprintf(stderr, "--- coût de partition global: %d --- \n", E);
    while (vrai)
    {
        vrai = 0;
        for (int i = 0; i < nbcluster; i++)
        {
            for (int j = 0; j < clust[i]->nbmembre; j++)
            {
                int buf = 0;
                for (int k = 0; k < clust[i]->nbmembre; k++)
                {
                    buf += distance_manathan(tableau[clust[i]->membre[j]], tableau[clust[i]->membre[k]], quantiquali[1]);
                }
                if (buf<clust[i]->coupartition)
                {
                    alea[i] = clust[i]->membre[j];
                    cleanMatman(matricemana, nbcluster);
                    matricemana = mat_manathan(tableau, alea, nbcluster, nbdonne, quantiquali[1]);
                    cleancluster(clust, nbcluster);
                    clust = initClustertab(matricemana, alea, nbcluster, nbdonne);
                    E = coupatglobal(clust, nbcluster);
                    vrai = 1;
                }
            }
        }
        fprintf(stderr, "--- coût de partition global: %d --- \n", E);
    }

    cleanMatman(matricemana, nbcluster);
    free(alea);
    return (clust);
}

//fonction de libération global
void DOOOOOOOOOOOM(Cluster **cluster, Donne **tableau, int *quantiquali, int nbobjet, int nbcluster)
{
    clear_DONNE(tableau, nbobjet, quantiquali[0]);
    cleancluster(cluster, nbcluster);
    free(quantiquali);
}

int main(int argc, char *argv[])
{

    if (verifarg(argc, argv) == 0)
    {
        return (1);
    }
    int header = atoi(argv[1]);
    int nbcluster = atoi(argv[2]);
    int nbobjet = atoi(argv[3]);

    int *quantiquali = nbdonnequaliquanti(argv[4], header);
    Donne **tableau= extractdonne(argv[4], nbobjet, header, quantiquali[0], quantiquali[1]);

    if (tableau == NULL)
    {
        fprintf(stderr, "problème d'extraction\n");
        return (32);
    }

    Cluster **cluster = Pam(tableau, nbobjet, nbcluster, quantiquali);
    affcluster(cluster, nbcluster,tableau);
    DOOOOOOOOOOOM(cluster, tableau, quantiquali, nbobjet, nbcluster);
    return (0);
}
