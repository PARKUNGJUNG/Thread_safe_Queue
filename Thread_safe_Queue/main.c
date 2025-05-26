#include <stdio.h>
#include <time.h>

int main(void) {
	srand((unsigned int)time(NULL));

	//��ũ�ε� ����(GETRANGE�� �н�)
	Request requests[REQUEST_PER_CLINET];
	for (int i = 0; i < REQUEST_PER_CLINET / 2; i++) {
		requests[i].op = SET;
		requests[i].item.key = i;
		requests[i].item.value = (void*)(rand() % 1000000);
	}
	for (int i = REQUEST_PER_CLINET / 2; i < REQUEST_PER_CLINET; i++) {
		requests[i].op = GET;
	}

	Queue* queue = init();
	//if (queue == NULL) return 0;

	//�ϴ� �� �� ���ε�, �׷��� multi client��� �����ϱ�
	thread client = thread(client_func, queue, requests, REQUEST_PER_CLINET);
	client.join();

	//�ǹ� ���� �۾�
	cout << "sum of returned keys = " << sum_key << endl;
	cout << "sum of returned values = " << sum_value << endl;
}