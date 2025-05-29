#pragma once
#include <mutex>

typedef int Key;
typedef void* Value;

typedef struct {
	Key key;
	Value value;
} Item;

typedef struct {
	bool success;   // true: ����, false: ����
	Item item;
	int error_code; // �ʵ� �߰�
	} Reply;

typedef struct node_t {
	Item item;
	struct node_t* next;
	// �ʵ��߰�����
} Node;

typedef struct {
	Node* head, * tail;
	std::mutex lock; // ���ؽ� �߰�
	size_t count; //�� ����
} Queue;
//���� ��� ��� �߰�: head ->  next�� ù ���� ����� ���� ���� ����