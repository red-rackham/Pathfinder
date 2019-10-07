#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/*Find a path through a matrix with start and endpoint. All fields must be passed on the way.
 *
 *
 *
 *
 */


int row, column, fields, step, xCurrent, yCurrent, xStart, yStart, xEnd, yEnd = 0;
const int xDirection[4] = {-1, 0, 1, 0};
const int yDirection[4] = {0, 1, 0, -1};


int checkNeighbours(int, int, int[][column], int[]);
int checkAreaSplit(int[][column]);
int goToNext(int direction[], int [][column], int[][4]);
int rollback(int [][column], int[][4]);
void printMap(int[][column]);


int main(int argc, char *argv[]) {
    
    printf("START MAIN....\n"); //DEBUG
    srand(666);

	if(argc == 1) {
     	row = 15;
        column = 15;
        xStart = 0;
        yStart = 0;
        xEnd = 14;
        yEnd = 14;
        srand(123);
    } else {
        row = atoi(argv[1]);
        column = atoi(argv[2]);
        xStart = atoi(argv[3]);
        yStart = atoi(argv[4]);
        xEnd = atoi(argv[5]);
        yEnd = atoi(argv[6]);
        srand(atoi(argv[7]));
    }

    
    
    int map[row][column];    


	fields = (row * column);
    int path[fields][4];
    
//    printf("fields: %d\n", fields); //DEBUG


    
    step = 1;
	xCurrent = xStart;
	yCurrent = yStart;
    
    for(int i = 0; i < fields; i++) {
        for(int j = 0; j < 4; j++) {
            path[i][j] = 0;
//            printf("fields: %d\n", fields); //DEBUG            
//            printf("&fields: %p\n", &fields); //DEBUG
//            printf("&path[i][j]: %p\n", &path[i][j]); //DEBUG
        }
    }    


	for(int i = 0; i < row; i++) {
		for(int j = 0; j < column; j++) {
			map[i][j] = 0;
		}
	}
    
	map[xStart][yStart] = 1;
	map[xEnd][yEnd] = fields;
    
//    printf("START: %d/%d\nEND: %d/%d\n", xStart, yStart, xEnd, yEnd); //DEBUG
//    printf("Start: %d\n", map[xStart][yStart]); //DEBUG
//    printf("END: %d\n", map[xEnd][yEnd]); //DEBUG
//    printf("Current Position: %d/%d\n", xCurrent, yCurrent); //DEBUG        
	
    int loopcounter = 0; // DEBUG
    
    while(step < fields -1) {
//  for(int loop = 0; loop < 55 ; loop++) {
            
//        printf("WHILE LOOP\n");                                     //DEBUG
//        printf("Loop Nr. %d\n", ++loopcounter);                     //DEBUG
//        printf("STEP: %d\n", step);                                 //DEBUG 
//        printf("Current Position: %d/%d\n", xCurrent, yCurrent);    //DEBUG
//      printf("Start: %d\n", map[xStart][yStart]);                 //DEBUG
//      printf("END: %d\n", map[xEnd][yEnd]);                       //DEBUG
            
		int freeNeighbours[4] = {0, 0, 0, 0}; 		/*Array of free neighbour cells {north, east, south, west} */
		int oneWayNeighbours[4] = {0, 0, 0, 0};	/*Array of neighbours that have only one 2nd grade neighbour*/
        
        int possibleSteps = (checkNeighbours(xCurrent, yCurrent, map, freeNeighbours));
//        printf("possibleSteps:%d\n", possibleSteps); //DEBUG
        
        for(int i = 0; i < 4; i++) {
            if(freeNeighbours[i] && path[step][i]) {
                possibleSteps--;
                freeNeighbours[i] = 0;
            }
        }
//                printf("possibleSteps:%d\n", possibleSteps); //DEBUG
        
        switch(possibleSteps) {
			case 0:
//                printf("case 0\n"); //DEBUG
				if((step == fields)) {
					printf("--- PATH FOUND! ---\n");
					step++;
					break;
				} else {
//					printf("No neighbours\n"); //DEBUG
					rollback(map, path);
					break;
				}
			case 1:
//                printf("case 1\n"); //DEBUG
				goToNext(freeNeighbours, map, path);
				printMap(map);
				break;
			case 2: case 3: case 4:
//                printf("case 2/3\n"); //DEBUG
//              oneWayNeighbours[4] = {0, 0, 0, 0};

				for(int i = 0; i < 4; i++) {
                    int x = xCurrent;
                    int y = yCurrent;
					if(freeNeighbours[i] == 1) {
						switch(i) {
							case 0:
								x -= 1;
								break;
							case 1: 
								y += 1;
								break;
							case 2:
								x += 1;
								break;
							case 3:
								y -= 1;
								break;
							default:
								exit(1);
								break;
						}
//                        printf("neighbour to check:%d/%d\n", x,y);
						int neighbours2ndGrade[4]; /*needed for function checkNeighbours() but not if used for 2nd grade*/
						if(checkNeighbours(x, y, map, neighbours2ndGrade) == 1) oneWayNeighbours[i] = 1;
					}
                    x = xCurrent;
                    y = yCurrent;
				}
//                printf("oneWayNeighbours = {");                                 //DEBUG
//                for(int i = 0; i < 4; i++) printf("%d,", oneWayNeighbours[i]);  //DEBUG
//                printf("}\n");                                                  //DEBUG
    
				int owCount = 0;
				for(int i = 0; i < 4; i++) owCount += oneWayNeighbours[i];
//                printf("owCount: %d\n", owCount);  //DEBUG
				if(owCount == 0){
					int randDirection[4] = {0, 0, 0, 0,};
					int found = 0;
					while(!found) {
                        
//                        printf("random loop\n");    //DEBUG
                        
						int r = (rand() % 3);
                        
//                        printf("r: %d\n", r);       //DEBUG
                        
						if(freeNeighbours[r] == 1) {
							randDirection[r] = 1;
							found = 1;
						}
					}
                    
//                    printf("randDirection= {");                                     //DEBUG
//                    for(int i = 0; i < 4; i++) printf("%d,", randDirection[i]);     //DEBUG
//                    printf("}\n");                                                  //DEBUG
    
					goToNext(randDirection, map, path);
				} else if (owCount == 1) {
					goToNext(oneWayNeighbours, map, path);
				} else {
					rollback(map, path);
				}
                printMap(map);
                break;
			default: 
				exit(1);
			 	break;
		}
	}			
}

