#include "includes.h"	// conn은 수정없이 사용
#define BUFSIZE 1024
#define PORT_NUM 3000
#define NAMESIZE 32
#define WIDTH 24
#define HEIGHT 16
#define MAX_PATH_LEN 4096
#define MAX_MSG_LEN 1024

void store(char* cwd);
int conn(int argc, char *argv[]);
void *controller(void *ptr);
void send_ints(int sock, int *host_arr, int arr_len);
void recv_ints(int sock, int *host_arr, int arr_len);
void send_int(int sock, int host_int);
int recv_int(int sock);
void send_chars(int sock, char *host_arr);
const char *recv_chars(int sock);
int sock;
int over;

int main(int argc, char *argv[]) {
    char cwd[MAX_PATH_LEN];
    getcwd(cwd, MAX_PATH_LEN);
    char name[10];
    strcat(cwd, "/");
    strcat(name, argv[2]);
    strcat(cwd, name);

    sock = conn(argc, argv);
    printf("Wait...\n");

    pthread_t thread;
    if (pthread_create(&thread, NULL, controller, NULL)) {
        fprintf(stderr, "thread 생성 에러");
        exit(1);
    }

    int state = 1;
    int over = 0;
    int n_clnts = recv_int(sock);
    //int myid = recv_int(sock);

    char **pnames = (char **) malloc(sizeof(char *) * n_clnts);
    int *pstate = (int *) malloc(sizeof(int) * n_clnts);
    char **pmsg = (char **) malloc(sizeof(char *) * n_clnts);

    for (int i = 0; i < n_clnts; i++) {
        //pscores[i] = recv_int(sock);
        pnames[i] = (char *) malloc(sizeof(char) * NAMESIZE);
        pmsg[i] = (char *) malloc(sizeof(char) * MAX_MSG_LEN);
        if (read(sock, pnames[i], NAMESIZE) < 0)
            perror("Reading from socket error");
    }

    while (1) {

        //state = recv_int(sock); //종료 체크
        for (int i = 0; i < n_clnts; i++) {
            pstate[i] = recv_int(sock);
            if (pstate[i] == 0) {
                over = 1;
            }
            //printf("return %s\n", recv_chars(sock));
            strcpy(&pmsg[i][0], recv_chars(sock));
            //usleep(10000);
            //printf("pmsg: %s", pmsg[i]);
            if(strcmp(&pmsg[i][0], "\0") !=0)
                printf("%s said, %s\n", pnames[i], pmsg[i]);
            if(strcmp(pmsg[i], "q\n")) {
                break;
            }
            free(pnames);
            free(pstate);
            free(pmsg);
        }
        //system("clear");
//printf("\n\n");

        sleep(1);
        if(over == 1)
            break;
    }
        close(sock);
        for (int i = 0; i < n_clnts; i++) {
            free(pnames[i]);
        }
        free(pnames);
        free(pstate);
        free(pmsg);
        return 0;
    }



void *controller(void *ptr){
    int itr;
    while (1)	//while (running)
    {
        char *msg;
        //memset(msg, 0x00, MAX_MSG_LEN);
        printf("say plz: ");
        scanf("%s",msg);
        //fgets(msg, sizeof(msg), stdin);
        if(strcmp(msg,"\0")){
            send_chars(sock, msg);
            printf("send_func_work\n");
        }


    }

    return NULL;
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
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connect() error");
    }

    // 이름 전송
    if (write(sock, name, NAMESIZE - 1) < 0)
        perror("Writing to socket error");

    // 다른 클라이언트 접속 대기
    m = recv_int(sock);	n = recv_int(sock);
    printf("%d/%d clients connected\n", n, m);

    while (n < m) {
        n = recv_int(sock);
        printf("%d/%d clients connected\n", n, m);
        //printf("n = %d, m = %d\n", n, m); debug
    }
    printf("\n");

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

int recv_int(int sock)
{
    int host_int, net_int;
    if (read(sock, &net_int, sizeof(int)) < 0)
        perror("Reading from socket error");
    host_int = ntohs(net_int);

    return host_int;
}

void send_chars(int sock, char *host_arr){
    if (write(sock, host_arr, sizeof(char) * MAX_MSG_LEN) < 0)
        perror("Writing to socket error");
    printf("%s\n",host_arr);
}
const char* recv_chars(int sock){
    static char host_arr[MAX_MSG_LEN] = "";
    if (read(sock, host_arr, sizeof(char) * MAX_MSG_LEN) < 0)
        perror("Reading from socket error");
    return host_arr;
}