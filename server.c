#include "includes.h"	// start, conn은 수정없이 사용
#define BUFSIZE 1024
#define PORT_NUM 50000
#define NAMESIZE 32

typedef struct _Clnt {
	int sock;
	char name[NAMESIZE];
	int num_test;
	int id;
	int new_val;
} Clnt;

typedef struct _Thr {
	Clnt *clnts;
} Thr;

int start_server(int argc, char *arg);
void conn(int serv_sock, Clnt *clnts, int n_clnts);
void send_ints(int sock, int *host_arr, int arr_len);
void recv_ints(int sock, int *host_arr, int arr_len);
void send_int(int sock, int host_int);
int recv_int(int sock);

void *input_test(void *ptr);

int main(int argc, char *argv[])
{
	int serv_sock = start_server(argc, argv[0]);
	
	int n_clnts = atoi(argv[1]);
	Clnt clnts[n_clnts];
	conn(serv_sock, clnts, n_clnts);

	pthread_t threads[n_clnts];

	for (int i = 0; i < n_clnts; i++) {

		/*
		while (1) {

			clnts[i].num_test = recv_int(clnts[i].sock);
			
			//for (int j = 0; j < n_clnts; j++) {
			//	send_int(clnts[j].sock, clnts[i].num_test);
			}
		
		input_test(&clnts[i]);
		*/

		pthread_create(&threads[i], NULL, input_test, &clnts[i]);
	}

	for (int i = 0; i < n_clnts; i++) {
		pthread_join(threads[i], NULL);
	}

	/*close(serv_sock);
	for (int i = 0; i < n_clnts; i++)
		close(clnts[i].sock);
	*/
	return 0;
}

void *input_test(void *ptr)
{
	Clnt clnt = *(Clnt *)ptr;
	while (1)
	
		clnt.num_test = recv_int(clnt.sock);
		printf("클라이언트%d로부터 (%d) 확인\n", clnt.id, clnt.num_test);
	}

	return NULL;
}

int start_server(int argc, char *arg)	// 오류처리 포함
{
	int serv_sock;
	struct sockaddr_in serv_addr;	//IPv4 address structure

	if (argc != 2) {
		fprintf(stderr, "Usage : %s <number of players>\n", arg);
		exit(1);
	}

	// 소켓 생성
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		perror("socket() error"); exit(1);
	}

	// 소켓 어드레스
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;	/* Allows IPv4 socket */
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* 모든 주소의 client 접속 가능 */
	serv_addr.sin_port = htons(PORT_NUM);
	// server socket binding
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		perror("bind() error"); exit(1);
	}
	if (listen(serv_sock, 5) == -1)	{
		perror("listen() error"); exit(1);
	}

	return serv_sock;
}

//clnt_sock, clnts, n_clnts
void conn(int serv_sock, Clnt *clnts, int n_clnts)
{
	int clnt_sock;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size = sizeof(clnt_addr);
	printf("Waiting for %d clients...\n", n_clnts);

	for (int i = 0; i < n_clnts; i++) { /* 접속된 client 별 thread 생성하여 detach */
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) {
			perror("accept() error"); exit(1);
		}
		//printf("Connection from: %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

		clnts[i].sock = clnt_sock;		

		// read client name
		if (read(clnts[i].sock, clnts[i].name, NAMESIZE - 1) < 0)
			perror("Reading from socket error");
		clnts[i].name[NAMESIZE - 1] = '\0';

		clnts[i].id = i + 1; // id 추가
		printf("Client #%d '%s' connected\n", clnts[i].id, clnts[i].name);

		/////////////////////////////////////////////
		// 접속할 때 마다 유저 count. 수학적 장난(?)이 포함.
		send_int(clnts[i].sock, n_clnts);	// m at clnt
		//printf("%d\n", i); debug
		for (int j = 0; j <= i; j++)
			send_int(clnts[j].sock, i+1);	// n at clnt
		/////////////////////////////////////////////
	}
	printf("\n");

	return 0;
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

int recv_int(int sock)
{
	int host_int, net_int;
	if (read(sock, &net_int, sizeof(int)) < 0)
		perror("Reading from socket error");
	host_int = ntohs(net_int);	// 엔디언 방식 차이

	return host_int;
}