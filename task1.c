#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task1.h"

#define NUMBER_OF_GRADES 3
#define MAX_LINE_LENGTH 256
#define MAX_STUDENT_NAME 40
#define MAX_SUBJECT_NAME 50
#define MAX_SUBJECT_TEACHER_NAME 50
#define MATERII_LENGTH 9
#define INROLARI_LENGTH 10
#define THREE_DECIMALS 1000.0f
#define TEN 10
#define FIVE 5

float rounding(float x) {
    double z = x * THREE_DECIMALS;
    int y = (int)z;
    if (y % TEN >= FIVE) {
        y += TEN - y % TEN;
    } else {
        y -= y % TEN;
    }

    return (float)y / THREE_DECIMALS;
}

secretariat *citeste_secretariat(const char *nume_fisier) {
    FILE *file = fopen(nume_fisier, "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    char line[MAX_LINE_LENGTH];

    secretariat *db = malloc(sizeof(secretariat));
    db->studenti = NULL;
    db->nr_studenti = 0;
    db->materii = NULL;
    db->nr_materii = 0;
    db->inrolari = NULL;
    db->nr_inrolari = 0;

    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "[MATERII]", MATERII_LENGTH) == 0) {
            break;
        }

        db->studenti = realloc(db->studenti, (db->nr_studenti + 1) * sizeof(student));
        if (db->studenti == NULL) {
            fclose(file);
            free(db);
            return NULL;
        }

        student *aux = &db->studenti[db->nr_studenti];

        sscanf(line, "%d, %40[^,], %d, %c", &aux->id, aux->nume, &aux->an_studiu, &aux->statut);
        db->nr_studenti++;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "[INROLARI]", INROLARI_LENGTH) == 0) {
            break;
        }

        db->materii = realloc(db->materii, (db->nr_materii + 1) * sizeof(materie));
        if (db->materii == NULL) {
            fclose(file);
            free(db->studenti);
            free(db);
            return NULL;
        }

        materie *aux = &db->materii[db->nr_materii];
        aux->nume = malloc(MAX_SUBJECT_NAME* sizeof(char));
        aux->nume_titular = malloc(MAX_SUBJECT_TEACHER_NAME * sizeof(char));
        sscanf(line, "%d, %50[^,], %50[^\n]", &aux->id, aux->nume, aux->nume_titular);
        db->nr_materii++;
    }

    while (fgets(line, sizeof(line), file)) {
        db->inrolari = realloc(db->inrolari, (db->nr_inrolari + 1) * sizeof(inrolare));
        if (db->inrolari == NULL) {
            fclose(file);
            free(db->studenti);
            free(db->materii);
            free(db);
            return NULL;
        }

        inrolare *aux = &db->inrolari[db->nr_inrolari];
        sscanf(line, "%d, %d, %f %f %f", &aux->id_student, &aux->id_materie,
            &aux->note[0], &aux->note[1], &aux->note[2]);
        db->nr_inrolari++;
    }

    int j = 0;
    for (int i = 0; i < db->nr_studenti; i++) {
        float score = 0.0f, average = 0.0f;
        int count = 0;
        while (j < db->nr_inrolari && db->inrolari[j].id_student == db->studenti[i].id) {
            for (int k = 0; k < NUMBER_OF_GRADES; k++) {
                score += db->inrolari[j].note[k];
            }
            count++;
            j++;
        }
        if (count > 0) {
            average = score / (float)count;
        }
        db->studenti[i].medie_generala = rounding(average);
    }
    fclose(file);
    return db;
}

void adauga_student(secretariat *s, int id, char *nume, int an_studiu, char statut, float medie_generala) {
    s->studenti = realloc(s->studenti, (s->nr_studenti + 1) * sizeof(student));
    if (s->studenti == NULL) {
        return;
    }

    student *aux = &s->studenti[s->nr_studenti];
    aux->id = id;
    snprintf(aux->nume, MAX_STUDENT_NAME, "%s", nume);
    aux->an_studiu = an_studiu;
    aux->statut = statut;
    aux->medie_generala = medie_generala;
    s->nr_studenti++;
}

void elibereaza_secretariat(secretariat **s) {
    free((*s)->studenti);
    (*s)->studenti = NULL;

    for (int i = 0; i < (*s)->nr_materii; i++) {
        free((*s)->materii[i].nume);
        free((*s)->materii[i].nume_titular);
        (*s)->materii[i].nume = NULL;
        (*s)->materii[i].nume_titular = NULL;
    }
    free((*s)->materii);
    (*s)->materii = NULL;

    free((*s)->inrolari);
    (*s)->inrolari = NULL;

    free(*s);
    *s = NULL;
}
