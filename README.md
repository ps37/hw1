# hw1
Author: Prudhvi Chandra Simhadri
Contact Information: pcsimhadri@uh.edu

Description:
First start the server program as - ./sserver -p port_number
The server then prints messages on the stdout to let user know about it’s status.
1.accepts the connection from the client
2.server receives the request
3.checks the request and computes the result
4.server then sends the Change Port Notice to client with port number.
5.receives the ACK for CPN.
6.changes to the new port number
7.sends the result through new port number

start the client program as - ./sclient -p 5000 -h bayou.cs.uh.edu
The client then prints messages on the stdout to let user know about it’s status.
1.connects to the server
2.sends the request
3.receives the message
4.if message is CPN, then accepts and sends the ACK for CPN as CPN_ACK 1.
5.connects to the new port number
6.receives the final message on the new port number
7.displays it and exits.

The server starts initially on the port number specified on the command line. Later, the server changes to the port numbers starting from 5004 with 1 step increments for each change.

Instructions: If there is a problem, please close both the client and server programs and start them again.
 
Limitations: Cannot service multiple clients at the same time.   