//TODO:  short function
int checkNeighbours(int x, int y, int map[row][column], int neighbours[]) {
	int n, e, s, w;
    n = e = s = w = 0;
	int n_count = 0;
	if(x > 0 && map[x-1][y] == 0) n = 1;
	if(x < row -1 && map[x+1][y] == 0) s = 1;
	if(y > 0 && map[x][y-1] == 0) w = 1;
	if(y < column -1 && map[x][y+1] == 0) e = 1;

	neighbours[0] = n;
	neighbours[1] = e;
	neighbours[2] = s;
	neighbours[3] = w;

	for(int i = 0; i < 4; i++) n_count += neighbours[i];
    
//    printf("freeNeighbours = {");                               //DEBUG
//    for(int i = 0; i < 4; i++) printf("%d,", neighbours[i]);    //DEBUG
//    printf("}\n");                                              //DEBUG
    
	return n_count;
}

int goToNext(int direction[], int map[row][column], int path[(row*column)][4]) {
    
//    printf("goToNext()\n");     //DEBUG
    
    /*Check if same path has already been taken one
    for(int i = 0; i < 4; i++) {
        if(direction[i] == 1 && path[step][i] == 1) {
            rollback(map, path);
            return 0;
        }
    }
    */
    
//    printf("direction= {");                                          //DEBUG
//    for(int i = 0; i < 4; i++) printf("%d,", direction[i]);          //DEBUG
//    printf("}\n");                                                   //DEBUG
    //printf("Position before change: %d/%d\n", xCurrent, yCurrent); //DEBUG
    
    
	for(int i = 0; i < 4 ; i++) path[step][i] += direction[i];
    step++;
	xCurrent = (xCurrent - direction[0] + direction[2]);
	yCurrent = (yCurrent - direction[3] + direction[1]);
	map[xCurrent][yCurrent] = step;
    
    //printf("Position after change: %d/%d\n", xCurrent, yCurrent);   //DEBUG
    
    /*Check if endpoint is not blocked*/
    int endNeighbours[4];
//    printf("endpoing: %d/%d\n", xEnd, yEnd);
    if(checkNeighbours(xEnd, yEnd, map, endNeighbours) == 0 && (step +1) != fields) {
        printf("Endpoint not free anymore..\n");                     //DEBUG
        rollback(map, path);
        return 0;
    }
    if(checkAreaSplit(map) != 1 ) {
        rollback(map, path);
        return 0;
    }
    return 1;
}

int rollback(int map[][column], int path[(row*column)][4]) {
    
//        printf("rollback()\n");     //DEBUG
        
        for(int i = 0; i < 4; i++) path[step][i] = 0;
        step--;
        map[xCurrent][yCurrent] = 0;
        for(int i = 0; i < row; i++) {
            for(int j = 0; j < column; j++) {
                if(map[i][j] == step) {
                    xCurrent = i;
                    yCurrent = j; 
                }
            }
        }
        return 1;
}

