#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int playerNumber;
int playerPosX[4], playerPosY[4];
int diceCurrent;
int turnCurrent = 1, turnMax;
char playerName[4];
int money[4];
int star[4] = {0,0,0,0};
int itemHold[4];
int diceType = 0;
int currentPlayer;
int startMoney = 10;
int startItem = 0;
int moneySpaceValue = 3;
int convertEnd = 1;
int ysize = 5;
int xsize = 6;

char mapFeatureWalk[5][6]{
	{'1','1','1','S','1','0'},
	{'1','1','0','1','1','1'},
	{'1','1','0','1','0','1'},
	{'1','1','0','1','0','1'},
	{'1','1','B','1','1','1'},
};

char mapFeature[5][6]{
	{'B','I','B','B','R','0'},
	{'B','E','0','R','R','B'},
	{'R','R','0','E','0','I'},
	{'B','R','0','B','0','B'},
	{'B','B','B','R','B','B'},
};
char mapTurn[5][6]{
	{'R','R','R','R','D','0'},
	{'U','U','0','D','2','D'},
	{'U','U','0','D','0','D'},
	{'U','U','0','D','0','D'},
	{'U','1','L','L','L','L'},
};

int itemPrice[7]{
	0,
	5, 7, 5,
	10, 5
};

char itemName[7][15]{
	"None",
	"Double Dice","Triple Dice","Slower Dice",
	"Star Jumper","Coin Stealer"
};

void randomItem(){
	int a = rand()%3+1;
	printf("%s\n" ,itemName[a]);
	if(itemHold[currentPlayer]!=0){
		printf("you already have: %s\ndiscard? (Y/N): ", itemName[itemHold[currentPlayer]]);
		char c;
		scanf("%c",&c);
		getchar();
		if(c!='Y'){
			return;
		}
	}
	itemHold[currentPlayer] = a;
}

void printMoneyItem(){
	printf("\nStatus:\n");
	for(int i=0;i<playerNumber;i++)
		printf("Player %d (%c) star = %d\tmoney = %d\titem = %s\n",i+1,playerName[i],star[i],money[i],itemName[itemHold[i]]);
}

void printMoney(){
	printf("\nStatus:\n");
	for(int i=0;i<playerNumber;i++)
		printf("Player %d (%c) star = %d\tmoney = %d\n",i+1,playerName[i],star[i],money[i]);
}

void buyStar(){
	char a;
	if(money[currentPlayer]>=10){
		printf("Current money: %d\nBuy star for 10 coins? (Y/N): ",money[currentPlayer]);
		scanf("%c", &a);
		if(a=='Y'){
			printf("You bought a star!");
			money[currentPlayer]-=10;
			star[currentPlayer]++;
		}
	}
	else printf("Not enough money");
	getchar();
}

void buyItem(){
	printf("Welcome to shop\n");
	for(int i=1;i<=5;i++){
		printf("%d. %s (%d money)\n",i,itemName[i],itemPrice[i]);
	}
	int a;
	printf("Choose item to buy (0 not buy)\nCurrent Item: %s\n",itemName[itemHold[currentPlayer]]);
	scanf("%d", &a);
	getchar();
	if(a<1||a>5) return;
	else{
		itemHold[currentPlayer] = a;
		money[currentPlayer] -= itemPrice[a];
		printf("You bought %s!", itemName[a]);
	}
	
}

void useItem(){
	printf("use item: %s?(Y/N): ",itemName[itemHold[currentPlayer]]);
	char a;
	scanf("%c", &a);
	if(a=='Y'){
		if(itemHold[currentPlayer]>=1&&itemHold[currentPlayer]<=3){ //dice
			diceType = itemHold[currentPlayer];
		}
		else diceType = 0;
		if(itemHold[currentPlayer]==4){ //star jumper
			playerPosX[currentPlayer] = 3;
			playerPosY[currentPlayer] = 0;
		}
		if(itemHold[currentPlayer]==5){ //coin stealer
			int b, c;
			printf("select player to steal money from (number): ");
			scanf("%d", &c);
			b = rand()%7+3;
			money[c-1] -= b;
			money[currentPlayer] += b;
			printf("stole %d money from player %d (%c)\n", b, c, playerName[c-1]);
		}
		itemHold[currentPlayer] = 0;
	}
	getchar();
}

