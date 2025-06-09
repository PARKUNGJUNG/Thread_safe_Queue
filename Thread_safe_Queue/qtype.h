#pragma once
#include <mutex>

typedef int Key;
typedef void* Value;

typedef struct { //Item ����ü: �켱���� �׸�
	Key key;
	Value value;
	int value_size; //byte
} Item;

typedef struct { //Reply ����ü: ���� ���
	bool success;   // true: ����, false: ����
	Item item;
	int error_code; // �ʵ� �߰�
	} Reply;

typedef struct node_t { //Node ����ü: ���� ����Ʈ ���
	Item item;
	struct node_t* next;
	// �ʵ��߰�����
} Node;

typedef struct { //Queue ����ü: ť ��Ÿ������
	Node* head, * tail;
	std::mutex lock; // ���ؽ� �߰�, ������ ������ ���� ���ؽ�
	size_t count; //�� ����
} Queue;
//���� ��� ��� �߰�: head ->  next�� ù ���� ����� ���� ���� ����