#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

        int flagBeginX = 0, flagBeginW = 0, flagBeginR = 0, flagMidX = 0, flagMidW = 0, flagMidR = 0, flagEndX = 0, flagEndW = 0, flagEndR = 0;

void help()
{
	printf("Первым идёт флаг группы атрибутов (user/group/other)\nВторой флаг – добавление или удаление (add/remove)\nДалее идут от одного до трёх флагов атрибутов (read/write/execute)\nПример:chmod -uaw file1 // добавить право пользователю на запись\nchmod -grr file1 // убрать право группы на чтение\nchmod -oarwx file1 // добавить права на запись, чтение и исполнение файла для всех остальных\n");
}

void notArg()
{
	printf("Used uncorrect attribute\nTry 'chmod -h' for more information.\n");
}

void charToOct(char* fileName, char* operand, char* buffBegin, char* buffMid, char* buffEnd, char* fullBuff)
{
	
	int intBegin = 0;
	int intMid = 0;
	int intEnd = 0; 
	
	int errBeginX = 0;
	int errMidX = 0;
	int errEndX = 0;

	int errBeginW = 0;
	int errMidW = 0;
	int errEndW = 0;
	
	int errBeginR = 0;
	int errMidR = 0;
	int errEndR = 0;
	
	for(int i = 0; i <= 4; i++)
	{
		
		if(buffBegin[i] == 'x')
		{
			intBegin += 1;
			flagBeginX = 1; 				
		}
		if(buffMid[i] == 'x' && intMid < 7)
		{
			intMid += 1;
			flagMidX = 1;
		} 
		if(buffEnd[i] == 'x' && intEnd < 7)
		{
			flagEndX = 1;
			intEnd += 1;
		}
		if(buffBegin[i] == 'w' && intBegin < 7)
		{
			flagBeginW = 1;
			intBegin += 2;
		}
		if(buffMid[i] == 'w' && intMid < 7)
		{
			flagMidW = 1;
			intMid += 2;
		}
		if(buffEnd[i] == 'w' && intEnd < 7)
		{
			flagEndW = 1;
			intEnd += 2;
			
		}
		if(buffBegin[i] == 'r' && intBegin < 7)
		{	
			intBegin += 4;
			flagBeginR = 1;
		}
		if(buffMid[i] == 'r' && intMid < 7)
		{
			flagMidR = 1;
			intMid += 4;
		}
		if(buffEnd[i] == 'r' && intEnd < 7)
		{
			flagEndR = 1;
			intEnd += 4;
		}
		 
	} 
	
	int countX = 0;
	int countW = 0;
	int countR = 0;
		
	for (int i = 3; operand[i] !='\0'; i++)
	{
        	if(operand[i] == 'x' && strlen(operand) < 7)
                {
                	countX += 1;
                }
                if(operand[i] == 'w' &&  strlen(operand)< 7)
                {
                	countW += 1;
                }
                if(operand[i] == 'r' && strlen(operand) < 7)
                {
                	countR += 1;
                }
		
	}
	
        if (countX > 1 || countW > 1 || countR > 1)
        {
                notArg();
                return;
        }
		
	if(operand[5] != 'r' && operand[5] != 'w' && operand[5] != 'x' && strlen(operand) > 5)
	{
		notArg();
		return;
	}
	
	if(strlen(operand) > 4 && operand[4] != 'r' && operand[4] != 'w' && operand[4] != 'x')
	{
		notArg();
                return;
	}
	
	if(strlen(operand) > 3 && operand[3] != 'r' && operand[3] != 'w' && operand[3] != 'x')
	{
		notArg();
                return;
	}
	 
	
        if(operand[1] != 'u' && operand[1] != 'g' && operand[1] != 'o')
        {
                notArg();
                return;
        }

	
	

	char str[10];
	char buffStrBegin[3];
	char buffStrMid[3];
	char buffStrEnd[3];
	sprintf(buffBegin, "%d", intBegin);
	sprintf(buffMid, "%d", intMid);
	sprintf(buffEnd, "%d", intEnd);
	strcpy(str, "0");
	

	if((operand[2] == 'a' || operand[2] == 'r')  && strlen(operand) > 3){
	if(operand[2] == 'a')
	{
		if(operand[1] == 'u')
		{ 	
			for(int i = 3; operand[i] != '\0'; i++)
			{
				if(operand[i] == 'x' && intBegin < 7 && flagBeginX == 0)
				{
					intBegin += 1;
					errBeginX += 1 ;
				}
				if(operand[i] == 'w' && intBegin < 7 && flagBeginW == 0)
				{
					intBegin += 2;
					errBeginW += 1;
				}
				if(operand[i] == 'r' && intBegin < 7 && flagBeginR == 0)
				{
					intBegin += 4;
					errBeginR += 1;
				}	
		
			}

			char begin[3];
			sprintf(begin, "%d", intBegin);	
			strcat(str, begin);
			strcat(str, buffMid);
			strcat(str, buffEnd);
		} 
		if(operand[1] == 'g')
		{
                	for(int i = 3; operand[i] != '\0'; i++)
                	{
                        	if(operand[i] == 'x' && intMid < 7 && flagMidX == 0)
                        	{
                                	intMid += 1;
					errMidX += 1;
                        	}
                        	if(operand[i] == 'w' && intMid < 7  && flagMidW == 0)
                        	{
                                	intMid += 2;
					errMidW += 1;
                        	}
                        	if(operand[i] == 'r' && intMid < 7 && flagMidR == 0)
                        	{
                                	intMid += 4;
					errMidR += 1;
                        	}

			}

                	char mid[3];
                	sprintf(mid, "%d", intMid);
                	strcat(str, buffBegin);
                	strcat(str, mid);
                	strcat(str, buffEnd);
	
		}
		if(operand[1] == 'o')
		{	
	
                	for(int i = 3; operand[i] != '\0'; i++)
                	{
                        	if(operand[i] == 'x' && intEnd < 7 && flagEndX == 0)
                        	{
                                	intEnd += 1;
					errEndX += 1;
                        	}
                        	if(operand[i] == 'w' && intEnd < 7 && flagEndW == 0)
                        	{
                                	intEnd += 2;
					errEndW += 1;
                        	}
                        	if(operand[i] == 'r' && intEnd < 7 && flagEndR == 0)
                        	{
                                	intEnd += 4;
					errEndR += 1;
                        	}

                	}
			
                	char end[3];
                	sprintf(end, "%d", intEnd);
                	strcat(str, buffBegin);
        	        strcat(str, buffMid);
                	strcat(str, end);

		}
	}
	if(operand[2] == 'r'){
		if(operand[1] == 'u')
		{
			
                        for(int i = 3; operand[i] != '\0' ; i++)
                        {
                                if(operand[i] == 'x' && intBegin > 0 && flagBeginX == 1)
                                {
                                        intBegin -= 1;
					errBeginX += 1;
                                }
                                if(operand[i] == 'w' && intBegin > 0 && flagBeginW == 1)
                                {
                                        intBegin -= 2;
					errBeginW += 1;
                                }
                                if(operand[i] == 'r' && intBegin > 0 && flagBeginR == 1)
                                {				
					
                                        intBegin -= 4;
					errBeginR += 1;
                                }

                        }
			
                        char begin[3];
                        sprintf(begin, "%d", intBegin);
                        strcat(str, begin);
                        strcat(str, buffMid);
                        strcat(str, buffEnd);


		}
		if(operand[1] == 'g')
                {
			 
                        for(int i = 3; operand[i] != '\0'; i++)
                        {
                                if(operand[i] == 'x' && intMid > 0 && flagMidX == 1)
                                {
                                        intMid -= 1;
					errMidX += 1;
                                }
                                if(operand[i] == 'w' && intMid > 0  && flagMidW == 1)
                                {
                                        intMid -= 2;
					errMidW += 1;
                                }
                                if(operand[i] == 'r' && intMid > 0 && flagMidR == 1)
                                {
                                        intMid -= 4;
					errMidR += 1;
                                }

                        }
                        			
                        char mid[3];
                        sprintf(mid, "%d", intMid);
                        strcat(str, buffBegin);
                        strcat(str, mid);
                        strcat(str, buffEnd);

                }
                if(operand[1] == 'o')
                {

                        for(int i = 3; operand[i] != '\0'; i++)
                        {
                                if(operand[i] == 'x' && intEnd > 0 && flagEndX == 1)
                                {
                                        intEnd -= 1;
					errEndX += 1;
                                }
                                if(operand[i] == 'w' && intEnd > 0 && flagEndW == 1)
                                {
                                        intEnd -= 2;
					errEndW += 1;
                                }
                                if(operand[i] == 'r' && intEnd > 0 && flagEndR == 1)
                                {
                                        intEnd -= 4;
					errEndR += 1;
                                }

                        }
                        	
                        char end[3];
                        sprintf(end, "%d", intEnd);
                        strcat(str, buffBegin);
                        strcat(str, buffMid);
                        strcat(str, end);

                }

	}
	}else{
	notArg();
	strcpy(str, buffBegin);
	strcat(str, buffMid);
	strcat(str, buffEnd);
	}
	int mode;
	mode = strtol(str, 0, 8); 
	if (chmod(fileName, mode) < 0)
	{
		perror("somthing wrong");
	}
}
int main(int argc, char* argv[])
{
	FILE *fp;
	struct stat buff;
	char bufferFull[512];
	char buffUser[5];
	char buffGrup[5];
	char buffOthr[5];
 
	if(argc > 2) {
		if (access(argv[2], F_OK)) {
			printf("File doesn't exist\n");
			exit(0);
		}
	}
	
	stat(argv[2], &buff);
	
	(buff.st_mode & S_IRUSR) ? strcpy(buffUser, "r") : strcpy(buffUser, "-");
        (buff.st_mode & S_IWUSR) ? strcat(buffUser, "w") : strcat(buffUser, "-");
        (buff.st_mode & S_IXUSR) ? strcat(buffUser, "x") : strcat(buffUser, "-");
        (buff.st_mode & S_IRGRP) ? strcpy(buffGrup, "r") : strcpy(buffGrup, "-");
        (buff.st_mode & S_IWGRP) ? strcat(buffGrup, "w") : strcat(buffGrup, "-");
        (buff.st_mode & S_IXGRP) ? strcat(buffGrup, "x") : strcat(buffGrup, "-");
        (buff.st_mode & S_IROTH) ? strcpy(buffOthr, "r") : strcpy(buffOthr, "-");
        (buff.st_mode & S_IWOTH) ? strcat(buffOthr, "w") : strcat(buffOthr, "-");
        (buff.st_mode & S_IXOTH) ? strcat(buffOthr, "x") : strcat(buffOthr, "-");
	strcpy(bufferFull, buffUser);
	strcat(bufferFull, buffGrup);
	strcat(bufferFull, buffOthr);
	


	switch(argc){
	case 1:
		notArg();
		break;
	case 2:
		(!strcmp(argv[1], "-h")) ? help() : notArg();
		break; 
	case 3:
		if(strlen(argv[1]) > 6)
		{
			notArg();
			return 0;
		}
		charToOct(argv[2], argv[1], buffUser, buffGrup, buffOthr, bufferFull);
		break;
	case 4: 
		help();
		break;
	}
	return 0;
}

