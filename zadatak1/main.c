#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#define SUCCESS 0
#define FILE_ERROR -1
#define ALLOCATION_ERROR -2
#define READ_ERROR -3

#define MAX_NAME 50
#define MAX_CHARACTERS 1024
#define MAX_POINTS 60

typedef struct {
    char name[MAX_NAME];
    char surname[MAX_NAME];
    int points;
} Student;

int Count_Students(char *filename, int *ct_students);
int Read_Students(char *filename, int n, Student *students);
int Print_Students(Student *students, int n);

int main() {

    int number = 0;
    int status = 0;
    Student *students = NULL;
    status = Count_Students("studenti.txt", &number);
    if (status != SUCCESS) {
        return status;
    }

    if(number == 0){
        printf("File is empty\n");
        return FILE_ERROR;
    }

    printf("Number of students: %d\n", number);

    students = malloc(sizeof(Student) * number);
    if(!students){
        printf("Memory allocation error!\n");
        return ALLOCATION_ERROR;
    }

    status = Read_Students("studenti.txt", number, students);
    if(status != SUCCESS) {
        free(students);
        return status;
    }

    status = Print_Students(students, number);
    if(status != SUCCESS){
        free(students);
        return status;
    }

    free(students);
    return SUCCESS;
}

int Count_Students(char *filename, int *ct_students) {
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if(!fp){
        printf("Error opening file!\n");
        return FILE_ERROR;
    }

    int count = 0;
    char line[MAX_CHARACTERS] = {0}; // da ne nastanu bugovi u memoriji niz postavi sve na 0 i onda je zapravo prazan string

    while (fgets(line, MAX_CHARACTERS, fp)){
        if(line[0] != '\n'){  //provjera ako postoji prazan redak da ga preskoci
            count++;
        }
    }

    *ct_students = count;
    fclose(fp);

    return SUCCESS;    
}

int Read_Students(char *filename, int n, Student *students) {
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if(!fp){
        printf("Error opening file!\n");
        return FILE_ERROR;
    }

    for(int i = 0; i < n; i++) {
        if (fscanf(fp, "%s %s %d", students[i].name, students[i].surname, &students[i].points) != 3) {
            printf("Error reading student data!\n");
            fclose(fp);
            return READ_ERROR;
        }    
    }

    fclose(fp);
    return SUCCESS;
}

int Print_Students(Student *students, int n){
    for(int i = 0; i < n; i++){
        double rel_points = (double)students[i].points / (double)MAX_POINTS * 100; //pretvorba u double jer su obe int varijable pa bi rez bia 0
        printf("Student details : %s %s %d\n", students[i].name, students[i].surname, students[i].points);
        printf("Student's relative points : %.2f\n", rel_points);
    }

    return SUCCESS;
}