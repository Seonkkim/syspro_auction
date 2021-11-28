#include "includes.h"	// start, conn은 수정없이 사용

#define PORT_NUM 3000
#define NAMESIZE 32
#define MAX_MSG_LEN 1024
typedef struct{
    char g_name[NAMESIZE];
    int lowest_p;
    int decided_p;
}Goods;

typedef struct{
	int sock;
	char name[NAMESIZE];
	int id;
	char msg[MAX_MSG_LEN];
    char pw[NAMESIZE];
	int state;
    int money;
    Goods *own_goods;
}Clnt;

typedef struct _Thr {
	Clnt *clnts;
	int n_clnts;
} Thr;

int start_server(int argc, char *arg);
void send_chars(int sock, char *host_arr);
char *recv_chars(int sock);
void conn(int serv_sock, Clnt *clnts, int n_clnts);
void send_int(int sock, int host_int);
int recv_int(int sock);
void process_login(int sock, Clnt clnt);
void savein_db(char *d_add, Clnt clnt);
void *controller(void *ptr);

int main(int argc, char *argv[])
{
	int serv_sock = start_server(argc, argv[0]);

	int n_clnts = atoi(argv[1]);
	Clnt clnts[n_clnts];
	conn(serv_sock, clnts, n_clnts); // connect success
	pthread_t threads[n_clnts];
	Thr THREAD;
	THREAD.clnts = (Clnt *)malloc(sizeof(Clnt) * n_clnts);
	THREAD.n_clnts = n_clnts;
	THREAD.clnts = clnts;
	for (int i = 0; i < n_clnts; i++) {
		THREAD.clnts[i].id = i;
		THREAD.clnts[i].state = 1;
		strcpy(THREAD.clnts[i].msg,"\0");

	}
	////////////////////////////////////////////////
	for (int i = 0; i < n_clnts; i++) {
		send_int(THREAD.clnts[i].sock, n_clnts);
		send_int(THREAD.clnts[i].sock, i);	// clnt thr에 id전달

		for (int j = 0; j < n_clnts; j++) {
			if (write(THREAD.clnts[i].sock, clnts[j].name, NAMESIZE) < 0)
				perror("Writing to socket error");

		}

	}
	for (int i = 0; i < n_clnts; i++) {
		if (pthread_create(&threads[i], NULL, controller, &THREAD.clnts[i])) {
			fprintf(stderr, "Creating thread error...\n");
			exit(1);
		}
	}

	sleep(1);
	while (1)
	{
		for (int i = 0; i < n_clnts; i++)
        {
            //memset(THREAD.clnts[j].msg, 0x00, MAX_MSG_LEN);
            //printf("thread: %s",THREAD.clnts[j].msg);
            if(strcmp(&THREAD.clnts[i].msg[0],"\0")) {
                //printf("%s",THREAD.clnts[j].msg);

                send_chars(THREAD.clnts[i].sock, THREAD.clnts[i].msg);
                printf("id %s said, %s\n", THREAD.clnts[i].name, THREAD.clnts[i].msg);
                //strcpy(&THREAD.clnts[j].msg[0],"\0");
            }


			}
        for (int i = 0; i <n_clnts; i++) {
            memset(THREAD.clnts[i].msg, 0x00, MAX_MSG_LEN);
            sleep(0.01);
        }

		}
		usleep(120000);	// 삐걱임 제거
		//printf("choose what you want: \n");

		for (int i = 0; i <n_clnts; i++) {
            memset(THREAD.clnts[i].msg, 0x00, MAX_MSG_LEN);
        sleep(0.01);
		}
	///////////////////////////////////////////
	close(serv_sock);
	for (int i = 0; i < n_clnts; i++)
		close(clnts[i].sock);

	free(THREAD.clnts);
	return 0;
	///////
	/// ////////////////////////////////////
}

void *controller(void *ptr)
{
	//Clnt clnt = *(Clnt *)ptr;
	while (1)
	{
        if(strcmp((*(Clnt *)ptr).msg,"exit") != 0){
            //printf("%s", recv_chars(clnt.sock));
            strcpy((*(Clnt *)ptr).msg, recv_chars((*(Clnt *)ptr).sock));
            printf("client %d <%s> (%s)\n", (*(Clnt *)ptr).id, (*(Clnt *)ptr).name, (*(Clnt *)ptr).msg);

        }
     //printf("클라이언트%d <%s>로부터 (%s) 확인\n", clnt.id, clnt.name, clnt.msg);

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

void conn(int serv_sock, Clnt *clnts, int n_clnts)
{
	int clnt_sock;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	printf("Waiting for %d clients...\n", n_clnts);

	for (int i = 0; i < n_clnts; i++) {
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) {
			perror("accept() error"); exit(1);
		}
		//printf("Connection from: %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

		clnts[i].sock = clnt_sock;

		// read client name
		if (read(clnts[i].sock, clnts[i].name, NAMESIZE - 1) < 0)
			perror("Reading from socket error");
        send_int(clnts[i].sock, 1); // read success

		clnts[i].name[NAMESIZE - 1] = '\0';
		clnts[i].id = i + 1; // id 추가
        strcpy(clnts[i].pw,"1234");

        process_login(clnts[i].sock, clnts[i]);

		printf("Client #%d '%s' connected\n", clnts[i].id, clnts[i].name);

		send_int(clnts[i].sock, n_clnts);	// m at clnt
		//printf("%d\n", i); debug
		for (int j = 0; j <= i; j++)
			send_int(clnts[j].sock, i+1);	// n at clnt
	}
	printf("\n");
}
/* data통신(read/write)을 쉽게 하려고 미리 작성한 함수*/

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

void send_chars(int sock, char *host_arr){
	if (write(sock, host_arr, sizeof(char) * MAX_MSG_LEN) < 0)
		perror("Writing to socket error");
}
char *recv_chars(int sock){
	static char host_arr[MAX_MSG_LEN] = "";
	if (read(sock, host_arr, sizeof(char) * MAX_MSG_LEN) < 0)
		perror("Reading from socket error");
	return host_arr;
}
void process_login(int sock, Clnt clnt){

    char* tmp = recv_chars(sock);

    if(!strcmp(clnt.pw, tmp)){
        send_int(sock,3);
        printf("pw right, login success\n");
    }
    else{
        send_int(sock,0);
        printf("pw wrong\n");
    }
}
void savein_db(char *d_add, Clnt clnt){
    int fd;
    Clnt *tmp;
    fd= open(d_add,O_CREAT|O_RDWR|O_TRUNC,0777);
    ftruncate(fd, sizeof(Clnt));

    if (fd== -1) { perror("open"); return; }

    tmp = (Clnt*)mmap(d_add, sizeof(Clnt), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (tmp == MAP_FAILED) { perror("mmapwe"); return; }

    if (close (fd) == -1) { perror("close"); return; }

    tmp = &clnt;

    if (munmap(tmp,sizeof(Clnt)) == -1) { perror("munmap"); return;}
}

