#include "AVL.h"
#include "hash_table.h"
#include <stdio.h>

Classroom *root = NULL;
HashTable *buildingHash = NULL;
void showMenu();
void importFromCSV(const char *filename);

int main()
{
    buildingHash = createHashTable();
    int choice;

    do
    {
        showMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            // ��ӽ���
            int id, capacity, is_scheduled;
            char building[MAX_BUILDING_NAME];
            char name[MAX_CLASSROOM_NAME];
            char type[20];

            printf("�������ID: ");
            scanf("%d", &id);
            printf("�����ѧ¥: ");
            scanf("%s", building);
            printf("�����������: ");
            scanf("%s", name);
            printf("�����������: ");
            scanf("%s", type);
            printf("��������: ");
            scanf("%d", &capacity);
            printf("�Ƿ��ſ�(1��/0��): ");
            scanf("%d", &is_scheduled);

            Classroom *newRoom = createClassroom(id, building, name,
                                                 type, capacity, is_scheduled);
            root = insertAVL(root, newRoom);
            addClassroomToBuilding(buildingHash, newRoom);
            printf("������ӳɹ���\n");
            break;
        }
        case 2:
        {
            // ���ҽ���
            int id;
            printf("����Ҫ���ҵĽ���ID: ");
            scanf("%d", &id);

            Classroom *room = findClassroomById(root, id);
            if (room)
            {
                printf("\n�ҵ�����:\n");
                printf("ID: %d\n��ѧ¥: %s\n����: %s\n����: %s\n����: %d\n",
                       room->id, room->building, room->name,
                       room->type, room->capacity);
            }
            else
            {
                printf("δ�ҵ�IDΪ%d�Ľ���\n", id);
            }
            break;
        }
        case 3:
        {
            // �鿴��ѧ¥�Ľ���
            char building[MAX_BUILDING_NAME];
            printf("�����ѧ¥����: ");
            scanf("%s", building);
            printClassroomsInBuilding(buildingHash, building);
            break;
        }
        case 4:
        {
            // ����CSV����
            printf("������CSV�ļ�·��: ");
            char filename[256];
            scanf("%s", filename);
            importFromCSV(filename);
            break;
        }
        case 0:
            printf("��лʹ�ã��ټ���\n");
            break;
        default:
            printf("��Ч��ѡ��������\n");
        }
    } while (choice != 0);

    // �ڴ��ͷ�
    destroyHashTable(buildingHash);
    destroyAVL(root);
    printf("�ڴ��������\n");

    return 0;
}