#include <stdio.h>
#include <stdlib.h>

struct RecordType {
    int id;
    char name;
    int order;
};

struct Node {
    struct RecordType data;
    struct Node* next;
};

struct HashType {
    struct Node** table;
    int size;
};

int hash(int x, int tableSize) {
    return x % tableSize;
}

void initHashTable(struct HashType* hashTable, int size) {
    hashTable->size = size;
    hashTable->table = (struct Node**)malloc(size * sizeof(struct Node*));
    for (int i = 0; i < size; i++) {
        hashTable->table[i] = NULL;
    }
}

void insert(struct HashType* hashTable, struct RecordType record) {
    int index = hash(record.id, hashTable->size);
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = record;
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
}

void displayRecordsInHash(struct HashType* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        printf("Index %d -> ", i);
        struct Node* current = hashTable->table[i];
        while (current != NULL) {
            printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}

int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    if (!inFile) {
        printf("Failed to open the file\n");
        return 0; 
    }

    int dataSz = 0;
    fscanf(inFile, "%d", &dataSz);
    *ppData = (struct RecordType*)malloc(dataSz * sizeof(struct RecordType));
    for (int i = 0; i < dataSz; i++) {
        fscanf(inFile, "%d %c %d", &((*ppData)[i].id), &((*ppData)[i].name), &((*ppData)[i].order));
    }

    fclose(inFile);
    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz) {
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; i++) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n");
}

void freeHashTable(struct HashType* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        struct Node* current = hashTable->table[i];
        while (current != NULL) {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->table);
}

int main(void) {
    struct RecordType* pRecords = NULL;
    int recordSz = parseData("input.txt", &pRecords);
    if (recordSz > 0) {
        printRecords(pRecords, recordSz);

        struct HashType hashTable;
        initHashTable(&hashTable, 10);
        for (int i = 0; i < recordSz; i++) {
            insert(&hashTable, pRecords[i]);
        }

        displayRecordsInHash(&hashTable);
        freeHashTable(&hashTable);
    } else {
        printf("No records found or file couldn't be read.\n");
    }

    free(pRecords);
    return 0;
}
