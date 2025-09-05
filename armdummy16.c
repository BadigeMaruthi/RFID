#include<errno.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include<unistd.h>
#include "uart.h"
#include<stdio_ext.h>
#include<time.h>
	int flag=1;
	char CurrentDate[12];
	char timeBuffer[12];
	char buff[11];
	char NAME[20];
	time_t rawTime;         // Variable to store the raw calendar time
        struct tm *localTime;   // Pointer to a structure that holds broken-down time components
//      char CurrentDate[11];
    /*    rawTime = time(NULL);
        localTime = localtime(&rawTime);
        strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localTime);*/


typedef struct userDetails{
    char UserId[11];
    char UserName[20];
    struct userDetails *link;
}UserDetails;

typedef struct Attendence {
    char UserId[11];
    char UserName[20];
    char Date[11];
    char workingHours[6];
    char status;
    char In[6];
    char Out[6];
    struct Attendence *link;
}attendence;

UserDetails* user_sync_file(UserDetails* temp){
        attendence v;
        //int i,j;
        FILE* fp=NULL;
        fp=fopen("userDetails1.txt","r");


        if(fp==NULL){
                printf("file is not existed\n");
                return temp;
        }
	//"%49[^,],%49[^,],%19[^,],%9[^,],%c,%9[^,],%9[^\n]\n"
	 while (fscanf(fp, "%s %s \n", v.UserId, v.UserName) == 2) {
	       
	//	 if (strcmp(v.UserId, "UserId") == 0) {
        //continue;  // Skip the header line
   // }

		printf("UserId: %s, UserName: %s\n", v.UserId, v.UserName);
        	UserDetails *node=malloc(sizeof(UserDetails));
		
                strcpy(node->UserId,v.UserId);
                strcpy(node->UserName,v.UserName);

                if(temp==NULL){
                        node->link=NULL;
                        temp=node;
                }
                else{
                        node->link=temp;
                        temp=node;
                }
        }
       fclose(fp);
        return temp;
}


UserDetails* Usersave(UserDetails* head){
        FILE* fp=NULL;
	UserDetails*t=head;
        if(head==NULL){
                printf("nothing to save on file\n");
                return head;
        }
        fp=fopen("userDetails1.txt","w");
//	  fprintf(fp, "UserId,UserName,Status\n");
        while(head!=NULL){
		//printf("hiiii\n");
                fprintf(fp,"%s %s \n",head->UserId,head->UserName);
                head=head->link;
        }
        fclose(fp);
	return t;
}

attendence* sync_file(attendence* temp){
        attendence v;
        FILE* fp=NULL;
        fp=fopen("readme5.csv","r");
        if(fp==NULL){
                printf("file is not existed\n");
                return temp;
        }
	char ch;
	while((ch=fgetc(fp))!=EOF){
		if(ch=='\n')
			break;
	}	
	 while (fscanf(fp, "%11[^,],%20[^,],%11[^,],%6[^,], %c,%6[^,],%6[^\n]\n", v.UserId, v.UserName,v.Date,v.workingHours,&v.status,v.In,v.Out) == 7) {
	      
	printf("UserId: %s, UserName: %s,Date:%s,workingHours: %s ,Status: %c In: %s,Out:%s\n", v.UserId, v.UserName,v.Date,v.workingHours ,v.status,v.In,v.Out);
                attendence *node=malloc(sizeof(attendence));
                //printf("%s %s %c\n",v.UserId,v.UserName,v.status);

                strcpy(node->UserId,v.UserId);
                strcpy(node->UserName,v.UserName);
		strcpy(node->Date,v.Date);
		strcpy(node->workingHours,v.workingHours);
		strcpy(node->In,v.In);
		strcpy(node->Out,v.Out);
                node->status=v.status;

                if(temp==NULL){
                        node->link=NULL;
                        temp=node;
                }
                else{
                        node->link=temp;
                        temp=node;
                }
        }
       fclose(fp);
        return temp;
}

