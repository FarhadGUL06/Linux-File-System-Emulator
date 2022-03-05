#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define MARIME 100

struct Dir;
struct File;

typedef struct Dir {
    char *name;
    struct Dir *parent;
    struct File *head_children_files;
    struct Dir *head_children_dirs;
    struct Dir *next;
} Dir;

typedef struct File {
    char *name;
    struct Dir *parent;
    struct File *next;
} File;

void init_dir(Dir **director, char *name, Dir *parent) {
    // Initializam directorul si alocam dinamic ce e necesar
    (*director) = malloc(sizeof(Dir));
    (*director)->name = malloc(strlen(name) + 1);
    strcpy((*director)->name, name);
    (*director)->parent = parent;
    (*director)->head_children_dirs = NULL;
    (*director)->head_children_files = NULL;
    (*director)->next = NULL;
}

void init_file(File **f, char *name, Dir *parent) {
    // Initializam fisierul si alocam dinamic cele necesare
    (*f) = (File *) malloc(sizeof(File));
    (*f)->next = NULL;
    (*f)->name = malloc(strlen(name) + 1);
    strcpy((*f)->name, name);
    (*f)->parent = parent;
}

void free_director(Dir **d) {
    Dir *curent_d;
    File *curent_f;
    free((*d)->name);
    // Cat timp avem fisiere, le eliberam
    while ((*d)->head_children_files != NULL) {
        curent_f = (*d)->head_children_files;
        (*d)->head_children_files = (*d)->head_children_files->next;
        free(curent_f->name);
        free(curent_f);
    }
    // Cat timp avem directoare, apelam recursiv si le eliberam
    while ((*d)->head_children_dirs != NULL) {
        curent_d = (*d)->head_children_dirs;
        (*d)->head_children_dirs = (*d)->head_children_dirs->next;
        free_director(&curent_d);
        free(curent_d);
    }
}

void add_file_in_dir(Dir **parent, File **f) {
    if (((*parent) == NULL) || ((*f) == NULL)) {
        // Caz in care nu am fisier sau parinte
        return;
    }
    // Daca nu am niciun fisier
    if ((*parent)->head_children_files == NULL) {
        (*parent)->head_children_files = (*f);
    } else {
        // Parcurgem si adaugam la final
        File *p = (*parent)->head_children_files;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = (*f);
    }
}

void add_dir_in_dir(Dir **parent, Dir **dir) {
    if (((*parent) == NULL) || ((*dir) == NULL)) {
        // Caz in care nu am director sau parinte
        return;
    }
    // Daca nu am niciun director
    if ((*parent)->head_children_dirs == NULL) {
        (*parent)->head_children_dirs = (*dir);
    } else {
        // Parcurgem si il adaugam la coada
        Dir *p = (*parent)->head_children_dirs;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = (*dir);
    }
}

void touch(Dir *parent, char *name) {
    Dir *curent_d = parent->head_children_dirs;
    File *curent_f = parent->head_children_files;
    int ok = 1;
    // Verificam daca exista un director cu numele acesta
    while (curent_d) {
        if (!strcmp(curent_d->name, name)) {
            printf("File already exists\n");
            ok = 0;
        }
        curent_d = curent_d->next;
    }
    // Verificam daca exista un fisier
    while (curent_f && ok == 1) {
        if (!strcmp(curent_f->name, name)) {
            printf("File already exists\n");
            ok = 0;
        }
        curent_f = curent_f->next;
    }
    if (ok == 1) {
        // Cream fisierul si il adaugam
        File *f;
        init_file(&f, name, parent);
        add_file_in_dir(&parent, &f);
    }
}

void mkdir(Dir *parent, char *name) {
    Dir *curent_d = parent->head_children_dirs;
    File *curent_f = parent->head_children_files;
    int ok = 1;
    // Verificam daca exista un fisier cu numele acesta
    while (curent_f) {
        if (!strcmp(curent_f->name, name)) {
            printf("Directory already exists\n");
            ok = 0;
        }
        curent_f = curent_f->next;
    }
    while (curent_d && ok == 1) {
        // Daca exista deja un director
        if (!strcmp(curent_d->name, name)) {
            printf("Directory already exists\n");
            ok = 0;
        }
        curent_d = curent_d->next;
    }
    // Daca n-am gasit directorul / fisierul
    if (ok == 1) {
        // Cream si il adaugam in directorul parinte
        Dir *d;
        init_dir(&d, name, parent);
        add_dir_in_dir(&parent, &d);
    }
}

