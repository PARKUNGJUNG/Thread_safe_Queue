#include <iostream>
#include <mutex>
#include "queue.h"


///�ʱ�ȭ/���� �Լ�
//������: �޸� �Ҵ� �� ��(lock) �ʱ�ȭ�� �̷������ ����.
//�ذ� ����: Queue ����ü �Ҵ� ��, head/tail �ʱ�ȭ �� ���ؽ� ���� �ʿ�.
Queue* init(void) {
	return NULL; //ť ����ü �Ҵ� �� �ʱ�ȭ �ʿ�
}


//ť ����
void release(Queue* queue) {
	Node* current = queue->head;
	while (current != NULL) {
		Node* temp = current;
		current = current->next;
		free(temp);
	}
	free(queue);
}


///��� ���� �Լ�(�޸� ����)
//������: ��� ����/����/���� ����� �������� ����.
//�ذ� ����: malloc/free�� �̿��� �޸� ���� �� ������ ���� ���� �߰�.
///��� ����, item���� �ʱ�ȭ
Node* nalloc(Item item) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->item = item;
	new_node->next = NULL;
	return new_node;
}

///��� ����
void nfree(Node* node) {
	return;
}

///��� ����
Node* nclone(Node* node) {
	return NULL;
}


///�켱���� ť ����
//������: ������ ������Ƽ�� �����ϴ� ���� ���� ���� - �̰� �����ߵǴ±���
//�ذ� ����: ���ؽ� ��/����� �̿��� ���ü� ���� �� �켱���� ť ���� ����
///���� �� ���� ����
Node* current = queue->head;
while (current->next != NULL && current->next->item.key < item.key) {
	current = current->next;
}
new_node->next = current->next;
current->next = new_node;

///�켱���� ����
Node* target = queue->head->next; //head�� ���� ��� ����
queue->head->next = target->next;
return target->item;

///�ٽ� ť ����
Reply enqueue(Queue* queue, Item item) {
	std::lock_guard <std::mutex> guard(queue->lock);
	//ũ��Ƽ�� ���� ����
}

Reply dequeue(Queue* queue) {
	//�̱���
}

Queue* range(Queue* queue, Key start, Key end) {
	//�̱���
}

///�޸���
/// ����: ��Ƽ������ ȯ�濡�� �����ϰ� �����ϴ�(������ ������) �켱���� ť(priority queue)�� ���� ����
//���� �����尡 ���ÿ� �����ص� �����Ͱ� �ջ���� �ʴ� thread-safe �켱���� ť�� ����.
//Ű ���� 0 <= key < 10,000,000���� �����Ǿ� ��뷮 ������ ó�� �ɷ� ��.
//������ ũ��: 1����Ʈ ~ 1KB���� �����Ͽ� �޸� ���� ȿ���� �׽�Ʈ.

//qtype.h: �ڷᱸ�� ����, ���� �� ����
//queue.cpp: ť�� ���� ����(�Լ�)����, ����
//queue.h: �Լ� ���� ����, ���� �� ���� �Ұ�
//main.c: �׽�Ʈ�� �ڵ�, ���� ���ʿ�

//���� �ܰ� ���
//qtype.h ����: Queue ����ü�� ���ؽ� �ʵ� �߰�
//init / release ���� : �޸� �Ҵ� / ���� �� �� �ʱ�ȭ / �ı�
//��� ���� �Լ� �ϼ� : nalloc, nfree, nclone
//enqueue / dequeue ���� �ۼ� : �켱���� ���� + �� ��ȣ
//range �Լ� ���� : Ű ���� �˻�(�߰� �� �ʿ�)
//���� �׽�Ʈ : �پ��� ������ ���� ��ũ�ε�� ����

///qtype.h
//Item, Reply, Node, Queue ����ü qtype.h ���� ���� �� ���� ����.

///main.c
//��Ƽ������ ȯ�濡�� �켱���� ť�� ������ �����ϱ� ���� �⺻ �����ӿ�ũ.
//


