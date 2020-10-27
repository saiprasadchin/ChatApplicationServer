compiler = g++

Server =  server

clean:
	rm -f $(Server) 

all :  $(Server)

$(Server):
		$(compiler) -pthread main/main.cpp Server/TcpListener.cpp Utils/Utils.cpp -o server