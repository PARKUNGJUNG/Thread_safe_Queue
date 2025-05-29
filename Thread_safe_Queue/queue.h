#pragma once
#include "qtype.h"

// ť �ʱ�ȭ, ����
Queue* init(void);
void release(Queue* queue);

//��� ����&�ʱ�ȭ, ����, ����
Node* nalloc(Item item);
void nfree(Node* node);
Node* nclone(Node* node);

Reply enqueue(Queue* queue, Item item);
Reply dequeue(Queue* queue);
Queue* range(Queue* queue, Key start, Key end);