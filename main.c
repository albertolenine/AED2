#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <C:\SI\AED2\AED2\dataStructure.c>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    int numRecords = 0;
    ScheduleRecord *records = readCSV(argv[1], &numRecords);
    
    if (records == NULL || numRecords == 0) {
        printf("Nenhum registro encontrado no arquivo.\n");
        return EXIT_FAILURE;
    }
    
    int numCollisions = 0;
    Collision *collisions = buildCollisionTree(records, numRecords, &numCollisions);
    
    // Imprime as colisões encontradas
    printf("Colisões encontradas: %d\n", numCollisions);
    for (int i = 0; i < numCollisions; i++) {
        printf("Colisão %d:\n", i+1);
        printf("  Aula 1: %s - %s (%s %s)\n", 
               collisions[i].record1->disciplineName, collisions[i].record1->teacherName,
               collisions[i].record1->beginTimeName, collisions[i].record1->endTimeName);
        printf("  Aula 2: %s - %s (%s %s)\n", 
               collisions[i].record2->disciplineName, collisions[i].record2->teacherName,
               collisions[i].record2->beginTimeName, collisions[i].record2->endTimeName);
        printf("  Tipo: %s, Nível: %s, Duração: %d minutos\n\n",
               collisions[i].collisionType, collisions[i].collisionLevel, collisions[i].collisionSize);
    }
    
    // Atualiza os registros com as colisões encontradas
    for (int i = 0; i < numCollisions; i++) {
        // Atualiza o primeiro registro da colisão
        strcpy(collisions[i].record1->idCollisionType, collisions[i].collisionType);
        strcpy(collisions[i].record1->collisionLevel, collisions[i].collisionLevel);
        collisions[i].record1->collisionSize = collisions[i].collisionSize;
        
        // Atualiza o segundo registro da colisão
        strcpy(collisions[i].record2->idCollisionType, collisions[i].collisionType);
        strcpy(collisions[i].record2->collisionLevel, collisions[i].collisionLevel);
        collisions[i].record2->collisionSize = collisions[i].collisionSize;
    }
    
    // Gera o nome do arquivo de saída
    char outputFilename[256];
    strncpy(outputFilename, argv[1], sizeof(outputFilename));
    char *dot = strrchr(outputFilename, '.');
    if (dot != NULL) *dot = '\0'; // Remove a extensão se existir
    strcat(outputFilename, "_com_colisoes.csv");
    
    // Salva os registros atualizados em um novo arquivo CSV
    saveUpdatedCSV(outputFilename, records, numRecords);
    
    free(collisions);
    free(records);
    
    return EXIT_SUCCESS;
}