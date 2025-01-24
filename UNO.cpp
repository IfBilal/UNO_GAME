                              //M.Bilal Tahir 24i-3166//
                              //Asad Ahmed 24i-3057//



#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>
using namespace std;

const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;

/////////////////////////////////////////////////////////////////////////////////////////////////////

void clrscr(){
	system("CLS");
}

void delay(){           //function to have delay in program
	for(int i=0; i<15000000;i++);
}


//////////////////////////////////////////////////////////////////////////////////////////////////


void setColor(int, int);

void initialiseDeck(int[][15]);

void shuffleDeck(int[][15]);

// Draw pile is the left over deck after giving 7 catds to each player,
// when a player picks a card he picks it from this pile 
void initialiseDrawPile(int[][15], int[]);  

void dealCards(int[],int[],int[]);

int DrawCard(int[]);

// This is the deck on which the users play their turn according to the topcard of this deck
void initialiseDiscardPile(int[], int[]);  

void PrintBoard(int, int[], int[], int[], int[]); 

void PrintCentrePile(int, int);

void PrintUnoCard(int[]);

void PrintPlayerHand(int[]);

int count_cards(int[]);

int card_color(int);

int card_type(int);

void playTurn(int, int[], int[], int[], int[]);

bool isValidPlay(int , int );

void IncreaseUserDeck(int ,int [], int[], int[]);

void DecreaseUserDeck(int ,int[]);

void swapping ( int &, int &);

void UpdateDiscardPile ( int , int []);

void handleSpecialCard(int, int, int, int[], int[], int[], int[]);

bool callUno(int[], int[]);

void saveLoadGameResult(int, int[]);

bool ValidPlayUNO(int , int);

/////////////////////////////////////////////////////////////////////////////////////////////////////

