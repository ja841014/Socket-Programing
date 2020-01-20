# Socket-Programing

## a. What your code files are and what each one of them does?  
   ### Backend-Server A:  
   	I. Received MAP_ID and start vertex from AWS by using UDP.  
   	II. Read a map.txt which storing the distance information of the city, propagation speed and transmission speed.  
   	III. Found out the target MAP_ID, number of vertices and number of edges.find out the target MAP_ID, number of vertices and number of edges.  
	IV. Used Dijkstra Algorithm to find the shortest path from an assigned vertex and print the minimum length of the each destination out.
	V. Sent the result, propagation and transmission speed back to AWS.
   ### Backend-Server B: 
	I. Received the propagation speed, transmission speed, File_Size and minimum length of each destination from AWS by using UDP.
	II. Calculated Tt, Tp and End-to-End delay of each destination from start vertex.
	III. Sent the result back to AWS. 
   ### AWS:
	I. Recevied MAP_ID, start vertex and File_size from client by using TCP.
	II. Sent Map_ID and start vertex to server A; received the result from serverA by using UDP.
	III. Sent File_Size, propagation speed and transmission speed to server B; received the result from server B by using UDP.
	IV. Sent the total result back to client by using TCP.
   ### Client:
	I. Sent MAP_ID, start vertex and File_Size to AWS; Received the result from AWS by using TCP.
	
## b. The format of all the messages exchanged
   ### Backend-Server A:
	The Server A is up and running using UDP on port<>.  
	The Server A has constructed a list of <number> maps:  
	--------------------------------  
	MapID 	Num Vertices	Num Edges  
	---------------------------------  
	data 		data	     data  
	---------------------------------  
	The Server A has received input for finding shortest paths:   
	---------------------------------  
	Destination	       Min length  
	---------------------------------  
	data 			     data  
	---------------------------------  
	The Server A has sent shortest paths to AWS.  
   ### Backend-Server B:
	The Server B is up and running using UDP on port<>.
	The Server B has received data for calculation:
	* Propagation speed: < > Km/s
	* Transmission speed: < > Bytes/s
	* Path length for destination <vertex> <Length>
	The Server B has finished the calculation of the delays:
	---------------------------------
	Destination		Delay
	---------------------------------
	Data			Data
	---------------------------------
	The Server B has finished sending the output to AWS.
   ### AWS:
	The AWS is up and running.
	The AWS has received map ID < >, start vertex < > and file size < > from the client using TCP over port < >.
	The AWS has sent map ID and starting vertex to server A using UDP over port< >
	The AWS has received shortest path from server A:
	---------------------------------
	Destination	      Min Length
	---------------------------------
	data			data
	---------------------------------
	The AWS has sent path length, propagation speed and transmission speed to server B using UDP over port < >
	The AWS has received delays from server B:
	---------------------------------
	Destination	Tt	Tp	Delay
	---------------------------------
	data		data	data	data
	---------------------------------
	The AWS has sent calculated delay to client using TCP over port < >.
   ### Client:
	The client is up and running.
	The client has sent query to AWS using TCP: start vertex < >; map < >; file size < >.
	The client has received results from AWS:
	---------------------------------
	Destination	Min Length	Tt	Tp	Delay
	---------------------------------
	data		data		data	data	data
	---------------------------------
	
## c. Any idiosyncrasy of your project. It should say under what conditions the project fails of any.
	*The each destination from the start vertex might not be in order.
	Reference:
		beej website
		Implement HASPMAP in C (kaushikbaruah.com)
		Socket programming example in Google
		Socket programming example in Youtube
