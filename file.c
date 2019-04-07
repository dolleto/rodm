#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time

typedef struct Element Element;
struct Element
{
    unsigned long nombre;
    Element *suivant;
};

typedef struct File File;
struct File
{
    Element *premier;
    int size;
};

File *initialiser()
{
    File *file = malloc(sizeof(*file));
    file->premier = NULL;
    file->size = 0;

    return file;
}


void enfiler(File *file, int nvNombre)
{
    Element *nouveau = malloc(sizeof(*nouveau));
    if (file == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->nombre = nvNombre;
    nouveau->suivant = NULL;

    if (file->premier != NULL) /* La file n'est pas vide */
    {
        /* On se positionne à la fin de la file */
        Element *elementActuel = file->premier;
        while (elementActuel->suivant != NULL)
        {
            elementActuel = elementActuel->suivant;
        }
        elementActuel->suivant = nouveau;
        file->size++;
    }
    else /* La file est vide, notre élément est le premier */
    {
        file->premier = nouveau;
        file->size++;
    }
}

int defiler(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombreDefile = 0;

    /* On vérifie s'il y a quelque chose à défiler */
    if (file->premier != NULL)
    {
        Element *elementDefile = file->premier;

        nombreDefile = elementDefile->nombre;
        file->premier = elementDefile->suivant;
        free(elementDefile);
        file->size--;
    }

    return nombreDefile;
}

void afficherFile(File *file)
{
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Element *actuel = file->premier;

    while (actuel != NULL)
    {
        printf("%lu\n", actuel->nombre);
        actuel = actuel->suivant;
    }

    printf("\n");
}