//printing the data
int PrintAttendence(attendence* temp){
        attendence v;
        FILE* fp=NULL;
        fp=fopen("readme5.csv","r");
        if(fp==NULL){
                printf("file is not existed\n");
                return temp;
        }
        char ch;
        while((ch=fgetc(fp))!=EOF){
                if(ch=='\n')
                        break;
        }
         while (fscanf(fp, "%11[^,],%20[^,],%11[^,],%6[^,], %c,%6[^,],%6[^\n]\n", v.UserId, v.UserName,v.Date,v.workingHours,&v.status,v.In,v.Out) == 7) {

        printf("UserId: %s, UserName: %s,Date:%s,workingHours: %s ,Status: %c In: %s,Out:%s\n", v.UserId, v.UserName,v.Date,v.workingHours ,v.status,v.In,v.Out);
                attendence *node=malloc(sizeof(attendence));
                //printf("%s %s %c\n",v.UserId,v.UserName,v.status);

                strcpy(node->UserId,v.UserId);
                strcpy(node->UserName,v.UserName);
                strcpy(node->Date,v.Date);
                strcpy(node->workingHours,v.workingHours);
                strcpy(node->In,v.In);
                strcpy(node->Out,v.Out);
                node->status=v.status;

                if(temp==NULL){
                        node->link=NULL;
                        temp=node;
                }
                else{
                        node->link=temp;
                        temp=node;
                }
        }
       fclose(fp);
        return 1;
}


attendence* save(attendence* head){
        FILE* fp=NULL;
	attendence* ptr=head;
        if(head==NULL){
                printf("nothing to save on file\n");
                return head;
        }
        fp=fopen("readme5.csv","w");
//	  fprintf(fp, "UserId,UserName,Status\n");
	  fprintf(fp, "UserId,UserName,Date,workingHours,status,In,Out\n");
        while(head!=NULL){
	//	printf("hello\n");
                fprintf(fp,"%s,%s,%s,%s,%c,%s,%s\n",head->UserId,head->UserName,head->Date,head->workingHours,head->status,head->In,head->Out);
                head=head->link;
        }
        fclose(fp);
	return ptr;
}

int checkAdmin( UserDetails* temp,char *UserId){
        while(temp!=NULL){
                if((strcmp(temp->UserId,UserId))==0){
			strcpy(NAME,temp->UserName);
                        return 1;
                }
                temp=temp->link;
        }
        return 0;
}

int  checkUserId(attendence **ptemp,char* userId,UserDetails *user){
	attendence* prev=NULL,*node;
	attendence *temp=*ptemp;
	//checking for user id in btabase or not if present 
	//check for csv file for attendence
	if(checkAdmin(user,userId)==0){
		printf("user details card is not matched\n");
		flag=0;
		return 0;
	}
        //checking csv file
	strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localTime);
	strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);

	char Inbuff[6],Outbuff[6];
	int Intime_H=0,Intime_M=0,Outtime_H=0,Outtime_M=0,work_H=0,work_M=0;
	//New date means no person in attendence so temp is null then
	if(temp==NULL){
		printf("file contain no data checkuserid\n"); 
		node=malloc(sizeof(attendence));
        	strcpy(node->UserId,userId);
        	strcpy(node->UserName,NAME);
        	node->status='0';
        	strncpy(node->Date,CurrentDate,10);
        	strcpy(node->workingHours,"0");
        	strncpy(node->In,timeBuffer,5);
        	strcpy(node->Out,"0");
        	node->link=NULL;
		//update the address location
		*ptemp=node;
		return 1;

	}

	while(temp!=NULL){
                if((strcmp(temp->UserId,userId))==0){
			//strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localTime);
			//strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
                        if(temp->status=='1'){
				strncpy(temp->In,timeBuffer,5);
				temp->status='0';
			}
			else{
				strncpy(temp->Out,timeBuffer,5);
				strcpy(Inbuff,temp->In);
				strcpy(Outbuff,temp->Out);
				Intime_H=((Inbuff[0]-48)*10)+(Inbuff[1]-48);
				Intime_M=((Inbuff[3]-48)*10)+(Inbuff[4]-48);
				Outtime_H=((Outbuff[0]-48)*10)+(Outbuff[1]-48);
				Outtime_M=((Outbuff[3]-48)*10)+(Outbuff[4]-48);
				work_H=Outtime_H-Intime_H;
				work_M=Outtime_M-Intime_M;
				printf("working time is %d :%d\n",work_H,work_M);
				 Outbuff[1]=work_H%10+48;
				 Outbuff[0]=work_H/10+48;
				 Outbuff[2]=':';
                                 Outbuff[4]=work_M%10+48;
				 Outbuff[3]=work_M/10+48;
                                temp->status='1';
				strcpy(temp->workingHours,Outbuff);
				strncpy(temp->Date,CurrentDate,10);
			}


			return 1;
                }
		prev=temp;
                temp=temp->link;
        }
	node=malloc(sizeof(attendence));
	strcpy(node->UserId,userId);
	strcpy(node->UserName,NAME);
	node->status='0';
        strncpy(node->Date,CurrentDate,10);
        strcpy(node->workingHours,"0");
        strncpy(node->In,timeBuffer,5);
        strcpy(node->Out,"0");
        node->link=NULL;
	prev->link=node;
        


	//printf("sorry card is not found\n");
	flag=1;
        return 1;

}

