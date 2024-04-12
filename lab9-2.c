#include <stdio.h>
#include <stdlib.h>

struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

struct Node
{
    struct RecordType data;
    struct Node* next;
};

struct HashType
{
    struct Node** table;
    int size;
};

int hash(int x)
{
    return x % 10; // Simple hash function for demonstration, modify based on your needs
}

void initHashTable(struct HashType* hashTable, int size)
{
    hashTable->table = (struct Node**) calloc(size, sizeof(struct Node*));
    hashTable->size = size;
}

void insert(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id);
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = record;
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
}

void displayRecordsInHash(struct HashType* hashTable)
{
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct Node* current = hashTable->table[i];
        if (current != NULL)
        {
            printf("Index %d -> ", i);
            while (current != NULL)
            {
                printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d\n", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void freeHashTable(struct HashType* hashTable)
{
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct Node* current = hashTable->table[i];
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->table);
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType hashTable;
    initHashTable(&hashTable, 10); 
    for (int i = 0; i < recordSz; i++)
    {
        insert(&hashTable, pRecords[i]);
    }

    displayRecordsInHash(&hashTable);

    freeHashTable(&hashTable);
    free(pRecords);

    return 0;
}
