#include <iostream>
#include <mutex>
#include "qtype.h"
#include "queue.h"



///�ʱ�ȭ/���� �Լ�
//������: �޸� �Ҵ� �� ��(lock) �ʱ�ȭ�� �̷������ ����.
//�ذ� ����: Queue ����ü �Ҵ� ��, head/tail �ʱ�ȭ �� ���ؽ� ���� �ʿ�.
//Queue ����ü �Ҵ� + head/tail �ʱ�ȭ + ���ؽ� ����
Queue* init(void) {
	Queue* queue = new Queue;
	queue->head = nalloc({ 0, nullptr }); //���� ���
	queue->tail = queue->head;
	new (&queue->lock) std::mutex(); //���ؽ� �ʱ�ȭ
	return queue;
}


//��� ��� �޸� ���� + ť ����ü ����
void release(Queue* queue) {
	Node* current = queue->head;
	while (current) {
		Node* temp = current;
		current = current->next;
		delete(temp);
	}
	queue->lock.~mutex(); //���ؽ� �ı�
	free(queue); //����
}


///��� ���� �Լ�(�޸� ����)
//������: ��� ����/����/���� ����� �������� ����.
//�ذ� ����: malloc/free�� �̿��� �޸� ���� �� ������ ���� ���� �߰�.
///�������� ������ ��� ����
Node* nalloc(Item item) {
	Node* new_node = new Node;
	new_node->item.key = item.key;

	//value_size ��� �޸� �Ҵ� �� ����
	new_node->item.value_size = item.value_size;
	new_node->item.value = malloc(item.value_size); //���� �Ҵ�
	memcpy(new_node->item.value, item.value, item.value_size); //���� ����

	new_node->next = nullptr;
	return new_node;
}

///��� �޸� ����
void nfree(Node* node) {
	free(node->item.value); //�߰��� �޸� ����
	delete node;
}

///��� ����(���� ����)
Node* nclone(Node* node) {
	if (!node) return nullptr;
	Node* new_node = nalloc(node->item);
	new_node->next = nclone(node->next);
	return new_node;
}


///�켱���� ť ����
//������: ������ ������Ƽ�� �����ϴ� ���� ���� ���� - �̰� �����ߵǴ±���
//�ذ� ����: ���ؽ� ��/����� �̿��� ���ü� ���� �� �켱���� ť ���� ����


///�ٽ� ť ���� (������ ������)
Reply enqueue(Queue* queue, Item item) {
	std::lock_guard<std::mutex> lock(queue->lock); //�� ȹ��

	//�� ��� ����
	Node* new_node = nalloc(item);

	//�켱���� Ž�� �� ���� ��ġ ����
	Node* current = queue->head;
	while (current->next && current->next->item.key < item.key) {
		current = current->next;
	}
	new_node->next = current->next;
	current->next = new_node;

	//�켱���� Ž��: (Key ��������)
	Node* prev = queue->head;
	while (prev->next && prev->next->item.key < item.key) {
		prev = prev->next;
	}

	//��� ����
	new_node->next = current->next;
	current->next = new_node;

	//tail ������Ʈ
	if (!new_node->next) queue->tail = new_node;

	return { true,item };
}

Reply dequeue(Queue* queue) {
	std::lock_guard<std::mutex> lock(queue->lock);

	//ť�� ������� Ȯ��
	if (queue->head->next == nullptr) {
		return{ false, {0, nullptr}, 1 };
	}

	//�ֿ켱���� ��� ���� (head->next)
	Node* target = queue->head->next;
	Reply reply = { true, target->item, 0 };

	//��� ����
	queue->head->next = target->next;

	//tail ������Ʈ (���� �� ť�� �� ���)
	if (!target->next) queue->tail = queue->head;

	//�޸� ����
	nfree(target);

	return reply;
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


