#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <iostream>
#include <ctype.h>



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1313"

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket2 = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "fair";
    char recvbuf[DEFAULT_BUFLEN];
	char recvbuf2[DEFAULT_BUFLEN];
	char recvbuf4[DEFAULT_BUFLEN];
    int iResult2 = 10;
	int wybor = 100;
	int wybory = 1;
	int Grudger = 1;
	int strategia = 1;
    int recvbuflen = DEFAULT_BUFLEN;
	int iloscIteracji = 100;
	int iteracja = 1;
	//srand ( time(NULL) );
    

    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult2 = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult2 != 0) {
        printf("WSAStartup failed with error: %d\n", iResult2);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult2 = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult2 != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult2);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket2 = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket2 == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult2 = connect( ConnectSocket2, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult2 == SOCKET_ERROR) {
            closesocket(ConnectSocket2);
            ConnectSocket2 = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket2 == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
	iloscIteracji = recv(ConnectSocket2, recvbuf2,  iloscIteracji, 0);
	strategia = recv(ConnectSocket2, recvbuf4,  wybor, 0);
	

	
	int losowaStrategiaRandom;
	int historiaRuchowPrzeciwnika[30];

	for (iteracja; iteracja <= iloscIteracji; iteracja++) {

	//STRATEGIE - pomysly na strategie zaczerpniete z www.iterated-prisoners-dilemma.net/prisoners-dilemma-strategies.shtml

		switch (strategia) {
		//Strategia Random
			case 1: 
				losowaStrategiaRandom = rand() % 11 + 0;
				if (losowaStrategiaRandom < 6) {
					sendbuf = "fair";
				} else { 
					sendbuf = "unfair";
				}
			break;

		//Strategia Always Fair
			case 2:
				sendbuf = "fair";
			break;

		//Strategia Always Unfair
			case 3:
				sendbuf = "unfair";
			break;

		//Strategia Tit for Tat - powtarza ostatni ruch przeciwnika 
			case 4:
				if (iteracja== 1) {
					sendbuf = "fair";
				} else {
					if (iResult2 == 6) {
						sendbuf = "unfair";
					} else {
						sendbuf = "fair";
					}
				}
			break;

		//Strategia Grudger - Wspolpracuj do momentu w ktorym przeciwnik zagra unfair, od tego momentu zawsze graj unfair
			case 5:
				if (iteracja == 1) {
					sendbuf = "fair";
				} else {
					if ((iResult2 == 4) && (Grudger == 1)) {
						sendbuf = "fair";		
					} else {	
						sendbuf = "unfair";
						Grudger = 2;
					}
				}
			break;

		//Tit For Two Tats and Random - Taka sama jak tit for tat z wyjatkiem tego, ze przeciwnik musi podjac taka sama decyzje dwukrotnie, zanim bedzie sie wspolpracowac. Wybor jest 'wypaczony' przez ustawienie przypadkowe.
			case 6:
				
				if (iResult2 == 4) {
					historiaRuchowPrzeciwnika[iteracja] = 0; //fair
				} else if (iResult2 == 6) {
					historiaRuchowPrzeciwnika[iteracja] = 1; //unfair
				}

				if (iteracja == 1, 2) {
						losowaStrategiaRandom = rand() % 11 + 0;
					if (losowaStrategiaRandom < 6) {
						sendbuf = "fair";
					} else { 
						sendbuf = "unfair";
					}
				} else {
					if ((historiaRuchowPrzeciwnika[ iteracja-1 ] + historiaRuchowPrzeciwnika[ iteracja ]) == 0) {
						sendbuf = "fair";		
					} else {	
						sendbuf = "unfair";
					}
				}
			break;

		//Suspicious Tit For Tat - Taka sama jak Tit for Tat jednak zaczyna od zagrania unfair
			case 7:
				if (iteracja == 1) {
					sendbuf = "unfair";
				} else {
					if (iResult2 == 6) {
						sendbuf = "unfair";
					} else {
						sendbuf = "fair";
					}
				}
			break;

		//Douchebag - Wszystkie rundy gra fair, a w ostatniej gra unfair
			case 8:
					if (iteracja == 1) {
						sendbuf = "fair";
					} else {
						if (iteracja == iloscIteracji) {
							sendbuf = "unfair";
						} else if (iResult2 == 4) {
							sendbuf = "fair";
						} else if (iResult2 == 6) {
							sendbuf = "fair";
						}
					}
			break;
			
			case 9:
				std::cout<<"Jak chcesz zagrac? 1. Fair  2. Unfair \n";
				std::cin>>wybory;
						do{
						if (wybory== 2)  sendbuf = "unfair";
						if (wybory== 1) sendbuf = "fair";	
						} while (wybory > 2 || wybory <= 0 );
						
					
			break;
	}
    // Send an initial buffer
    iResult2 = send( ConnectSocket2, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult2 == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket2);
        WSACleanup();
        return 1;
    }

    printf("Wyslano bajtow: %ld\n", iResult2);
	
	iResult2 = recv(ConnectSocket2, recvbuf, recvbuflen, 0);
    printf("Odebrano bajtow: %d\n", iResult2);
	}
    // shutdown the connection since no more data will be sent
    iResult2 = shutdown(ConnectSocket2, SD_SEND);
    if (iResult2 == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket2);
        WSACleanup();
        return 1;
    }	// cleanup
	
    // Receive until the peer closes the connection
    do {

        iResult2 = recv(ConnectSocket2, recvbuf, recvbuflen, 0);
        if ( iResult2 > 0 )
            printf("Wyslano bajtow: %d\n", iResult2);
        else if ( iResult2 == 0 ){
			printf("Polaczenie zakonczone\n");
			Sleep(5000);
			}
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult2 > 0 );
	

	
	


    closesocket(ConnectSocket2);
    WSACleanup();

    return 0;
}
