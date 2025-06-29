#include <stdarg.h>
#include "task2.h"

#define THREE_DECIMALS 1000.0f
#define TEN 10
#define FIVE 5
#define OPERATORS 5
#define FIFTY 50
#define TWENTY 20
#define TEN_TENS 100
#define TEN_TENS_AND_ONE_DOLLA 101

float rounding1(float x) {
    double z = x * THREE_DECIMALS;
    int y = (int)z;
    if (y % TEN >= FIVE) {
        y += TEN - y % TEN;
    } else {
        y -= y % TEN;
    }

    return (float)y / THREE_DECIMALS;
}

void func(char condition[], ...) {
    char op[3], operators[][OPERATORS] = {"<", ">", "<=", ">=", "=", "!="};

    va_list va;
    va_start(va, condition);

    if (strncmp(condition, "nume", strlen("nume")) == 0) {
        char *table = va_arg(va, char *);
        char *name = va_arg(va, char *);
        sscanf(condition, "%49s %3s %49[^\n]", table, op, name);
    } else if (strncmp(condition, "medie_generala", strlen("medie_generala")) == 0) {
        char *table = va_arg(va, char *);
        float *value = va_arg(va, float *);
        sscanf(condition, "%49s %3s %f", table, op, value);
    } else if (strncmp(condition, "statut", strlen("statut")) == 0) {
        char *table = va_arg(va, char *);
        int *value = va_arg(va, int *);
        char statut = 0;
        sscanf(condition, "%49s %3s %c", table, op, &statut);
        *value = (int)statut;
    } else if (strncmp(condition, "note", strlen("note")) == 0) {
        char *table = va_arg(va, char *);
        float *value1 = va_arg(va, float *);
        float *value2 = va_arg(va, float *);
        float *value3 = va_arg(va, float *);
        sscanf(condition, "%49s %3s %f %f %f", table, op, value1, value2, value3);
    } else {
        char *table = va_arg(va, char *);
        int *value = va_arg(va, int *);
        sscanf(condition, "%49s %3s %d", table, op, value);
    }
    int *operator= va_arg(va, int *);
    for (int i = 0; i <= OPERATORS; i++) {
        if (strcmp(op, operators[i]) == 0) {
            *operator= i;
            break;
        }
    }
    va_end(va);
}

int less_than(int a, int b) {
    return a < b;
}

int greater_than(int a, int b) {
    return a > b;
}

int less_or_equal(int a, int b) {
    return a <= b;
}

int greater_or_equal(int a, int b) {
    return a >= b;
}

int equal(int a, int b) {
    return a == b;
}

int not_equal(int a, int b) {
    return a != b;
}

int equal_but_string(char a[], char b[]) {
    return strcmp(a, b) == 0;
}

int not_equal_but_string(char a[], char b[]) {
    return strcmp(a, b) != 0;
}

typedef int (*compare_func)(int, int);
compare_func const ops[] = {
    less_than, greater_than, less_or_equal,
    greater_or_equal, equal, not_equal};

int *query_enrollments(int n, secretariat *db, ...) {
    int *enrollments = malloc(db->nr_inrolari * sizeof(int));
    for (int i = 0; i < db->nr_inrolari; i++) {
        enrollments[i] = i;
    }
    va_list va;
    va_start(va, db);
    while (n--) {
        char condition[FIFTY], table[TWENTY];
        int operator = 0, value = 0;

        snprintf(condition, sizeof(condition), "%s", va_arg(va, char *));
        if (strncmp(condition, "id_student", strlen("id_student")) == 0 ||
        strncmp(condition, "id_materie", strlen("id_materie")) == 0) {
            func(condition, table, &value, &operator);
        }

        for (int i = 0; i < db->nr_inrolari; i++) {
            if (enrollments[i] == -1) {
                continue;
            }
            if (strcmp(table, "id_student") == 0) {
                if (!ops[operator](db->inrolari[i].id_student, value)) {
                    enrollments[i] = -1;
                }
            } else if (strcmp(table, "id_materie") == 0) {
                if (!ops[operator](db->inrolari[i].id_materie, value)) {
                    enrollments[i] = -1;
                }
            }
        }
    }
    va_end(va);
    return enrollments;
}

