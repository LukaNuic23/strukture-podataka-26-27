#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define SUCCESS 0
#define ALLOCATION_ERROR -1
#define NULL_POINTER_ERROR -2
#define PERSON_NOT_FOUND -3
#define FILE_ERROR -4

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
int FreeList(Position p);
int FindPrevious(Position p, char* surname, Position *result);
int InsertBefore(Position p, Position newPerson, char* surname);
int WriteToFile(char* filename, Position p);
int ReadFromFile(char* filename, Position p);
int SortList(Position p);
int InsertAfterPerson(Position p, Position newPerson, char* surname);



int main() {
    Person head = { 0 };
    Position newPerson = NULL;
    int status = 0;

    status = CreateNode("Ivo", "Ivic", 2003, &newPerson);
    if (status != SUCCESS) {
        return status;
    }
    status = InsertAfter(&head, newPerson);
    if (status != SUCCESS) {
        free(newPerson);
        return status;
    }

    status = CreateNode("Ana", "Anic", 2002, &newPerson);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    status = InsertAfter(&head, newPerson);
    if (status != SUCCESS) {
        free(newPerson);
        FreeList(&head);
        return status;
    }

    status = CreateNode("Marko", "Maric", 2004, &newPerson);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    status = InsertLast(&head, newPerson);
    if (status != SUCCESS) {
        free(newPerson);
        FreeList(&head);
        return status;
    }

    status = CreateNode("Petar", "Babic", 2001, &newPerson);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    status = InsertLast(&head, newPerson);
    if (status != SUCCESS) {
        free(newPerson);
        FreeList(&head);
        return status;
    }

    printf("List:\n");
    PrintList(&head);

    
    status = CreateNode("Luka", "Lukic", 2000, &newPerson);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    status = InsertAfterPerson(&head, newPerson, "Anic");
    if (status == PERSON_NOT_FOUND) {
        printf("Person not found\n");
        free(newPerson);
    }
    else if (status != SUCCESS) {
        free(newPerson);
        FreeList(&head);
        return status;
    }
    printf("\nList after insert after Anic:\n");
    PrintList(&head);

    
    status = CreateNode("Jure", "Juric", 1999, &newPerson);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    status = InsertBefore(&head, newPerson, "Ivic");
    if (status == PERSON_NOT_FOUND) {
        printf("Person not found\n");
        free(newPerson);
    }
    else if (status != SUCCESS) {
        free(newPerson);
        FreeList(&head);
        return status;
    }
    printf("\nList after insert before Ivic:\n");
    PrintList(&head);

    
    status = DeletePerson(&head, "Maric");
    if (status == PERSON_NOT_FOUND) {
        printf("Person not found\n");
    }
    else if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    printf("\nList after delete Maric:\n");
    PrintList(&head);

    
    status = SortList(&head);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    printf("\nSorted list:\n");
    PrintList(&head);


    status = WriteToFile("persons.txt", &head);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }

    FreeList(&head);
    printf("\nList after FreeList:\n");
    PrintList(&head);

    
    status = ReadFromFile("persons.txt", &head);
    if (status != SUCCESS) {
        FreeList(&head);
        return status;
    }
    printf("\nList read from file:\n");
    PrintList(&head);

    FreeList(&head);
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
    Position prev = NULL;
    int status = 0;
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }

    status = FindPrevious(p, surname, &prev);

    if(status != SUCCESS){
        return status;
    }
    
    toDelete = prev->next;
        prev->next = toDelete->next;
        free(toDelete);

    return SUCCESS;
}

int FreeList(Position p){
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }

    Position temp = NULL;
    Position current = NULL;
    current = p->next; //ode p ostaje samo da zapanti adresu heada kako bi na kraju funkcije postavili head->next da je NULL
    
    while(current != NULL){
        temp = current->next;
        free(current);
        current = temp;
    }
    p->next = NULL;

    return SUCCESS;
}

int FindPrevious(Position p, char* surname,Position *result){
    if(p == NULL){
        printf("Invalid position\n");
        return NULL_POINTER_ERROR;
    }

    while (p->next != NULL && strcmp(p->next->surname, surname) != 0 ){
        p = p->next;
    }
    if(p->next == NULL){
        return PERSON_NOT_FOUND;
    }

    else{
        *result = p;
    }

    return SUCCESS;
}

int InsertBefore(Position p, Position newPerson, char* surname){
    if(p == NULL || newPerson == NULL){
        return NULL_POINTER_ERROR;
    }
    int status = 0;
    Position prev = NULL;

    status = FindPrevious(p, surname, &prev);

    if(status != SUCCESS){
        return status;
    }

    status = InsertAfter(prev, newPerson);

    if(status != SUCCESS){
        return status;
    }

    return SUCCESS;
}

int WriteToFile(char* filename, Position p){
    if(p == NULL || filename == NULL){
        return NULL_POINTER_ERROR;
    }
    FILE* fp = NULL;
    p = p->next;

    fp = fopen(filename, "w");

    if(!fp){
        printf("Unable to open file\n");
        return FILE_ERROR;
    }

    while(p != NULL){
        fprintf(fp, "%s %s %d\n", p->name, p->surname, p->birthyear);
        p = p->next;
    }

    fclose(fp);
    return SUCCESS;
}

int ReadFromFile(char* filename, Position p){
    if(p == NULL || filename == NULL){
        return NULL_POINTER_ERROR;    
    }
    char name[MAX_NAME] = {0};
    char surname[MAX_NAME] = {0};
    int birthyear = 0;
    Position adr = NULL;
    Position last = p;
    int status = 0;
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    if(!fp){
        printf("Error opening file\n");
        return FILE_ERROR;
    }

    while(last->next != NULL){
        last = last->next;
    } /* optimizira brzinu rada ako datoteka ima bas puno osoba, 
    za mali broj nepotrebno medutim ako ih je tipa 10000 znacit ce da ne mora uvijek prolaziti
    od pocetka pa kroz cijelu listu dok dodaje vec samo dodavati iza zadnjeg elementa */

    while(fscanf(fp, "%s %s %d", name, surname, &birthyear) == 3){
        status = CreateNode(name, surname, birthyear, &adr);
        if(status != SUCCESS){
            fclose(fp);
            return status;
        }
        status = InsertAfter(last, adr);
        if(status != SUCCESS){
            free(adr);
            fclose(fp);
            return status;
        }
        last = adr; 
    }

    fclose(fp);
    return SUCCESS;
}

int SortList(Position p){
    if(p == NULL){
        return NULL_POINTER_ERROR;
    }
    int status = 0;
    Person newHead = {0};
    Position temp = NULL;
    Position pos = NULL;

    while(p->next != NULL){
        temp = p->next;
        p->next = temp->next;
        pos = &newHead;
        while (pos->next != NULL && strcmp(pos->next->surname, temp->surname) < 0 ){
            pos = pos->next;
    }
    status = InsertAfter(pos, temp);
    if(status != SUCCESS){
        return status;
    }
    }
    p->next = newHead.next;

    return SUCCESS;
}

int InsertAfterPerson(Position p, Position newPerson, char* surname){
    if(p == NULL || newPerson == NULL){
        return NULL_POINTER_ERROR;
    }
    int status = 0;
    Position found = NULL;

    status = FindPerson(p, surname, &found);

    if(status != SUCCESS){
        return status;
    }

    status = InsertAfter(found, newPerson);

    if(status != SUCCESS){
        return status;
    }

    return SUCCESS;
}