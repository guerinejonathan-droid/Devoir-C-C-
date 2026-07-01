#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Structure for a professor
typedef struct Professeur {
    int IdP;
    char NomP[30];
    int Age;
    char Profession[30];
    double Salaire;
} Professeur;

// Structure for doubly linked list
typedef struct ListeProfesseur {
    Professeur donne;
    struct ListeProfesseur *suivant;
    struct ListeProfesseur *precedent;
} ListeProfesseur;

// Global variables
ListeProfesseur *tete = NULL;
GtkWidget *tree_view;
GtkListStore *list_store;
GtkWidget *statusbar;
GtkWidget *main_window;

// ============ LIST FUNCTIONS ============

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

// ============ UPDATE DISPLAY ============

void mettreAJourAffichage() {
    gtk_list_store_clear(list_store);

    ListeProfesseur *p = tete;
    GtkTreeIter iter;
    int count = 0;

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
        count++;
    }

    char status_text[100];
    sprintf(status_text, "Total Professeurs: %d", count);
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "status"), status_text);
}

// ============ DIALOG FUNCTIONS ============

GtkWidget* create_input_dialog(const char *title, const char *button_label) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        title,
        GTK_WINDOW(main_window),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        "_Cancel", GTK_RESPONSE_CANCEL,
        button_label, GTK_RESPONSE_OK,
        NULL
    );

    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 350);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);

    GtkWidget *label_id = gtk_label_new("ID:");
    GtkWidget *entry_id = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_id), "Enter ID number");
    gtk_widget_set_hexpand(entry_id, TRUE);

    GtkWidget *label_nom = gtk_label_new("Nom:");
    GtkWidget *entry_nom = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nom), "Enter full name");
    gtk_widget_set_hexpand(entry_nom, TRUE);

    GtkWidget *label_age = gtk_label_new("Âge:");
    GtkWidget *entry_age = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_age), "Enter age");
    gtk_widget_set_hexpand(entry_age, TRUE);

    GtkWidget *label_prof = gtk_label_new("Profession:");
    GtkWidget *entry_prof = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_prof), "Enter profession");
    gtk_widget_set_hexpand(entry_prof, TRUE);

    GtkWidget *label_salaire = gtk_label_new("Salaire:");
    GtkWidget *entry_salaire = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_salaire), "Enter salary");
    gtk_widget_set_hexpand(entry_salaire, TRUE);

    gtk_grid_attach(GTK_GRID(grid), label_id, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_nom, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nom, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_age, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_age, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_prof, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_prof, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_salaire, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_salaire, 1, 4, 1, 1);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);
    gtk_widget_show_all(dialog);

    g_object_set_data(G_OBJECT(dialog), "entry_id", entry_id);
    g_object_set_data(G_OBJECT(dialog), "entry_nom", entry_nom);
    g_object_set_data(G_OBJECT(dialog), "entry_age", entry_age);
    g_object_set_data(G_OBJECT(dialog), "entry_prof", entry_prof);
    g_object_set_data(G_OBJECT(dialog), "entry_salaire", entry_salaire);

    return dialog;
}

// ============ FIXED: Get data from dialog with proper casts ============
Professeur get_professeur_from_dialog(GtkWidget *dialog) {
    Professeur p;
    GtkWidget *entry_id = (GtkWidget*)g_object_get_data(G_OBJECT(dialog), "entry_id");
    GtkWidget *entry_nom = (GtkWidget*)g_object_get_data(G_OBJECT(dialog), "entry_nom");
    GtkWidget *entry_age = (GtkWidget*)g_object_get_data(G_OBJECT(dialog), "entry_age");
    GtkWidget *entry_prof = (GtkWidget*)g_object_get_data(G_OBJECT(dialog), "entry_prof");
    GtkWidget *entry_salaire = (GtkWidget*)g_object_get_data(G_OBJECT(dialog), "entry_salaire");

    p.IdP = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
    strcpy(p.NomP, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
    p.Age = atoi(gtk_entry_get_text(GTK_ENTRY(entry_age)));
    strcpy(p.Profession, gtk_entry_get_text(GTK_ENTRY(entry_prof)));
    p.Salaire = atof(gtk_entry_get_text(GTK_ENTRY(entry_salaire)));

    return p;
}

// ============ BUTTON CALLBACKS ============

int show_confirmation_dialog(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(main_window),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "%s", message
    );

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return (response == GTK_RESPONSE_YES);
}

