# hw1
Author: Prudhvi Chandra Simhadri
Contact Information: pcsimhadri@uh.edu

Files:
sserver.cpp
sclient.cpp
aotp_lib.h
"aotp_lib.h" is the file containing reusable functions required for the server.cpp program.

How to execute:
1. "make" generates 2 executables: 'sclient' and 'sserver'.

2. start the server program as - ./sserver -p port_number.

3. ./sclient -p port_number -h bayou.cs.uh.edu

The server starts initially with the port number specified on the command line. Later, the server changes to the port numbers starting from 5004 with 1 step increments for each port number change.
After the initial connection, the port on which server is currently hearing is displayed by the client.cpp on stdout after displaying the result and before exiting the execution.
Client exits after obtaining the result of a single operation.
Server keeps on running by changing its port numbers and waits for the connections from the client side.
Start the client.cpp again with the new port number to again connect with the server.

Instructions: If there is a problem, please close both the client and server programs and start them again.

Software Description: 
Program Works for more than 2 operands and generates errors for wrong input formats.

First start the server program as - ./sserver -p port_number
The server then prints messages on the stdout to let user know about it’s status.
1.accepts the connection from the client
2.server receives the request
3.checks the request and computes the result
4.server then sends the Change Port Notice to client with port number.
5.receives the ACK for CPN.
6.changes to the new port number
7.sends the result through new port number

start the client program as - ./sclient -p port_number -h bayou.cs.uh.edu
The client then prints messages on the stdout to let user know about it’s status.
1.connects to the server
2.sends the request
3.receives the message
4.if message is CPN, then accepts and sends the ACK for CPN as CPN_ACK 1.
5.connects to the new port number
6.receives the final message on the new port number
7.displays it and exits.
 
Limitations: Cannot service multiple clients at the same time.