UserDetails* addUser(UserDetails* temp){
	puts("place card");
	scanf("%s",buff);
	if(checkAdmin(temp,buff)){
			puts("sorry user id already existed");
			return temp;
	}
	else{

		UserDetails* node=malloc(sizeof(UserDetails));
		if(temp==NULL){
			
			strcpy(node->UserId,buff);
			__fpurge(stdin);
			puts("enter username");
			scanf("%s",node->UserName);
			node->link=NULL;
			temp=node;
			return temp;
		}
		else{
			strcpy(node->UserId,buff);
			__fpurge(stdin);
			puts("enter username");
			scanf("%s",node->UserName);
			node->link=temp;
			temp=node;
		}
	}
	return temp;
}

int editUser(UserDetails* temp){
	char userId[11];
	char NewName[20];
	char ch;
	puts("enter card you want modify");
	__fpurge(stdin);
	scanf("%s",userId);
	while(temp!=NULL){
                if((strcmp(temp->UserId,userId))==0){
			printf("Enter choice to which one you want edit\n");
			printf("1.Edit userId\n2.Edit UserName\n");
			__fpurge(stdin);
			scanf("%c",&ch);
			if(ch=='1'){
				puts("enter new id number");
				__fpurge(stdin);
				scanf("%s",userId);
				strcpy(temp->UserId,userId);
			}
			else if(ch=='2'){
				puts("Enter Name");
				scanf("%s",NewName);
				strcpy(temp->UserName,NewName);
			}
			return 1;
		}

		temp=temp->link;
	}
	printf("Sorry UserId Not Matched\n");
}

UserDetails* deleteUser(UserDetails* current){
	char Id[11];
	printf("Enter user id to Delete\n");
	__fpurge(stdin);
	scanf("%s",Id);

	UserDetails* temp=current,*prev=NULL;
	if(current==NULL){
		printf("No node to delete\n");
		return prev;
	}
	else if((current!=NULL)&&(strcmp(current->UserId,Id)==0)){
		if(current->link!=NULL){
			prev=current->link;
		}
		free(current);
		return prev;
	}
	else{
		while(current!=NULL){
			if(strcmp(current->UserId,Id)==0){
				if(current->link!=NULL){
					prev->link=current->link;
					free(current);
				}
				else{
					free(current);
					prev->link=NULL;
				}
				return temp;
			}
			prev=current;
			current=current->link;
		}
		printf("sorry! user Id is not matchrd\n");
		return temp;
	}

}
int Readmain() {
	rawTime = time(NULL);
        localTime = localtime(&rawTime);
        strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localTime);
	strftime(timeBuffer,sizeof(timeBuffer),"%H:%M:%S",localTime);
	printf("%s %s\n",CurrentDate,timeBuffer);


	char ch;
/*	FILE* fp=NULL;
	fp=fopen("readme4.csv","w");
	fprintf(fp,"A1234$,HARISH,2025-06-28,0,1,0,0\n");
	fprintf(fp,"A1237$,RAMU,2025-06-28,0,1,0,0\n");
	fprintf(fp,"A1236$,RAMBABU,2025-06-28,0,1,0,0\n");
	fprintf(fp,"A1235$,SAMABA,2025-06-28,0,1,0,0\n");
    fclose(fp);*/
        UserDetails *userHead=NULL;
	userHead=user_sync_file(userHead);
	userHead=Usersave(userHead);

	attendence * head=NULL;
      	head=sync_file(head);
