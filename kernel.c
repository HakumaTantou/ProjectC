// OS Kernel
// Jarrod LeComte

void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void readFile(char*, char*, int*);
void executeProgram(char*);
void terminate();

void main(){

	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int white = 0x7;
	char* letters = "Hello World\0";
    char line[80];
    char buffer[13312];

    //Project C section Part 1
    /*
	int sectorsRead;
	makeInterrupt21();
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);
	if (sectorsRead > 0){
		interrupt(0x21, 0, buffer, 0, 0);
	}
	else{
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);
	}
    */
    //End of Part 1

    
    
    //Part 2
    /*
    makeInterrupt21();
    interrupt(0x21, 4, "tstpr1", 0, 0);
    */
    //End of part 2

    
    //Part 3
    /*
    makeInterrupt21();
	interrupt(0x21, 5, "tstpr2", 0, 0);
    */
    //Part 4; Unable to test due to part 1
    makeInterrupt21();
    interrupt(0x21, 5, "shell", 0, 0);
    //End of part 4

	while(*letters != 0x0){
		//Sets message
		putInMemory(startVidMem, vidMemOffset, *letters);
		vidMemOffset++;
		//Sets text color
		putInMemory(startVidMem, vidMemOffset, white);
		vidMemOffset++;

		//Moves pointer
		letters++;
	}

	while(1);

}

void printString(char* chars){
    
    //Loops until \0 is reached. Uses 0x10 interrupt for each input character
    int i = 0;
    while(chars[i] != '\0'){
        int charvalue = chars[i];
        interrupt(0x10, 0xe*256+charvalue, 0, 0, 0);
        i++;
    }
}

void readString(char* line){

    //Values of keys
    int enter = 0xd;
    int backspace = 0x8;

    int i = 0;

    while(1){

        int asciivalue = interrupt(0x16, 0, 0, 0, 0);

        if(asciivalue == enter){
	        line[i] = 0xd;
            interrupt(0x10, 0xe*256+0xd, 0, 0, 0);
			i++;
			line[i] = 0xa;
            interrupt(0x10, 0xe*256+0xa, 0, 0, 0);
			i++;
			line[i] = 0x0;
            interrupt(0x10, 0xe*256+0x0, 0, 0, 0);
			break;
        }
        else if(asciivalue == backspace){
            if(i>0){
                line[i] = 0x0;
                i--;
                interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
				i++;
				interrupt(0x10, 0xe*256+0x0, 0, 0, 0);
				i--;
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
            }
        }
        else{
            line[i] = asciivalue;
            interrupt(0x10, 0xe*256+asciivalue, 0, 0, 0);
            i++;
        }

    }//end of while loop
}

void readSector(char* buffer, int sector){

	int ah = 2; // 2 is read
	int al = 1; // Number of sectors to read
	int bx = buffer; // Data storage address
	int ch = 0; // Track number
	int cl = sector + 1; //Relative sector number (sector + 1)
	int dh = 0; // Head number
	int dl = 0x80; //Device number (0x80 for hard disk)

	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx = dh*256+dl;

	interrupt(0x13, ax, bx, cx, dx);

}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	if (ax == 0){
	    printString(bx);
    }
	else if (ax == 1){
		readString(bx);
	}
	else if (ax == 2){
		readSector(bx, 30);
	}
    else if (ax == 3){
        readFile(bx,cx,dx);
    }
    else if (ax == 4){
        executeProgram(bx);
    }
    else if (ax == 5){
        terminate();
    }
	else if (ax > 6){
		printString("Error\0");
	}
}

//TESTING FOR PROJECT C
void readFile(char* filename, char* buffer, int* sectornumber){
    char directorysector[512];
    int offset;
    int sectorpointer = 0;
    int bufferpointer = 0;

    readSector(&directorysector, 2);

    

    //Loops until a file match is made, or the sector is finished
    while(1){

        //Checks current location for file name. If found, save the position
        if(directorysector[sectorpointer]==filename[0] && directorysector[sectorpointer+1]==filename[1] && directorysector[sectorpointer+2]==filename[2] && directorysector[sectorpointer+3]==filename[3] && directorysector[sectorpointer+4]==filename[4] && directorysector[sectorpointer+5]==filename[5]){

            while (directorysector[sectorpointer+6] != 0){
		        readSector(buffer + bufferpointer, directorysector[sectorpointer+6]);
		        bufferpointer += 512;
		        sectorpointer++;
                *sectornumber = *sectornumber + 1;
	        }
            return;
        }

        //Went to far without finding anything!
        else if(sectorpointer>=512){
            char errorMessage[15];
		    errorMessage[0] = 'F';
		    errorMessage[1] = 'i';
		    errorMessage[2] = 'l';
		    errorMessage[3] = 'e';
		    errorMessage[4] = ' ';
		    errorMessage[5] = 'n';
		    errorMessage[6] = 'o';
		    errorMessage[7] = 't';
		    errorMessage[8] = ' ';
		    errorMessage[9] = 'f';
		    errorMessage[10] = 'o';
		    errorMessage[11] = 'u';
		    errorMessage[12] = 'n';
		    errorMessage[13] = 'd';
		    errorMessage[14] = '\0';
		    printString(errorMessage);

		    *buffer = '\0';
		    return;
        }

        //Nothing found, so keep skipping ahead
        else{
            sectorpointer+=32;
        }
    }
}

void executeProgram(char* name){
    char buffer[13312];
    int i;

    readFile(name,buffer,2);

    for(i = 0; i < 13312 ; i++){
        putInMemory(0x2000, i, buffer[i]);   
    }
    launchProgram(0x2000);
    
}

void terminate(){

    char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);

    while(1);
}
