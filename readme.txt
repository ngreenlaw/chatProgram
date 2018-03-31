Nathan Greenlaw
ONID:greenlan
CS 372
Project 1

chatserve is implemented in python and can be ran using:
	python chatserve.py [port_number]

chatclient is implemented in C and can be compiled using
	gcc -o chatclient chatclient.c

chatclient can be ran using the command
	chatclient [server_address] [port_number]

Example usage the programs take turns sending messages:

1. on flip1.engr.oregonstate.edu run the command
	python chatserve.py 49152

2. on flip2.engr.oregonstate.edu after compiling run the command
	chatclient flip1.engr.oregonstate.edu 49152

3. on chatclient enter username, will take the first 10 characters entered

4. on chatclient send first message, will take the first 500 charaacters entered

5. on chatserver send response, will not allow you to send mroe than 500 characters

6. on chatclient send response, will take the first 500 characters entered

7. Repeat steps 5 and 6 until either client or server sends \quit

8. chatserver will stay open accepting a new connection and chatclient will close

9. close chatserver with ctrl+c SIGINT