int *query_classes(int n, secretariat *db, ...) {
    int *classes = malloc(db->nr_materii * sizeof(int));
    for (int i = 0; i < db->nr_materii; i++) {
        classes[i] = db->materii[i].id;
    }
    va_list va;
    va_start(va, db);
    while (n--) {
        char condition[FIFTY], table[TWENTY], name[FIFTY] = "";
        int operator = 0, value = 0;

        snprintf(condition, sizeof(condition), "%s", va_arg(va, char *));

        if (strncmp(condition, "id", strlen("id")) == 0) {
            func(condition, table, &value, &operator);
        } else {
            func(condition, table, name, &value, &operator);
        }

        for (int i = 0; i < db->nr_materii; i++) {
            if (classes[i] == -1) {
                continue;
            }
            if (strcmp(table, "nume") == 0) {
                if (operator == 4 && !equal_but_string(db->materii[i].nume, name)) {
                    classes[i] = -1;
                } else if (operator == FIVE && !not_equal_but_string(db->materii[i].nume, name)) {
                    classes[i] = -1;
                }
            } else if (strcmp(table, "nume_titular") == 0) {
                if (operator == 4 && !equal_but_string(db->materii[i].nume_titular, name)) {
                    classes[i] = -1;
                } else if (operator == FIVE && !not_equal_but_string(db->materii[i].nume_titular, name)) {
                    classes[i] = -1;
                }
            } else {
                int field_value = 0;
                if (strcmp(table, "id") == 0) {
                    field_value = db->materii[i].id;
                }
                if (!ops[operator](field_value, value)) {
                    classes[i] = -1;
                }
            }
        }
    }
    va_end(va);
    return classes;
}

int *query_students(int n, secretariat *db, ...) {
    int *students = malloc(db->nr_studenti * sizeof(int));
    for (int i = 0; i < db->nr_studenti; i++) {
        students[i] = db->studenti[i].id;
    }
    va_list va;
    va_start(va, db);
    while (n--) {
        char condition[FIFTY], table[TWENTY], name_of_student[FIFTY] = "";
        int operator = 0, value = 0;
        float value_but_float = 0;

        snprintf(condition, sizeof(condition), "%s", va_arg(va, char *));

        if (strncmp(condition, "nume", strlen("nume")) == 0) {
            func(condition, table, name_of_student, &operator);
        } else if (strncmp(condition, "medie_generala", strlen("medie_generala")) == 0) {
            func(condition, table, &value_but_float, &operator);
            value = (int)(value_but_float * TEN_TENS);
        } else if (strncmp(condition, "statut", strlen("statut")) == 0) {
            func(condition, table, &value, &operator);
        } else {
            func(condition, table, &value, &operator);
        }

        if (name_of_student[0] != '\0') {
            name_of_student[strlen(name_of_student) - 1] = '\0';
        }

        for (int i = 0; i < db->nr_studenti; i++) {
            if (students[i] == -1) {
                continue;
            }
            int field_value = 0;
            if (strcmp(table, "id") == 0) {
                field_value = db->studenti[i].id;
            } else if (strcmp(table, "medie_generala") == 0) {
                field_value = (int)(db->studenti[i].medie_generala * TEN_TENS);
            } else if (strcmp(table, "an_studiu") == 0) {
                field_value = db->studenti[i].an_studiu;
            } else if (strcmp(table, "nume") == 0) {
                field_value = -1;
            } else if (strcmp(table, "statut") == 0) {
                field_value = (int)db->studenti[i].statut;
            }
            if (field_value >= 0 && !ops[operator](field_value, value)) {
                students[i] = -1;
            }
            if (strcmp(table, "nume") == 0) {
                if (operator== 4 && !equal_but_string(db->studenti[i].nume, name_of_student)) {
                    students[i] = -1;
                } else if (operator == FIVE && !not_equal_but_string(db->studenti[i].nume, name_of_student)) {
                    students[i] = -1;
                }
            }
        }
    }
    va_end(va);
    return students;
}