void randomEvent(){
	int a = rand()%11-5;
	money[currentPlayer] += a;
	itemHold[currentPlayer] = a;
	printf("random money get: %d\n",a);
}

void checkPlayerPosition(int y,int x){
	int count=0;
	for(int i=0;i<playerNumber;i++)
		if(playerPosX[i]==x&&playerPosY[i]==y)
			count++;
	if(count==0){
		if(mapFeatureWalk[y][x]=='S')
			printf("[ ]");
		else if(mapFeatureWalk[y][x]=='B')
			printf("{ }");
		else
			printf("( )");
	}
	else{
		if(mapFeatureWalk[y][x]=='S')
			printf("[");
		else if(mapFeatureWalk[y][x]=='B')
			printf("{");
		else
			printf("(");
		if(count==1){
			for(int i=0;i<playerNumber;i++)
				if(playerPosX[i]==x&&playerPosY[i]==y)
					printf("%c",playerName[i]);
			}
		else printf("%d",count);
		if(mapFeatureWalk[y][x]=='S')
			printf("]");
		else if(mapFeatureWalk[y][x]=='B')
			printf("}");
		else
			printf(")");
	}
}

int rollDice(){
	int sum = 0;
	if(diceType!=3){
		for(int i=0;i<diceType+1;i++){
			sum += rand()%6+1;
		}
	}
	if(diceType==3){
		sum += rand()%3+1;
	}
	printf("dice: %d", sum);
	diceType = 0;
	return sum;
}

void printMap(){
	printf("\n");
	for(int j=0;j<ysize;j++){
		for(int i=0;i<xsize;i++){
			if(mapTurn[j][i]=='0')
				printf("   ");
			else checkPlayerPosition(j,i);
			if(mapTurn[j][i+1]=='L'||mapTurn[j][i+1]=='2'||mapTurn[j][i+1]=='1'){
				printf("<--");
			}
			else if(mapTurn[j][i]=='R'||mapTurn[j][i]=='2'){
				printf("-->");
			}
			else{
				printf("   ");
			}
		}
		printf("\n");
		if(j==ysize-1) break;
		for(int i=0;i<xsize;i++){
			if(mapTurn[j][i]=='D'){
				printf(" | ");
			}
			else if(mapTurn[j+1][i]=='U'||mapTurn[j+1][i]=='1'){
				printf(" ^ ");
			}
			else printf("   ");
			printf("   ");
		}
		printf("\n");
	}
	printf("\n");
}

void checkMapFeatureWalk(){
	switch(mapFeatureWalk[playerPosY[currentPlayer]][playerPosX[currentPlayer]]){
		case('S'):
			printMap();
			printf("Star Space!\n");
			buyStar();
			break;
		
		case('B'):
			printMap();
			printf("Shop Space!\n");
			buyItem();
			break;
	}
}

void checkMapFeature(){
	printf("\nplayer %d (%c) ",currentPlayer+1,playerName[currentPlayer]);
	switch(mapFeature[playerPosY[currentPlayer]][playerPosX[currentPlayer]]){
		case('B'):
			printf("got %d money!\n",moneySpaceValue);
			money[currentPlayer]+=moneySpaceValue;
			break;
		case('R'):
			printf("lost %d money!\n",moneySpaceValue);
			money[currentPlayer]-=moneySpaceValue;
			break;
		case('I'):
			printf("get item: ");
			randomItem();
			break;
		case('E'):
			printf("event:");
			randomEvent();
			break;
	}
}

