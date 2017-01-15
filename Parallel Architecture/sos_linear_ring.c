#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<math.h>

#define NO_OF_PROCESSOR 16

static int total_sets;

//structure to pass values to processor_thread
struct processor_buffer
{
	int processor_id; //processor identifier
	long queue_id;	  //message queue identifier 	
	int * numbers;	  //array for finding subset	
	int size;	  //size of array	
	int target_sum;   //sum to be achieved in subset
};

//structure to pass value from one processor to another
struct message_buffer
{
	long mess_type;      //message identifier in queue
	int size_of_subset;  //size of subset to be passed between processor 
};

//function to be used for printing error messages
void error(char* msg)
{
	fprintf(stderr,"%s %s \n",msg,strerror(errno));
	exit(1);
}

//function for printing subset of given size
void print_all_subset(int numbers[],size_t size,int begin_index, int print[],int target_sum,int processor_id,int size_of_subset)
{
	if(begin_index == size)
        {	
		int sum=0,i,count=0;
		int arr[size];
        	for(i = 0; i<size; i++)
        	{
       			if (print[i]) 
		        {
				arr[count++]=numbers[i];
				sum+=numbers[i];
			}
       	        }
		if(sum==target_sum && count==size_of_subset)
		{
			total_sets++;
			printf("\n set printed by processor %d  \t",processor_id);
			for (i=0;i<count;i++)
			{
				printf("%d ",arr[i]); 
				
			}
		}
        return;
    	}
    	print[begin_index] = 1;
    	print_all_subset(numbers, size, begin_index + 1, print,target_sum,processor_id,size_of_subset);
    	print[begin_index] = 0;
    	print_all_subset(numbers, size, begin_index + 1, print,target_sum,processor_id,size_of_subset);
}