// ============ BUTTON HANDLERS ============

void on_ajouter_tete_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = create_input_dialog("Ajouter à la tête", "_Ajouter");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        Professeur p = get_professeur_from_dialog(dialog);
        ajoutTete(p);
        mettreAJourAffichage();

        GtkWidget *msg = gtk_message_dialog_new(
            GTK_WINDOW(main_window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Professeur ajouté à la tête avec succès!"
        );
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
    }

    gtk_widget_destroy(dialog);
}

void on_ajouter_fin_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = create_input_dialog("Ajouter à la fin", "_Ajouter");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        Professeur p = get_professeur_from_dialog(dialog);
        ajoutFin(p);
        mettreAJourAffichage();

        GtkWidget *msg = gtk_message_dialog_new(
            GTK_WINDOW(main_window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Professeur ajouté à la fin avec succès!"
        );
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
    }

    gtk_widget_destroy(dialog);
}

void on_fusionner_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = create_input_dialog("Fusionner (tri par ID)", "_Fusionner");

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        Professeur p = get_professeur_from_dialog(dialog);
        fusionner(p);
        mettreAJourAffichage();

        GtkWidget *msg = gtk_message_dialog_new(
            GTK_WINDOW(main_window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Fusion réussie! La liste est triée par ID."
        );
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
    }

    gtk_widget_destroy(dialog);
}

void on_supprimer_tete_clicked(GtkWidget *widget, gpointer data) {
    if (tete == NULL) {
        GtkWidget *msg = gtk_message_dialog_new(
            GTK_WINDOW(main_window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "La liste est déjà vide!"
        );
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    if (show_confirmation_dialog("Voulez-vous vraiment supprimer le premier professeur?")) {
        supprimerTete();
        mettreAJourAffichage();
    }
}

void on_supprimer_fin_clicked(GtkWidget *widget, gpointer data) {
    if (tete == NULL) {
        GtkWidget *msg = gtk_message_dialog_new(
            GTK_WINDOW(main_window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "La liste est déjà vide!"
        );
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    if (show_confirmation_dialog("Voulez-vous vraiment supprimer le dernier professeur?")) {
        supprimerFin();
        mettreAJourAffichage();
    }
}

void on_supprimer_position_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Supprimer par ID",
        GTK_WINDOW(main_window),
        (GtkDialogFlags)GTK_DIALOG_MODAL,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Supprimer", GTK_RESPONSE_OK,
        NULL
    );

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 20);

    GtkWidget *label = gtk_label_new("Entrez l'ID du professeur à supprimer:");
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "ID number");

    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int id = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));

        ListeProfesseur *p = tete;
        int found = 0;
        while (p != NULL) {
            if (p->donne.IdP == id) {
                found = 1;
                break;
            }
            p = p->suivant;
        }

        if (!found) {
            GtkWidget *msg = gtk_message_dialog_new(
                GTK_WINDOW(main_window),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Aucun professeur trouvé avec l'ID %d!", id
            );
            gtk_dialog_run(GTK_DIALOG(msg));
            gtk_widget_destroy(msg);
        } else {
            if (show_confirmation_dialog("Voulez-vous vraiment supprimer le professeur avec l'ID?")) {
                supprimerPosition(id);
                mettreAJourAffichage();
            }
        }
    }

    gtk_widget_destroy(dialog);
}

