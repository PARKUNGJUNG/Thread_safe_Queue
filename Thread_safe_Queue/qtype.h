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
} Node;


//Queue ����ü �� ����� �ʱ�ȭ
typedef struct Queue {
	Node* head = nullptr;
	Node* tail = nullptr;
	std::mutex lock;
	size_t count = 0;
} Queue;
//���� ��� ��� �߰�: head ->  next�� ù ���� ����� ���� ���� ����