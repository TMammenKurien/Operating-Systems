	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <sys/wait.h>
	#include <time.h>

	int main()
	{
		int status;
		pid_t pid;

		//setting up communication between the parent and children	
		int prisoner1Police0[2];	//child0Parent
		int prisoner2Police0[2];	//child1Parent
		int Police0Prisoner1[2];	//parentChild0
		int Police0Prisoner2[2];	//parentChild1

		//creating buffers for each relationship
		char P1p0[128];				//bufP0
		char P2p0[128];				//bufP1
		char p0P1[128];				//bufC0
		char p0P2[128];				//bufC1
			
		//turning into pipes
		pipe(prisoner1Police0);
		pipe(prisoner2Police0);
		pipe(Police0Prisoner1);
		pipe(Police0Prisoner2);

		//prisoner1 child process
		pid = fork();
		if(pid == -1)
		{
	        	perror("ERROR FORK FAILURE");
	        	exit(1);
		}
		
		if(!pid)
		{
			int random1;
			srand(time(NULL));
	        sleep(1);
	        char error1[128];
	        if(write(prisoner1Police0[1], error1, 512) == -1)
	        {
	            perror("ERROR WHILE WRITING");
	            exit(1);
	        }

	        while(1)
	        {
	        	read(Police0Prisoner1[0], p0P1, 128);
				int start = p0P1[0] - '0';
				if(start == 1)
				{
									
					random1 = rand() % 2;
					char r[128]; //randomint in char form
					sprintf(r, "%d", random1);
					printf("Random Int: %d\n", random1);
					fflush(stdout);
					close(prisoner1Police0[0]);
					write(prisoner1Police0[1], random1, 1);
					close(prisoner1Police0[1]);
				}
				else
				{				
					exit(1);
				} 
			}
			return 0;
		}
		else
			{
				
				pid_t pid2;
	        	pid2 = fork();
	        	if(pid2 == 0)	
	        	{
	        		srand(time(NULL));
	        		char error2[128];
	        		if(write(prisoner2Police0[1], error2, 512) == -1)
	        		{
	            		perror("ERROR WHILE WRITING");
	            		exit(1);
	        		}

	        		while(1)
	        		{
		        		read(Police0Prisoner2[0], p0P2, 128);
						int start2 = p0P2[0] - '0';
						if(start2 == 1)
						{		
							int random2 = rand() % 2;
							char r2[128]; //randomint in char form
							sprintf(r2, "%d", random2);
							printf("Random Int: %d\n", random2);
							fflush(stdout);
							close(prisoner2Police0[0]);
							write(prisoner2Police0[1], random2, 1);
							close(prisoner2Police0[1]);
						}	
						else
						{				
							exit(1);
						} 
					}
				return 0;
        		}
        		//Police Processes here
        		else
        		{	//First Process
        			read(prisoner1Police0[0],P1p0,512);
        			char parent1[32];
        			sprintf(parent1, "%s", P1p0);

        			//Second Process
        			read(prisoner2Police0[0],P2p0,512);
        			char parent2[32];
        			sprintf(parent2,"%s",P2p0);

        			//Decision Evaluation
        			float time1; //Sentence variable 1
        			float time2; //Sentence variable 2
         			
         			for(int i = 0; i <=10; i++)
        			{
        				write(Police0Prisoner1[1], "1" - 0, 512);
        				read(prisoner1Police0[0],P1p0,512);
        				write(prisoner2Police0[1], "1" - 0, 512);
        				read(Police0Prisoner2[0],P2p0, 512);
        				fprintf(stdout, "Start Decision Tree %d\n", i);
        				int decision1 = P1p0[0] - '0';
        				int decision2 = P2p0[0] - '0';
        				if(decision1 == 1 && decision2 == 1)
        				{
           					printf("%s: Cooperate\n%s: Cooperate\n", parent1, parent2);
							time1 = time1+6;
							time2 = time2+6;
        				}
        				else if(decision1 == 0 && decision2 == 0)
        				{
        					printf("%s: Defect\n%s: Defect\n", parent1, parent2);
							time1 += 0.5;
							time2 += 0.5;
        				}
        				else if (decision1 == 1 && decision2 == 0)
        				{
						printf("%s: Cooperate\n%s: Defect\n", parent1, parent2);
						time2 += 10;
						}
						else if (decision1 == 0 && decision2 == 1)
						{
						printf("%s: Defect\n%s: Cooperate\n", parent1, parent2);
						time1 += 10;
						}
						else
						{
						perror("CAN'T COMPUTE CHOICES");
						}
					}

					printf("Time Served:\n%s: %f years\n%s: %f years\n", parent1, time1, parent2, time2);		
 
					wait(NULL);
					wait(NULL);
				
					if (close(Police0Prisoner1[0]) == -1)
					{
						perror("Error closing pipe from Parent to child 1");
					}
					if (close(Police0Prisoner2[0]) == -1)
					{
	                    perror("Error closing Pipe from Parent to Child 2");
		            }
					if (close(prisoner1Police0[1]) == -1)
					{
		                perror("Error closing pipe from Child 1 to Parent");
		            }
					if (close(prisoner2Police0[1]) == -1)
					{
                       perror("Error closing pipe from Child 2 to Parent");
                    }
                    exit(EXIT_SUCCESS);
              }
          return 0;
          }
      }