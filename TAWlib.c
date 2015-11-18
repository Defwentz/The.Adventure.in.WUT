#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
//Max of stuff.
#define List_size 32
#define Map_num_rows 7
#define Map_num_columns 3
#define Max_input 100
#define Max_tokens 20
#define Max_carries 4
#define Max_sizeofword 16
#define Max_geometry 5
#define Max_des 100
#define Max_filename 50
#define Max_condition 3
//Game status.
#define Game_Running 1
#define Game_Exit -1
//Digitalized map.
#define w_n -1
#define c_5_d_31 135031
#define c_5_d_31_b 135331
#define c_5_d_30 135030
#define c_5_d_30_b 135330
#define c_5_h_s 135102
#define c_5_s_e 135200
#define o_n -1
//Condition.
#define Condi_Alarm 0
#define Condi_Oldman 1
#define Condi_Fakezombiebite 2
//Vocabulary.
#define Dir_north 8
#define Dir_northwest 7
#define Dir_northeast 9
#define Dir_south 2
#define Dir_southwest 1
#define Dir_southeast 3
#define Dir_west 4
#define Dir_east 6
#define Obj_book 19
#define Obj_phone 24
#define Obj_body 30
#define uObj_zombie 31
#define uObj_man 27
#define uObj_bunkbed 20 
#define Action_move 5
#define Action_pick 15
#define Action_put 18
#define Action_look 12
#define Action_talk 26
#define Action_inventory 21
#define Action_save 22
#define Action_restore 23
#define Action_examine 25
#define Action_exit -1
#define Adj_old 28
#define Adj_wise_looking 29
#define Prep_to 32
#define Prep_toward 14
#define Prep_at 13
#define Prep_up 16
#define Prep_down 17
#define Art_the 33
//Vocabulary.
struct token_typ 
{
	char word[Max_sizeofword];
	int value;
}language[]=
	{
	//To add or deduct word to the vocabulary,must add or deduct List_size as well. 
		{"north",Dir_north},
		{"northwest",Dir_northwest},
		{"northeast",Dir_northeast},
		{"south",Dir_south},
		{"southwest",Dir_southwest},
		{"southeast",Dir_southeast},
		{"west",Dir_west},
		{"east",Dir_east},
		{"book",Obj_book},
		{"phone",Obj_phone},
		{"body",Obj_body},
		{"zombie",uObj_zombie},
		{"man",uObj_man},
		{"bunkbed",uObj_bunkbed},
		{"move",Action_move},
		{"pick",Action_pick},
		{"put",Action_put},
		{"talk",Action_talk},
		{"look",Action_look},
		{"inventory",Action_inventory},
		{"save",Action_save},
		{"restore",Action_restore},
		{"examine",Action_examine},
		{"exit",Action_exit},
		{"old",Adj_old},
		{"wise-looking",Adj_wise_looking},
		{"the",Art_the},
		{"to",Prep_to},
		{"toward",Prep_toward},
		{"at",Prep_at},
		{"up",Prep_up},
		{"down",Prep_down},
	};
//To change the size of map,must also change Map_num_rows and Map_num_columns.
int map[Map_num_rows][Map_num_columns]=
{
	{w_n,w_n,w_n},
	{w_n,c_5_d_30_b,w_n},
	{w_n,c_5_d_30,w_n},
	{w_n,c_5_h_s,w_n},
	{w_n,c_5_d_31,w_n},
	{w_n,c_5_d_31_b,w_n},
	{w_n,w_n,w_n},
};

typedef struct _object
{
	int obj;	//code for the objects.
	struct _object *next;
}node;

node *map_object[Map_num_rows][Map_num_columns];
struct player_typ
{
	int x,y;	//cordinates of player.
	int health;	//health of player.
	//Somehow the money can't be a float.
	int money;	//money of player.
	int inventory[Max_carries];	//objects a player is holding.
	int num_object;	//The number of objects the player is holding.
}player={4,1,0,1,{Obj_phone,o_n,o_n,o_n},1};
//To identify whether the file is a game saving file.
const int signiture[7]={'t','a','w','h','u','t',987};
int condition[Max_condition]={1,1,1};
int step=0;
int For_geometry_dess_sake=0;
//The Number of each word.For example,"I am","I"will be 0,"am" will be 1.
int Tokens_Num;
//The status of the game.
int game_status=Game_Running;
//This function gets each word's beginning position and ending position.
void Get_Token(char *input,int *start,int *end,int *error);
//This function compares token with game's vocabulary and change token into number so it's easier for later use.
void Extract_Tokens(char *input,int start,int end,int *tokens,int *error);
//This function calls the action function accordingly.
void Verb_Parser(int *tokens);
//
void Event_Generator();
//Action function.
void Exit(int *tokens);
void Examine(int *tokens);
void Talk(int *tokens);
void Restore(int *tokens);
void Save(int *tokens);
void Inventory(int *tokens);
void Move(int *tokens);
void Look(int *tokens);
void Pick(int *tokens);
void Put(int *tokens);
//Action semantic analyser function.
int Pick_Semantic_Analyser(int *tokens,int *i);
int Put_Semantic_Analyser(int *tokens,int *i,int *whereabout);
int Move_Semantic_Analyser(int *tokens,int *i);
int Look_Semantic_Analyser(int *tokens,int *i);
int Inventory_Semantic_Analyser(int *tokens,int *i);
int Save_Semantic_Analyser(int *tokens,int *i);
int Restore_Semantic_Analyser(int *tokens,int *i);
int Examine_Semantic_Analyser(int *tokens,int *i,int *whereabout);
int Talk_Semantic_Analyser(int *tokens,int *i);
int Exit_Semantic_Analyser(int *tokens,int *i);
int isverb(int t);
int isobj(int t);
int isuobj(int t);
int isart(int t);
int isdir(int t);
void Geometry_des();
void Object_des_g();
void Object_des_i(int obj);
int Input_Process(char *input,int *tokens,int *error);
//Object System.
void _pick(int o,int whereabout);
void _mpick(int x,int y,int o);
void _put(int o,int whereabout);
void _mput(int x,int y,int o);
int _find(int o);
int _mfind(int x,int y,int o);
void _clear();
//Initialization.
void Initialize_Tokens(int *tokens);
void Initialize_Map_Object();
void title();
//NPC function.
void old_man();
void zombie();

