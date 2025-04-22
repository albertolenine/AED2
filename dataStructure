#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estrutura para representar um registro do CSV
typedef struct {
    int id;
    int idSolution;
    char solutionName[100];
    char solutionInitials[10];
    int idTeacher;
    char teacherName[100];
    int idDay;
    int idInstitution;
    int idUnit;
    char unitName[100];
    int idUnitCourse;
    int idCourse;
    char courseName[100];
    int idClass;
    char className[100];
    int idDiscipline;
    char disciplineName[100];
    int idRoom;
    char roomName[100];
    int studentsNumber;
    int sequence;
    int idBeginSlot;
    char beginTimeName[9]; // formato hh:mm:ss
    int idEndSlot;
    char endTimeName[9];   // formato hh:mm:ss
    int idYear;
    int idTerm;
    char idCollisionType[10]; // "--OK--", "--PRF--", etc.
    char collisionLevel[10];  // "OK-----", "PARCIAL", etc.
    int collisionSize;
} ScheduleRecord;

// Nó da árvore binária para detecção de colisões
typedef struct TreeNode {
    ScheduleRecord *record;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Estrutura para armazenar colisões encontradas
typedef struct {
    ScheduleRecord *record1;
    ScheduleRecord *record2;
    char collisionType[10];
    char collisionLevel[10];
    int collisionSize;
} Collision;

// Cria um novo nó da árvore
TreeNode* createNode(ScheduleRecord *record) {
    TreeNode *newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para novo nó.\n");
        exit(EXIT_FAILURE);
    }
    newNode->record = record;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insere um novo registro na árvore
TreeNode* insert(TreeNode *root, ScheduleRecord *record, bool (*compare)(ScheduleRecord*, ScheduleRecord*)) {
    if (root == NULL) {
        return createNode(record);
    }
    
    if (compare(record, root->record)) {
        root->left = insert(root->left, record, compare);
    } else {
        root->right = insert(root->right, record, compare);
    }
    
    return root;
}

// Percorre a árvore em ordem
void inOrderTraversal(TreeNode *root, void (*visit)(TreeNode*)) {
    if (root != NULL) {
        inOrderTraversal(root->left, visit);
        visit(root);
        inOrderTraversal(root->right, visit);
    }
}

// Libera a memória da árvore
void freeTree(TreeNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

// Verifica se dois intervalos de tempo se sobrepõem
bool timeOverlap(int begin1, int end1, int begin2, int end2) {
    return (begin1 < end2) && (begin2 < end1);
}

// Compara dois registros para verificar colisão de professor
bool teacherCollision(ScheduleRecord *r1, ScheduleRecord *r2) {
    return (r1->idTeacher == r2->idTeacher) && 
           (r1->idDay == r2->idDay) &&
           timeOverlap(r1->idBeginSlot, r1->idEndSlot, r2->idBeginSlot, r2->idEndSlot);
}

// Compara dois registros para verificar colisão de sala
bool roomCollision(ScheduleRecord *r1, ScheduleRecord *r2) {
    return (r1->idRoom == r2->idRoom) && 
           (r1->idDay == r2->idDay) &&
           timeOverlap(r1->idBeginSlot, r1->idEndSlot, r2->idBeginSlot, r2->idEndSlot);
}

// Compara dois registros para verificar colisão de professor e sala
bool teacherAndRoomCollision(ScheduleRecord *r1, ScheduleRecord *r2) {
    return teacherCollision(r1, r2) && roomCollision(r1, r2);
}

// Calcula o tamanho da colisão em minutos (considerando slots de 15 minutos)
int calculateCollisionSize(ScheduleRecord *r1, ScheduleRecord *r2) {
    int overlapStart = (r1->idBeginSlot > r2->idBeginSlot) ? r1->idBeginSlot : r2->idBeginSlot;
    int overlapEnd = (r1->idEndSlot < r2->idEndSlot) ? r1->idEndSlot : r2->idEndSlot;
    return (overlapEnd - overlapStart) * 15; // Cada slot representa 15 minutos
}

// Classifica o tipo e nível da colisão
void classifyCollision(ScheduleRecord *r1, ScheduleRecord *r2, Collision *collision) {
    bool teacherCol = teacherCollision(r1, r2);
    bool roomCol = roomCollision(r1, r2);
    
    // Determina o tipo de colisão
    if (teacherCol && roomCol) {
        strcpy(collision->collisionType, "PRF-SAL");
    } else if (teacherCol) {
        strcpy(collision->collisionType, "--PRF--");
    } else if (roomCol) {
        strcpy(collision->collisionType, "--SAL--");
    }
    
    // Determina o nível da colisão
    if (r1->idBeginSlot == r2->idBeginSlot && r1->idEndSlot == r2->idEndSlot) {
        strcpy(collision->collisionLevel, "TOTAL--");
    } else {
        strcpy(collision->collisionLevel, "PARCIAL");
    }
    
    // Calcula o tamanho da colisão
    collision->collisionSize = calculateCollisionSize(r1, r2);
}

// Constrói a árvore de colisões e retorna uma lista de colisões encontradas
Collision* buildCollisionTree(ScheduleRecord *records, int numRecords, int *numCollisions) {
    TreeNode *teacherRoot = NULL;
    TreeNode *roomRoot = NULL;
    Collision *collisions = malloc(numRecords * sizeof(Collision)); // Alocação máxima possível
    *numCollisions = 0;
    
    // Insere registros nas árvores e verifica colisões
    for (int i = 0; i < numRecords; i++) {
        // Verifica colisões com professores
        TreeNode *current = teacherRoot;
        while (current != NULL) {
            if (teacherCollision(&records[i], current->record)) {
                classifyCollision(&records[i], current->record, &collisions[*numCollisions]);
                collisions[*numCollisions].record1 = &records[i];
                collisions[*numCollisions].record2 = current->record;
                (*numCollisions)++;
            }
            current = (teacherCollision(&records[i], current->record)) ? current->left : current->right;
        }
        
        // Verifica colisões com salas
        current = roomRoot;
        while (current != NULL) {
            if (roomCollision(&records[i], current->record)) {
                classifyCollision(&records[i], current->record, &collisions[*numCollisions]);
                collisions[*numCollisions].record1 = &records[i];
                collisions[*numCollisions].record2 = current->record;
                (*numCollisions)++;
            }
            current = (roomCollision(&records[i], current->record)) ? current->left : current->right;
        }
        
        // Insere o registro nas árvores
        teacherRoot = insert(teacherRoot, &records[i], (bool (*)(ScheduleRecord*, ScheduleRecord*))teacherCollision);
        roomRoot = insert(roomRoot, &records[i], (bool (*)(ScheduleRecord*, ScheduleRecord*))roomCollision);
    }
    
    // Libera as árvores
    freeTree(teacherRoot);
    freeTree(roomRoot);
    
    return collisions;
}

// Lê o arquivo CSV e retorna um array de registros
ScheduleRecord* readCSV(const char *filename, int *numRecords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    // Conta o número de linhas (assumindo que a primeira linha é cabeçalho)
    int lines = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) lines++;
    rewind(file);
    
    *numRecords = lines - 1; // Desconta o cabeçalho
    if (*numRecords <= 0) {
        fclose(file);
        return NULL;
    }
    
    // Aloca memória para os registros
    ScheduleRecord *records = malloc(*numRecords * sizeof(ScheduleRecord));
    if (records == NULL) {
        fclose(file);
        fprintf(stderr, "Erro ao alocar memória para registros\n");
        exit(EXIT_FAILURE);
    }
    
    // Lê o cabeçalho e ignora
    fgets(buffer, sizeof(buffer), file);
    
    // Lê os registros
    for (int i = 0; i < *numRecords; i++) {
        fscanf(file, "%d,%d,%[^,],%[^,],%d,%[^,],%d,%d,%d,%[^,],%d,%d,%[^,],%d,%[^,],%d,%[^,],%d,%[^,],%d,%d,%d,%[^,],%d,%[^,],%d,%d,%[^,],%[^,],%d",
               &records[i].id, &records[i].idSolution, records[i].solutionName, records[i].solutionInitials,
               &records[i].idTeacher, records[i].teacherName, &records[i].idDay, &records[i].idInstitution,
               &records[i].idUnit, records[i].unitName, &records[i].idUnitCourse, &records[i].idCourse,
               records[i].courseName, &records[i].idClass, records[i].className, &records[i].idDiscipline,
               records[i].disciplineName, &records[i].idRoom, records[i].roomName, &records[i].studentsNumber,
               &records[i].sequence, &records[i].idBeginSlot, records[i].beginTimeName, &records[i].idEndSlot,
               records[i].endTimeName, &records[i].idYear, &records[i].idTerm, records[i].idCollisionType,
               records[i].collisionLevel, &records[i].collisionSize);
    }
    
    fclose(file);
    return records;
}

// Função para salvar os registros atualizados em um novo arquivo CSV
void saveUpdatedCSV(const char *filename, ScheduleRecord *records, int numRecords) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Erro ao criar o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    // Escreve o cabeçalho do CSV
    fprintf(file, "id,idSolution,solutionName,solutionInitials,idTeacher,teacherName,idDay,idInstitution,");
    fprintf(file, "idUnit,unitName,idUnitCourse,idCourse,courseName,idClass,className,idDiscipline,");
    fprintf(file, "disciplineName,idRoom,roomName,studentsNumber,sequence,idBeginSlot,beginTimeName,");
    fprintf(file, "idEndSlot,endTimeName,idYear,idTerm,idCollisionType,collisionLevel,collisionSize\n");
    
    // Escreve cada registro
    for (int i = 0; i < numRecords; i++) {
        fprintf(file, "%d,%d,%s,%s,%d,%s,%d,%d,%d,%s,%d,%d,%s,%d,%s,%d,%s,%d,%s,%d,%d,%d,%s,%d,%s,%d,%d,%s,%s,%d\n",
               records[i].id, records[i].idSolution, records[i].solutionName, records[i].solutionInitials,
               records[i].idTeacher, records[i].teacherName, records[i].idDay, records[i].idInstitution,
               records[i].idUnit, records[i].unitName, records[i].idUnitCourse, records[i].idCourse,
               records[i].courseName, records[i].idClass, records[i].className, records[i].idDiscipline,
               records[i].disciplineName, records[i].idRoom, records[i].roomName, records[i].studentsNumber,
               records[i].sequence, records[i].idBeginSlot, records[i].beginTimeName, records[i].idEndSlot,
               records[i].endTimeName, records[i].idYear, records[i].idTerm, records[i].idCollisionType,
               records[i].collisionLevel, records[i].collisionSize);
    }
    
    fclose(file);
    printf("Arquivo atualizado salvo com sucesso: %s\n", filename);
}

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