int *(*get_query_function(const char *table))(int, secretariat *, ...) {
    if (strcmp(table, "studenti") == 0)
        return query_students;
    if (strcmp(table, "materii") == 0)
        return query_classes;
    if (strcmp(table, "inrolari") == 0)
        return query_enrollments;
    return NULL;
}


void print_students(secretariat *db, int index, char fields[][FIFTY], int fields_len) {
    for (int i = 0; i < fields_len; i++) {
        if (strcmp(fields[i], "id") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%d", db->studenti[index].id);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "nume") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%s", db->studenti[index].nume);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "an_studiu") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%d", db->studenti[index].an_studiu);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "statut") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%c", db->studenti[index].statut);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "medie_generala") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%.2f", db->studenti[index].medie_generala);
        }
        if (i < fields_len - 1) {
            printf(" ");
        }
    }
}

void print_classes(secretariat *db, int index, char fields[][FIFTY], int fields_len) {
    for (int i = 0; i < fields_len; i++) {
        if (strcmp(fields[i], "id") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%d", db->materii[index].id);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "nume") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%s", db->materii[index].nume);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "nume_titular") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%s", db->materii[index].nume_titular);
        }
        if (i < fields_len - 1) {
            printf(" ");
        }
    }
}

void print_enrollments(secretariat *db, int index, char fields[][FIFTY], int fields_len) {
    for (int i = 0; i < fields_len; i++) {
        if (strcmp(fields[i], "id_student") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%d", db->inrolari[index].id_student);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "id_materie") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%d", db->inrolari[index].id_materie);
        }
        if (strcmp(fields[i], "*") == 0) {
            printf(" ");
        }
        if (strcmp(fields[i], "note") == 0 || strcmp(fields[i], "*") == 0) {
            printf("%.2f %.2f %.2f", db->inrolari[index].note[0],
            db->inrolari[index].note[1], db->inrolari[index].note[2]);
        }
        if (i < fields_len - 1) {
            printf(" ");
        }
    }
}

void print_function(secretariat *db, int *vector, int vector_len, char fields[][FIFTY], int fields_len, char *table) {
    for (int i = 0; i < vector_len; i++) {
        if (strcmp(table, "studenti") == 0 && vector[i] != -1) {
            print_students(db, i, fields, fields_len);
            printf("\n");
        } else if (strcmp(table, "materii") == 0 && vector[i] != -1) {
            print_classes(db, i, fields, fields_len);
            printf("\n");
        } else if (strcmp(table, "inrolari") == 0 && vector[i] != -1) {
            print_enrollments(db, i, fields, fields_len);
            printf("\n");
        }
    }
}


void update_students(secretariat *db, int *vector, int vector_len, char update_value[]) {
    int new_value = 0, operator= 0;
    float new_value_but_float = 0;
    char table[FIFTY] = "", new_name[FIFTY] = "";
    for (int i = 0; i < vector_len; i++) {
        if (strncmp(update_value, "nume", strlen("nume")) == 0 && vector[i] != -1) {
            func(update_value, table, new_name, &operator);
            if (new_name[0] == '"') {
                memmove(new_name, new_name + 1, strlen(new_name));
                new_name[strlen(new_name) - 1] = '\0';
            }
            strncpy(db->studenti[i].nume, new_name, sizeof(db->studenti[i].nume));
        } else if (strncmp(update_value, "medie_generala", strlen("medie_generala")) == 0 && vector[i] != -1) {
            func(update_value, table, &new_value_but_float, &operator);
            db->studenti[i].medie_generala = new_value_but_float;
        } else if (strncmp(update_value, "an_studiu", strlen("an_studiu")) == 0 && vector[i] != -1) {
            func(update_value, table, &new_value, &operator);
            db->studenti[i].an_studiu = new_value;
        } else if (strncmp(update_value, "statut", strlen("statut")) == 0 && vector[i] != -1) {
            func(update_value, table, &new_value, &operator);
            db->studenti[i].statut = (char)new_value;
        }
    }
}