void LU(){
	printf("Dice Left = %d\nturn Left or Up?(L or U): ",diceCurrent);
	char turnDirection;
	scanf("%c", &turnDirection);
	if(turnDirection=='L')
		playerPosX[currentPlayer]--;
	else
		playerPosY[currentPlayer]--;
}
void LR(){
	printf("Dice Left = %d\nturn Left or Right?(L or R): ",diceCurrent);
	char turnDirection;
	scanf("%c", &turnDirection);
	if(turnDirection=='L')
		playerPosX[currentPlayer]--;
	else
		playerPosX[currentPlayer]++;
}

void walk(){
	checkMapFeatureWalk();
	switch(mapTurn[playerPosY[currentPlayer]][playerPosX[currentPlayer]]){
		case('L'):
			playerPosX[currentPlayer]--;
			break;
		case('R'):
			playerPosX[currentPlayer]++;
			break;
		case('U'):
			playerPosY[currentPlayer]--;
			break;
		case('D'):
			playerPosY[currentPlayer]++;
			break;
		case('1'):
			printMap();
			LU();
			getchar();
			break;
		case('2'):
			printMap();
			LR();
			getchar();
			break;
	}
	return;
}

void onStart(){
	printf("Welcome to C Party Board Game Ver.alpha04\n---------------------------------------\n");
	srand(time(NULL));
	printf("insert number of players (max 4): ");
	scanf("%d", &playerNumber);
	printf("insert number of turns: ");
	scanf("%d", &turnMax);
	char a;
	printf("setup: Default or Custom? (D/C):");
	scanf(" %d", &a);
	getchar();
	if(a=='C'){
		printf("insert starting money: ");
		scanf("%d", &startMoney);
		printf("insert starting item (0:none, 1:double, 2:triple, 3:slower): ");
		scanf("%d", &startItem);
		printf("convert items to coins at the end (0:no, 1:yes): ");
		scanf("%d", &convertEnd);
	}
	for(currentPlayer=0;currentPlayer<playerNumber;currentPlayer++){
		money[currentPlayer] = startMoney;
		itemHold[currentPlayer] = startItem;
		printf("insert name player %d (char): ",currentPlayer+1);
		scanf(" %c", &playerName[currentPlayer]);
		playerPosY[currentPlayer] = 4;
		playerPosX[currentPlayer] = 5;
	}
	getchar();
	printMap();
	printf("start?");
	getchar();
}

void convertItems(){
	for(int i=0;i<playerNumber;i++){
		if(itemHold[i]!=0){
			itemHold[i]=0;
			money[i]+=5;
		}
	}
}

void winner(){
	int max = 0;
	int max2 = 0;
	for(int i=0;i<playerNumber;i++){
		if(star[i]>max) max = star[i];
	}
	printf("\nThe Winner is :");
	for(int i=0;i<playerNumber;i++){
		if(star[i]==max&&money[i]>max2) max2 = money[i];
	}
	for(int i=0;i<playerNumber;i++){
		if(star[i]==max&&money[i]==max2)
			printf(" Player %d (%c)",i+1,playerName[i]);
	}
}

void endGame(){
	getchar();
	system("cls");
	printf("item not used = 5 coins");
	if(convertEnd==1)
		convertItems();
	printMoney();
	winner();
}

int main(){
	onStart();
	while(turnCurrent<=turnMax){
		for(currentPlayer = 0; currentPlayer<playerNumber; currentPlayer++){
			getchar();
			system("cls");
			printf("turn = %d, Player %d (%c)\n",turnCurrent,currentPlayer+1,playerName[currentPlayer]);
			if(itemHold[currentPlayer]!=0){
				printMap();
				useItem();
			}
			diceCurrent = rollDice();
			for(;diceCurrent>0;diceCurrent--)
				walk();
			printMap();
			checkMapFeature();
			printMoneyItem();
			if(currentPlayer<playerNumber-1)
				printf("Next Player?");
			else if(turnCurrent!=turnMax)
				printf("Next Turn?");
			else{
				printf("\nthe end\nsee results?");
			}
		}
		turnCurrent++;
	}
	endGame();
	return 0;
}
