/*
reference beej
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 24872
#define APORT 21872
#define BPORT 22872


	int UDP_Bsocket;
	int UDP_Asocket;
	int sock;

	struct sockaddr_in server_addr, cli_addr, UDPAserver_addr, UDPBserver_addr;
	socklen_t clilen;
	
	int child_sock; //connect to client
	char buff[1024];
	char recemessage[100] = {};
	int rval;
	
	char buff1[1024] = {};
	int  n, len;
	char message_AUDP[1024] = {};
	char AUDPrecemessage[1024] = {};
	socklen_t Aaddr;

	// B parameters
	int m;
	char buff2[1024] = {};
	char message_BUDP[2048] = {};
	char BUDPrecemessage[1024] = {};
	socklen_t Baddr;

	char sendbackclient[2048] = {};



void set_up_socket(){
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		printf("Fail to create a socket.");
		exit(1);
	}
	//printf("Server Socket Created Sucessful.\n");
}

void bind_TCP_socket(){
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORT);

	//call bind(int sock, local address, address length)
	//local address is ip and port num.
	//if it successful return 0 otherwise -1
	//In second parameter we force it to 
	//change from type (sockaddr_in) to type (struct sockaddr*)
	if(bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("TCP Bind Failed");
		exit(1);
	}
	//printf("Bind to the Port Number successful. \n");
}

void listen_TCP_socket(){
	//Listen(socket, queue limit)
 	//if it successful return 0 otherwise -1
 	listen(sock, 5);//listen 5 connection
 	printf("The AWS is up and running.\n");
}
//UDP_A_Server
void set_up_udpAsocket(){
 	if((UDP_Asocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
 		perror("UDP A_SOCKET CREATE FAILED");
 		exit(1);
 	}
 	else{
 		//printf("UDP A_SOCKET CREATE SUCCESSFUL\n" );
 	}
}

void udpAsent(){
	memset(&UDPAserver_addr, 0 , sizeof(UDPAserver_addr));
 	UDPAserver_addr.sin_family = AF_INET;
 	UDPAserver_addr.sin_port = htons(APORT);
 	UDPAserver_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

 	int sent_succesful = sendto(UDP_Asocket, message_AUDP, sizeof(message_AUDP), 0, (struct sockaddr *)& UDPAserver_addr, sizeof(UDPAserver_addr));
 	if(sent_succesful < 0){
 		perror("UDPA SENT FALIED");
 	}
 	else{
 		//printf("UDPA SENT SUCCESSFUL\n");
 	}
}

void udpAreceive(){
	n = recvfrom(UDP_Asocket, AUDPrecemessage, sizeof(AUDPrecemessage), 0, (struct sockaddr* )&UDPAserver_addr, &Aaddr);
 	if(n < 0){
		printf("UDPA Receive failed\n");
		exit(0);
	}
	else{
		//printf("UDPA Receive Successful\n");
		AUDPrecemessage[n] = '\0';
		//puts(AUDPrecemessage);
	}

//////////////////////////////////////////////
	char temp[1024] = {};
	strcpy(temp, AUDPrecemessage);
	char Tt[1024];
	char Tp[1024];
	char n[1024];
	int noden;
	char nodeorinum[1024];
	char minlength[1024];
	char *p;
	p = strtok(temp, " ");

	//copy *type to type buffer[] 
	strcpy(Tt, p);
	//printf("unchage%s\n", File_Size );
	
	//printf("filesize:%lld\n", file_size );
////////////////////////////////////
	p = strtok(NULL, " ");
	if(p){
		strcpy(Tp, p);
		//printf("%s\n", prop );
	}
///////////////////////////////
	p = strtok(NULL, " ");
	if(p){
		strcpy(n, p);
		//printf("%s\n", prop );
	}
	noden = atoi(n);
//////////////////////////////////
	for(int i = 0; i < noden; i++){
	p = strtok(NULL, " ");
	if(p){
		nodeorinum[i] = atoi(p);
		//printf("%s\n", prop );
		}
	}
////////////////////////////////////
	for(int i = 0; i < noden; i++){
	p = strtok(NULL, " ");
	if(p){
		minlength[i] = atoi(p);
		//printf("%s\n", prop );
		}
	}
	printf("The AWS has received shortest path from server A:\n");
	printf("-------------------------------------------------\n");
	printf("Destination\tMin Length\n");
	printf("-------------------------------------------------\n");
	for(int i = 0; i < noden; i++){
		printf("%d\t\t%d\n", nodeorinum[i],minlength[i]);
	}
	printf("-------------------------------------------------\n");
	printf("The AWS has sent path length, propagation speed and transmission speed to server B using UDP over port <%d>\n", BPORT);


	strcat(message_BUDP, AUDPrecemessage);
	//strcat(message_BUDP, blank);
	
}



// UDP_B_Server
void set_up_udpBsocket(){
 	if((UDP_Bsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
 		perror("UDP SOCKET B CREATE FAILED");
 		exit(1);
 	}
 	else{
 		//printf("UDP SOCKET B CREATE SUCCESSFUL\n" );
 	}
}

void udpBsent(){
	memset(&UDPBserver_addr, 0 , sizeof(UDPBserver_addr));
 	UDPBserver_addr.sin_family = AF_INET;
 	UDPBserver_addr.sin_port = htons(BPORT);
 	UDPBserver_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

 //	strcat(message_BUDP, 

 	int sent_succesful = sendto(UDP_Bsocket, message_BUDP, sizeof(message_BUDP), 0, (struct sockaddr *)& UDPBserver_addr, sizeof(UDPBserver_addr));
 	if(sent_succesful < 0){
 		perror("UDPB SENT FALIED");
 	}
 	else{
 		//printf("UDPB SENT SUCCESSFUL\n");
 	}
}

void udpBreceive(){
	m = recvfrom(UDP_Bsocket, BUDPrecemessage, sizeof(BUDPrecemessage), 0, (struct sockaddr* )&UDPBserver_addr, &Baddr);
 	if(m < 0){
		printf("UDPB Receive failed\n");
		exit(0);
	}
	else{
		//printf("UDPB Receive Successful\n");
		BUDPrecemessage[n] = '\0';
		//puts(BUDPrecemessage);
	}
	strcpy(sendbackclient, BUDPrecemessage);


	char node_num[1024];
	memset(node_num, 0 , sizeof(node_num));
	int intnode_num;

	char trans[1024];
	memset(trans, 0 , sizeof(trans));
	double Trans;

	int NODENUMBER_label[1024];
	memset(NODENUMBER_label, 0 , sizeof(NODENUMBER_label));

	float tp[1024];
	memset(tp, 0 , sizeof(tp));

	float toal_delay[1024];
	memset(toal_delay, 0 , sizeof(toal_delay));




	char *q;
	q = strtok(BUDPrecemessage, " ");
	
	//printf("%s\n", p );
	
	//copy *type to type buffer[] 
	strcpy(node_num, q);
	//printf("%s\n", node_num );
	intnode_num = atoi(node_num);
////////////////////////////////////
	q = strtok(NULL, " ");
	if(q){
		strcpy(trans, q);
		//printf("trans,%s\n", trans );
	}
	Trans = atof(trans);
/////////////put original node label(int)////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
			NODENUMBER_label[i] = atoi(q);//////put the strtok in the matrix		
		//printf("node label,%d\n", NODENUMBER_label[i] );
		}
	}
/////////////////put Tt delay(double)///////////////////////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
		tp[i] = atof(q);//////put the strtok in the matrix		
		//printf("Tp,%.2f\n", tp[i] );
		}
	}
/////////////put total delay////////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
		toal_delay[i] = atof(q);//////put the strtok in the matrix		
		//printf("total,%.2f\n", toal_delay[i] );
		}
	}

	printf("The AWS has received delays from server B:\n");
	printf("------------------------------------------\n");
	printf("Destination\tTt\t\tTp\tDelay\n");
	printf("------------------------------------------\n");
	for(int i = 0; i < intnode_num; i++){
		printf("%d\t\t%.2f\t\t%.2f\t%.2f\n", NODENUMBER_label[i], Trans, tp[i], toal_delay[i]);
	}
	printf("------------------------------------------\n");
	printf("The AWS has sent calculated delay to client using TCP over port <%d>\n", PORT);

}








void accept_TCP_socket(){
	clilen = sizeof(cli_addr);
	while(1){
 	child_sock =accept(sock, (struct sockaddr *) &cli_addr, &clilen);
	if(child_sock < 0){
			perror("Accept Failed");
		}
	//printf("Accept Successful\n");

 	
	recv(child_sock, recemessage, sizeof(recemessage), 0);
 	//printf("Receive Successful:%s\n", recemessage);
 	

 	char map_id[1024];
 	memset(map_id, 0 , sizeof(map_id));
 	char Source_Node[1024];
 	memset(Source_Node, 0 , sizeof(Source_Node));
 	char File_Size[1024];
 	memset(File_Size, 0 , sizeof(File_Size));
 	char *p;
 	char blank[] = " ";
 	p = strtok(recemessage, " ");
	//printf("%s\n", p );
	
	//copy *type to type buffer[] 
	strcpy(map_id, p);
	//printf("%s\n", map_id );
	p = strtok(NULL, " ");
	if(p){
		strcpy(Source_Node, p);
		//printf("%s\n", Source_Node );
	}
	p = strtok(NULL, " ");
	if(p){
		strcpy(File_Size, p);
		//printf("%s\n", File_Size );
	}
	// combine mapid and source
	strcpy(message_AUDP, map_id);
	strcat(message_AUDP, blank);
	strcat(message_AUDP, Source_Node);

	strcpy(message_BUDP, File_Size);
	strcat(message_BUDP, blank);
	strcat(message_BUDP, Source_Node);////////////////////////
	strcat(message_BUDP, blank);
	/*while(p = strtok(NULL, " ")){
		printf("%s\n", p);
	}*/
		

	printf("The AWS has received map ID <%s>, start vertex <%s> and file size <%s> from the client using TCP over port <%d>\n", map_id, Source_Node, File_Size, PORT);
	printf("The AWS has sent map ID and starting vertex to server A using UDP over port<%d>\n", APORT);
 	//printf("%s\n", strtok(recemessage, " "));

 	set_up_udpAsocket();
 	udpAsent();
 	udpAreceive();
 	close(UDP_Asocket);

 	set_up_udpBsocket();
 	udpBsent();
 	udpBreceive();
 	close(UDP_Bsocket);



 	//strcpy(buff, BUDPrecemessage);
 	send(child_sock, sendbackclient, sizeof(sendbackclient), 0 );
 	//printf("Send Successful:%s\n", sendbackclient);

 	close(child_sock);
 	}
}


int main(int argc , char *argv[]){


	set_up_socket();
	bind_TCP_socket();
	listen_TCP_socket();
	accept_TCP_socket();

	return 0;
}
