#Nathan Greenlaw
#CS 372
#ONID: greenlan
#chatserve.py 

from socket import *
import sys

#Main function from Lecture 15 slide 9 Python TCP server

def main():
	argNum = int(sys.argv[1]);
	serverPort = argNum;
	serverSocket = socket(AF_INET, SOCK_STREAM)
	serverSocket.bind(('',serverPort))
	serverSocket.listen(1)
	serverHandle = "ChatServer>"
	print("Server ready to receive")
	
	while 1:
		end = 0;
		connectionSocket, adr = serverSocket.accept()
		msg = connectionSocket.recv(1024)
		#msgquit = msg.split(">");
		if "\quit" in msg:#msgquit == "\quit":
			end = 1;
			connectionSocket.close();
			print("Waiting for connection: Closed on Client")

		while end == 0:
			#Print message
			print(msg);
			#Get response and send
			response = raw_input(serverHandle+" ");
			while len(response) > 500:
				print("Enter Message <= 500 characters");
				response = raw_input(serverHandle+" ");
			responseFull = serverHandle + response;
			connectionSocket.sendall(responseFull);
			if response == "\quit":
				end = 1;
				connectionSocket.close();
				print("Waiting for connection: Closed on Server")
			else:
				#Wait for message
				msg = connectionSocket.recv(1024);
				if "\quit" in msg:#msgquit == "\quit":
					end = 1;
					connectionSocket.close();
					print("Waiting for connection: Closed on Client")

if __name__ == "__main__":
	main();
