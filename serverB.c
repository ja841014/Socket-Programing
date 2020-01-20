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
#include <netdb.h>
#include <memory.h>

#define PORT 22872
#define MAXLINE 1000

int main(int argc, char *argv[]){
	/*if(argc != 2){
		perror("Usage: %s <port>\n", argv[0]);
		exit(0);
	}
	int port = atoi(argv[1]);*/
while(1){
	int sock;
	struct sockaddr_in my_addr, remote_addr; //built my addr and remote addr.
	//int remoteAddr_len = sizeof(remote_addr);
	char recemessage[1024] = {};
	char message[1024] = {};
	socklen_t remoaddr;
	remoaddr =sizeof(remote_addr);
	//addr_size = sizeof()

	//Create an UDP socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		printf("Fail to create an UDP socket.\n");
		exit(1);
	}
	else{
		//printf("Create UDP Socket Successful\n");
	}
	bzero((char *)& my_addr, sizeof(my_addr));	
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY;
	my_addr.sin_port = htons(PORT);

	//bind server address to socket
	if(bind(sock, (struct sockaddr* )&my_addr, sizeof(my_addr)) < 0){
		perror("Bind Failed\n");
		exit(1);
	}
	else{
		printf("The Server B is up and running using UDP on port <%d>\n",PORT);
	}
	//receive the datagram
	int n = recvfrom(sock, recemessage, sizeof(recemessage), 0, (struct sockaddr* )&remote_addr, &remoaddr);
	if(n < 0){
		printf("Receive failed\n");
		exit(0);
	}
	else{
		printf("The Server B has received data for calculation:\n");
		recemessage[n] = '\0';
		//puts(recemessage);
	}
	

	long long file_size;
	double Prop;
	double Trans;
	int intsource_node;

	char prop[1024];
 	memset(prop, 0 , sizeof(prop));

 	char source_node[1024];
 	memset(source_node, 0, sizeof(source_node));

 	char trans[1024];
 	memset(trans, 0 , sizeof(trans));

	char File_Size[1024];
 	memset(File_Size, 0 , sizeof(File_Size));
	char *p;
 	char blank[] = " ";
 	
 	int min_length[1024];
 	memset(min_length, 0 , sizeof(min_length));
 	int count = 0;

 	char node_num[1024];
 	memset(node_num, 0 , sizeof(node_num));
 	int Node_Num;

 	char NODENUMBER[1024];
 	memset(NODENUMBER, 0 , sizeof(NODENUMBER));



 	p = strtok(recemessage, " ");

	//printf("%s\n", p );
	
	//copy *type to type buffer[] 
	strcpy(File_Size, p);
	//printf("unchage%s\n", File_Size );
	file_size = atof(File_Size);
	//printf("filesize:%lld\n", file_size );
////////////////////////////////////
	p = strtok(NULL, " ");
	if(p){
		strcpy(source_node, p);
		//printf("%s\n", prop );
	}
	intsource_node = atoi(source_node);
////////////////////////////////////
	p = strtok(NULL, " ");
	if(p){
		strcpy(prop, p);
		//printf("%s\n", prop );
	}
	Prop = atof(prop);
	//printf("Prop,%.2f\n", Prop );
///////////////////////////////////
	p = strtok(NULL, " ");
	if(p){
		strcpy(trans, p);
		//printf("%s\n", trans );
	}
	Trans = atof(trans);
	//printf("Trans%.2f\n", Trans );
/////////////////////////////////
	p = strtok(NULL, " ");
	if(p){
		strcpy(node_num, p);
		//printf("%s\n", node_num );
	}
	Node_Num = atoi(node_num);


	for(int i = 0; i < Node_Num; i++){
		p = strtok(NULL, " ");
		if(p){
			NODENUMBER[i] = atoi(p);//////put the strtok in the matrix		
		//printf("%d\n", NODENUMBER[i] );
		}
	}


	for(int i = 0; i < Node_Num; i++){
		p = strtok(NULL, " ");
		if(p){
		min_length[i] = atoi(p);
		//strcpy(min_length[i], p); //////put the strtok in the matrix
		//printf("%d\n", min_length[i] );
		//count++;
		}
	}


	printf("* Propagation speed: <%.2f> km/s\n",Prop);
	printf("* Transmission speed: <%.2f> Bytes/s\n",Trans);
	for(int i = 0; i < Node_Num; i++){
		if(intsource_node != NODENUMBER[i]){
		printf("* Path length for destination <%d>: <%d>\n", NODENUMBER[i], min_length[i]);
		}
		else{
			//printf("* Path length for destination <%d>: <%d>\n", NODENUMBER[i], min_length[i]);
		}
	}
	printf("The Server B has finished the calculation of the delays:\n");
	printf("--------------------------------------------------------\n");
	printf("Destination\tDelay\n");
	printf("--------------------------------------------------------\n");

	int Min_Length[Node_Num];
	double  Propagation_all[Node_Num];
	double  Transmission = 0;
	

	for(int i = 0; i < Node_Num; i++){
		//printf("nodenum = %d\n", Node_Num);
		Min_Length[i] = min_length[i];
		Propagation_all[i] = (Min_Length[i])/Prop;
		//printf("prop,%.2f\n",Propagation_all[i]);
	}
	file_size = file_size/8;
	//printf("filesize%lld\n",file_size );
	Transmission = file_size/Trans;
	//printf("trans,%f\n", Transmission);
	

	float Delay[Node_Num];
	memset(Delay, 0, sizeof(Delay));
	for(int i = 0; i < Node_Num; i++){
		//printf("nodenum = %d\n", Node_Num);
		Delay[i] = Propagation_all[i] + Transmission;
		//printf("total,%.2f\n",Delay[i]);
	}

	for(int i = 0; i < Node_Num; i++){
		if(intsource_node != NODENUMBER[i]){
		printf("%d\t\t%.2f\n",NODENUMBER[i], Delay[i]);
		}
		else{
			//printf("%d\t\t%.2f\n",NODENUMBER[i], Delay[i]);
		}
	}
	printf("--------------------------------------------------------\n");


	char buff[1024];
	char buff1[1024];
	char buff2[1024];
	char buff3[1024];
	char buff4[1024];
	char buff5[1024];
	//char blank[] = " ";
	memset(buff, 0, sizeof(buff));


	
	///transmit node number
	strcpy(message, node_num);
	strcat(message, blank);
	//transmit transmission delay
	sprintf(buff1, "%f", Transmission);
	strcat(message, buff1);
	strcat(message, blank);
	//transmit original destination node label
	for(int i = 0; i < Node_Num; i++){
		sprintf(buff4, "%d", NODENUMBER[i]);
		strcat(message, buff4);
		strcat(message, blank);
	}
	//transmit propagation delay
	for(int i = 0; i < Node_Num; i++){
		sprintf(buff2, "%f", Propagation_all[i]);
		strcat(message, buff2);
		strcat(message, blank);
	}
	//transmit total delay
	for(int i = 0; i < Node_Num; i++){
		sprintf(buff3, "%f", Delay[i]);
		strcat(message, buff3);
		strcat(message, blank);
	}
	/////minimum length///////
	for(int i = 0; i < Node_Num; i++){
		sprintf(buff5, "%d", min_length[i]);
		strcat(message, buff5);
		strcat(message, blank);
	}

	//send the response
	int a = sendto(sock, message, sizeof(message), 0, (struct sockaddr* )&remote_addr, sizeof(remote_addr));
	if(a < 0){
		printf("Send Failed\n");
		exit(0);
	}
	else{
		printf("The Server B has finished sending the output to AWS\n");
	}

	close(sock);
}
}