void ls(Dir *parent) {
    // Parcurgem directoarele si le afisam
    Dir *curent_d = parent->head_children_dirs;
    while (curent_d != NULL) {
        printf("%s\n", curent_d->name);
        curent_d = curent_d->next;
    }
    // Parcurgem fisierele si le afisam
    File *curent_f = parent->head_children_files;
    while (curent_f != NULL) {
        printf("%s\n", curent_f->name);
        curent_f = curent_f->next;
    }
}

void rm(Dir *parent, char *name) {
    File *f = parent->head_children_files;
    int ok = 0;
    // Daca nu exista fisiere
    if (f == NULL) {
        printf("Could not find the file\n");
        return;
    }
    // Daca este primul fisier
    if (!strcmp(f->name, name)) {
        parent->head_children_files = parent->head_children_files->next;
        free(f->name);
        free(f);
        return;
    }
    // Parcurgem lista
    while (f->next != NULL) {
        // Pana cand il gasim
        if (strcmp(f->next->name, name) == 0) {
            ok = 1;
            File *curent = f->next;
            if (f->next->next != NULL) {
                f->next = f->next->next;
                free(curent->name);
                free(curent);
            } else {
                // Daca este ultimul
                free(f->next->name);
                free(f->next);
                f->next = NULL;
                break;
            }
        }
        f = f->next;
    }
    if (ok == 0) {
        printf("Could not find the file\n");
    }
}

void rmdir(Dir *parent, char *name);

void rm_dir_files(Dir *d) {
    File *curent_f = d->head_children_files;
    while (curent_f != NULL) {
        // Stergem toate fisierele din director
        rm(d, curent_f->name);
        curent_f = d->head_children_files;
    }
    Dir *curent_d = d->head_children_dirs;
    while (curent_d != NULL) {
        // Stergem si subdirectoarele
        rmdir(d, curent_d->name);
        curent_d = d->head_children_dirs;
    }
}

void rmdir(Dir *parent, char *name) {
    Dir *d = parent->head_children_dirs;
    int ok = 0;
    // Daca nu exista niciun director
    if (d == NULL) {
        printf("Could not find the dir\n");
        return;
    }
    // Daca directorul de sters este primul
    if (!strcmp(d->name, name)) {
        parent->head_children_dirs = parent->head_children_dirs->next;
        rm_dir_files(d);
        free(d->name);
        free(d);
        return;
    }
    // Parcurgem lista pana gasim directorul indicat
    while (d->next != NULL) {
        if (strcmp(d->next->name, name) == 0) {
            ok = 1;
            Dir *curent = d->next;
            // Verificam sa nu fie ultimul element
            if (d->next->next != NULL) {
                d->next = d->next->next;
                free(curent->name);
                free(curent);
            } else {
                // Caz in care este fix ultimul element
                free(d->next->name);
                rm_dir_files(d->next);
                free(d->next);
                d->next = NULL;
                break;
            }
        }
        d = d->next;
    }
    if (ok == 0) {
        printf("Could not find the dir\n");
    }

}

void cd(Dir **target, char *name) {
    int ok = 0;
    // Daca este rulat cd .. merge maxim pana in home
    if (!strcmp(name, "..")) {
        if ((*target)->parent != NULL) {
            (*target) = (*target)->parent;
        }
        return;
    }
    // Cauta directorul indicat si seteaza target-ul pe el
    Dir *curent = (*target)->head_children_dirs;
    while (curent != NULL) {
        // Daca este gasit
        if (!strcmp(curent->name, name)) {
            (*target) = curent;
            ok = 1;
        }
        curent = curent->next;
    }
    if (ok == 0) {
        printf("%s\n", "No directories found!");
    }
}

char *pwd(Dir *target) {
    char *cale = malloc(MARIME * sizeof(char *));
    // Am luat un vector de siruri de caractere pentru a retine
    // toate directoarele parcurse din home pana in dir curent
    char **directoare = (char **) malloc(MARIME * sizeof(char *));
    int i, cai = 0;
    for (i = 0; i < MARIME; i++) {
        // Alocam memorie pentru fiecare string
    }
    Dir *curent = target;
    // Parcurgem din dir curent pana in home
    // si punem string-ul in vector
    while (curent != NULL) {
        *(directoare + cai) = (char *) malloc(MARIME);
        strcpy(*(directoare + cai), curent->name);
        cai++;
        curent = curent->parent;
    }
    // Construim calea in "cale"
    strcpy(cale, "");
    i = cai;
    while (i) {
        strcat(cale, "/");
        strcat(cale, *(directoare + i - 1));
        i--;
    }
    // Eliberam memoria pentru vectorul de stringuri
    for (i = 0; i < cai; i++) {
        free(*(directoare + i));
    }
    free(directoare);
    return cale;
}

