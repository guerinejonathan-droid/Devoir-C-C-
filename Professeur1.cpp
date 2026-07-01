#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

// Structure pour un professeur
typedef struct Professeur {
    int IdP;
    char NomP[30];
    int Age;
    char Profession[30];
    double Salaire;
} Professeur;

// Structure pour la liste doublement chaînée
typedef struct ListeProfesseur {
    Professeur donne;
    struct ListeProfesseur *suivant;
    struct ListeProfesseur *precedent;
} ListeProfesseur;

// Variables globales
ListeProfesseur *tete = NULL;
GtkWidget *tree_view;
GtkListStore *list_store;

// Fonctions de gestion de la liste
void initialiser() {
    tete = NULL;
}

void ajoutTete(Professeur x) {
    ListeProfesseur *maillon = (ListeProfesseur*)malloc(sizeof(ListeProfesseur));
    maillon->donne = x;
    maillon->precedent = NULL;

    if (tete == NULL) {
        maillon->suivant = NULL;
        tete = maillon;
    } else {
        maillon->suivant = tete;
        tete->precedent = maillon;
        tete = maillon;
    }
}

void ajoutFin(Professeur x) {
    ListeProfesseur *maillon = (ListeProfesseur*)malloc(sizeof(ListeProfesseur));
    maillon->donne = x;
    maillon->suivant = NULL;

    if (tete == NULL) {
        maillon->precedent = NULL;
        tete = maillon;
        return;
    }

    ListeProfesseur *p = tete;
    while (p->suivant != NULL)
        p = p->suivant;

    maillon->precedent = p;
    p->suivant = maillon;
}

void supprimerTete() {
    if (tete == NULL) return;

    ListeProfesseur *p = tete;
    tete = tete->suivant;
    if (tete != NULL)
        tete->precedent = NULL;
    free(p);
}

void supprimerFin() {
    if (tete == NULL) return;

    if (tete->suivant == NULL) {
        free(tete);
        tete = NULL;
        return;
    }

    ListeProfesseur *p = tete;
    while (p->suivant != NULL)
        p = p->suivant;

    p->precedent->suivant = NULL;
    free(p);
}

void supprimerPosition(int id) {
    if (tete == NULL) return;

    ListeProfesseur *p = tete;
    while (p != NULL && p->donne.IdP != id)
        p = p->suivant;

    if (p == NULL) return;

    if (p == tete) {
        tete = p->suivant;
        if (tete != NULL)
            tete->precedent = NULL;
    } else if (p->suivant == NULL) {
        p->precedent->suivant = NULL;
    } else {
        p->precedent->suivant = p->suivant;
        p->suivant->precedent = p->precedent;
    }
    free(p);
}

void fusionner(Professeur x) {
    ListeProfesseur *nouveau = (ListeProfesseur*)malloc(sizeof(ListeProfesseur));
    nouveau->donne = x;
    nouveau->suivant = NULL;
    nouveau->precedent = NULL;

    if (tete == NULL) {
        tete = nouveau;
        return;
    }

    if (x.IdP < tete->donne.IdP) {
        nouveau->suivant = tete;
        tete->precedent = nouveau;
        tete = nouveau;
        return;
    }

    ListeProfesseur *p = tete;
    while (p->suivant != NULL && p->suivant->donne.IdP < x.IdP)
        p = p->suivant;

    nouveau->suivant = p->suivant;
    if (p->suivant != NULL)
        p->suivant->precedent = nouveau;
    nouveau->precedent = p;
    p->suivant = nouveau;
}

// Fonction pour mettre à jour l'affichage
void mettreAJourAffichage() {
    gtk_list_store_clear(list_store);

    ListeProfesseur *p = tete;
    GtkTreeIter iter;

    while (p != NULL) {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
            0, p->donne.IdP,
            1, p->donne.NomP,
            2, p->donne.Age,
            3, p->donne.Profession,
            4, p->donne.Salaire,
            -1);
        p = p->suivant;
    }
}

// Fonctions callback des boutons
void on_ajouter_tete_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Ajouter a la tete",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GtkWidget *entry_id = gtk_entry_new();
    GtkWidget *entry_nom = gtk_entry_new();
    GtkWidget *entry_age = gtk_entry_new();
    GtkWidget *entry_prof = gtk_entry_new();
    GtkWidget *entry_salaire = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Age:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_age, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Profession:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prof, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Salaire:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_salaire, 1, 4, 1, 1);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        Professeur p;
        p.IdP = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        strcpy(p.NomP, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
        p.Age = atoi(gtk_entry_get_text(GTK_ENTRY(entry_age)));
        strcpy(p.Profession, gtk_entry_get_text(GTK_ENTRY(entry_prof)));
        p.Salaire = atof(gtk_entry_get_text(GTK_ENTRY(entry_salaire)));

        ajoutTete(p);
        mettreAJourAffichage();
    }

    gtk_widget_destroy(dialog);
}

