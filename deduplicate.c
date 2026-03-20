#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 30007 // Tamaño primo para manejar hasta 20k-30k registros
#define MAX_LINE 1024

typedef struct Node {
    char id[20];
    struct Node* next;
} Node;

unsigned int hash_function(char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_SIZE;
}

bool is_duplicate_and_insert(Node** table, char* id) {
    unsigned int index = hash_function(id);
    Node* current = table[index];
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) return true;
        current = current->next;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->id, id);
    newNode->next = table[index];
    table[index] = newNode;
    return false;
}

int main(int argc, char *argv[]) {
    const char *filename = (argc > 1) ? argv[1] : "transaction_data.csv";
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: No se pudo abrir el archivo '%s'.\n", filename);
        return 1;
    }

    Node* hashTable[HASH_SIZE] = {NULL};
    char line[MAX_LINE];
    int total = 0, uniques = 0;

    // Saltar la primera línea (cabecera: transaction_id,amount)
    fgets(line, MAX_LINE, file);

    printf("Procesando archivo...\n");

    while (fgets(line, MAX_LINE, file)) {
        total++;
        // Usamos strtok para obtener el ID antes de la coma
        char *id = strtok(line, ",");
        char *amount_str = strtok(NULL, ",");

        if (id && !is_duplicate_and_insert(hashTable, id)) {
            uniques++;
            
            printf("Unico: %s, Monto: %s", id, amount_str);
        }
    }

    fclose(file);

    printf("\n--- Reporte de Ejecucion (C) ---\n");
    printf("Total de registros leidos: %d\n", total);
    printf("Registros unicos encontrados: %d\n", uniques);
    printf("Duplicados eliminados: %d\n", total - uniques);

    return 0;
}