/*Check if next step would divide free area in two parts*/
int checkAreaSplit(int map[row][column]){
    
//    printf("checkAreaSplit()...\n"); //DEBUG
    
    /*make local copy of map*/
	int mapCopy[row][column];
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < column; j++) {
            mapCopy[i][j] = map[i][j];
        }
    }
    
//    printf("print copy of map\n");  //DEBUG
//    printMap(mapCopy);              //DEBUG
    
    int counter;
    counter = fields + 1;
    
//    printf("counter: %d\n", counter);
    
    /*find first free field in matrix*/
    for(int i = 0; i < row; i++) {
		for(int j = 0; j < column; j++) {
            /*Mark first free field with int bigger than total amount of fields*/
			if(mapCopy[i][j] == 0) {
                mapCopy[i][j] = counter++;
                
                
//                printf("Found first free field: %d/%d\n", i, j);        //DEBUG
//                printf("mapCopy[%d][%d] = %d\n", i, j, mapCopy[i][j]);  //DEBUG
                
            	i = row;
                j= column;
            }
		}
    }
    /*find rest of free field in matrix*/
    for(int i = 0; i < row; i++) {
		for(int j = 0; j < column; j++) {
            /*Check if neighbours are already marked*/
            if(mapCopy[i][j] == 0) { 
                int neighbours[4] = {0,0,0,0};
                if(i > 0 && mapCopy[i - 1][j] > fields) neighbours[0] = mapCopy[i - 1][j];
                if(j < column && mapCopy[i][j + 1] > fields) neighbours[3] = mapCopy[i][j + 1];
                if(i < row && mapCopy[i + 1][j] > fields) neighbours[3] = mapCopy[i + 1][j];
                if(j > 0 && mapCopy[i][j -1] > fields) neighbours[3] = mapCopy[i][j - 1];
                
                /*
                printf("marked neighbours for %d/%d= {", i, j);                            //DEBUG
                for(int i = 0; i < 4; i++) printf("%d,", neighbours[i]);    //DEBUG
                printf("}\n");                                              //DEBUG
                */
                
                int highMarker = 0;
                int lowMarker = 0;
                int markCounter = 0;
                
                /*
                printf("highMarker = %d\n", highMarker);        //DEBUG
                printf("lowMarker = %d\n", lowMarker);          //DEBUG
                printf("markCounter = %d\n", markCounter);      //DEBUG
                */
                
                for(int k = 0; k < 4; k++) {
                    if(neighbours[k] > fields) {
                        markCounter++;
                        if(highMarker < neighbours[k]) {
                            highMarker = neighbours[k];
                            if(lowMarker == 0) lowMarker = highMarker;
                        }
                        if(lowMarker > neighbours[k]) lowMarker = neighbours[k];
                    }
                }
                /*
                printf("highMarker = %d\n", highMarker);        //DEBUG
                printf("lowMarker = %d\n", lowMarker);          //DEBUG
                printf("markCounter = %d\n", markCounter);      //DEBUG
                */
                if(markCounter == 0) {
                    mapCopy[i][j] = counter++;
                } else if(markCounter == 1) {
                    mapCopy[i][j] = lowMarker;
                } else {
                    mapCopy[i][j] = lowMarker;
                    for(int l = 0; l < 4; l++) {
                        if(neighbours[l] > lowMarker)
                            for(int m = 0; m < row; m++) {
                                for(int n = 0; n < column; n++) {
                                    if(mapCopy[m][n] == neighbours[l]) mapCopy[m][n] = lowMarker;
                                }
                            }
                    }
                }
            }
        }
    }
    int areaCheck = 1;
    for(int i = 0; i < row; i++) {
		for(int j = 0; j < column; j++) {
            /*Check if neighbours are already marked*/
            if(mapCopy[i][j] > (fields + 1)) areaCheck = 0;
        }
    }
    
//    printf("areaCheck = %d\n", areaCheck); //DEBUG
    
    return areaCheck;
}

void printMap(int map[][column]) {
    
//    printf("test.....\n");
    char output[5000] ="";
    strcat(output, "\n\n\n");
//    puts(output);


//	for(int i = 0; i < (column * 2 + 1); i++) strcat(output, "---");
//    strcat(output, "\n");
	for(int i = 0; i < row; i++) {
        for(int j = 0; j < column; j++) {
            char temp[10];
            sprintf(temp, "%3d", map[i][j]);
            strcat(output, temp);
        }
        strcat(output, "\n");
		for(int i = 0; i < (column * 2 + 1); i++) strcat(output, "--");
		strcat(output, "\n");
	}
    puts(output);
}
