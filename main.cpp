#include<stdio.h>
#include<stdlib.h>

typedef struct Professeur SP;
struct Professeur{

    int IdP;
    char NomP[30];
    int Age;
    char Profession[30];
    double Salaire;
};
typedef struct ListeProfesseur SL;
struct ListeProfesseur{

    SP donne;
    SL *suivant;
    SL *precedent;
};

SL *tete,*maillon;
void initialiser();
void ajoutTete(SP x);
void ajoutFin(SP x);
void ajoutPosition(SP x, int post);
void supprimerTete();
void supprimerFin();
void supprimerPosition(int post);
void fusionner(SP x);
void afficher();


int main(){
        int n,i;
        int post;
        SP donne;

        do{
            printf("Saisir n:");
            scanf("%d",&n);
        }while(n<=0);
        initialiser();

        for(i=1; i<=n; i++){
            printf("Donnee #%d a la fin:\n",i);
            printf("l'id du professeur....... :");
            scanf("%d",&donne.IdP);
            printf("Le nom du professeur........:");
            scanf("%29s",donne.NomP);
            printf("L'age du professeur......:");
            scanf("%d", &donne.Age);
            printf("La profession du professeur.......:");
            scanf("%29s", donne.Profession);
            printf("Le salaire du professeur........:");
            scanf("%lf", &donne.Salaire);
            ajoutFin(donne);

        }
        system("cls");
        afficher();
        system("pause");

            printf("Professeur a la tete:");
            printf("l'id du professeur....... :");
            scanf("%d",&donne.IdP);
            printf("Nom professeur........:");
            scanf("%29s",donne.NomP);
            printf("L'age professeur......:");
            scanf("%d", &donne.Age);
            printf("Profession du professeur.......:");
            scanf("%29s", donne.Profession);
            printf("Salaire du professeur........:");
            scanf("%lf", &donne.Salaire);
            ajoutTete(donne);

            system("cls");
            afficher();
            system("pause");

            printf("Professeur a la position:");
            printf("l'id du professeur....... :");
            scanf("%d",&donne.IdP);
            printf("Nom professeur........:");
            scanf("%29s",donne.NomP);
            printf("L'age professeur......:");
            scanf("%d", &donne.Age);
            printf("Profession du professeur.......:");
            scanf("%29s", donne.Profession);
            printf("Salaire du professeur........:");
            scanf("%lf", &donne.Salaire);
            printf("Entrer la position du professeur:");
            scanf("%d",&post);
            ajoutPosition(donne,post);

            system("cls");
            afficher();
            system("pause");

            printf("Supprimer de la tete");
            supprimerTete();
            afficher();
            system("pause");

            printf("Supprimer a la fin");
            supprimerFin();
            afficher();
            system("pause");

            printf("Supprimer la position:");
            printf("Entrer l'id du professeur a supprimer:");
            scanf("%d",&post);
            supprimerPosition(post);
            afficher();
            system("pause");

            printf("test de fussion:");
            printf("l'id du professeur....... :");
            scanf("%d",&donne.IdP);
            printf("Nom professeur........:");
            scanf("%29s",donne.NomP);
            printf("L'age professeur......:");
            scanf("%d", &donne.Age);
            printf("Profession du professeur.......:");
            scanf("%29s", donne.Profession);
            printf("Salaire du professeur........:");
            scanf("%lf", &donne.Salaire);
            fusionner(donne);
            afficher();
            system("pause");

            while(tete != NULL){
                supprimerTete();
            }

            printf("Programme termine");

            return 0;

}

void initialiser(){
    tete = NULL;
    return;
}

void ajoutTete(SP x){
    maillon = new SL;
    maillon -> donne=x;
    maillon -> precedent=NULL;
    if(tete == NULL){
        maillon -> suivant=tete;
        tete=maillon;
    }else{
        maillon -> suivant=tete;
        tete->precedent=maillon;
        tete=maillon;
    }
}