//
//
//Initialization.
//
//
//

void Initialize_Tokens(int *tokens)

{
	int i;
	for(i=0;i<Max_tokens;i++)
		tokens[i]=0;
}

void title()

{
	FILE *fp=fopen("title.txt","r");
	char ch;
	system("cls");
	while(!feof(fp))
	{
		ch=getc(fp);
		putchar(ch);
	}
	_sleep(400);
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                         ");
	_sleep(400);
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b    PRESS ANY KEY TO CONTINUE");
	fclose(fp);
	while(!kbhit())
	{;}
	getch();
	system("cls");
}

void Initialize_Map_Object()

{
	int i,j;
	node *p;
	for(i=0;i<Map_num_rows;i++)
	{
		for(j=0;j<Map_num_columns;j++)
		{
			map_object[i][j]=malloc(sizeof(node));
			map_object[i][j]->obj=o_n;
			map_object[i][j]->next=0;
		}
	}
	p=map_object[2][1];
	p->obj=uObj_bunkbed;
	p=map_object[3][1];
	p->obj=uObj_man;
	p=map_object[4][1];
	p->obj=uObj_bunkbed;
	p=malloc(sizeof(node));
	p->obj=Obj_book;
	p->next=0;
	map_object[4][1]->next=p;
}

//
//
//Object system part.
//
//
//
//

void _pick(int o,int whereabout)
{
	node *p,*q;
	p=map_object[player.x][player.y];
	q=map_object[player.x][player.y];
	if(p->obj==o)
	{
		p->obj=o_n;
		if(whereabout!=-1)
			player.inventory[whereabout]=o;
		return;
	}
	p=p->next;
	while(p)
	{
		if(p->obj==o)
		{
			q->next=p->next;
			if(whereabout!=-1)
				player.inventory[whereabout]=o;
			free(p);
			return;
		}
		p=p->next;
		q=q->next;
	}
	printf("The object is either not here or not important.\n");
}

void _mpick(int x,int y,int o)
{
	node *p,*q;
	p=map_object[x][y];
	q=map_object[x][y];
	if(p->obj==o)
	{
		p->obj=o_n;
		return;
	}
	p=p->next;
	while(p)
	{
		if(p->obj==o)
		{
			q->next=p->next;
			free(p);
			return;
		}
		p=p->next;
		q=q->next;
	}
}

void _put(int o,int whereabout)
{
	node *p,*q;
	p=map_object[player.x][player.y];
	if(p->obj==o_n)
	{
		p->obj=o;
		if(whereabout!=-1)
			player.inventory[whereabout]=o_n;
		return;
	}
	while(p)
	{
		q=p;
		p=p->next;
	}
	p=malloc(sizeof(node));
	p->obj=o;
	p->next=0;
	q->next=p;
	if(whereabout!=-1)
		player.inventory[whereabout]=o_n;
}

void _mput(int x,int y,int o)

{
	node *p,*q;
	p=map_object[x][y];
	if(p->obj==o_n)
	{
		p->obj=o;
		return;
	}
	while(p)
	{
		q=p;
		p=p->next;
	}
	p=malloc(sizeof(node));
	p->obj=o;
	p->next=0;
	q->next=p;
}

int _find(int o)

{
	node *p;
	p=map_object[player.x][player.y];
	if(p->obj==o_n)
		return 0;
	while(p)
	{
		if(p->obj==o)
			return 1;
		p=p->next;
	}
	return 0;
}

int _mfind(int x,int y,int o)

{
	node *p;
	p=map_object[x][y];
	if(p->obj==o_n)
		return 0;
	while(p)
	{
		if(p->obj==o)
			return 1;
		p=p->next;
	}
	return 0;
}

void _clear()

{
	;//A function that clear the linklist of map_object.Free some space.
}

//
//
//Input processing part.
//
//
//
//

int Input_Process(char *input,int *tokens,int *error)

