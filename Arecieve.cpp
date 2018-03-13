#define COM3BASE 0x3E8
#define COM4BASE 0x2E8
#define TXDATA COM3BASE
#define LCR (COM3BASE+3) /*0x3F8 line control*/
#define LSR (COM3BASE+5) /*0x3FD line status*/
#include<conio.h>
#include<dos.h>
#include<stdio.h>
#include<string.h>
#define framelen 8

void setup_serial();
void send_character(char ch);
char get_character();

int main(){

setup_serial();

/* reciever */
int i = 0, framecount = 0, waitframe = 0;
char saveFILE[100], recieveFileName[255], frameRecieve[framelen] = {0};
char ack;
FILE *ptrSave;

/* get file name */
do{
    recieveFileName[i] = get_character();
    i++;
}while(recieveFileName[i-1] != '\0');
i = 0;
printf("sender send : %s\n", recieveFileName);

printf("Save as : ");
gets(saveFILE);

while(1){

    /* recieve frame number */
    framecount = (int)(get_character());
    printf("recieve frame : %d\n", framecount);

    /* recieve data */
	i = 0;
	do{
		frameRecieve[i] = get_character();
		i++;
	}while(frameRecieve[i-1] != '\0');
	printf("data : %s\n", frameRecieve);
	
	/* action frame part */
	printf("Actin frame : ");
	ack = getch();
	printf("%c\n", ack);
	
	if(waitframe == framecount){
		waitframe += 1;
		printf("Recieve & ");
		// save data to file
		ptrSave = fopen(saveFILE,"a");
		fprintf(ptrSave, "%s", frameRecieve);
		fclose(ptrSave);
	}
	else{
		printf("Reject & ");
	}

	if(ack == 'a'){
		printf("send ACK%d\n",waitframe);
	}
	else{
		printf("sleep\n");
	}
	

	/* clear frameRecieve */
	for(i = 0; i < strlen(frameRecieve); i++){
		frameRecieve[i] = '\0';
	}
}

return 0;
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