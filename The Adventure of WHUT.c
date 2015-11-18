#include "TAWlib.c"

void main()

{
	char input[Max_input];
	int tokens[Max_tokens],error=0;
	printf("Loading...");
	title();
	Initialize_Map_Object();
	printf("This is where the Adventure of WuHan University of Technology begins.\n\n");
	//Exit if game status is exit.
	while(game_status!=Game_Exit)
	{
		Initialize_Tokens(tokens);
		Tokens_Num=0;
		if(For_geometry_dess_sake!=1)
			Geometry_des();
		Event_Generator();
		if(game_status==Game_Running)
			printf("                                                      step:%d\n",step);
		gets(input);
		For_geometry_dess_sake=0;
		error=0;
		if(Input_Process(input,tokens,&error)==-1)
			continue;
		if(error==-1)
			continue;
		Verb_Parser(tokens);
	}
	printf("Exiting game..\n");
	_sleep(500);
}

