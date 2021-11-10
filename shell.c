//Jarrod LeComte

int main(){
	char prompt[4];
	char buffer[512];
    char fileSize[13312];
    char filename[6];
	char badCommand[15];
	char commandType[5];
	char commandExecute[5];
    int pointer = 0;
    int i;
    int results = -1;
    

	prompt[0] = 'A';
	prompt[1] = ':';
	prompt[2] = '>';
	prompt[3] = '\0';

    
	commandType[0] = 't';
	commandType[1] = 'y';
	commandType[2] = 'p';
	commandType[3] = 'e';
	commandType[4] = '\0';

	commandExecute[0] = 'e';
	commandExecute[1] = 'x';
	commandExecute[2] = 'e';
	commandExecute[3] = 'c';
	commandExecute[4] = '\0';
    

	while (1){
        //Prints out the A:> prompt
		syscall(0, prompt, 0, 0);
        //Accepts user input and displays it
		syscall(1, buffer, 30, 0);

        for(i = 0; i < 4; i++){
            if(buffer[i]==commandType[i]){
                results = 0;
            }
            else if(buffer[i]==commandExecute[i]){
                results = 1;
            }
            else{
                badCommand[0] = 'B';
			    badCommand[1] = 'a';
			    badCommand[2] = 'd';
			    badCommand[3] = ' ';
			    badCommand[4] = 'C';
			    badCommand[5] = 'o';
			    badCommand[6] = 'm';
			    badCommand[7] = 'm';
			    badCommand[8] = 'a';
			    badCommand[9] = 'n';
			    badCommand[10] = 'd';
			    badCommand[11] = '!';
                badCommand[12] = '\r';
                badCommand[13] = '\n';
                badCommand[14] = '\0';
			    syscall(0, badCommand, 0, 0);
                results = -1;
                break;
            }
        }
        
        //Sets up file name
        for(i = 0; i < 6; i++){
            filename[i]=buffer[i+5];
        }

        //type
        if(results==0){
            syscall(3, filename, buffer, 2);
            syscall(0, buffer, 0, 0);
        }
        //exec
        if(results==1){
            syscall(5, filename, 0, 0);
        
        }
        /*
        // Error message
		else{
			badCommand[0] = 'B';
			badCommand[1] = 'a';
			badCommand[2] = 'd';
			badCommand[3] = ' ';
			badCommand[4] = 'C';
			badCommand[5] = 'o';
			badCommand[6] = 'm';
			badCommand[7] = 'm';
			badCommand[8] = 'a';
			badCommand[9] = 'n';
			badCommand[10] = 'd';
			badCommand[11] = '!';
            badCommand[12] = '\r';
            badCommand[13] = '\n';
            badCommand[14] = '\0';
			syscall(0, badCommand, 0, 0);
		}
        */
	}
}
