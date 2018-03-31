/*Nathan Greenlaw
 * CS 372
 * Co-opted from my own Program 4: otp_dec decrypt text with a given key by sending it to otp_dec_d from CS344*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/ioctl.h>

void error(const char *msg) { fprintf(stderr,"%s\n",msg);exit(0);}//perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead, kcharsWritten;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
    
	if (argc < 2) { fprintf(stderr,"USAGE: servername port\n", argv[0]); exit(0); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
	serverAddress.sin_addr = *((struct in_addr *)serverHostInfo->h_addr);
	
	//Set up variable for input clearing
	char* p;

	//Enter the username
	printf("Enter your username: ");
	char username[11];
	fgets(username,10,stdin);

	//Clear input found on : https://stackoverflow.com/questions/38767967/clear-input-buffer-after-fgets-in-c
	if(p=strchr(username,'\n'))
	{
		*p = 0;
	}
	else
	{
		scanf("%*[^\n]");
		scanf("%*c");
	}

	username[strcspn(username,"\n")] = 0;
	
	strcat(username,">");

	//Set up the message size
	char msg[500];
	memset(msg, '\0', sizeof(msg)); // Clear out the buffer array

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) {error("CLIENT: ERROR opening socket");exit(2);}

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		{error("CLIENT: ERROR connecting to server");exit(2);}

	//Send first message
	printf("Send First Message\n");
	printf("%s ",username);

	fgets(msg,500,stdin);
	if(p=strchr(msg,'\n'))
		{
			*p = 0;
		}
		else
		{
			scanf("%*[^\n]");
			scanf("%*c");
		}

	msg[strcspn(msg,"\n")] = 0;

	char buffer[510];
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
	strcat(buffer,username);
	strcat(buffer,msg);
	
	//Set up recv message
	char response[510];
	memset(response,'\0',sizeof(response));
	int curBuffer;
	int left;
	int chr;
	int end = 0; //Check if a quit message is issued	

	//send the text and loop until it is finished
	int chrs;
	int cBuffer = 0;
	int lefts = sizeof(buffer)-1;
	while(cBuffer < sizeof(buffer)-1)
	{
		chrs = send(socketFD, buffer+cBuffer,lefts,0);
		cBuffer += chrs;
		lefts -= chrs;
	}
	
	//Check if first message is quit
	if(strcmp(msg,"\\quit") == 0)
	{
		end =1;	
		close(socketFD); // Close the socket
		printf("Client Closed Socket\n");
	}

	//Loop until there is a quit issued
	while(end == 0)
	{
		// loop until all of the response is received and then print it	
		memset(response,'\0',sizeof(response));	

		curBuffer = 0;
		left = sizeof(response)-1;
		//while(curBuffer < sizeof(response)-1)
		//{
			chr = recv(socketFD,response,1024,0);
			//if(chr < 0){error("Client recv error");exit(1);}
			//curBuffer += chr;
			//left -= chr;
		//}
		
		printf("%s\n",response);

		if(strstr(response,"\\quit") == NULL)
		{
			//Send Message
			memset(msg, '\0', sizeof(msg)); // Clear out the buffer array
			printf("%s ",username);
			fgets(msg,500,stdin);
			msg[strcspn(msg,"\n")] = 0;
			memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
			strcat(buffer,username);
			strcat(buffer,msg);
			
			//send the text and loop until it is finished
			
			cBuffer = 0;
			lefts = sizeof(buffer)-1;
			while(cBuffer < sizeof(buffer)-1)
			{
				chrs = send(socketFD, buffer+cBuffer,lefts,0);
				cBuffer += chrs;
				lefts -= chrs;
			}
			
			//Close the socket if send message says quit
			if(strcmp(msg,"\\quit") == 0)
			{
				end =1;	
				close(socketFD); // Close the socket
				printf("Client Closed Socket\n");
			}
		}
		
		//Close the socket if the response says quit
		else
		{
			end =1;
			close(socketFD); // Close the socket
			printf("Server Closed Socket\n");
		}
	}
	
	return 0;
}
