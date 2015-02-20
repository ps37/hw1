cmd: clear
	g++ sserver.cpp -o sserver
	g++ sclient.cpp -o sclient

clear:
	rm -rf *~ sserver sclient