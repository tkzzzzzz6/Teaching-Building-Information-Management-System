#ifndef AVL_H
#define AVL_H

#define MAX_BUILDING_NAME 50  // ��ѧ¥������󳤶�
#define MAX_CLASSROOM_NAME 50 // ����������󳤶�

// ͷ�ļ��г�����ѭ������
struct HashTable;

// ���ҽڵ�ṹ
typedef struct Classroom
{
    int id;                           // ����ID
    char building[MAX_BUILDING_NAME]; // ��ѧ¥
    char name[MAX_CLASSROOM_NAME];    // ��������
    char type[20];                    // �������ͣ���ý��/ʵ���ҵȣ�
    int capacity;                     // ����
    int is_scheduled;                 // �Ƿ��ſ�
    int height;                       // AVL���ڵ�߶�
    struct Classroom *left;           // ���ӽڵ�
    struct Classroom *right;          // ���ӽڵ�
} Classroom;

// ȫ�ֱ�������
extern Classroom *root;
extern HashTable *buildingHash;

// AVL����غ�������
Classroom *createClassroom(int id, const char *building, const char *name, const char *type, int capacity, int is_scheduled);
Classroom* insertClassroom(Classroom* node, Classroom* newRoom);
void addClassroomToBuilding(HashTable* ht, Classroom* room);
Classroom* findClassroomById(Classroom* node, int id);
void printClassroomsInBuilding(HashTable* ht, const char* building);
void showMenu();
void importFromCSV(const char* filename);
int getHeight(Classroom* node);
int getBalanceFactor(Classroom* node);
void updateHeight(Classroom* node);
Classroom* rightRotate(Classroom* y);
Classroom* leftRotate(Classroom* x);
Classroom* insertAVL(Classroom* node, Classroom* newRoom);
Classroom* findMin(Classroom* node);
Classroom* deleteAVL(Classroom* root, int id);
void destroyAVL(Classroom *node);
void printUnscheduledClassrooms(HashTable *ht);
#endif 