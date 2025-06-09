#include "qtype.h"
#include "queue.h"
#include <stdio.h>
#include <iostream>
#include <time.h>

int main(void) {
	srand((unsigned int)time(NULL)); //�������� ������ srand.

	//��ũ�ε� ����(GETRANGE�� �н�)
	Request requests[REQUEST_PER_CLINET]; //Ŭ���̾�Ʈ �����尡 ������ �۾��� ����. //requests �迭: Ŭ���̾�Ʈ �����尡 ������ �۾�(SET/GET)�� ����.
	//��ũ�ε� �й�
	for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
		requests[i].op = SET; //��ü ��û�� ������ ����
		requests[i].item.key = i; //key�� ������
		requests[i].item.value = (void*)(rand() % 1000000); //value�� ������ �Ҵ�.
	}
	for (int i = REQUEST_PER_CLINET / 2; i < REQUEST_PER_CLINET; i++) {
		requests[i].op = GET; //������ ������ �����ϸ�, ť���� ������ ���� �׽�Ʈ.
	}

	Queue* queue = init(); //ť �ʱ�ȭ �Լ� ����.
	//if (queue == NULL) return 0;

	//�ϴ� �� �� ���ε�, �׷��� multi client��� �����ϱ�
	thread client = thread(client_func, queue, requests, REQUEST_PER_CLINET); //���� ť ����(SET/GET)�� �����ϴ� �Լ�. ���� �����忡�� �����.
	client.join(); //������ �Ϸ� ���.

	//�ǹ� ���� �۾�
	//������ ��� ��. ������ ���Ἲ ������.
	cout << "sum of returned keys = " << sum_key << endl;
	cout << "sum of returned values = " << sum_value << endl;
}