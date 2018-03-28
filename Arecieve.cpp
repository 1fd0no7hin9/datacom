#define COM3BASE 0x3E8
#define COM4BASE 0x2E8
#define TXDATA COM3BASE
#define LCR (COM3BASE+3) /*0x3F8 line control*/
#define LSR (COM3BASE+5) /*0x3FD line status*/
#include<conio.h>
#include<dos.h>
#include<stdio.h>
#include<string.h>
#define framelen 10

void setup_serial();
void send_character(char ch);
char get_character();

int main(){

setup_serial();

/* reciever */
int i = 0, waitframe = 0, mode = 0;
char saveFILE[100], recieveFileName[255], frameRecieve[framelen] = {0};
char ack;
int eof = 0;
FILE *ptrSave;

/* get file name */
printf("sender send : ");
do{
    recieveFileName[i] = get_character();
    i++;
}while(recieveFileName[i-1] != '\0');
i = 0;
printf("%s\n", recieveFileName);

printf("Save as : ");
gets(saveFILE);
printf("Wait for data...\n\n");

while(1){

    /* recieve data */
	i = 0;
	do{
		frameRecieve[i++] = get_character();
		if(frameRecieve[i-1] == 6){
			mode = 1;
			frame[i-1] = '\0'
			break;
		}
	}while(frameRecieve[i-1] != '\0');

	printf("frame recieve : ");
	printf("%d",(int)frameRecieve[0]);
	printf("data : ");
	for(i = 1; i < strlen(frameRecieve); i++){
		printf("%c", frameRecieve[i]);
	}
	printf("\n");

	/* action frame part */
	printf("Action frame : ");
	ack = getch();
	printf("%c\n", ack);

	if(waitframe == (int)frameRecieve[0]){
		printf("Recieve & ");
		waitframe ^= 1;
		// save data to file
		ptrSave = fopen(saveFILE,"a");
		for(i = 1;i < framelen; i++){
			fprintf(ptrSave, "%c", frameRecieve[i]);
		}
		fclose(ptrSave);
	}
	else{
		printf("Reject & ");
	}
	
	if(ack == 'a'){
		printf("send ACK%d\n\n",waitframe);
	}
	else{
		printf("sleep\n");
	}

	if(mode == 1){
		send_character(6);
		return 0;
	}
	else {
		send_character('\0');
	}
}

}

void setup_serial(){
	outportb(LCR, 0x80);
	outportb(TXDATA, 0x0C);
	outportb(TXDATA + 1, 0x00);
	outportb(LCR, 0x0A);
}

void send_character(char ch){
	int status;
	do{
		status = inportb(LSR) & 0x40;
	}while(status != 0x40);
	outportb (TXDATA, ch);
}

char get_character(){
	int status;
	do{
		status = inportb(LSR) & 0x01;
	}while(status != 0x01);
return (inportb(TXDATA));
}