void update_classes(secretariat *db, int *vector, int vector_len, char update_value[]) {
    for (int i = 0; i < vector_len; i++) {
        if (strncmp(update_value, "nume", strlen("nume")) == 0 && vector[i] != -1) {
            char table[FIFTY] = "", new_name[FIFTY] = "";
            int operator= 0;
            func(update_value, table, new_name, &operator);
            if (new_name[0] == '"') {
                memmove(new_name, new_name + 1, strlen(new_name));
                new_name[strlen(new_name) - 1] = '\0';
            }
            strncpy(db->materii[i].nume, new_name, strlen(db->materii[i].nume));
        } else if (strncmp(update_value, "nume_titular", strlen("nume_titular")) == 0 && vector[i] != -1) {
            char table[FIFTY] = "", new_name[FIFTY] = "";
            int operator= 0;
            func(update_value, table, new_name, &operator);
            if (new_name[0] == '"') {
                memmove(new_name, new_name + 1, strlen(new_name));
                new_name[strlen(new_name) - 1] = '\0';
            }
            strncpy(db->materii[i].nume_titular, new_name, strlen(db->materii[i].nume_titular));
        }
    }
}

void update_average(secretariat *db, int student_id) {
    float score = 0.0f, average = 0.0f;
    int count = 0;
    for (int i = 0; i < db->nr_inrolari; i++) {
        if (db->inrolari[i].id_student == student_id) {
            for (int j = 0; j < NUMBER_OF_GRADES; j++) {
                score += db->inrolari[i].note[j];
            }
            count++;
        }
    }
    if (count > 0) {
        average = score / (float)count;
    }
    for (int i = 0; i < db->nr_studenti; i++) {
        if (db->studenti[i].id == student_id) {
            db->studenti[i].medie_generala = (float)rounding1(average);
            break;
        }
    }
}

void update_enrollments(secretariat *db, int *vector, int vector_len, char update_value[]) {
    for (int i = 0; i < vector_len; i++) {
        if (strncmp(update_value, "id_student", strlen("id_student")) == 0 && vector[i] != -1) {
            int new_value = 0, operator= 0;
            char table[FIFTY] = "";
            func(update_value, table, &new_value, &operator);
            db->inrolari[i].id_student = new_value;
        } else if (strncmp(update_value, "id_materie", strlen("id_materie")) == 0 && vector[i] != -1) {
            int new_value = 0, operator= 0;
            char table[FIFTY] = "";
            func(update_value, table, &new_value, &operator);
            db->inrolari[i].id_materie = new_value;
        } else if (strncmp(update_value, "note", strlen("note")) == 0 && vector[i] != -1) {
            int operator= 0;
            char table[FIFTY] = "";
            float grade1 = 0, grade2 = 0, grade3 = 0;
            func(update_value, table, &grade1, &grade2, &grade3, &operator);
            db->inrolari[i].note[0] = grade1;
            db->inrolari[i].note[1] = grade2;
            db->inrolari[i].note[2] = grade3;

            update_average(db, db->inrolari[i].id_student);
        }
    }
}

void update_database(secretariat *db, char table[], int *vector, int vector_len, char update_value[]) {
    if (strcmp(table, "studenti") == 0) {
        update_students(db, vector, vector_len, update_value);
    } else if (strcmp(table, "materii") == 0) {
        update_classes(db, vector, vector_len, update_value);
    } else if (strcmp(table, "inrolari") == 0) {
        update_enrollments(db, vector, vector_len, update_value);
    }
}


void delete_students(secretariat *db, int *vector, int vector_len) {
    for (int i = 0; i < vector_len; i++) {
        if (vector[i] != -1) {
            for (int j = vector[i]; j < db->nr_studenti - 1; j++) {
                db->studenti[j] = db->studenti[j + 1];
            }
            db->nr_studenti--;

            for (int k = i + 1; k < vector_len; k++) {
                if (vector[k] > vector[i]) {
                    vector[k]--;
                }
            }
        }
    }
    db->studenti = realloc(db->studenti, db->nr_studenti * sizeof(student));
}


void delete_classes(secretariat *db, int *vector, int vector_len) {
    for (int i = 0; i < vector_len; i++) {
        if (vector[i] != -1) {
            free(db->materii[vector[i]].nume);
            free(db->materii[vector[i]].nume_titular);

            for (int j = vector[i]; j < db->nr_materii - 1; j++) {
                db->materii[j] = db->materii[j + 1];
            }
            db->nr_materii--;

            for (int k = i + 1; k < vector_len; k++) {
                if (vector[k] > vector[i]) {
                    vector[k]--;
                }
            }
        }
    }
    db->materii = realloc(db->materii, db->nr_materii * sizeof(materie));
}