void stop(Dir *target) {
    // Parcurgem inapoi pentru a ajunge in home
    while (target->parent != NULL) {
        target = target->parent;
    }
    // Eliberam memoria directorului home
    free_director(&target);
    free(target);
    // Oprim programul (setam ok din main pe 0)
}

void afisare_din_tree(char *nume, int level) {
    int i;
    for (i = 0; i < level; i++) {
        printf("    ");
    }
    printf("%s\n", nume);
}

void tree(Dir *target, int level) {
    Dir *curent_d = target->head_children_dirs;
    File *curent_f = target->head_children_files;
    // Parcurgem recursiv toate directoarele din dir curent
    while (curent_d != NULL) {
        afisare_din_tree(curent_d->name, level);
        tree(curent_d, level + 1);
        curent_d = curent_d->next;
    }
    // Afisam fisierele din cadrul directorului
    while (curent_f != NULL) {
        afisare_din_tree(curent_f->name, level);
        curent_f = curent_f->next;
    }
}

void mv_dir(Dir **parent, Dir **dir) {
    Dir *curent = (*parent)->head_children_dirs;
    // Daca este singurul, ramane neschimbat
    if (!strcmp((*parent)->head_children_dirs->name, (*dir)->name) && (*parent)->head_children_dirs->next == NULL) {
        return;
    }
    // Daca este primul dar nu singurul, il mutam la final
    if (!strcmp((*parent)->head_children_dirs->name, (*dir)->name)) {
        (*parent)->head_children_dirs = (*parent)->head_children_dirs->next;
    }
    // Parcurgem lista
    while (curent->next != NULL) {
        // Cautam
        if (!strcmp(curent->next->name, (*dir)->name)) {
            if (curent->next->next != NULL) {
                curent->next = curent->next->next;
            }
        }
        curent = curent->next;
    }
    // Setam la final
    curent->next = (*dir);
    (*dir)->next = NULL;

}

void mv_file(Dir **parent, File **dir) {
    File *curent = (*parent)->head_children_files;
    // Daca este singurul fisier, ramane neschimbat
    if (!strcmp((*parent)->head_children_files->name, (*dir)->name) && (*parent)->head_children_files->next == NULL) {
        return;
    }
    // Daca este primul element, il scoatem de la inceput
    if (!strcmp((*parent)->head_children_files->name, (*dir)->name)) {
        (*parent)->head_children_files = (*parent)->head_children_files->next;
    }
    // Parcurgem lista
    while (curent->next != NULL) {
        // Parcurgem pana gasim fisierul
        if (!strcmp(curent->next->name, (*dir)->name)) {
            if (curent->next->next != NULL) {
                curent->next = curent->next->next;
            }
        }
        curent = curent->next;
    }
    // Setam la final
    curent->next = (*dir);
    (*dir)->next = NULL;
}

void mv(Dir *parent, char *oldname, char *newname) {
    Dir *curent_d = parent->head_children_dirs;
    File *curent_f = parent->head_children_files;
    int found_old = 0;
    int already_exists = 0;
    while (curent_d) {
        // Cautam daca exista directorul cu numele acesta
        if (!strcmp(curent_d->name, newname)) {
            already_exists = 1;
        }
        curent_d = curent_d->next;
    }
    while (curent_f) {
        // Verificam daca exista deja fisier cu numele newname
        if (!strcmp(curent_f->name, newname)) {
            already_exists = 1;
        }
        curent_f = curent_f->next;
    }
    if (already_exists == 0) {
        // Daca nu exista un fisier / director cu newname
        curent_d = parent->head_children_dirs;
        while (curent_d) {
            if (!strcmp(curent_d->name, oldname)) {
                // Schimbam directorul si-l mutam la capat
                curent_d->name = realloc(curent_d->name, strlen(newname) + 1);
                strcpy(curent_d->name, newname);
                found_old = 1;
                // Apelam functia de mutare
                mv_dir(&curent_d->parent, &curent_d);
            }
            curent_d = curent_d->next;
        }
        if (found_old == 0) {
            curent_f = parent->head_children_files;
            while (curent_f) {
                if (!strcmp(curent_f->name, oldname)) {
                    // Schimbam fisierul si-l mutam la capat
                    curent_f->name = realloc(curent_f->name, strlen(newname) + 1);
                    strcpy(curent_f->name, newname);
                    // Apelam functia de mutare
                    mv_file(&curent_f->parent, &curent_f);
                    found_old = 1;
                }
                curent_f = curent_f->next;
            }
        }
    }
    if (already_exists == 1) {
        printf("File/Director already exists\n");
    } else {
        if (found_old == 0) {
            printf("File/Director not found\n");
        }
    }
}