void ajoutFin(SP x){
    SL *p;
    maillon = new SL;
    maillon -> donne=x;
    if(tete!= NULL){
        p = tete;
        while(p->suivant!=NULL)
            p=p->suivant;
        maillon->suivant=NULL;
        p->suivant=maillon;
        maillon->precedent=p;
    }else{
        ajoutTete(x);
    }
}

void ajoutPosition(SP x, int post) {
    SL* p = tete;
    SL* nouveau = new SL;
    nouveau->donne = x;

    if (tete == NULL) {

        nouveau->suivant = NULL;
        nouveau->precedent = NULL;
        tete = nouveau;
        return;
    }

    int i = 1;
    while (p != NULL && i < post) {
        p = p->suivant;
        i++;
    }

    if (p == NULL) {

        ajoutFin(x);
    } else {

        nouveau->suivant = p->suivant;
        nouveau->precedent = p;
        if (p->suivant != NULL) {
            p->suivant->precedent = nouveau;
        }
        p->suivant = nouveau;
    }
}

void supprimerTete() {
    if (tete != NULL) {
        SL* p = tete;
        tete = tete->suivant;
        if (tete != NULL) {
            tete->precedent = NULL;
        }
        delete p;
        printf("Le premier professeur a ete supprime.\n");
    } else {
        printf("La liste est vide, rien a supprimer.\n");
    }
}

void supprimerFin() {
    SL *p;

    if(tete == NULL) {
        printf("Liste vide !\n");
        return;
    }

    if(tete->suivant == NULL) {
        delete tete;
        tete = NULL;
    } else {
        p = tete;
        while(p->suivant != NULL)
            p = p->suivant;

        p->precedent->suivant = NULL;
        delete p;
    }

    printf("Fin supprimee !\n");
}

void supprimerPosition(int post) {
    SL *p;

    if(tete == NULL) {
        printf("Liste vide !\n");
        return;
    }

    p = tete;
    while(p != NULL && p->donne.IdP != post)
        p = p->suivant;

    if(p == NULL) {
        printf("ID %d non trouve !\n", post);
        return;
    }

    if(p == tete) {
        tete = p->suivant;
        if(tete != NULL)
            tete->precedent = NULL;
    } else if(p->suivant == NULL) {
        p->precedent->suivant = NULL;
    } else {
        p->precedent->suivant = p->suivant;
        p->suivant->precedent = p->precedent;
    }

    delete p;
    printf("Element supprime !\n");
}

void fusionner(SP x) {
    SL *p, *nouveau;
    nouveau = new SL;
    nouveau->donne = x;
    nouveau->suivant = NULL;
    nouveau->precedent = NULL;

    if(tete == NULL) {
        tete = nouveau;
        return;
    }


    if(x.IdP < tete->donne.IdP) {
        nouveau->suivant = tete;
        tete->precedent = nouveau;
        tete = nouveau;
        return;
    }

    p = tete;
    while(p->suivant != NULL && p->suivant->donne.IdP < x.IdP)
        p = p->suivant;

    nouveau->suivant = p->suivant;
    if(p->suivant != NULL)
        p->suivant->precedent = nouveau;
    nouveau->precedent = p;
    p->suivant = nouveau;

    printf("Fusion reussie !\n");
}

void afficher() {
    SL *p;
    int compteur = 1;

    if(tete == NULL) {
        printf("Liste vide !\n");
        return;
    }

    printf("\n--- Liste des Professeurs ---\n");
    printf("N°\tID\tAge\tNom\t\tProfession\tSalaire\n");
    printf("--------------------------------------------------------\n");

    p = tete;
    while(p != NULL) {
        printf("%d\t%d\t%d\t%s\t\t%s\t\t%.2lf\n",
               compteur,
               p->donne.IdP,
               p->donne.Age,
               p->donne.NomP,
               p->donne.Profession,
               p->donne.Salaire);
        p = p->suivant;
        compteur++;
    }
    printf("--------------------------------------------------------\n");
}
