#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


using namespace std;

struct HTTP_Request{

    string headers;
    string data;

};

HTTP_Request Read_Request(int new_client){

    char data[1024];
    char headers[4096];
    string content;
    bool loop = true;
    bool headers_found = false;
    int n;
    while (loop){

       bzero(data, 1024);

       n = recv(new_client, data, sizeof(data), 0);
       if ( n < 0 ){

           break;

       }

        int i = 0;
        for (int placer(0); placer < 1023; placer++){

            if ( data[placer] == '}' ){

                i = placer;
                break;

            }

        }

        if ( i == 0 ){

            strncpy( headers, data, sizeof(data) );

        }else if( !headers_found ){

            strncpy( headers, data, i);
            headers[sizeof(headers)+1]='\0';
            headers_found = true;
            char temp[1024];
            int a = 0;
            int b = i+1;
            for (b; b < sizeof(data); b++){

                temp[a] = data[b];

            }
            content = temp;

        }else{

            content += data;

        }

    }

    HTTP_Request received;
    received.headers = headers;
    received.data = content;
    return received;

}

int main(int argc, char *argv[])
{

    sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(3333);

    int HTTPSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( HTTPSock == -1 ){

        cout << "Error establishing socket." << endl;
        return 0;

    }

    if ( bind(HTTPSock, (struct sockaddr *)&server, sizeof(server)) < 0 ){

        cout << "Can't bind." << endl;
        return 0;

    }

    listen(HTTPSock, 1);
    socklen_t clilen = sizeof(client);

    int i = 0;
    while ( i < 1 ){

        int connection = accept(HTTPSock, (struct sockaddr *)&client, &clilen);
        if ( connection < 0 ){

            cout << "Connection failed." << endl;
            return 0;

        } else {

            cout << "Connected to a new client." << endl;

        }

        Read_Request(connection);
        i++;

    }

}
