#include <iostream>
#include "queue.h"


//������: �޸� �Ҵ� �� ��(lock) �ʱ�ȭ�� �̷������ ����.
//�ذ� ����: Queue ����ü �Ҵ� ��, head/tail �ʱ�ȭ �� ���ؽ� ���� �ʿ�.
Queue* init(void) {
	return NULL; //ť ����ü �Ҵ� �� �ʱ�ȭ �ʿ�
}


void release(Queue* queue) {
	return; //�޸� ���� ���� ����
}

//������: ��� ����/����/���� ����� �������� ����.
//�ذ� ����: malloc/free�� �̿��� �޸� ���� �� ������ ���� ���� �߰�.
Node* nalloc(Item item) {
	//Node ����, item���� �ʱ�ȭ
	return NULL; //��� ���� ���� ����
}


void nfree(Node* node) {
	return; //�޸� ���� ����
}


Node* nclone(Node* node) {
	return NULL; //��� ���� ��� �̱���
}

//