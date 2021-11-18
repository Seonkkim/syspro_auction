#include "includes.h"	// conn은 수정없이 사용
#define BUFSIZE 1024
#define PORT_NUM 50000
#define NAMESIZE 32

typedef struct _ThrInfo {
	int sock;
	int *running;
} ThrInfo;

int conn(int argc, char *argv[]);
void send_ints(int sock, int *host_arr, int arr_len);
void recv_ints(int sock, int *host_arr, int arr_len);
void send_int(int sock, int host_int);
int recv_int(int sock);
void *controller(void *ptr);
int sock;

int main(int argc, char *argv[])
{
	sock = conn(argc, argv);

	printf("Wait...\n");

///////////////////////////////////
	pthread_t thread;
	
	if (pthread_create(&thread, NULL, controller, NULL))
		{
			fprintf(stderr, "thread 생성 에러");
			exit(1);
		}
	pthread_join(thread, NULL);
//////////////////////////////////
	close(sock);
	
	return 0;
}


void start(int sock)
{
	int running = recv_int(sock);
	int rst;
	pthread_t thr;
	ThrInfo info;
	info.sock = sock;	info.running = &running;
	rst = pthread_create(&thr, NULL, controller, (void*)&info);
	assert(!rst);

	while (running) {
		system("clear");
		//do what you want
		running = recv_int(sock);
	}

	pthread_cancel(thr);
	pthread_join(thr, NULL);
}

int conn(int argc, char *argv[])
{
	char name[NAMESIZE];
	if (argc != 3) {
		fprintf(stderr, "Usage : %s <IP> <user name>\n", argv[0]);
		exit(1);
	}
	strcpy(name, argv[2]);

	int sock, n, m;
	struct sockaddr_in serv_addr;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("socket() error");
		exit(1);
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(PORT_NUM);
	if (connect(sock, (struct socaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		perror("connect() error");
	}

	// 이름 전송
	if (write(sock, name, NAMESIZE - 1) < 0)
		perror("Writing to socket error");

	/////////////////////////////////////////////////
	// 다른 클라이언트 접속 대기
	m = recv_int(sock);	n = recv_int(sock);
	printf("%d/%d clients connected\n", n, m);
	
	while (n < m) {
		n = recv_int(sock);
		printf("%d/%d clients connected\n", n, m);
		//printf("n = %d, m = %d\n", n, m); debug
	}
	printf("\n");
	/////////////////////////////////////////////////

	return sock;
}

/* data통신(read/write)을 쉽게 하려고 미리 작성한 함수*/

void send_ints(int sock, int *host_arr, int arr_len)
{
	int net_arr[arr_len];
	for (int i = 0; i < arr_len; i++)
		net_arr[i] = htons(host_arr[i]);
	if (write(sock, net_arr, sizeof(int) * arr_len) < 0)
		perror("Writing to socket error");
}

void recv_ints(int sock, int *host_arr, int arr_len)
{
	int net_arr[arr_len];
	if (read(sock, net_arr, sizeof(int) * arr_len) < 0)
		perror("Reading from socket error");
	for (int i = 0; i < arr_len; i++)
		host_arr[i] = ntohs(net_arr[i]);
}

void send_int(int sock, int host_int)
{
	int net_int = htons(host_int);
	if (write(sock, &net_int, sizeof(int)) < 0)
		perror("Writing to socket error");
}

void *controller(void *ptr){
	// to do
	return NULL;
}

int recv_int(int sock)
{
	int host_int, net_int;
	if (read(sock, &net_int, sizeof(int)) < 0)
		perror("Reading from socket error");
	host_int = ntohs(net_int);

	return host_int;
}
