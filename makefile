#compile the source code
all:
	g++ galactic_exchange.cpp -o galactic_exchange.cgi
#clean, asks user if you want to deete the .cgi file created
clean:
	rm -i *.cgi