{
	int start,end;
	end=start=0;	
	while(input[end]!='\0')
	{
		//if the first character is punct then break.
		if(ispunct(input[end])&&end==0)
		{
			*error=-1;
			printf("You can't start with these thing instead of words.\n");
			return -1;
		}
		if(ispunct(input[end])&&input[end]!='-')
			return -1;			
		Get_Token(input,&start,&end,&(*error));
		if(*error==-1)
			return -1;
		//if the input doesn't have a match with the vocabulary then break.
		Extract_Tokens(input,start,end,tokens,&(*error));
		if(*error==-1)
			return -1;
	}
	return 0;
}

void Get_Token(char *input,int *start,int *end,int *error)

{
	if(input[*start]=='\0')
	{
		if(Tokens_Num==0)
		{
			printf("That's just blank stuff,man.\n");
			(*error)=-1;
			return;		
		}
		return;
	}
	//Get start into position.
	if((*end)!=0)
		(*start)=(*end)++;
	//Eat space.
	while(input[*start]==' ')
		(*start)=(*end)++;
	while(!isspace(input[*end])&&(!ispunct(input[*end])||input[*end]=='-')&&input[*end]!='\0')
	{
		if(isupper(input[*end]))
			input[*end]=tolower(input[*end]);
		(*end)++;
	}
}

void Extract_Tokens(char *input,int start,int end,int *tokens,int *error)