//fuction to be executed by each thread_processor
void *thread_function(void *proc_buf)
{
	int receiver_id,ite;
	struct processor_buffer* proc_buffer = (struct processor_buffer *)proc_buf;
	struct message_buffer msg_buf ,recv_buf;
	long queue_id= proc_buffer->queue_id;
	int processor_id= proc_buffer->processor_id;
	int *numbers=proc_buffer->numbers;
	int size=proc_buffer->size;
	int target_sum=proc_buffer->target_sum;
	int *print = (int *)malloc(size * sizeof(int));	
	int p=NO_OF_PROCESSOR;	
	if(processor_id==0)
	{
			int receiver_id=1;
			int size_of_subset=2;
			msg_buf.mess_type=1;
			msg_buf.size_of_subset=receiver_id+2;
			print_all_subset(numbers, size, 0, print,target_sum,processor_id,size_of_subset);
			printf("\n Size of subset passed by %d is %d for %d ",processor_id,msg_buf.size_of_subset,receiver_id);
			if(msgsnd(queue_id, &msg_buf,sizeof(struct message_buffer),0) == -1)
                   	{   
				char *msg;
				sprintf(msg,"\n error on sending data from processor %d for processor %d ",processor_id,receiver_id);	
                    	 	error(msg);    
                  	 }
			
			receiver_id=p-1;
			msg_buf.mess_type=receiver_id;
			msg_buf.size_of_subset=p+1;
			printf("\n Size of subset passed by %d is %d for %d  ",processor_id,msg_buf.size_of_subset,receiver_id);
			if(msgsnd(queue_id, &msg_buf,sizeof(struct message_buffer),0) == -1)
                   	{   
				char *msg;
				sprintf(msg,"\n error on sending data from processor %d for processor %d ",processor_id,receiver_id);	
                    	 	error(msg);    
                  	 }
					
	}
	else if(processor_id==p-1)
	{
			int sender_id=0;			
			if(msgrcv(queue_id, &recv_buf,sizeof(struct message_buffer),processor_id, 0) == -1)
                        {   
                   	   	char *msg;
				sprintf(msg,"\n error on receiving data from processor %d for processor %d ",sender_id,processor_id);	
                    	 	error(msg);
                 	}
			int size_of_subset=recv_buf.size_of_subset;
			printf("\n Size of subset received by %d is %d form %d ",processor_id,size_of_subset,sender_id);
			print_all_subset(numbers, size, 0, print,target_sum,processor_id,size_of_subset);	
	
			int receiver_id=p-2;
			msg_buf.mess_type=receiver_id;
			msg_buf.size_of_subset=p;
			printf("\n Size of subset passed by %d is %d for %d ",processor_id,msg_buf.size_of_subset,receiver_id);
			if(msgsnd(queue_id, &msg_buf,sizeof(struct message_buffer),0) == -1)
                   	{   
				char *msg;
				sprintf(msg,"\n error on sending data from processor %d for processor %d ",processor_id,receiver_id);	
                    	 	error(msg);    
                  	 }		

	}
	else if(processor_id==p/2)
	{
			int sender_id=p/2-1;			
			if(msgrcv(queue_id, &recv_buf,sizeof(struct message_buffer),processor_id, 0) == -1)
                        {   
                   	   	char *msg;
				sprintf(msg,"\n error on receiving data from processor %d for processor %d ",sender_id,processor_id);	
                    	 	error(msg);
                 	}
			int size_of_subset=recv_buf.size_of_subset;
			printf("\n Size of subset received by %d is %d form %d ",processor_id,size_of_subset,sender_id);
			print_all_subset(numbers, size, 0, print,target_sum,processor_id,size_of_subset);
	
	}
	else if(processor_id==p/2+1)
	{
			int sender_id=(p/2+2)%p;			
			if(msgrcv(queue_id, &recv_buf,sizeof(struct message_buffer),processor_id, 0) == -1)
                        {   
                   	   	char *msg;
				sprintf(msg,"\n error on receiving data from processor %d for processor %d ",sender_id,processor_id);	
                    	 	error(msg);
                 	}
			int size_of_subset=recv_buf.size_of_subset;
			printf("\n Size of subset received by %d is %d form %d ",processor_id,size_of_subset,sender_id);
			print_all_subset(numbers, size, 0, print,target_sum,processor_id,size_of_subset);
	}
	else if(1<=processor_id && processor_id <=p/2-1)
	{
			int sender_id=processor_id - 1;			
			if(msgrcv(queue_id, &recv_buf,sizeof(struct message_buffer),processor_id, 0) == -1)
                        {   
                   	   	char *msg;
				sprintf(msg,"\n error on receiving data from processor %d for processor %d ",sender_id,processor_id);	
                    	 	error(msg);
                 	}
			int size_of_subset=recv_buf.size_of_subset;
			printf("\n Size of subset received by %d is %d form %d ",processor_id,size_of_subset,sender_id);
			print_all_subset(numbers, size, 0, print,target_sum,processor_id,size_of_subset);

			int receiver_id=processor_id + 1;
			msg_buf.mess_type=receiver_id;
			msg_buf.size_of_subset=receiver_id+2;
			printf("\n Size of subset passed by %d is %d for %d ",processor_id,msg_buf.size_of_subset,receiver_id);
			if(msgsnd(queue_id, &msg_buf,sizeof(struct message_buffer),0) == -1)
                   	{   
				char *msg;
				sprintf(msg,"\n error on sending data from processor %d for processor %d ",processor_id,receiver_id);	
                    	 	error(msg);    
                  	 }
	}
	else if(p/2+1<=processor_id && processor_id<=p-2)
	{
			int sender_id=processor_id +1;			
			if(msgrcv(queue_id, &recv_buf,sizeof(struct message_buffer),processor_id, 0) == -1)
                        {   
                   	   	char *msg;
				sprintf(msg,"\n error on receiving data from processor %d for processor %d ",sender_id,processor_id);	
                    	 	error(msg);
                 	}
			int size_of_subset=recv_buf.size_of_subset;
			printf("\n Size of subset received by %d is %d form %d ",processor_id,size_of_subset,sender_id);
			print_all_subset(numbers, size, 0, print,target_sum,processor_id,size_of_subset);
			
			int receiver_id=processor_id -1;
			msg_buf.mess_type=receiver_id;
			msg_buf.size_of_subset=receiver_id+2;
			printf("\n Size of subset passed by %d is %d for %d ",processor_id,msg_buf.size_of_subset,receiver_id);
			if(msgsnd(queue_id, &msg_buf,sizeof(struct message_buffer),0) == -1)
                   	{   
				char *msg;
				sprintf(msg,"\n error on sending data from processor %d for processor %d ",processor_id,receiver_id);	
                    	 	error(msg);    
                  	 }
	}	
	free(print);	
}

int main()
{
	int i;
        int size,target ;
	printf("\n Enter the no of elements in set ");
	scanf("%d",&size);
	int numbers[size];
	printf("\n Enter the elements of set ");
	for(i=0;i<size;i++)
		scanf("%d",&numbers[i]);
	printf("\n Enter target sum to be achieved ");
	scanf("%d",&target);

	pthread_t processor[NO_OF_PROCESSOR];
	struct processor_buffer processor_data[NO_OF_PROCESSOR];	
	long msg_buf_id=msgget(IPC_PRIVATE,0666|IPC_CREAT);
	printf("queue id is %ld",msg_buf_id);	

	for(i=0;i<NO_OF_PROCESSOR;i++)
      	{
		processor_data[i].queue_id = msg_buf_id;
     		processor_data[i].processor_id = i; 		
		processor_data[i].numbers=numbers;
		processor_data[i].size=size;
		processor_data[i].target_sum=target;
         	if(pthread_create(&processor[i], NULL, thread_function, (void *)&processor_data[i]))
     		{	
			char *msg;
			sprintf(msg,"ERROR; return code from pthread_create()");
                    	error(msg);
     		       	exit(-1);
     	    	}			
      	}		
	for(i=0;i<NO_OF_PROCESSOR;i++)
       	{
		pthread_join(processor[i],NULL);
	}
	
	 printf("\n Number of sets generated %d\n", total_sets);
    	return 0;
}