void delete_enrollments(secretariat *db, int *vector, int vector_len) {
    for (int i = 0; i < vector_len; i++) {
        if (vector[i] != -1) {
            for (int j = vector[i]; j < db->nr_inrolari - 1; j++) {
                db->inrolari[j] = db->inrolari[j + 1];
            }
            db->nr_inrolari--;

            for (int k = i + 1; k < vector_len; k++) {
                if (vector[k] > vector[i]) {
                    vector[k]--;
                }
            }
        }
    }

    db->inrolari = realloc(db->inrolari, db->nr_inrolari * sizeof(inrolare));

    for (int i = 0; i < db->nr_studenti; i++) {
        update_average(db, db->studenti[i].id);
    }
}

void delete_database(secretariat *db, char table[], int *vector, int vector_len) {
    if (strcmp(table, "studenti") == 0) {
        delete_students(db, vector, vector_len);
    } else if (strcmp(table, "materii") == 0) {
        delete_classes(db, vector, vector_len);
    } else if (strcmp(table, "inrolari") == 0) {
        delete_enrollments(db, vector, vector_len);
    }
}


int main(int argc, char *argv[]) {
    secretariat *db = citeste_secretariat(argv[1]);

    int number = 0;
    scanf("%d", &number);
    getchar();

    while (number--) {
        int nr_of_comm = 0;
        char table[TWENTY] = "", command[TEN_TENS_AND_ONE_DOLLA] = "", sep[] = " ",
        update_value[FIFTY] = "", condition1[FIFTY] = "\0", condition2[FIFTY] = "\0";
        char vector_comm[TWENTY][TEN_TENS_AND_ONE_DOLLA] = {{0}};

        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        char *p = strtok(command, sep);
        while (p) {
            char aux[TEN_TENS_AND_ONE_DOLLA];
            snprintf(aux, sizeof(aux), "%s", p);

            if (aux[strlen(aux) - 1] == ';') {
                aux[strlen(aux) - 1] = '\0';
            }

            strncpy(vector_comm[nr_of_comm], aux, strlen(aux));
            snprintf(aux, sizeof(aux), "%s", vector_comm[nr_of_comm]);
            nr_of_comm++;

            p = strtok(NULL, sep);
        }

        if (strcmp(vector_comm[0], "SELECT") == 0) {
            int i = 0;

            while (i < nr_of_comm && strncmp(vector_comm[i], "FROM", 4) != 0) {
                i++;
            }

            strncpy(table, vector_comm[++i], sizeof(table));

            while (i < nr_of_comm && strncmp(vector_comm[i], "WHERE", strlen("WHERE")) != 0) {
                i++;
            }

            if (i < nr_of_comm) {
                i++;
                while (i < nr_of_comm && strncmp(vector_comm[i], "AND", 3) != 0) {
                    strncat(condition1, vector_comm[i], sizeof(condition1) - strlen(condition1) - 1);
                    strncat(condition1, " ", sizeof(condition1) - strlen(condition1) - 1);
                    i++;
                }
                condition1[strlen(condition1) - 1] = '\0';
                if (i < nr_of_comm && strncmp(vector_comm[i], "AND", 3) == 0) {
                    i++;
                    while (i < nr_of_comm) {
                        strncat(condition2, vector_comm[i], sizeof(condition2) - strlen(condition2) - 1);
                        strncat(condition2, " ", sizeof(condition2) - strlen(condition2) - 1);
                        i++;
                    }
                    condition2[strlen(condition2) - 1] = '\0';
                }
            }
        } else if (strcmp(vector_comm[0], "UPDATE") == 0) {
            int j = 0;
            while (j < nr_of_comm && strncmp(vector_comm[j], "AND", 3) != 0) {
                j++;
            }
            if (j < nr_of_comm) {
                j++;
                while (j < nr_of_comm) {
                    strncat(condition2, vector_comm[j], sizeof(condition2) - strlen(condition2) - 1);
                    strncat(condition2, " ", sizeof(condition2) - strlen(condition2) - 1);
                    j++;
                }
                condition2[strlen(condition2) - 1] = '\0';
            }

            int i = 1;
            strncpy(table, vector_comm[i], sizeof(table) - 1);
            table[sizeof(table) - 1] = '\0';
            i += 2;
            while (i < nr_of_comm && strcmp(vector_comm[i], "WHERE") != 0) {
                strncat(update_value, vector_comm[i], sizeof(update_value) - strlen(update_value) - 1);
                strncat(update_value, " ", sizeof(update_value) - strlen(update_value) - 1);
                i++;
            }
            if (strlen(update_value) > 0) {
                update_value[strlen(update_value) - 1] = '\0';
            }
            i++;
            while (i < nr_of_comm && strncmp(vector_comm[i], "AND", 3) != 0) {
                strncat(condition1, vector_comm[i], sizeof(condition1) - strlen(condition1) - 1);
                strncat(condition1, " ", sizeof(condition1) - strlen(condition1) - 1);
                i++;
            }
            if (strlen(condition1) > 0) {
                condition1[strlen(condition1) - 1] = '\0';
            }

        } else if (strcmp(vector_comm[0], "DELETE") == 0) {
            int i = 2;
            strncpy(table, vector_comm[i], sizeof(table) - 1);
            table[sizeof(table) - 1] = '\0';
            i += 2;
            while (i < nr_of_comm && strncmp(vector_comm[i], "AND", 3) != 0) {
                strncat(condition1, vector_comm[i], sizeof(condition1) - strlen(condition1) - 1);
                strncat(condition1, " ", sizeof(condition1) - strlen(condition1) - 1);
                i++;
            }
            if (strlen(condition1) > 0) {
                condition1[strlen(condition1) - 1] = '\0';
            }
            if (i < nr_of_comm) {
                i++;
                while (i < nr_of_comm) {
                    strncat(condition2, vector_comm[i], sizeof(condition2) - strlen(condition2) - 1);
                    strncat(condition2, " ", sizeof(condition2) - strlen(condition2) - 1);
                    i++;
                }
                condition2[strlen(condition2) - 1] = '\0';
            }
        }
        int *vector = NULL, vector_len = 0;
        int *(*query_function)(int, secretariat *, ...) = get_query_function(table);

        if (strcmp(table, "studenti") == 0) {
            vector_len = db->nr_studenti;
        } else if (strcmp(table, "materii") == 0) {
            vector_len = db->nr_materii;
        } else if (strcmp(table, "inrolari") == 0) {
            vector_len = db->nr_inrolari;
        }

        if (query_function) {
            if (condition1[0] != '\0' && condition2[0] != '\0') {
                vector = query_function(2, db, condition1, condition2);
            } else if (condition1[0] != '\0') {
                vector = query_function(1, db, condition1);
            } else {
                vector = query_function(0, db);
            }
        }

        if (strcmp(vector_comm[0], "SELECT") == 0) {
            char fields[FIVE][FIFTY];
            int index = 1, i = 0;
            while (index < nr_of_comm && strncmp(vector_comm[index], "FROM", 4) != 0) {
                char aux[FIFTY];
                snprintf(aux, sizeof(aux), "%s", vector_comm[index]);
                if (aux[strlen(aux) - 1] == ',') {
                    strncpy(fields[i], aux, strlen(aux) - 1);
                    fields[i][strlen(aux) - 1] = '\0';
                } else {
                    snprintf(fields[i], sizeof(fields[i]), "%s", aux);
                }
                i++;
                index++;
            }
            int fields_len = i;
            print_function(db, vector, vector_len, fields, fields_len, table);
        } else if (strcmp(vector_comm[0], "UPDATE") == 0) {
            update_database(db, table, vector, vector_len, update_value);
        } else if (strcmp(vector_comm[0], "DELETE") == 0) {
            delete_database(db, table, vector, vector_len);
        }
        free(vector);
    }

    elibereaza_secretariat(&db);
    free(db);
    return 0;
}
