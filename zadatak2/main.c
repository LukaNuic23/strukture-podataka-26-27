#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define SUCCESS 0
#define ALLOCATION_ERROR -1
#define NULL_POINTER_ERROR -2
#define PERSON_NOT_FOUND -3

#define MAX_NAME 50

struct _person; //najava
typedef struct _person* Position; //pokazivac na strukturu

typedef struct _person {
    char name[MAX_NAME];
    char surname[MAX_NAME];
    int birthyear;
    Position next;
} Person;

int InsertAfter(Position p, Position newPerson);
int CreateNode(char* name, char* surname, int birthyear, Position* result);
int PrintList(Position p);
int InsertLast(Position p, Position newPerson);
int FindPerson(Position p, char* surname, Position *result);
int DeletePerson(Position p, char* surname);


int main() {
    Person head = { 0 };
    Position newPerson = NULL;
    Position found = NULL;
    int status = 0;
    
    status = CreateNode("Ivo", "Ivic", 2003, &newPerson);
    if (status != SUCCESS) {
        return status;
    }
    status = InsertAfter(&head, newPerson);
    if (status != SUCCESS) {
        return status;
    }

    status = CreateNode("Ana", "Anic", 2002, &newPerson);
    if (status != SUCCESS) {
        return status;
    }
    status = InsertAfter(&head, newPerson);
    if (status != SUCCESS) {
        return status;
    }

    status = CreateNode("Marko", "Maric", 2004, &newPerson);
    if (status != SUCCESS) {
        return status;
    }
    status = InsertLast(&head, newPerson);
    if (status != SUCCESS) {
        return status;
    }

    printf("List:\n");
    PrintList(&head);

    status = FindPerson(&head, "Anic", &found);
    if (status == SUCCESS) {
        printf("Found: %s %s %d\n", found->name, found->surname, found->birthyear);
    }
    else if (status == PERSON_NOT_FOUND) {
        printf("Person not found\n");
    }
    else {
        return status;
    }

    status = DeletePerson(&head, "Anic");
    if (status == PERSON_NOT_FOUND) {
        printf("Person not found\n");
    }
    else if (status != SUCCESS) {
        return status;
    }

    printf("List after delete:\n");
    PrintList(&head);

    DeletePerson(&head, "Ivic");
    DeletePerson(&head, "Maric");

    return SUCCESS;
}

int InsertAfter(Position p, Position newPerson) {
    if (p==NULL || newPerson==NULL){
        printf("Invalid position!\n");
        return NULL_POINTER_ERROR;
    }

    newPerson->next = p->next;
    p->next = newPerson;

    return SUCCESS;
}

int CreateNode(char* name, char* surname, int birthyear, Position* result){
    if(result == NULL){
        printf("Invalid argument\n");
        return NULL_POINTER_ERROR;
    }

    Position person = malloc(sizeof(Person));
    if(!person){
        printf("Memory allocation error\n");
        return ALLOCATION_ERROR;
    }
    
    strcpy(person->name, name);
    strcpy(person->surname, surname);
    person->birthyear = birthyear;

    person->next = NULL;
    *result = person;

    return SUCCESS;
}

int PrintList(Position p){
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }
    if(p->next == NULL){
        printf("Empty list\n");
        return SUCCESS; //da funkcija ne triba dalje razmisljat i ic dalje bezveze
    }

    p = p->next;

    while(p != NULL){
        printf("%s %s %d\n", p->name, p->surname, p->birthyear);
        p = p->next;
    }

    return SUCCESS;
}


int InsertLast(Position p, Position newPerson){
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }

    while(p->next != NULL){
        p = p->next;
    }

    return InsertAfter(p, newPerson);
}

int FindPerson(Position p, char* surname, Position* result){
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }

    p = p->next;

    while(p != NULL && strcmp(p->surname, surname) != 0){
        p = p->next;
    }

    if(p == NULL){
        return PERSON_NOT_FOUND;
    }
    else{
        *result = p;
    }

    return SUCCESS;
}


int DeletePerson(Position p, char* surname){
    Position toDelete = NULL;
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }

    while(p->next != NULL && strcmp(p->next->surname, surname) != 0){
        p = p->next;
    }

    if(p->next == NULL){
        return PERSON_NOT_FOUND;
    }

    else{
        toDelete = p->next;
        p->next = toDelete->next;
        free(toDelete);
    }

    return SUCCESS;
}