void on_ajouter_fin_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Ajouter a la fin",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GtkWidget *entry_id = gtk_entry_new();
    GtkWidget *entry_nom = gtk_entry_new();
    GtkWidget *entry_age = gtk_entry_new();
    GtkWidget *entry_prof = gtk_entry_new();
    GtkWidget *entry_salaire = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Age:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_age, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Profession:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prof, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Salaire:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_salaire, 1, 4, 1, 1);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        Professeur p;
        p.IdP = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        strcpy(p.NomP, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
        p.Age = atoi(gtk_entry_get_text(GTK_ENTRY(entry_age)));
        strcpy(p.Profession, gtk_entry_get_text(GTK_ENTRY(entry_prof)));
        p.Salaire = atof(gtk_entry_get_text(GTK_ENTRY(entry_salaire)));

        ajoutFin(p);
        mettreAJourAffichage();
    }

    gtk_widget_destroy(dialog);
}

void on_fusionner_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Fusionner (tri par ID)",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GtkWidget *entry_id = gtk_entry_new();
    GtkWidget *entry_nom = gtk_entry_new();
    GtkWidget *entry_age = gtk_entry_new();
    GtkWidget *entry_prof = gtk_entry_new();
    GtkWidget *entry_salaire = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nom:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Age:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_age, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Profession:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prof, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Salaire:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_salaire, 1, 4, 1, 1);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        Professeur p;
        p.IdP = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        strcpy(p.NomP, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
        p.Age = atoi(gtk_entry_get_text(GTK_ENTRY(entry_age)));
        strcpy(p.Profession, gtk_entry_get_text(GTK_ENTRY(entry_prof)));
        p.Salaire = atof(gtk_entry_get_text(GTK_ENTRY(entry_salaire)));

        fusionner(p);
        mettreAJourAffichage();
    }

    gtk_widget_destroy(dialog);
}

void on_supprimer_tete_clicked(GtkWidget *widget, gpointer data) {
    supprimerTete();
    mettreAJourAffichage();
}

void on_supprimer_fin_clicked(GtkWidget *widget, gpointer data) {
    supprimerFin();
    mettreAJourAffichage();
}

void on_supprimer_position_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Supprimer par ID",
        GTK_WINDOW(data),
        GTK_DIALOG_MODAL,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    GtkWidget *entry_id = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("ID a supprimer:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int id = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        supprimerPosition(id);
        mettreAJourAffichage();
    }

    gtk_widget_destroy(dialog);
}

void on_vider_liste_clicked(GtkWidget *widget, gpointer data) {
    while (tete != NULL) {
        supprimerTete();
    }
    mettreAJourAffichage();
}

// Fonction principale
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    initialiser();

    // Création de la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gestion des Professeurs");
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Conteneur principal
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Zone des boutons
    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);

    // Création des boutons
    GtkWidget *btn_ajouter_tete = gtk_button_new_with_label("Ajouter a la tete");
    GtkWidget *btn_ajouter_fin = gtk_button_new_with_label("Ajouter a la fin");
    GtkWidget *btn_fusionner = gtk_button_new_with_label("Fusionner (tri)");
    GtkWidget *btn_supprimer_tete = gtk_button_new_with_label("Supprimer tete");
    GtkWidget *btn_supprimer_fin = gtk_button_new_with_label("Supprimer fin");
    GtkWidget *btn_supprimer_pos = gtk_button_new_with_label("Supprimer par ID");
    GtkWidget *btn_vider = gtk_button_new_with_label("Vider la liste");

    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_ajouter_tete, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_ajouter_fin, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_fusionner, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_supprimer_tete, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_supprimer_fin, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_supprimer_pos, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_vider, TRUE, TRUE, 0);

    // Connexion des signaux
    g_signal_connect(btn_ajouter_tete, "clicked", G_CALLBACK(on_ajouter_tete_clicked), window);
    g_signal_connect(btn_ajouter_fin, "clicked", G_CALLBACK(on_ajouter_fin_clicked), window);
    g_signal_connect(btn_fusionner, "clicked", G_CALLBACK(on_fusionner_clicked), window);
    g_signal_connect(btn_supprimer_tete, "clicked", G_CALLBACK(on_supprimer_tete_clicked), window);
    g_signal_connect(btn_supprimer_fin, "clicked", G_CALLBACK(on_supprimer_fin_clicked), window);
    g_signal_connect(btn_supprimer_pos, "clicked", G_CALLBACK(on_supprimer_position_clicked), window);
    g_signal_connect(btn_vider, "clicked", G_CALLBACK(on_vider_liste_clicked), window);

    // Création du TreeView
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Création du modèle de données
    list_store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT,
                                    G_TYPE_STRING, G_TYPE_DOUBLE);

    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);

    // Ajout des colonnes
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Age", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Profession", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Salaire", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view);

    // Ajout de quelques données de test
    Professeur p1 = {1, "Dupont", 45, "Mathematique", 3500.50};
    Professeur p2 = {2, "Martin", 38, "Physique", 3200.00};
    Professeur p3 = {3, "Bernard", 52, "Chimie", 3800.75};

    ajoutFin(p1);
    ajoutFin(p2);
    ajoutFin(p3);
    mettreAJourAffichage();

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