{
	int i,k;
	//When the Max_sizeofword is adjust,the initalization will need to adjust as well.
	char token[Max_sizeofword]={"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
	if(input[start]=='\0')
		return;
	for(i=start,k=0;i!=end;i++,k++)
		token[k]=input[i];
	i=0;
	while(strcmp(token,language[i].word)!=0)
	{
		i++;
		if(i>List_size)
		{
			printf("Sorry.I don't recognize \"%s\".\n",token);
			(*error)=-1;
			return;
		}
	}
	tokens[Tokens_Num]=language[i].value;
	Tokens_Num++;
}

//
//
//Output part.
//
//
//

void Object_des_i(int obj)

{
	switch(obj)
	{
	case Obj_book:
		{
			printf("  A book\n");
			break;
		}
	case Obj_phone:
		{
			printf("  A phone\n");
			break;
		}
	case Obj_body:
		{
			printf("  A wise-looking old man's body\n");
			break;
		}
	default:
		{
			break;
		}
	}
}

void Object_des_g()

{
	node *p;
	p=map_object[player.x][player.y];
	while(p)
	{
		switch(p->obj)
		{
		case Obj_book:
			{
				printf("And there's a book.");
				break;
			}
		case Obj_phone:
			{
				printf("And there's a phone.");
				break;
			}
		case Obj_body:
			{
				printf("And there's a body.");
				break;
			}
		case uObj_man:
			{
				printf("And there's a wise-looking old man.");
				break;
			}
		case uObj_bunkbed:
			{
				printf("And there's a bunkbed.");
				break;
			}
		case uObj_zombie:
			{
				printf("And there's a zombie.");
				break;
			}
		default:
			{
				break;
			}
		}
		p=p->next;
	}
	putchar('\n');
}

void Geometry_des()

{
	static int i[Max_geometry]={0,0,0,0,0};	//Quite a waste to be using int here.
	switch(map[player.x][player.y])
	{
	case c_5_d_31:
		{
			printf("Dorm Room 531\n");
			if(!i[0])
			{
				printf("It's a nice dorm room.A bathroom lies to the south and the front door to the north,outside of which is hallway.");
				Object_des_g();
				i[0]=1;
			}
			break;
		}
	case c_5_h_s:
		{
			printf("Hallway\n");
			if(!i[1])
			{
				printf("It's a neat hallway.Dorm room 531 in the south and dorm room 530 in the north.The road end here,and to the east there seems to be a turn.");
				Object_des_g();
				i[1]=1;
			}
			break;
		}
	case c_5_d_30:
		{
			printf("Dorm Room 530\n");
			if(!i[2])
			{
				printf("It's a really massy dorm room.A bathroom lies to the nouth and the front door to the sorth,outside of which is hallway.");
				Object_des_g();
				i[2]=1;
			}
			break;
		}
	case c_5_d_30_b:
		{
			printf("Bathroom of Dorm Room 530\n");
			if(!i[3])
			{
				printf("It's quite simpe and crude,but functional.A place for No.1&No.2&shower.");
				Object_des_g();
				i[3]=1;
			}
			break;
		}
	case c_5_d_31_b:
		{
			printf("Bathroom of Dorm Room 531\n");
			if(!i[4])
			{
				printf("It's quite simpe and crude,but functional.A place for No.1&No.2&shower.");
				Object_des_g();
				i[4]=1;
			}
			break;
		}
	default:
		{
			printf("No way this is happening.\n");
			break;
		}
	}
}

void Event_Generator()

{
	int i,j,con_phone=0;
	static int Zobtt=0;
	if(player.health==1)
	{
		printf("You are feeling a little dizzy.\n");
	}
	for(i=0;i<Max_carries;i++)
	{
		if(player.inventory[i]==Obj_phone)
			con_phone=1;
	}
	if(_find(Obj_phone))
		con_phone=1;
	if(condition[Condi_Alarm]&&con_phone)
		printf("The alarm of the phone on you is going off.\n");
	if(_find(uObj_zombie)&&player.health==0)
	{
		printf("The zombie is comming at you.\n");
		zombie();
	}
	if(!condition[Condi_Oldman])
	{
		condition[Condi_Oldman]=1;
		Zobtt=step-1;
	}
	if((step-Zobtt)==5)
	{
		for(i=0;i<Max_carries;i++)
		{
			if(player.inventory[i]==Obj_body)
				break;
		}
		if(i<Max_carries)
		{
			player.inventory[i]=o_n;
			//unfinished.
			_put(uObj_zombie,-1);
			printf("The body seems to start moving.\nYou dropped it on the floor.You soon realize,IT A ZOMBIE!!!!!\n");
		}
		else
		{
			for(i=0;i<Map_num_rows;i++)
			{
				for(j=0;j<Map_num_columns;j++)
				{
					if(_mfind(i,j,Obj_body))
					{
						_mpick(i,j,Obj_body);
						//unfinished.
						_mput(i,j,uObj_zombie);
						if(_find(uObj_zombie))
							printf("The body seems to start moving.\nYou soon realize,IT A ZOMBIE!!!!!\n");
					}
				}
			}
		}
	}
}

//
//
//NPC function part.
//
//
//

void old_man()

{
	printf("\"I see strength in you,son.Now I'm going to tell something very important.\"\n");
	_sleep(4000);
	printf("\"WuHan University of Technology is in great danger now.Only you can save WHUT now.There's a secret that had been passed in generations.My grandfather told me this,actually.\"\n");
	_sleep(5000);
	printf("\"Once upon a time,this was a land of freedom.the king was very kind and good.People lived here happily.Until one day,A evil wizard called \'Blah\' showed up in this land with his evil dragon pet \'Blahh\'.He killed the king and kidnapped the princess.\"\n");
	_sleep(7500);
	printf("\"And then he ruled the kingdom.It's was a dark time.People suffered.Until one day,a hero stood out.The sword made by his father he held,he slayed the dragon and then defeated Blah and save the princess.Then he renamed what's left of the kingdom\'WuHan University of Technology.\'\"\n");
	_sleep(75000);
	//This is where I left off.
	printf("\"As time passed the place became what we see today.It was said that the hero's spirit got into the sword after he died.And then weird things began to happen around the sword.In the end,people kept the sword in a secret location pertected by a group of people.Great evil has fallen today.You must find the sword.Find the man named F...\"\n");
	_sleep(75000);
	printf("\".......eh....\"\n(The wise-looking old man just dropped dead.You held him in your arm.And you realized that he had been losing a lot of blood.)\n");
	_pick(uObj_man,-1);
	_put(Obj_body,-1);
	condition[Condi_Oldman]=0;
}

void zombie()

{
	int s;
	srand((unsigned)time(0));
	s=rand()%4+1;
	switch(s)
	{
	case 1:
		{
			printf("Oh no!You are bitten.You should probably leave now...\nThe zombie seems statified that his job well done and.....\n");
			_sleep(2000);
			printf("Now he's leaving.\n");
			_sleep(2000);
			_pick(uObj_zombie,-1);
			player.health=1;
			condition[Condi_Fakezombiebite]=0;
			break;
		}
	default:
		{
			printf("You alomst got bitten.Thankfully not,you need to go.Like,right now!\n");
			break;
		}
	}
	
}

//
//
//Verb parser part.
//
//
//
//

void Verb_Parser(int *tokens)

{
	switch(tokens[0])
	{
	case Action_move:
		{
			Move(tokens);
			break;
		}
	case Action_pick:
		{
			Pick(tokens);
			break;
		}
	case Action_put:
		{
			Put(tokens);
			break;
		}
	case Action_look:
		{
			Look(tokens);
			break;
		}
	case Action_inventory:
		{
			Inventory(tokens);
			break;
		}
	case Action_examine:
		{
			Examine(tokens);
			break;
		}
	case Action_talk:
		{
			Talk(tokens);
			break;
		}
	case Action_save:
		{
			Save(tokens);
			break;
		}
	case Action_restore:
		{
			Restore(tokens);
			break;
		}
	case Action_exit:
		{
			Exit(tokens);
			break;
		}
	default:
		{
			printf("You must start a sentence with an action verb.\n");
			break;
		}
	}
}

//
//
//Semantic analyzing part.
//
//
//

int Pick_Semantic_Analyser(int *tokens,int *i)

{
	char input[Max_input];
	int t[Max_tokens],error=0;
	if(player.num_object+1>Max_carries)
	{
		printf("You can't carry this much.\n");
		return -1;
	}
	Initialize_Tokens(t);
	if(tokens[*i]=='\0'||(tokens[*i]==Prep_up&&tokens[*i+1]=='\0'))
	{
		printf("What do you want to pick up?\n");
		gets(input);
		Tokens_Num=0;
		if(Input_Process(input,t,&error)==-1)
			return -1;
		if(t[1])
		{
			printf("Just \"object\" is okay.\n");
			return -1;
		}
		if(!isobj(t[0]))
		{
			printf("Illegal sentence.\n");
			return -1;
		}
		if(isuobj(t[0]))
		{
			printf("Yeah.Man,Like you can just pick it up.\n");
			return -1;
		}
		if(!_find(t[0]))
		{
			printf("The object is either not here or not important.\n");
			return -1;
		}
		tokens[*i]=t[0];
		return 0;
	}
	if(tokens[*i]==Prep_up)
		(*i)++;
	if(isart(tokens[*i]))
		(*i)++;
	if(!isobj(tokens[*i])||tokens[*i+1]!='\0')
	{
		printf("Illegal sentence.\n");
		return -1;
	}
	if(isuobj(tokens[*i]))
	{
		printf("Yeah.Man,Like you can just pick it up.\n");
		return -1;
	}
	if(!_find(tokens[*i]))
	{
		printf("The object is either not here or not important.\n");
		return -1;
	}
	return 0;
}

int Put_Semantic_Analyser(int *tokens,int *i,int *whereabout)

{
	char input[Max_input];
	//8 is where object begins.
	int t[Max_tokens],k=0,error=0;
	Initialize_Tokens(t);
	if(tokens[*i]=='\0'||(tokens[*i]==Prep_down&&tokens[*i+1]=='\0'))
	{
		printf("What do you want to put down?\n");
		gets(input);
		Tokens_Num=0;
		if(Input_Process(input,t,&error)==-1)
			return -1;
		if(t[1])
		{
			printf("Just \"object\" is okay.\n");
			return -1;
		}
		if(!isobj(t[0]))
		{
			printf("Illegal sentence.\n");
			return -1;
		}
		if(isuobj(t[0]))
		{
			printf("You can't possibly be in possession of this.\n");
			return -1;
		}
		while(k<Max_carries)
		{
			if(t[0]==player.inventory[k])
			{
				*whereabout=k;
				tokens[*i]=t[0];
				return 0;
			}
			k++;
		}
		printf("You are not in possession of this.\n");
		return -1;
	}
	if(tokens[*i]==Prep_down)
		(*i)++;
	if(isart(tokens[*i]))
		(*i)++;
	if(!isobj(tokens[*i])||tokens[*i+1]!='\0')
	{
		printf("Illegal sentence.\n");
		return -1;
	}
	if(isuobj(tokens[*i]))
	{
		printf("You can't possibly have it.\n");
		return -1;
	}
	k=0;
	while(k<Max_carries)
	{
		if(tokens[*i]==player.inventory[k])
		{
			*whereabout=k;
			return 0;
		}
		k++;
	}
	printf("You are not in possession of this.\n");
	return -1;
}

int Move_Semantic_Analyser(int *tokens,int *i)

{
	int error=0,t[Max_tokens];
	char input[Max_input];
	Initialize_Tokens(t);
	if(tokens[*i]=='\0'||((tokens[*i]==Prep_to||tokens[*i]==Prep_toward)&&tokens[*i+1]=='\0'))
	{
		printf("To which direction?\n");
		gets(input);
		Tokens_Num=0;
		if(Input_Process(input,t,&error)==-1)
			return -1;
		if(t[1])
		{
			printf("Just \"direction\" is okay.\n");
			return -1;
		}
		if(!isdir(t[0]))
		{
			printf("Illegal sentence.\n");
			return -1;
		}
		tokens[*i]=t[0];
		return 0;
	}
	if(tokens[*i]==Prep_to||tokens[*i]==Prep_toward)
		(*i)++;
	if(isart(tokens[*i]))
		(*i)++;
	if(!isdir(tokens[*i])||tokens[*i+1]!='\0')
	{
		printf("Illegal sentence.\n");
		return -1;
	}
	return 0;
}

int Look_Semantic_Analyser(int *tokens,int *i)

{
	if(tokens[*i]!='\0')
	{
		printf("Just \"look\" is okay.\n");
		return -1;
	}
	return 0;
}

int Save_Semantic_Analyser(int *tokens,int *i)

{
	if(tokens[*i]!='\0')
	{
		printf("Just \"save\" is okay.\n");
		return -1;
	}
	return 0;
}

int Restore_Semantic_Analyser(int *tokens,int *i)

{
	if(tokens[*i]!='\0')
	{
		printf("Just \"restore\" is okay.\n");
		return -1;
	}
	return 0;
}

int Exit_Semantic_Analyser(int *tokens,int *i)

{
	if(tokens[*i]!='\0')
	{
		printf("Just \"exit\" is okay.\n");
		return -1;
	}
	return 0;
}

int Inventory_Semantic_Analyser(int *tokens,int *i)

{
	if(tokens[*i]!='\0')
	{
		printf("Just \"Inventory\" is okay.\n");
		return -1;
	}
	return 0;
}

int Examine_Semantic_Analyser(int *tokens,int *i,int *whereabout)

{
	char input[Max_input];
	int t[Max_tokens],error=0,k=0;
	Initialize_Tokens(t);
	if(tokens[*i]=='\0')
	{
		printf("What do you want to examine?\n");
		gets(input);
		Tokens_Num=0;
		if(Input_Process(input,t,&error)==-1)
			return -1;
		if(t[1])
		{
			printf("Just \"object\" is okay.\n");
			return -1;
		}
		while(k<Max_carries)
		{
			if(t[0]==player.inventory[k])
			{
				*whereabout=k;
				tokens[*i]=t[0];
				return 0;
			}
			k++;
		}
		if(_find(t[0]))
		{
			tokens[*i]=t[0];
			return 0;
		}
		printf("The object is either not here or not important.\n");
		return -1;
	}
	if(isart(tokens[*i]))
		(*i)++;
	if(!isobj(tokens[*i])||tokens[*i+1]!='\0')
	{
		printf("Illegal sentence.\n");
		return -1;
	}
	while(k<Max_carries)
	{
		if(tokens[*i]==player.inventory[k])
		{
			*whereabout=k;
			return 0;
		}
		k++;
	}
	if(_find(tokens[*i]))
		return 0;
	printf("The object is either not here or not important.\n");
	return -1;
}

int Talk_Semantic_Analyser(int *tokens,int *i)

{
	char input[Max_input];
	int t[Max_tokens],error=0,j=0;
	Initialize_Tokens(t);
	if(tokens[*i]=='\0'||(tokens[*i]==Prep_to&&tokens[*i+1]=='\0'))
	{
		printf("Who do you want to talk to?\n");
		gets(input);
		Tokens_Num=0;
		if(Input_Process(input,t,&error)==-1)
			return -1;
		if(isart(t[j]))
			j++;
		if(t[j]==Adj_wise_looking||t[j]==Adj_old)
			j++;
		if(t[j]==Adj_old)
			j++;
		if(t[j+1])
		{
			printf("Just the \"subject\" is okay.\n");
			return -1;
		}
		if(!isobj(t[j]))
		{
			printf("Illegal sentence.\n");
			return -1;
		}
		if(t[j]==uObj_man)
		{
			if(!_find(uObj_man))
			{
				printf("Well,he's not here.\n");
				return -1;
			}
		}
		tokens[*i]=t[j];
		return 0;
	}
	if(tokens[*i]==Prep_to)
		(*i)++;
	if(isart(tokens[*i]))
		(*i)++;
	if(tokens[*i]==Adj_wise_looking||tokens[*i]==Adj_old)
		(*i)++;
	if(tokens[*i]==Adj_old)
		(*i)++;
	if(!isobj(tokens[*i])||tokens[*i+1]!='\0')
	{
		printf("Illegal sentence.\n");
		return -1;
	}
	if(tokens[*i]==uObj_man)
	{
		if(!_find(uObj_man))
		{
			printf("Well,he's not here.\n");
			return -1;
		}
		return 0;
	}
	return 0;
}

//
//
//Verb part.
//
//
//

void Pick(int *tokens)

{
	int i=1,whereabout;
	if(Pick_Semantic_Analyser(tokens,&i)==-1)
		return;
	for(whereabout=0;player.inventory[whereabout]!=o_n;whereabout++);
	switch(tokens[i])
	{
	case Obj_book:
		{
			_pick(Obj_book,whereabout);
			player.num_object++;
			printf("You have picked up the book.\n");
			step++;
			break;
		}
	case Obj_phone:
		{
			_pick(Obj_phone,whereabout);
			player.num_object++;
			printf("You have picked up the phone.\n");
			step++;
			break;
		}
	case Obj_body:
		{
			_pick(Obj_body,whereabout);
			player.num_object++;
			printf("You have picked up the body.\n");
			step++;
			break;
		}
	default:
		{
			printf("This object is either not here or not important.\n");
			break;
		}
	}
}

void Put(int *tokens)

{
	int i=1,j=0,whereabout=0;
	if(Put_Semantic_Analyser(tokens,&i,&whereabout)==-1)
		return;
	switch(tokens[i])
	{
	case Obj_book:
		{
			_put(Obj_book,whereabout);
			player.num_object--;
			printf("You have put down the book.\n");
			step++;
			break;
		}
	case Obj_phone:
		{
			_put(Obj_phone,whereabout);
			player.num_object--;
			printf("You have put down the phone.\n");
			step++;
			break;
		}
	case Obj_body:
		{
			_put(Obj_body,whereabout);
			player.num_object--;
			printf("You have put down the body.\n");
			step++;
			break;
		}
	default:
		{
			printf("This object is either not here or not important.\n");
			break;
		}
	}
}

void Look(int *tokens)

{
	int i=1;
	if(Look_Semantic_Analyser(tokens,&i)==-1)
		return;
	switch(map[player.x][player.y])
	{
	case c_5_d_31:
		{
			printf("It's a nice dorm room.A bathroom lies to the south and the front door to the north,outside of which is hallway.");
			Object_des_g();
			step++;
			break;
		}
	case c_5_h_s:
		{
			printf("It's a neat hallway.Dorm room 531 in the south and dorm room 530 in the north.The road end here,and to the east there seems to be a turn.");
			Object_des_g();
			step++;
			break;
		}
	case c_5_d_30:
		{
			printf("It's a really massy dorm room.A bathroom lies to the nouth and the front door to the sorth,outside of which is hallway.");
			Object_des_g();
			step++;
			break;
		}
	case c_5_d_30_b:
		{
			printf("It's quite simpe and crude,but functional.A place for No.1&No.2&shower.");
			Object_des_g();
			step++;
			break;
		}
	case c_5_d_31_b:
		{
			printf("It's quite simpe and crude,but functional.A place for No.1&No.2&shower.");
			Object_des_g();
			step++;
			break;
		}
	default:
		{
			printf("\nNo way this is happening.\n");
			break;
		}
	}
}

void Move(int *tokens)

{
	int i=1;
	if(Move_Semantic_Analyser(tokens,&i)==-1)
		return;
	switch(tokens[i])
	{
	case Dir_north:
		{
			if(map[(player.x)-1][player.y]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.x)--;
			Geometry_des();
			step++;
			break;
		}
	case Dir_south:
		{
			if(map[(player.x)+1][player.y]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.x)++;
			Geometry_des();
			step++;
			break;
		}
	case Dir_east:
		{
			if(map[player.x][(player.y)+1]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.y)++;
			Geometry_des();
			step++;
			break;
		}
	case Dir_west:
		{
			if(map[player.x][(player.y)-1]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.y)--;
			Geometry_des();
			step++;
			break;
		}
	case Dir_northwest:
		{
			if(map[(player.x)-1][(player.y)-1]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.x)--;
			(player.y)--;
			Geometry_des();
			step++;
			break;
		}
	case Dir_northeast:
		{
			if(map[(player.x)-1][(player.y)+1]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.y)++;
			(player.x)--;
			Geometry_des();
			step++;
			break;
		}
	case Dir_southwest:
		{
			if(map[(player.x)+1][(player.y)-1]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.x)++;
			(player.y)--;
			Geometry_des();
			step++;
			break;
		}
	case Dir_southeast:
		{
			if(map[(player.x)+1][(player.y)+1]==w_n)
			{
				printf("You can't go that way.\n");
				return;
			}
			(player.x)++;
			(player.y)++;
			Geometry_des();
			step++;
			break;
		}
	default:
		{
			printf("Type in direction.\n");
			break;
		}
	}
	For_geometry_dess_sake=1;
}

void Talk(int *tokens)

{
	int i=1;
	if(Talk_Semantic_Analyser(tokens,&i)==-1)
		return;
	switch(tokens[i])
	{
	case uObj_man:
		{
			old_man();
			step++;
			break;
		}
	default:
		{
			printf("...What,you crazy?\n");
			break;
		}
	}
}

void Inventory(int *tokens)

{
	int i=1;
	if(Inventory_Semantic_Analyser(tokens,&i)==-1)
		return;
	printf("You have:\n");
	for(i=0;i<Max_carries;i++)
	{
		if(player.inventory[i]==o_n)
			continue;
		Object_des_i(player.inventory[i]);
	}
	putchar('\n');
	step++;
}

void Save(int *tokens)

{
	int i=1,j;
	FILE *fp;
	node *p,*q;
	char filename[Max_filename];
	p=map_object[0][0];
	if(Save_Semantic_Analyser(tokens,&i)==-1)
		return;
	printf("Enter the directory then file's name.\n");
	gets(filename);
	if((fp=fopen(filename,"wb"))==NULL)
	{
		printf("Save failed.\n");
		return;
	}
	for(i=0;i<7;i++)
		fprintf(fp,"%d|",signiture[i]);
	fprintf(fp,"%d|%d|%d|%d",player.x,player.y,player.health,player.money);
	for(i=0;i<Max_carries;i++)
		fprintf(fp,"|%d",player.inventory[i]);
	fprintf(fp,"|%d",player.num_object);
	for(i=0;i<Map_num_rows;i++)
	{
		for(j=0;j<Map_num_columns;j++)
		{
			p=map_object[i][j];
			q=map_object[i][j];
			while(p)
			{
				q=p;
				fprintf(fp,"|%d",q->obj);
				p=p->next;
				if(p)
					fprintf(fp," ");
				else
					fprintf(fp,"#");
			}
		}
	}
	for(i=0;i<Max_condition;i++)
		fprintf(fp,"|%d",condition[i]);
	fprintf(fp,"|%d",step);
	fclose(fp);
}

void Restore(int *tokens)

{
	int i=1,j,cmp[7],o;
	FILE *fp;
	node *p,*q;
	char filename[Max_filename],c;
	if(Restore_Semantic_Analyser(tokens,&i)==-1)
		return;
	printf("Enter the directory then file's name.\n");
	gets(filename);
	if((fp=fopen(filename,"rb"))==NULL)
	{
		printf("Restore failed.\n");
		return;
	}
	for(i=0;i<7;i++)
	{
		fscanf(fp,"%d|",&cmp[i]);
		if(cmp[i]!=signiture[i])
		{
			printf("This file isn't a game file.\n");
			return;
		}
	}
	_clear();
	//Maybe examine the parameters in a limited range.Just so all the data make sense.
	//And Maybe get some sort good stuff to alter the data into more sophisitcated data,which is hard to transfer.You know,to cheat.
	fscanf(fp,"%d|%d|%d|%d",&player.x,&player.y,&player.health,&player.money);
	for(i=0;i<Max_carries;i++)
		fscanf(fp,"|%d",&player.inventory[i]);
	fscanf(fp,"|%d",&player.num_object);
	for(i=0;i<Map_num_rows;i++)
	{
		for(j=0;j<Map_num_columns;j++)
		{
			p=map_object[i][j];
			q=map_object[i][j];
			fscanf(fp,"|%d%c",&o,&c);
			p->obj=o;
			p->next=0;
			while(c==' ')
			{
				q=p;
				p=p->next;
				p=malloc(sizeof(node));
				fscanf(fp,"|%d%c",&o,&c);
				p->obj=o;
				p->next=0;
				q->next=p;
			}
		}
	}
	for(i=0;i<Max_condition;i++)
		fscanf(fp,"|%d",&condition[i]);
	fscanf(fp,"|%d",&step);
	fclose(fp);
	//Maybe an error check funtion in the end,making sure all the stuff are within its possible range.
}

void Examine(int *tokens)

{
	int whereabout=-1,i=1;
	if(Examine_Semantic_Analyser(tokens,&i,&whereabout)==-1)
		return;
	if(whereabout==-1)
	{
		switch(tokens[i])
		{
		case Obj_book:
			{
				printf("It's a book of some sort.\n");
				step++;
				break;
			}
		case Obj_phone:
			{
				if(condition[Condi_Alarm]==1)
				{
					printf("You turned off the alarm.And you noticed that there are some voice mails left by your best friend Python:\n\"Where the hell are you?What!?(He stopped talking to the phone,obviously somethine drew his attention away.And there are few other rather strange voices in the background.And then it's over.)\"\n\"Come and meet me at the usual place.If we(someone suddenly shouted\'COME ON!I CAN'T HO\'As something sounded like a door was busted open,the strange voices appeared in the background again.And this time there's also a scream.And then it's over.)\"\n");
					condition[Condi_Alarm]=0;
					_sleep(2000);
				}
//There might be a bug.When condition[Condi_Alarm]!=0
				else
					printf("It's just a regular phone.\n");
				step++;
				break;
			}
		case Obj_body:
			{
				printf("Well,I didn't really want to,but okay.\nHe lost a lot of blood.There's a pretty serious wound on him.It appears to be a bite of some sort.\nOkay,this is getting weirder.\n");
				step++;
				break;
			}
		default:
			{
				printf("You find norhing particular.\n");
				break;
			}
		}
	}
	else
	{
		switch(player.inventory[whereabout])
		{
		case Obj_book:
			{
				printf("It's a book of some sort.\n");
				step++;
				break;
			}
		case Obj_phone:
			{
				if(condition[Condi_Alarm]==1)
				{
					printf("You turned off the alarm.And you noticed that there are some voice mails left by your best friend Python:\n\"Where the hell are you?What!?(He stopped talking to the phone,obviously somethine drew his attention away.And there are few other rather strange voices in the background.And then it's over.)\"\n\"Come and meet me at the usual place.If we(someone suddenly shouted\'COME ON!I CAN'T HO\'As something sounded like a door was busted open,the strange voices appeared in the background again.And this time there's also a scream.And then it's over.)\"\n");
					condition[Condi_Alarm]=0;
				}
//There might be a bug.When condition[Condi_Alarm]!=0
				else
					printf("It's just a regular phone.\n");
				step++;
				break;
			}
		default:
			{
				printf("You find nothing particular.\n");
				break;
			}
		}
	}
}
void Exit(int *tokens)

{
	int i=1;
	if(Exit_Semantic_Analyser(tokens,&i)==-1)
		return;
	game_status=Game_Exit;
}

//
//
//Semantic Stuff
//
//
//

int isverb(int t)

{
	switch(t)
	{
	case Action_move:return 1;
	case Action_pick:return 1;
	case Action_put:return 1;
	case Action_talk:return 1;
	case Action_look:return 1;
	case Action_inventory:return 1;
	case Action_save:return 1;
	case Action_restore:return 1;
	case Action_examine:return 1;
	case Action_exit:return 1;
	default:return 0;
	}
}

int isobj(int t)

{
	switch(t)
	{
	case Obj_book:return 1;
	case Obj_phone:return 1;
	case Obj_body:return 1;
	case uObj_zombie:return 1;
	case uObj_man:return 1;
	case uObj_bunkbed:return 1;
	default:return 0;
	}
}

int isuobj(int t)

{
	switch(t)
	{
	case uObj_zombie:return 1;
	case uObj_man:return 1;
	case uObj_bunkbed:return 1;
	default:return 0;
	}
}

int isart(int t)

{
	switch(t)
	{
	case Art_the:return 1;
	default:return 0;
	}
}

int isdir(int t)

{
	switch(t)
	{
	case Dir_north:return 1;
	case Dir_northwest:return 1;
	case Dir_northeast:return 1;
	case Dir_south:return 1;
	case Dir_southwest:return 1;
	case Dir_southeast:return 1;
	case Dir_west:return 1;
	case Dir_east:return 1;
	default:return 0;
	}
}