void apelare_zero_parametrii(Dir **director, char *comanda) {
    if (!strcmp(comanda, "ls")) {
        ls((*director));
    }
    if (!(strcmp(comanda, "stop"))) {
        stop((*director));
    }
    if (!strcmp(comanda, "pwd")) {
        char *cale;
        cale = pwd((*director));
        printf("%s\n", cale);
        free(cale);
    }
    if (!strcmp(comanda, "tree")) {
        tree((*director), 0);
    }
}

void apelare_un_parametru(Dir **director, char *comanda, char *par1) {
    if (!strcmp(comanda, "touch")) {
        touch((*director), par1);
    }
    if (!strcmp(comanda, "mkdir")) {
        mkdir((*director), par1);
    }
    if (!strcmp(comanda, "rm")) {
        rm((*director), par1);
    }
    if (!strcmp(comanda, "rmdir")) {
        rmdir((*director), par1);
    }
    if (!strcmp(comanda, "cd")) {
        cd(&(*director), par1);
    }
}

void apelare_doi_parametrii(Dir **director, char *comanda, char *par1, char *par2) {
    if (!strcmp(comanda, "mv")) {
        mv((*director), par1, par2);
    }
}

void apelare(Dir **director, char *sir, char *comanda, int nr_parametrii) {
    int i = (int) strlen(comanda) + 1, j = 0;
    int len = (int) strlen(sir);
    if (nr_parametrii == 0) {
        // Apelam functiile care nu au parametrii
        apelare_zero_parametrii(&(*director), comanda);
    } else {
        // Stabilim primul parametru dat
        char *par1 = malloc(MARIME);
        while (j + i < len && *(sir + i + j) != ' ') {
            *(par1 + j) = *(sir + i + j);
            j++;
        }
        *(par1 + j) = '\0';
        if (nr_parametrii == 2) {
            // Daca mai avem un parametru, il stabilim si pe acesta
            char *par2 = malloc(MARIME);
            i = i + j + 1;
            j = 0;
            while (j + i < len && *(sir + i + j) != ' ') {
                *(par2 + j) = *(sir + i + j);
                j++;
            }
            *(par2 + j) = '\0';
            // Apelam functia pentru 2 parametrii dati
            apelare_doi_parametrii(&(*director), comanda, par1, par2);
            free(par1);
            free(par2);
        } else {
            // Apelam functia pentru un parametru dat
            apelare_un_parametru(&(*director), comanda, par1);
            free(par1);
        }
    }
}

int main(void) {
    Dir *director;
    init_dir(&director, "home", NULL);
    int i, nr_arg;
    char *sir = malloc(MAX_INPUT_LINE_SIZE + 1);
    do {
        i = 0;
        nr_arg = 0;
        fgets(sir, MAX_INPUT_LINE_SIZE + 1, stdin);
        // Cazuri speciale cu \n sau space dupa text
        if ((*(sir + strlen(sir) - 2) == ' ') || (*(sir + strlen(sir) - 2) == '\n')) {
            *(sir + strlen(sir) - 2) = '\0';
        }
        if (*(sir + strlen(sir) - 1) == '\n') {
            *(sir + strlen(sir) - 1) = '\0';
        }
        char *comanda = malloc(MARIME);
        // Punem comanda data in comanda
        while ((*(sir + i) != ' ') && (*(sir + i) != '\n') && (*(sir + i) != '\0')) {
            *(comanda + i) = *(sir + i);
            i++;
        }
        *(comanda + i) = '\0';
        // Calculam numarul de argumente
        while (*(sir + i) != '\0') {
            if (*(sir + i) == ' ') {
                nr_arg++;
            }
            i++;
        }
        // Apelam functia
        apelare(&director, sir, comanda, nr_arg);
        // Eliberam comanda spre a fi reinitializata
        free(comanda);
    } while (strncmp(sir, "stop", 4) != 0);
    free(sir);
    return 0;
}
