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
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 1313


void initWinSock() {
    WSADATA wsaData;
    int iResult;
	int iResult2;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit( 1 );
    }
	iResult2 = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit( 1 );
    }
}

SOCKET bindOnAny(unsigned int port) {
    int iResult;
	int iResult2;
	
	
    SOCKET ListenSocket = INVALID_SOCKET;
	sockaddr_in service;
    service.sin_family = AF_INET;
	service.sin_addr.s_addr = 0;//inet_addr("127.0.0.1");
    service.sin_port = htons(port);

    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //----------------------
    //   Bind the socket.
    iResult = bind(ListenSocket, (SOCKADDR *) &service, sizeof (service));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
        wprintf(L"bind returned success\n");
	return ListenSocket;
	
	iResult2 = bind(ListenSocket, (SOCKADDR *) &service, sizeof (service));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"bind failed with error %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
        wprintf(L"bind returned success\n");
	return ListenSocket;
}

int __cdecl main(void) 
{
    int iResult;
	int iResult2;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
	SOCKET ClientSocket2 = INVALID_SOCKET;
	
	int iloscIteracji = 0;
	int wybor = 0;
	int wybor2 = 0;
	int iteracja = 0;
    int iSendResult;
	int wiezien1 = 0;
	int wiezien2 = 0;
	int wynik[30] [2];
    char recvbuf[DEFAULT_BUFLEN];
	char recvbuf2[DEFAULT_BUFLEN];
	char recvbuf3[DEFAULT_BUFLEN];
	char recvbuf4[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
	
	
	initWinSock();
	ListenSocket = bindOnAny(DEFAULT_PORT);    
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	 iResult2 = listen(ListenSocket, SOMAXCONN);
    if (iResult2 == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	
	
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	
	ClientSocket2 = accept(ListenSocket, NULL, NULL);
    if (ClientSocket2 == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

	//Podanie ilosci iteracji programu oraz sprawdzenie czy podana cyfra znajduje sie w dopuszczalnym zakresie
	do {
		std::cout << "Podaj ilosc iteracji gry (min 1 - max 30): _\b";
		std::cin >> iloscIteracji;
		system("CLS");
	} while (iloscIteracji > 30 || iloscIteracji <= 0);

	send(ClientSocket, recvbuf2, iloscIteracji, 0);
	send(ClientSocket2, recvbuf2, iloscIteracji, 0);

	//Ekran wybrania strategii dla wiezniow
	srand ( time(NULL) );
	int sposobWyboruWiezien1, sposobWyboruWiezien2;

	//do-while - Petle sprawdzajace czy podana cyfra miesci sie w wymaganym zakresie

	//WIEZIEN 1----------------------------------------------------------------------------------
	do {
		std::cout << "Wybierz strategie dla wieznia pierwszego lub pozwol na wybor losowy\n";
		std::cout << "\t1. Wybierz strategie\n";
		std::cout << "\t2. Losuj strategie\n";
		std::cout << "Twoja decyzja: _\b";
		std::cin >> sposobWyboruWiezien1;

		system("CLS");
	} while (sposobWyboruWiezien1 > 2 || sposobWyboruWiezien1 <= 0);

	if (sposobWyboruWiezien1 == 2) {
		wybor = rand() % 8 + 0;
	} else {
		//Wybieranie strategii
		do {
			std::cout << "Dostepne strategie:\n\n1. Random  \n\n2. Always Fair  \n\n3. Always Unfair  \n\n4. Tit For Tat - powtarza ostatni ruch przeciwnika  \n\n5. Grudger - Wspolpracuj do momentu w ktorym przeciwnik zagra unfair, od tego momentu zawsze graj unfair \n\n6. Tit For Two Tats and Random\n\n7. Suspicious Tit for Tat - Taka sama jak Tit for Tat jednak zaczyna od zagrania unfair\n\n8. Douchebag - Wszystkie rundy gra fair jednak w ostatniej gra unfair\n\n9. Player - Sam w kliencie podajesz co chcesz grac.\n";
			std::cout << "\n";
			std::cout << "Wybierz jaka strategia ma grac wiezien pierwszy: _\b";
			std::cin >> wybor;

			system("CLS");
		} while (wybor > 9 || wybor <= 0 );
	}

	//WIEZIEN 2----------------------------------------------------------------------------------
	do {
		std::cout << "Wybierz strategie dla wieznia drugiego lub pozwol na wybor losowy\n";
		std::cout << "\t1. Wybierz strategie\n";
		std::cout << "\t2. Losuj strategie\n";
		std::cout << "Twoja decyzja: _\b";
		std::cin >> sposobWyboruWiezien2;

		system("CLS");
	} while (sposobWyboruWiezien2 > 2 || sposobWyboruWiezien2 <= 0);

	if (sposobWyboruWiezien2 == 2) {
		wybor2 = rand() % 8 + 0;
	} else {
		//Wybieranie strategii
		do {
			std::cout << "Dostepne strategie:\n\n1. Random  \n\n2. Always Fair  \n\n3. Always Unfair  \n\n4. Tit For Tat - powtarza ostatni ruch przeciwnika  \n\n5. Grudger - Wspolpracuj do momentu w ktorym przeciwnik zagra unfair, od tego momentu zawsze graj unfair \n\n6. Tit For Two Tats and Random\n\n7. Suspicious Tit for Tat - Taka sama jak Tit for Tat jednak zaczyna od zagrania unfair\n\n8. Douchebag - Wszystkie rundy gra fair jednak w ostatniej gra unfair\n\n9. Player - Sam w kliencie podajesz co chcesz grac.\n";
			std::cout << "\n";
			std::cout << "Wybierz jaka strategia ma grac wiezien drugi: _\b";
			std::cin >> wybor2;

			system("CLS");
		} while (wybor2 > 9 || wybor2 <= 0 );
	}

	std::cout << "Wiezien pierwszy gra strategia: \t";

	switch (wybor) {
		case 1: 
			std::cout << "Random\n";
		break;

		case 2: 
			std::cout << "Always Fair\n";
		break;

		case 3: 
			std::cout << "Always Unfair\n";
		break;

		case 4: 
			std::cout << "Tit for Tat\n";
		break;

		case 5: 
			std::cout << "Grudger\n";
		break;

		case 6: 
			std::cout << "Tit For Two Tats and Random\n";
		break;

		case 7: 
			std::cout << "Suspicious Tit For Tat\n";
		break;

		case 8: 
			std::cout << "Douchebag\n";
		break;
		case 9: 
			std::cout << "Player\n";
		break;
	}

	std::cout << "Wiezien    drugi gra strategia: \t";

	switch (wybor2){
		case 1: 
			std::cout << "Random";
		break;

		case 2: 
			std::cout << "Always Fair";
		break;

		case 3: 
			std::cout << "Always Unfair";
		break;

		case 4: 
			std::cout << "Tit for Tat";
		break;

		case 5: 
			std::cout << "Grudger";
		break;

		case 6: 
			std::cout << "Tit For Two Tats and Random";
		break;

		case 7: 
			std::cout << "Suspicious Tit For Tat";
		break;

		case 8: 
			std::cout << "Douchebag";
		break;
		case 9: 
			std::cout << "Player\n";
		break;
	}

	std::cout << "\n\n\n";

	send(ClientSocket, recvbuf3, wybor, 0);
	send(ClientSocket2, recvbuf4, wybor2, 0);
	

    // No longer need server socket
    closesocket(ListenSocket);
	
	
    // Receive until the peer shuts down the connection
    do {
		printf("\t");
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        if (iResult > 0) {

			if (iResult == 4) {
				printf("Wiezien 1 zagral fair  ");
			} else if (iResult == 6) {
				printf("Wiezien 1 zagral unfair");
			}

        // Echo the buffer back to the sender
		} else if (iResult == 0) {
            //printf("Connection closing...\n");
		} else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
		
            
		//Sleep(1000);

		printf("\t\t\t");
        iResult2 = recv(ClientSocket2, recvbuf, recvbuflen, 0);

        if (iResult2 > 0) {

			if (iResult2 == 4) {
				printf("Wiezien 2 zagral fair\n");
			} else if (iResult2 == 6) {
				printf("Wiezien 2 zagral unfair\n");
			}

			iSendResult = send(ClientSocket2, recvbuf, iResult, 0 );

            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                
                WSACleanup();
                return 1;
            }
       
			Sleep(1000);
			
            iSendResult = send(ClientSocket, recvbuf, iResult2, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                
                WSACleanup();
                return 1;
            }
            
		} else if (iResult2 == 0) {
            printf("\nZamykanie polaczenia...\n");
		} else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket2);
            WSACleanup();
            return 1;
			
        }
		

		//Sumowanie wynikow
		if ((iResult2 == iResult) && (iResult2==4)) {

			wynik[iteracja][0] = 1;
			wynik[iteracja][1] = 1;
			wiezien1 = wiezien1 + 1;
			wiezien2 = wiezien2 + 1;

		} else if (iResult > iResult2) {

			wynik [iteracja] [0] = 0;
			wynik [iteracja] [1] = 5;
			wiezien1 = wiezien1 + 0;
			wiezien2 = wiezien2 + 5;

		} else if (iResult < iResult2) {

			wynik [iteracja] [0] = 5;
			wynik [iteracja] [1] = 0;
			wiezien1 = wiezien1 + 5;
			wiezien2 = wiezien2 + 0;

		} else if ((iResult == iResult2) && (iResult2==6)) {

			wynik [iteracja] [0] = 3;
			wynik [iteracja] [1] = 3;
			wiezien1 = wiezien1 + 3;
			wiezien2 = wiezien2 + 3;

		}

		if (iteracja < iloscIteracji) {
			std::cout<<"                                 [" << wynik[iteracja][0] << "]    [" << wynik[iteracja][1] << "] \n";
		}
		iteracja++;

    } while (iResult > 0);
	char close;
	
	
	std::cout << "\nWynik:                [" << wiezien1 << "]\t\t               [" << wiezien2 << "] \n";
	std::cout<<"Jesli chcesz zakonczyc napisz: close \n";
	std::cin>>close;

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
	 
	iResult2 = shutdown(ClientSocket2, SD_SEND);
    if (iResult2 == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket2);
        WSACleanup();
        return 1;
    }
	

    // cleanup
    closesocket(ClientSocket);
	closesocket(ClientSocket2);
    WSACleanup();

    return 0;
}