//      head=save(head);
 //  puts("waiting for card");
  // scanf("%s",buff);
    if(buff[0]=='A'){
	    printf("matched\n");
	    printf("MENU:\n 1. ADD USER\n 2. DELETE USER\n 3. EDIT USER\n 4. EXIT 5.PRINT_ATTENDENCE\n");
	    printf("Enter your choice\n");
	    __fpurge(stdin);
	    scanf("%c",&ch);
	    switch(ch){
	   	case '1': userHead=addUser(userHead);userHead=Usersave(userHead);break;
		case '2':userHead=deleteUser(userHead);userHead=Usersave(userHead);break;
	        case '3':editUser(userHead);userHead=Usersave(userHead);break;
		case '4':userHead=Usersave(userHead);exit(0);
//		case '4':userHead=Usersave(userHead);break;
		case '5':PrintAttendence(head);break;
	    }
    }
    else if(checkUserId(&head,buff,userHead)){
	    printf("user founded check status\n");
    }
    else{
    	printf("card is not valid, place again\n");
    }
/*    if(flag==1)
    	printf("card readed is-->%s\n",buff);
    else 
	    printf("not matched\n");
	    */
    save(head);
  
    return 0;

}

void send_response(int fd,const char* msg){
	for(int i=0;msg[i];i++){
		serialPutchar(fd,msg[i]);
	}
	serialPutchar(fd,'@');
}

/*int main()
{
	rawTime = time(NULL);
        localTime = localtime(&rawTime);
        strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localTime);
        strftime(timeBuffer,sizeof(timeBuffer),"%H:%M:%S",localTime);
	char dayOfWeekChar = localTime->tm_wday;
	CurrentDate[10]=dayOfWeekChar;
        printf("%s %s %d\n",CurrentDate,timeBuffer,dayOfWeekChar);

        int fd ;
        int tx,rx ,i=0;
        puts("Opening serial port\n");

        if ((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0)
        {

                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
                return 1 ;
        }

        puts("serial port is opened\n");
	send_response(fd,CurrentDate);
        usleep(2000000);
	send_response(fd,timeBuffer);
	
        while(1){
        do
        {
               printf("waiting card\n");
                rx=serialGetchar(fd);
                buff[i++]=rx;
               // printf ("recvd : %c\n",rx) ;
        }while(rx!='$');
        buff[i]='\0';
        printf("Readed %s\n",buff);
        i=0;
      Readmain();
     if(flag==1){
	     //printf("111111\n");
     	 send_response(fd,buff);
    }
    else{
	 //   printf("harish\n");
	     send_response(fd,"Invalid");
        }
	flag=1;
	}
        serialClose(fd);
        puts("Closing serial port\n");
        return 0 ;
}
void send_response(int fd,const char* msg){
        for(int i=0;msg[i];i++){
                serialPutchar(fd,msg[i]);
        }
        serialPutchar(fd,'@');
}*/

int main ()
{
        rawTime = time(NULL);
        localTime = localtime(&rawTime);
        strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localTime);
        strftime(timeBuffer,sizeof(timeBuffer),"%H:%M:%S",localTime);
        char dayOfWeekChar = localTime->tm_wday;
        CurrentDate[10]=dayOfWeekChar;
        printf("%s %s %d\n",CurrentDate,timeBuffer,dayOfWeekChar);

        int fd ;
        int tx,rx ,i=0;
        puts("Opening serial port\n");

        if ((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0)
        {

                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
                return 1 ;
        }

        puts("serial port is opened\n");
        send_response(fd,CurrentDate);
        usleep(2000000);
        send_response(fd,timeBuffer);

        while(1){
        do
  //	printf("waiting card\n");
  	{
//               printf("waiting card\n");
                rx=serialGetchar(fd);
                buff[i++]=rx;
               // printf ("recvd : %c\n",rx) ;
        }while(rx!='$');
        buff[i]='\0';
        printf("Readed %s\n",buff);
        i=0;
      Readmain();
     if(flag==1){
             //printf("111111\n");
         send_response(fd,buff);
    }
    else{
         //   printf("harish\n");
             send_response(fd,"Invalid");
        }
        flag=1;
        }
        serialClose(fd);
        puts("Closing serial port\n");
        return 0 ;
}

