#pragma once
#include "qtype.h"

// ť �ʱ�ȭ, ����
Queue* init(void); //ť �ʱ�ȭ
void release(Queue* queue); //ť �޸� ����

//��� ����&�ʱ�ȭ, ����, ����
Node* nalloc(Item item);
void nfree(Node* node);
Node* nclone(Node* node);

//�ٽ� ť ����
Reply enqueue(Queue* queue, Item item); //������ ����
Reply dequeue(Queue* queue); //������ ����
Queue* range(Queue* queue, Key start, Key end); //Ư�� ������ ������ ��ȯ
//������: ������ ������Ƽ�� �����ϴ� ���� ���� ����.
//�ذ� ����: ���ؽ� ��/����� �̿��� ���ü� ���� �� �켱���� ť ���� ����.

