#pragma once
#include <mutex>

typedef int Key;
typedef void* Value;


//Item ����ü: �켱���� �׸�
typedef struct { //����: ť�� ����� �� �׸��� Ű(�켱����)�� ���� ����
	Key key; //�켱���� ����
	Value value; //���� ������
	int value_size; //byte
} Item;


//Reply ����ü: ���� ���
typedef struct {
	bool success;   //���� ���� ����
	Item item; //��ȯ�� ������
	int error_code; //���� ����
} Reply;


//��� ����ü: ���� ����Ʈ ���
typedef struct node_t { //Node ����ü: ���� ����Ʈ ���
	Item item;
	struct node_t* next;
	struct node_t* prev; //������ Ž�� ���� //�ʱ�� �ܹ��� ���Ḹ ��������, �켱���� �˻� ��ȿ����.
	int priorty; //Ű ĳ��(���� ���)
} Node;


//Queue ����ü: ť ��Ÿ������
typedef struct {
	Node* head, * tail;
	std::mutex lock; // ���ؽ� �߰�, ������ ������ ���� ���ؽ�
	size_t count; //���� ������ ��(���� ����͸�)
} Queue;
//���� ��� ��� �߰�: head ->  next�� ù ���� ����� ���� ���� ����