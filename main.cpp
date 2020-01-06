#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <windows.h>

// https://stackoverflow.com/questions/18559028/undefined-reference-to-imp-wsacleanup
// https://www.binarytides.com/winsock-socket-programming-tutorial/
// https://www.bogotobogo.com/cplusplus/multithreading_win32A.php
// https://cboard.cprogramming.com/cplusplus-programming/54830-sending-arguments-function-using-createthread.html

struct recvARGS {
    SOCKET *socket;
    char* buffer;
    int bufSize;
    int flags;
};

int inputFromUser(){
    std::string input;
    std::string quitStr = "\\quit";
    int found;
    do {
        scanf("%s", &input); 
        found = input.find(quitStr);
    } while (found == 0);
}

DWORD WINAPI myThread(LPVOID lpParameter)
{
    int recvSize;
    // Cast to recvARGS type
    recvARGS *pArgs = (recvARGS*) lpParameter;

    while (true)  {

        memset(pArgs->buffer, 0, strlen(pArgs->buffer));

        recvSize = recv(*pArgs->socket,
                        pArgs->buffer,
                        pArgs->bufSize,
                        pArgs->flags);

        if(recvSize != SOCKET_ERROR)
        {
            printf("%s\n", pArgs->buffer);
        }
    }
	return 0;
}
 
int main (int argc, char* argv[]) {

    if (argc > 1) {
        for(int i = 1; i < argc; i++) {
            printf("argv[%d]: %s\n", i, argv[i]);
        }
    }

    WSADATA WSAData;
    SOCKET serverSocket;
    SOCKADDR_IN addr;

    char recvBuffer[1024];
    char sendBuffer[1024]={'h', 'e', 'l', 'l', 'o', '.'};

    int recvSize;
 
    WSAStartup(MAKEWORD(2,0), &WSAData);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
 
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
 
    connect(serverSocket, (SOCKADDR *)&addr, sizeof(addr));
    printf("Connected to server!\n");
 
    //send(serverSocket, sendBuffer, sizeof(sendBuffer), 0);
    //printf("Message sent!\n");

    /*
    if((recvSize = recv(serverSocket, recvBuffer, 1024, 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}*/
    
    using namespace std;

    recvARGS args = {
        &serverSocket,
        recvBuffer,
        1024,
        0
    };

    DWORD myThreadID;

    HANDLE myHandle = CreateThread(0, 0, myThread, &args, 0, &myThreadID);

    while(sendBuffer[0] != 'q') {

        memset(sendBuffer, 0, strlen(sendBuffer));
        //scanf(" %1024c[^\n]", sendBuffer);

        if (scanf("%1024[^\n]%*c", sendBuffer))
        {
            //sendBuffer[strcspn(sendBuffer, "\n")] = 0;
            
        }

        send(serverSocket, sendBuffer, sizeof(sendBuffer), 0);
    }
    
    CloseHandle(myHandle);
    
    closesocket(serverSocket);
    WSACleanup();
    printf("Socket closed.\n");
}