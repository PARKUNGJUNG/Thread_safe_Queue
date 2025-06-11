#include <iostream>
#include <mutex>
#include <cstring>
#include "qtype.h"
#include "queue.h"


///�ʱ�ȭ/���� �Լ�
//������: �޸� �Ҵ� �� ��(lock) �ʱ�ȭ�� �̷������ ����.
//�ذ� ����: Queue ����ü �Ҵ� ��, head/tail �ʱ�ȭ �� ���ؽ� ���� �ʿ�.
//Queue ����ü �Ҵ� + head/tail �ʱ�ȭ + ���ؽ� ����
Queue* init(void) {
	Queue* queue = new Queue;
	queue->head = new Node{ {0, nullptr, 0}, nullptr }; //���� ���
	queue->tail = queue->head;
	new (&queue->lock) std::mutex(); //���ؽ� �ʱ�ȭ
	queue->count = 0; //����� �ʱ�ȭ
	return queue;
}


//��� ��� �޸� ���� + ť ����ü ����
void release(Queue* queue) {
	Node* current = queue->head;
	while (current) {
		Node* temp = current;
		current = current->next;
		nfree(temp);
	}
	queue->lock.~mutex(); //���ؽ� �ı�
	delete(queue); //����
}


///��� ���� �Լ�(�޸� ����)
//������: ��� ����/����/���� ����� �������� ����.
//�ذ� ����: malloc/free�� �̿��� �޸� ���� �� ������ ���� ���� �߰�.
///�������� ������ ��� ����
Node* nalloc(Item item) {
	Node* new_node = new Node;
	new_node->item.key = item.key;
	new_node->item.value_size = item.value_size;

	//Null �� ũ�� üũ
	if (item.value != nullptr && item.value_size > 0) {
		new_node->item.value = malloc(item.value_size);
		if (new_node->item.value != nullptr) {
			memcpy(new_node->item.value, item.value, item.value_size);
		}
	}
	else {
		new_node->item.value = nullptr;
	}

	new_node->next = nullptr;
	return new_node;
}

///��� �޸� ����
void nfree(Node* node) {
	if (node != nullptr && node->item.value != nullptr) {
		free(node->item.value);
	}
	delete node;
}

///��� ����(���� ����)
Node* nclone(Node* node) {
	if (node == nullptr) return nullptr;
	Node* new_node = nalloc(node->item);
	if (new_node != nullptr) {
		new_node->next = nclone(node->next);
	}
	return new_node;
}


///�켱���� ť ����
//������: ������ ������Ƽ�� �����ϴ� ���� ���� ���� - �̰� �����ߵǴ±���
//�ذ� ����: ���ؽ� ��/����� �̿��� ���ü� ���� �� �켱���� ť ���� ����
///�ٽ� ť ���� (������ ������)
Reply enqueue(Queue* queue, Item item) {
	std::lock_guard<std::mutex> lock(queue->lock); //�� ȹ��

	//�� ��� ����
	Node* prev = queue->head;
	while (prev->next != nullptr) {
		if (prev->next->item.key == item.key) {
			// ���� �� ��ü
			if (prev->next->item.value != nullptr) {
				free(prev->next->item.value);
			}
			prev->next->item.value_size = item.value_size;

			if (item.value != nullptr && item.value_size > 0) {
				prev->next->item.value = malloc(item.value_size);
				if (prev->next->item.value != nullptr) {
					memcpy(prev->next->item.value, item.value, item.value_size);
				}
			}
			else {
				prev->next->item.value = nullptr;
			}
			return { true, item, 0 };
		}
		if (prev->next->item.key > item.key) break;
		prev = prev->next;
	}

	//�� ��� ����
	Node* new_node = nalloc(item);
	new_node->next = prev->next;
	prev->next = new_node;

	if (new_node->next == nullptr) {
		queue->tail = new_node;
	}
	queue->count++;

	return { true, item, 0 };
}

Reply dequeue(Queue* queue) {
	std::lock_guard<std::mutex> lock(queue->lock);

	//ť�� ������� Ȯ��
	if (queue->head->next == nullptr) {
		//��� �ʵ� ����� �ʱ�ȭ
		Reply reply;
		reply.success = false;
		reply.item.key = 0;
		reply.item.value = nullptr;
		reply.item.value_size = 0;
		reply.error_code = 1;
		return reply;
	}

	//�ֿ켱���� ��� ���� (head->next)
	Node* target = queue->head->next;
	Reply reply;
	reply.success = true;
	reply.item = target->item; // target->item�� �̹� �ʱ�ȭ��
	reply.error_code = 0;

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