int turn=0;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
	srand(time(0));
	string text;
	int deck[8][15] = {0};
	int DrawPile[108] = {0};
	int DiscardPile[108] = {0};
	int p1Hand[108] = {0}, p2Hand[108] = {0};
	int player;
	int win=0;
	int score[2];
	char input;
	
	clrscr();
	
	cout<<"                    ___  "<<endl;
	cout<<" |     |  |\\   |   /   \\ "<<endl; 
	cout<<" |     |  | \\  |  |     |"<<endl;
	cout<<" |     |  |  \\ |  |     |"<<endl;
	cout<<"  \\___/   |   \\|   \\___/ "<<endl;
      
      
	cout<<endl;
	cout<<"------------------------------\n";
	cout<<endl<<endl;
	
	cout<<"       1. Play UNO\n\n";
	cout<<"       2. Score\n";	
	
	do           //input validaation for main menu
	{
			input = getch();
	}while(input!='1' && input!='2');;
	
	
	
	switch(input){      //main runner of the program
	
	case '1':               //if play game so starting the game
		initialiseDeck(deck);   //initializing deck
		shuffleDeck(deck);      //shufflin deck
		
		// Convert 2d deck array to 1d array.	
		initialiseDrawPile(deck, DrawPile);   
		
		// deal Cards to each player. 7 each
		dealCards(DrawPile, p1Hand, p2Hand);  
		
		// Place first Card of Discard-Pile from Draw-Pile
		initialiseDiscardPile(DiscardPile, DrawPile);
		
		
		do{     
			if(turn%2==0)      //shifts turn between players
				player = 1;
			else 
				player = 2;
				
			PrintBoard(player ,p1Hand, p2Hand, DrawPile, DiscardPile);
			playTurn(player, p1Hand, p2Hand, DrawPile,  DiscardPile);
			
			if(count_cards(p1Hand)==0){
				clrscr();
				cout<<"Congratulations!!\n\nPlayer 1 Won !!\n";
				win = 1;
			}
			else if(count_cards(p2Hand)==0){
				clrscr();
				cout<<"Congratulations!!\n\nPlayer 2 Won !!\n";
				win = 2;
			}
			
			turn++;
		}while(win==0);
		
		saveLoadGameResult(win, score);
		
		break;
		
	case '2':       //shows the score
		clrscr();
		saveLoadGameResult(win, score);
		cout<<"Player 1 = "<<score[0]<<endl<<"Player 2 = "<<score[1];
		cout<<endl<<endl<<"Press 'E' to go back";
		do
		{
			input = getch();
			if(input=='e'||input=='E')
				main();
		}while(input!='e' && input!='E');
		
		break;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////




void saveLoadGameResult(int win, int score[])
{
	score[0]=0;
	score[1]=0;
	ifstream read("History.txt");
	string txt;
	
	if(!read){
//		cout<<"Error: save file not found... \ncreating new file...\n";
		ofstream create("History.txt");
		create<<"Player 1 = 0 \nPlayer 2 = 0";
	}
	
	int j=0;
	while( getline(read, txt) ){
		int Equal_index;
		
		for(int i=0; txt[i]!=0; i++){
			if(txt[i]=='=')
				Equal_index = i;
						
			if(i>Equal_index && txt[i]>='0' && txt[i]<='9'){
				score[j] = score[j]*10 + txt[i]-'0';
			}
			
		}
		
		j++;
		
	}
	
	if(win!=0){
		ofstream write("history.txt");
		
		(win==1)? score[0]++ : score[1]++ ;
		 
		write<<"Player 1 = "<<score[0]<<endl<<"Player 2 = "<<score[1];
	}


}





void initialiseDeck(int deck[][15]){
	// Color --> Red=0, Green=1, Blue=2, Yellow=3, Special=4;	
	// Type  --> Num_cards=0-9, Skip=10, Reverse=11, Draw2=12, Wild=13, Draw4=14;  
	
	// assign real values to all 108 cards in deck[8][15]
	for(int i=0, color=1; i<=7, color<=4; i+=2,color++){
		for(int type=0; type<=14; type++){
			
			if(type>0 && type<13)
				for(int j=0; j<2;j++)
					deck[i+j][type] = (type<10)? color*10+type : color*100+type; 
			else if(type==0)
				deck[i][type] = color*10+type;
			else if(type>=13)
				deck[i][type] = color*100+type;
				
		}
	}
}




// shuffle the deck
void shuffleDeck(int deck[8][15])
{
    int row1,col1,row2,col2;    

	for(int i=0;i<120;i++)
	{
		row1=rand()%8;     //shuffling the 2D deck by generating 2 random values and swapping them
		col1=rand()%15;
		row2=rand()%8;
		col2=rand()%15;

		while((row1==row2)&&(col1==col2))   //if random values generated are same generates the values again
		{
		row1=rand()%8;
		col1=rand()%15;
		row2=rand()%8;
		col2=rand()%15;
		}
		
		// swap
		deck[row1][col1]=deck[row1][col1]+deck[row2][col2];
		deck[row2][col2]=deck[row1][col1]-deck[row2][col2];
		deck[row1][col1]=deck[row1][col1]-deck[row2][col2];
	}
}



// Convert 2d deck array to 1d array.
void initialiseDrawPile(int deck[8][15], int DrawPile[])
{	
	for(int i=0, k=0; i<8, k<108; i++){
		for(int j=0; j<15; j++){
			if(deck[i][j]!=0){ //this 'if' ensures no null value is assigned to the drawpile
				DrawPile[k] = deck[i][j] ;
				k++;
			}
		}
	}
}




// deal Cards to each player hand. 
void dealCards(int deck[], int p1Hand[], int p2Hand[])
{
	// deal 7 Cards to each player. 
	for(int i=0; i<7; i++){
		p1Hand[i] = DrawCard(deck);
		p2Hand[i] = DrawCard(deck);
	}


}




int DrawCard(int DrawPile[]){
	// take top Card from Draw-Pile 
	int card = DrawPile[0];

	// Replace top card by next cards in line.	
	// shifts the cards in pile by one place to the left
	for(int i=0;i<107;i++)
	{
		DrawPile[i]=DrawPile[i+1];
	}
	
	// assign last card zero value to avoid duplication of last card
	DrawPile[107]=0;		
		
	return card;	
}



// initialise first Card of Discard-Pile from Draw-Pile
void initialiseDiscardPile(int DiscardPile[], int DrawPile[])
{    
	do
	{ // if Draw-Pile top card > 9 , replace it with random card.
		if(card_type(DrawPile[0])>9)
		{
			int i=rand()%count_cards(DrawPile);
			swapping(DrawPile[i],DrawPile[0]);
		}
		
	}while(card_type(DrawPile[0])>9);
	
	// draw top card from draw pile
	DiscardPile[0] = DrawCard(DrawPile);
}




void PrintBoard(int player, int p1Hand[], int p2Hand[], int DrawPile[], int DiscardPile[])
{
	system("CLS");
	
// print Player 1 hand
	setColor(BLACK, YELLOW);
	cout<<" Player 1 "<<endl;
	setColor(WHITE, BLACK);
	cout<<endl;
// if it is player 1 turn print hand else print uno cards 
	if(player == 1)
		PrintPlayerHand(p1Hand);
	else
		PrintUnoCard(p1Hand);
//////////////////////////////////////////////////////////////////
	
	cout<<endl<<endl<<endl<<endl<<endl;
	
	PrintCentrePile(DrawPile[0], DiscardPile[0]);

	cout<<endl<<endl<<endl<<endl;
//////////////////////////////////////////////////////////////////

// print Player 2 hand	
	setColor(BLACK, YELLOW);
	cout<<" Player 2 "<<endl;
	setColor(WHITE, BLACK);
	cout<<endl;
// if it is player 1 turn print uno cards else print hand 
	if(player == 1)
		PrintUnoCard(p2Hand);
	else
		PrintPlayerHand(p2Hand);
		
///////////////////////////////////////////////////////////////////
	cout<<endl<<endl;
	
	// if topcardof dicard pile is wild or wdraw4, display options for color change
	if( card_type(DiscardPile[0]) == 13 || card_type(DiscardPile[0]) == 14){
		cout<<"New Color: ";
		switch(card_color(DiscardPile[0])){
			case RED: cout<<"RED"; 	break;
			case GREEN: cout<<"GREEN"; 	break;
			case BLUE: cout<<"BLUE";	break;
			case YELLOW: cout<<"YELLOW"; break;
		}
		cout<<endl;
	}
	else if(card_type(DiscardPile[0]) == 10 || card_type(DiscardPile[0]) == 11)
		cout<< " Player "<<((player==1)? "2": "1")<<" turn Skipped\n\n ";
}




// DIPLAY PLAYER HAND
void PrintPlayerHand(int hand[])
{	
	int loop = count_cards(hand)/16;
	(count_cards(hand)%16==0)? 0 : loop++;
	
	int footer = 1;
	int j=0; // Card number in hand
	
	// number of rows
	for(loop; loop>0; loop--){
		
		//number of cards in one row
		int count = 16;
		int type, bgColor;
		
		if(count_cards(hand)%16!=0 && loop == 1){
			count = count_cards(hand)%16;
		}
		
		for(int i=0 ;i<count;i++)
		{
			type = card_type(hand[i+j*16]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(hand[i+j*16]);

			setColor(BLACK,bgColor);
			cout<<"-------";
			setColor(WHITE,BLACK);
			cout<<" ";
	    }
		cout<<endl;
		
		for(int i=0 ;i<count;i++)
		{
			type = card_type(hand[i+j*16]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(hand[i+j*16]);

			setColor(BLACK,bgColor);
			cout<<"-     -";
			setColor(WHITE,BLACK);
			cout<<" ";
	    }
	    cout<<endl;
	    
		for(int i=0 ;i<count;i++)
		{
			type = card_type(hand[i+j*16]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(hand[i+j*16]);

			setColor(BLACK,bgColor);
			if(type==14)
				cout<<"-  D4 -";
			else if(type==13)
				cout<<"-  W  -";
			else if(type==12)
				cout<<"-  D2 -";
			else if(type==11)
				cout<<"-  S  -";
			else if(type==10)
				cout<<"-  R  -";
			else if(type<=9)
				cout<<"-  "<<type<<"  -";
			setColor(WHITE,BLACK);
			cout<<" ";
	    }
		cout<<endl;
		
		for(int i=0 ;i<count;i++)
		{
			type = card_type(hand[i+j*16]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(hand[i+j*16]);

			setColor(BLACK,bgColor);
			cout<<"-     -";
			setColor(WHITE,BLACK);
			cout<<" ";
	    }
		cout<<endl;
		
		for(int i=0 ;i<count;i++)
		{
			type = card_type(hand[i+j*16]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(hand[i+j*16]);

			setColor(BLACK,bgColor);
			cout<<"-------";
			setColor(WHITE,BLACK);
			cout<<" ";
	    }
	    cout<<endl<<endl;
		
		// display card number 1,2,3,4,....
		for(int i=0; i<count; i++){
			cout<<"  "<<setw(2)<<footer<<"    ";
			footer++;
		}
		cout<<endl<<endl;
		
		j++;
	}
	 
}

// DISPLAY DRAW-PILE && DISCARD PILE
void PrintCentrePile(int DrawPile, int DiscardPile){
	
	int pile_card[2]={DrawPile, DiscardPile};
	int type, bgColor, txtColor;
	
	cout<<"\t";
	for(int i=0;i<2;i++)
	{
		if(i!=0){
			type = card_type(pile_card[i]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(pile_card[i]);
		} 
		else 
			bgColor = BLACK;

		txtColor = (i==0)? WHITE : BLACK;
		
		setColor(txtColor,bgColor);
		cout<<"-------";
		setColor(WHITE,BLACK);
		cout<<"\t\t";
    }
	cout<<endl<<"\t";
	
	for(int i=0;i<2;i++)
	{
		if(i!=0){
			type = card_type(pile_card[i]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(pile_card[i]);
		} 
		else 
			bgColor = BLACK;

		txtColor = (i==0)? WHITE : BLACK;
		
		setColor(txtColor,bgColor);
		cout<<"-     -";
		setColor(WHITE,BLACK);
		cout<<"\t\t";
    }
	cout<<endl<<"\t";
	
	for(int i=0;i<2;i++)
	{
		if(i!=0){
			type = card_type(pile_card[i]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(pile_card[i]);
		} 
		else 
			bgColor = BLACK;

		txtColor = (i==0)? WHITE : BLACK;
		
		setColor(txtColor,bgColor);
		if(i==0){
			cout<<"- UNO -";
		}
		else if(type==14)
			cout<<"-  D4 -";
		else if(type==13)
			cout<<"-  W  -";
		else if(type==12)
			cout<<"-  D2 -";
		else if(type==11)
			cout<<"-  S  -";
		else if(type==10)
			cout<<"-  R  -";
		else if(type<=9)
			cout<<"-  "<<type<<"  -";
		setColor(WHITE,BLACK);
		cout<<"\t\t";
    }
	cout<<endl<<"\t";


	for(int i=0;i<2;i++)
	{
		if(i!=0){
			type = card_type(pile_card[i]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(pile_card[i]);
		} 
		else 
			bgColor = BLACK;

		txtColor = (i==0)? WHITE : BLACK;
		
		setColor(txtColor,bgColor);
		cout<<"-     -";
		setColor(WHITE,BLACK);
		cout<<"\t\t";
    }
	cout<<endl<<"\t";
	
	
	for(int i=0;i<2;i++)
	{
		if(i!=0){
			type = card_type(pile_card[i]);
			if(type==13 || type==14)
				bgColor = WHITE;
			else 
				bgColor = card_color(pile_card[i]);
		} 
		else 
			bgColor = BLACK;

		txtColor = (i==0)? WHITE : BLACK;
		
		setColor(txtColor,bgColor);
		cout<<"-------";
		setColor(WHITE,BLACK);
		cout<<"\t\t";
    }
   
	cout<<endl<<endl<<"       Draw Pile";

}

// DISPLAY UNO CARDS 
void PrintUnoCard(int hand[]){


	int loop = count_cards(hand)/16;
	(count_cards(hand)%16==0)? 0 : loop++;
	
	
	for(loop; loop>0; loop--){
		
		int count = 16;
		
		if(loop == 1){
			count = count_cards(hand)%16;
		}
		
		for(int i=0;i<count;i++)
		{
			cout<<"-------";
			cout<<" ";
	    }
		cout<<endl;
	    
		for(int i=0;i<count;i++)
		{
			cout<<"-     -";
			cout<<" ";
	    }
		
		    cout<<endl;
		
		for(int i=0;i<count;i++)
		{
			cout<<"- UNO -";
			cout<<" ";
	    }
		
		cout<<endl;
		
		for(int i=0;i<count;i++)
		{
			cout<<"-     -";
			cout<<" ";
	    }
	    
		cout<<endl;
		
		for(int i=0;i<count;i++)
		{
			cout<<"-------";
			cout<<" ";
	    }
		cout<<endl<<endl;    
	}
}




void playTurn(int player, int p1Hand[], int p2Hand[], int DrawPile[], int DiscardPile[])
{
	bool CALL=0,CALL2=0;
	int type;
	int Card;
	char action,AfterDraw;
	char Exiting;
	char paused;
	while(true){   //loop so that we can restart the body any time by using continue
	while(true)  //loop so that we can restart through continue when E or U or P is pressed
	{
		setColor(BLACK,CYAN);
		cout<<"1. DRAW CARD ";
		setColor(WHITE,BLACK);
		cout<<"     ";
		setColor(BLACK,CYAN);
		cout<<"2. PLAY CARD ";
		setColor(WHITE,BLACK);
		cout<<"     ";
		setColor(BLACK,CYAN);
		cout<<"U. Call UNO ";
		setColor(WHITE,BLACK);
		cout<<"     ";
		setColor(BLACK,CYAN);
		cout<<"E. Exit ";
		setColor(WHITE,BLACK);
		cout<<"     ";
		setColor(BLACK,CYAN);
		cout<<"P. Pause ";
		setColor(WHITE,BLACK);
		cout<<"     ";
		cout<<endl;
		
		
		action = getch();
		int Card;
	
		while(action!='1' && action!='2' && action!='U' && action!='E' && action!='P')  //validating input
		{
			cout<<"\nInvalid\n";
			action = getch();
		}
		if(action=='P')  //if P is pressed pausing the game
		{
			clrscr();
			cout<<"Game paused ! \n\nPress any key to continue ";
			paused = getch();
			PrintBoard(player ,p1Hand, p2Hand, DrawPile, DiscardPile);
			continue;  //continue to second loop
			
		}
		if(action=='E')  //if E is pressed confirming the exit
		{
			cout<<"Do you really want to quit\nPress N to Continue or Press Y to go back to main menu ";
					Exiting = getch();
					while(Exiting!='Y' && Exiting !='N')  //validating input
					{
						cout<<"\nInvalid \n";
						Exiting = getch();	
					}
					if(Exiting=='Y')  //if Y is pressed  after E exit program
					{
						clrscr();
						cout<<"Program exited ;";
						exit(0);
					}
					if(Exiting='N')  //If N is pressed after E continue the program
					continue;  //continue to second loop
		}
		if(action=='U')  //if U is pressed then call uno
		{
		if(callUno(DiscardPile,(player==1)?p1Hand:p2Hand)==0)   //checking for vaalid call
				{
				cout<<"\nInvalid call\n"; 
				continue;  //continue to second loop
				}
			     if(callUno(DiscardPile,(player==1)?p1Hand:p2Hand)==1)  
			     {
			    	CALL=1;    //assigning CALL value of 1 so that we know afterwards that whether call was done or not
		       	cout<<"\nUno Called \n";
		        	continue;  //continue to second loop
				}
		}
		break;   // breaking the second loop 
	}
	if(action=='1')  //1 pressed so card drawing
	{
				IncreaseUserDeck(DrawPile[0],(player==1)?p1Hand:p2Hand,DrawPile,DiscardPile);  //adding the drawn deck to user's hand
				DrawCard(DrawPile);  //removing drawn card from draw pile
					clrscr();
					PrintBoard(player ,p1Hand, p2Hand, DrawPile, DiscardPile);  //printing board
				while(true)  //loop so we can restart taking input through continue
		    	{
					setColor(BLACK,CYAN);
					cout<<"1. Skip PLAY";
					setColor(WHITE,BLACK);
					cout<<"     ";
					setColor(BLACK,CYAN);
					cout<<"2. PLAY CARD ";
					setColor(WHITE,BLACK);
					cout<<"     ";
					setColor(BLACK,CYAN);
					cout<<"U. Call UNO ";
					setColor(WHITE,BLACK);
					cout<<"     ";
					setColor(BLACK,CYAN);
					cout<<"E. Exit ";
					setColor(WHITE,BLACK);
					cout<<"     ";
					setColor(BLACK,CYAN);
					cout<<"P. Pause ";
					setColor(WHITE,BLACK);
					cout<<"     ";
					cout<<endl;
					AfterDraw = getch();
					while(AfterDraw!='1' && AfterDraw!='2' && AfterDraw!='U' && AfterDraw!='E' && AfterDraw!='P')  //validating input
					{
						cout<<"\nInvalid \n";
						AfterDraw = getch();
					}
					if(AfterDraw=='P')  //if P is pressed than pause the game
					{
						clrscr();
						cout<<"Game paused ! \n\nPress any key to continue ";
						paused = getch();
						PrintBoard(player ,p1Hand, p2Hand, DrawPile, DiscardPile);
						continue;	//continue to third loop
					}
					if(AfterDraw=='E')  //if E is pressed confirming exit
	            	{
	        		cout<<"Do you really want to quit\nPress N to Continue or Press Y to go back to main menu ";
					cin>>Exiting;
					while(Exiting!='Y' && Exiting !='N')    //validating input
				    	{
						cout<<"\nInvalid \n";
						cin>>Exiting;	
					}
					if(Exiting=='Y')
					{
					    	clrscr();
						cout<<"Program exited ;";
						exit(0);
				    	}
					if(Exiting=='N')
					continue; //continue  to third loop
		            }
		            if(AfterDraw=='U')  //if U pressed uno is called
		            {
		            if(callUno(DiscardPile,(player==1)?p1Hand:p2Hand)==0)    //checking for valid call
				    {
    				cout<<"\nInvalid Call\n";
	    			continue;  //continue to third loop
		    		    }
			         if(callUno(DiscardPile,(player==1)?p1Hand:p2Hand)==1)
			     	{
			    	CALL2=1;
		        	cout<<"\nUno Called \n";
		        	continue;  //continue to third loop
				    }
	            	}
		if(AfterDraw=='1')  //if 1 pressed after drawing card than skip the turn
		{
		    return;
		}
		if(AfterDraw=='2')  //if 2 pressed taking  card number
		{
        cout<<"Card : ";
        cin>>Card;
        while(Card>count_cards((player==1)?p1Hand:p2Hand)  || Card<1  )  //If card number is pressed which user doesnt have asks for card again 
			{
				cout<<"\nInvalid\n";
				cout<<"\nCard : ";
				cin>>Card;
			}
        
        if(isValidPlay((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile[0])==0)  //but if an invalid card is played user again gets option to 
            {                                                     //draw a card or skip the turn 
            cout<<"\nInvalid\n";
            continue;  //continue to third loop
            
            }
            type=card_type((player==1)?p1Hand[Card-1]:p2Hand[Card-1]);  //finding type of card played
        if(isValidPlay((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile[0])==1)  //checking if valid than playing the card
      		{
             	if(CALL==0 && CALL2==0 && callUno(DiscardPile,(player==1)?p1Hand:p2Hand)==1)  //checking that if user cold have called uno and didnt call so giving penalty of 2 cards
					{
						cout<<"\nUno not called, penalty of two cards : ";
						IncreaseUserDeck(DrawPile[0],(player==1)?p1Hand:p2Hand,DrawPile,DiscardPile);  
						DrawCard(DrawPile);
						IncreaseUserDeck(DrawPile[0],(player==1)?p1Hand:p2Hand,DrawPile,DiscardPile);
						DrawCard(DrawPile);
					}
					if(type>9) //if special card card then calling special card function
						{
						handleSpecialCard(type, player, Card, DrawPile,  p1Hand,  p2Hand,  DiscardPile);  //function for special cards
						UpdateDiscardPile((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile);  //adding the played card to discard pile
						DecreaseUserDeck((player==1)?p1Hand[Card-1]:p2Hand[Card-1],(player==1)?p1Hand:p2Hand);  //removing played card from user hand
						return;
						}
					if(type<=9)
					{
						UpdateDiscardPile((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile);
						DecreaseUserDeck((player==1)?p1Hand[Card-1]:p2Hand[Card-1],(player==1)?p1Hand:p2Hand);
						return;
 					}
			}
		}
        break;
	}
}
     
     //almost same logic as used in third loop body
    if(action=='2')
    {
        	cout<<"\nCard : ";
		cin>>Card;
		while(Card>count_cards((player==1)?p1Hand:p2Hand)  || Card<1  )
		{
		cout<<"\nInvalid\n";
		cout<<"\nCard : ";
		cin>>Card;
		}
												
												
		if(isValidPlay((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile[0])==0)
            {
            cout<<"\nInvalidd\n";
            continue; //continue to first loop
            
            }
            type=card_type((player==1)?p1Hand[Card-1]:p2Hand[Card-1]);
        if(isValidPlay((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile[0])==1)
        		{
             	if(CALL==0 && CALL2==0 && callUno(DiscardPile,(player==1)?p1Hand:p2Hand)==1)
				{
				cout<<"\nUno not called, penalty of two cards : ";
				IncreaseUserDeck(DrawPile[0],(player==1)?p1Hand:p2Hand,DrawPile,DiscardPile);
				DrawCard(DrawPile);
				IncreaseUserDeck(DrawPile[0],(player==1)?p1Hand:p2Hand,DrawPile,DiscardPile);
				DrawCard(DrawPile);
				}
				if(type>9)
				{
				handleSpecialCard(type, player, Card, DrawPile,  p1Hand,  p2Hand,  DiscardPile);
				UpdateDiscardPile((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile);
				DecreaseUserDeck((player==1)?p1Hand[Card-1]:p2Hand[Card-1],(player==1)?p1Hand:p2Hand);
                    return;
				}
				if(type<=9)
				{
				UpdateDiscardPile((player==1)?p1Hand[Card-1]:p2Hand[Card-1],DiscardPile);
				DecreaseUserDeck((player==1)?p1Hand[Card-1]:p2Hand[Card-1],(player==1)?p1Hand:p2Hand);
                    return;			
				}
			}
        }	
	   break;				
    }
}




void handleSpecialCard(int specialCardType, int player, int card_num, int DrawPile[], int p1Hand[], int p2Hand[], int DiscardPile[]){
	
	int wildColor;
	
	switch(specialCardType){
		
		case 10: // skip
		case 11: // reverse
			turn++;
			break;
		
		case 12: // draw two
				//add 2 cards to enemy hand
			for(int i=0; i<2; i++){
				IncreaseUserDeck(DrawCard(DrawPile), (player==1)?p2Hand:p1Hand, DrawPile, DiscardPile);
			}
			turn++;	// player will play another turn
			break;	
		
		case 13: // wild
		case 14: // Draw 4
			do{
				cout<<"\n Change Color to: \n"
					<<"1. Red\n"
					<<"2. Green\n"
					<<"3. Blue\n"
					<<"4. Yellow\n";  
				cin>>wildColor;
				if(wildColor<1||wildColor>4)
					cout<<"\n invalid input\n";
			}while(wildColor<1||wildColor>4);

			switch(wildColor){
				case 1:
					if(specialCardType==13)
						(player==1)? p1Hand[card_num-1] = 113 : p2Hand[card_num-1] = 113;
					else
						(player==1)? p1Hand[card_num-1] = 114 : p2Hand[card_num-1] = 114;
					break;

				case 2:
					if(specialCardType==13)
						(player==1)? p1Hand[card_num-1] = 213 : p2Hand[card_num-1] = 213;
					else
						(player==1)? p1Hand[card_num-1] = 214 : p2Hand[card_num-1] = 214;
					break;

				case 3:
					if(specialCardType==13)
						(player==1)? p1Hand[card_num-1] = 313 : p2Hand[card_num-1] = 313;
					else
						(player==1)? p1Hand[card_num-1] = 314 : p2Hand[card_num-1] = 314;
					break;
					
				case 4:
					if(specialCardType==13)
						(player==1)? p1Hand[card_num-1] = 413 : p2Hand[card_num-1] = 413;
					else
						(player==1)? p1Hand[card_num-1] = 414 : p2Hand[card_num-1] = 414;
					break;
			}		
					
							
			
			
			
			
			// add 4 cards to enemy's hand
			if(specialCardType == 14){
				for(int i=0; i<4; i++){
					IncreaseUserDeck(DrawCard(DrawPile), (player==1)? p2Hand:p1Hand, DrawPile, DiscardPile);
				}
				turn++;	// player will play another turn
			}
			
			break;
	}
	
}





bool isValidPlay(int playedCard, int topCard)   //is valid play to find validity of cards
{	bool valid=0;
	//checking types and colors of top and played cards
	int type1 = card_type(playedCard);  
	int type2 = card_type(topCard);
	int color1 = card_color(playedCard);
	int color2 = card_color(topCard);
	
//	if(type2==13||type2==14){   //in case that the top card is wild, the its colored is assigned the called color by the user
//		switch(wildColor){
//			case 1: color2 = RED;	break;
//			case 2: color2 = GREEN; break;
//			case 3: color2 = BLUE;	break;
//			case 4: color2 = YELLOW;break;
//		}
//	}
	
	if(type1==13 || type1==14)    //if wild card is playedd so always valid
		return 1;
	
	if(( type1==type2 ) || ( color1==color2 ) )
		valid = 1;
	else
		valid=0;
	if(valid==0 && (color1!=color2) && card_color(playedCard)!=7)
	cout<<"\nNot the right color \n";
	return valid;
}




bool ValidPlayUNO(int playedCard, int topCard)  //valid play function only called in uno call function, bcz previous func also prints not the right color
{	bool valid=0;                              //same logic only "not right color" not included
	int type1 = card_type(playedCard);
	int type2 = card_type(topCard);
	int color1 = card_color(playedCard);
	int color2 = card_color(topCard);
	
	if(type1==13 || type1==14)
		return 1;
	
	if(( type1==type2 ) || ( color1==color2 ) )
		return 1;
	else
		return 0;
}






bool callUno(int DiscardPile[], int hand[])   //checks if call is valid
{   
	bool valid=0;
	if(hand[0]!=0 && hand[1]!=0 && hand[2]==0)  //checks if player has only 2 cards left
	{    
		if(card_type(hand[0])==13 || card_type(hand[0])==14 || card_type(hand[1])==13 || card_type(hand[0])==14 )  //check if the two remaining cards are wild or not  (if wild so valid)
	    	valid=1;
		if(ValidPlayUNO(hand[0],DiscardPile[0])==1 || ValidPlayUNO(hand[1],DiscardPile[0])==1)  //check if the two remaining cards are valid or not
			valid=1;
	}
	return valid;
}





void UpdateDiscardPile ( int playedCard, int DiscardPile[])
{
	for(int i=106;i>=0;i--)   //adds the played card to the discard pile by shifting the discard pile one element left and then assigning 
	{                         //the played card to the first element of discard pile
		DiscardPile[i+1]=DiscardPile[i];
	}
	DiscardPile[0]=playedCard;
}





void IncreaseUserDeck( int pickedCard,int hand[],int DrawPile[],int DiscardPile[])
{     
	if(pickedCard==0)   //if the picked card  is zero this means drawpile is empty, it reinitializes
	{
		cout<<"\nReinitialing Draw Deck ";
		for(int i=0;i<600;i++)  //adds delay
		delay();
		int temp=DiscardPile[0];  //stores first element of discard pile in temp variable
		DiscardPile[0]=0; //assigns the first element zero
		DrawCard(DiscardPile);//shifts the discard pile one element left
		int h=0,k,j;
		for(int i=0;i<108;i++)
		{
			k=rand()%108;       //shuffle the discard pile
			j=rand()%108;
			while(k==j)  //if random elements are same generate again
			{
			k=rand()%108;
			j=rand()%108;
			}
			swapping(DiscardPile[k],DiscardPile[j]);  //swapping the values
		}
		for(int i=0;i<108;i++)
		{      //as after shuffling the discard pile,zero values are scattered so we check every value if it is zero or not
			if(DiscardPile[i]!=0)     //assigning the values of shuffled discard pile to draw pile
			{
				DrawPile[h]=DiscardPile[i];  
				DiscardPile[i]=0; //assign the value of card to zero after giving it to draw pile
				h++;
			}
		}
		DiscardPile[0]=temp; //giving the discard pile the first card again
	
	}
	for(int i=0;i<108;i++)  //adding the picked card from draw pile to user hand
	{
		if(hand[i]==0)
		{
			hand[i]=DrawPile[0];
			return;
		}
	}
}




void DecreaseUserDeck(int playedCard,int hand[])   //when user plays a card, it removes that card from users hand 
{
	int cards=count_cards(hand);  //count the cards
 	for(int i=0;i<108;i++)       //it first finds the card played by user and assigns it zero value 
	{
		if(hand[i]==playedCard)
		{
			hand[i]=0;
			swapping(hand[i],hand[cards-1]);  //after that it swaps that zero value with the last card that the user has
	     	return;
          }
     }
}


void swapping(int &a,int &b) //swapping values
{
	a=a+b;
	b=a-b;
	a=a-b;
}




// number of cards in players hand
int count_cards(int hand[])  //count the number of cards the passed array has
{
	int counter=0;    //it counts all non zero values

	while(hand[counter]!=0){
		counter++;
	}
	
	return counter;
}




int card_color(int x)    //checks the color of card passed 
{ 
	int color;
	
	color = (x/100==0)? x/10 : x/100;   //first digit of the value tells us the color 
		
	switch(color)
	{
		case 1:  
			color=RED;
		break;
		case 2:
			color=GREEN;
		break;
		case 3:
			color=BLUE;
		break;
		case 4:
			color=YELLOW;
		break;
	}
		
	return color;
}


int card_type(int x)
{
	int type;
	 
	// if card is a 2 digit number --> x%10 returns last digit -- else card is 3 digit number, returns last 2 digits
	type = (x/100==0)? x%10 : x%100;

	return type;
}







void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}