void on_vider_liste_clicked(GtkWidget *widget, gpointer data) {
    if (tete == NULL) {
        GtkWidget *msg = gtk_message_dialog_new(
            GTK_WINDOW(main_window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "La liste est déjà vide!"
        );
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    if (show_confirmation_dialog("Voulez-vous vraiment supprimer TOUS les professeurs?")) {
        while (tete != NULL) {
            supprimerTete();
        }
        mettreAJourAffichage();
    }
}

void on_quit_clicked(GtkWidget *widget, gpointer data) {
    if (show_confirmation_dialog("Voulez-vous vraiment quitter l'application?")) {
        gtk_main_quit();
    }
}

void on_about_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_about_dialog_new();
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Gestion des Professeurs");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "2.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "© 2026");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
        "Application de gestion des professeurs\n"
        "Développée avec GTK+ 3.0\n\n"
        "Fonctionnalités:\n"
        "• Ajouter à la tête/fin\n"
        "• Fusionner triée par ID\n"
        "• Supprimer (tête/fin/par ID)\n"
        "• Vider la liste\n"
        "• Interface professionnelle");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/yourusername");

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// ============ MAIN APPLICATION ============

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    initialiser();

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Gestion des Professeurs - Système Professionnel");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1000, 600);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), main_vbox);

    // ============ TOOLBAR ============
    GtkWidget *toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH_HORIZ);
    gtk_box_pack_start(GTK_BOX(main_vbox), toolbar, FALSE, FALSE, 0);

    GtkToolItem *tool_new = gtk_tool_button_new(NULL, "Ajouter Tête");
    GtkToolItem *tool_add = gtk_tool_button_new(NULL, "Ajouter Fin");
    GtkToolItem *tool_merge = gtk_tool_button_new(NULL, "Fusionner");
    GtkToolItem *tool_sep1 = gtk_separator_tool_item_new();
    GtkToolItem *tool_del_head = gtk_tool_button_new(NULL, "Suppr. Tête");
    GtkToolItem *tool_del_tail = gtk_tool_button_new(NULL, "Suppr. Fin");
    GtkToolItem *tool_del_id = gtk_tool_button_new(NULL, "Suppr. par ID");
    GtkToolItem *tool_sep2 = gtk_separator_tool_item_new();
    GtkToolItem *tool_clear = gtk_tool_button_new(NULL, "Vider");
    GtkToolItem *tool_sep3 = gtk_separator_tool_item_new();
    GtkToolItem *tool_about = gtk_tool_button_new(NULL, "À propos");
    GtkToolItem *tool_quit = gtk_tool_button_new(NULL, "Quitter");

    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_new, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_add, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_merge, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_sep1, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_del_head, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_del_tail, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_del_id, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_sep2, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_clear, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_sep3, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_about, -1);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), tool_quit, -1);

    g_signal_connect(tool_new, "clicked", G_CALLBACK(on_ajouter_tete_clicked), NULL);
    g_signal_connect(tool_add, "clicked", G_CALLBACK(on_ajouter_fin_clicked), NULL);
    g_signal_connect(tool_merge, "clicked", G_CALLBACK(on_fusionner_clicked), NULL);
    g_signal_connect(tool_del_head, "clicked", G_CALLBACK(on_supprimer_tete_clicked), NULL);
    g_signal_connect(tool_del_tail, "clicked", G_CALLBACK(on_supprimer_fin_clicked), NULL);
    g_signal_connect(tool_del_id, "clicked", G_CALLBACK(on_supprimer_position_clicked), NULL);
    g_signal_connect(tool_clear, "clicked", G_CALLBACK(on_vider_liste_clicked), NULL);
    g_signal_connect(tool_about, "clicked", G_CALLBACK(on_about_clicked), NULL);
    g_signal_connect(tool_quit, "clicked", G_CALLBACK(on_quit_clicked), NULL);

    // ============ BUTTON BAR ============
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(main_vbox), button_box, FALSE, FALSE, 0);

    GtkWidget *btn_ajouter_tete = gtk_button_new_with_label("➕ Ajouter Tête");
    GtkWidget *btn_ajouter_fin = gtk_button_new_with_label("➕ Ajouter Fin");
    GtkWidget *btn_fusionner = gtk_button_new_with_label("🔄 Fusionner");
    GtkWidget *btn_supprimer_tete = gtk_button_new_with_label("❌ Suppr. Tête");
    GtkWidget *btn_supprimer_fin = gtk_button_new_with_label("❌ Suppr. Fin");
    GtkWidget *btn_supprimer_pos = gtk_button_new_with_label("❌ Suppr. par ID");
    GtkWidget *btn_vider = gtk_button_new_with_label("🗑️ Vider");

    gtk_widget_set_tooltip_text(btn_ajouter_tete, "Ajouter un professeur en tête de liste");
    gtk_widget_set_tooltip_text(btn_ajouter_fin, "Ajouter un professeur en fin de liste");
    gtk_widget_set_tooltip_text(btn_fusionner, "Insérer en maintenant le tri par ID");
    gtk_widget_set_tooltip_text(btn_supprimer_tete, "Supprimer le premier professeur");
    gtk_widget_set_tooltip_text(btn_supprimer_fin, "Supprimer le dernier professeur");
    gtk_widget_set_tooltip_text(btn_supprimer_pos, "Supprimer un professeur par son ID");
    gtk_widget_set_tooltip_text(btn_vider, "Supprimer tous les professeurs");

    gtk_box_pack_start(GTK_BOX(button_box), btn_ajouter_tete, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_ajouter_fin, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_fusionner, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_supprimer_tete, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_supprimer_fin, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_supprimer_pos, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), btn_vider, TRUE, TRUE, 0);

    g_signal_connect(btn_ajouter_tete, "clicked", G_CALLBACK(on_ajouter_tete_clicked), NULL);
    g_signal_connect(btn_ajouter_fin, "clicked", G_CALLBACK(on_ajouter_fin_clicked), NULL);
    g_signal_connect(btn_fusionner, "clicked", G_CALLBACK(on_fusionner_clicked), NULL);
    g_signal_connect(btn_supprimer_tete, "clicked", G_CALLBACK(on_supprimer_tete_clicked), NULL);
    g_signal_connect(btn_supprimer_fin, "clicked", G_CALLBACK(on_supprimer_fin_clicked), NULL);
    g_signal_connect(btn_supprimer_pos, "clicked", G_CALLBACK(on_supprimer_position_clicked), NULL);
    g_signal_connect(btn_vider, "clicked", G_CALLBACK(on_vider_liste_clicked), NULL);

    // ============ TREE VIEW ============
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(main_vbox), scrolled_window, TRUE, TRUE, 0);

    list_store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT,
                                    G_TYPE_STRING, G_TYPE_DOUBLE);

    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), TRUE);
    gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tree_view), TRUE);

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_column_set_sort_column_id(column, 0);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_sort_column_id(column, 1);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Âge", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_sort_column_id(column, 2);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Profession", renderer, "text", 3, NULL);
    gtk_tree_view_column_set_sort_column_id(column, 3);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Salaire (€)", renderer, "text", 4, NULL);
    gtk_tree_view_column_set_sort_column_id(column, 4);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view);

    // ============ STATUS BAR ============
    statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(main_vbox), statusbar, FALSE, FALSE, 0);

    // ============ ADD SAMPLE DATA ============
    Professeur p1 = {1, "Dupont Jean", 45, "Mathématique", 3500.50};
    Professeur p2 = {2, "Martin Sophie", 38, "Physique", 3200.00};
    Professeur p3 = {3, "Bernard Pierre", 52, "Chimie", 3800.75};
    Professeur p4 = {4, "Dubois Marie", 41, "Biologie", 3400.25};
    Professeur p5 = {5, "Lambert Paul", 49, "Informatique", 4200.00};

    ajoutFin(p1);
    ajoutFin(p2);
    ajoutFin(p3);
    ajoutFin(p4);
    ajoutFin(p5);
    mettreAJourAffichage();

    gtk_widget_show_all(main_window);
    gtk_main();

    return 0;
}
