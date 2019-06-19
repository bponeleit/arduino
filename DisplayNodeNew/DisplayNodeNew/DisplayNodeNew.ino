#include "LCD4884.h"




/////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_MENU_ITEM  4

const char menuList[NUM_MENU_ITEM][NBCHAR_X] = {
"fct1",
"fct2",
"fct3",
"fct4"};

void fct1(void);
void fct2(void);
void fct3(void);
void fct4(void);
FONCTION menuFunction[NUM_MENU_ITEM] = {fct1, fct2, fct3, fct4};

const char *projectName = "Uniwav:";
/////////////////////////////////////////////////////////////////////////////////////////////////////




void setup()
{
	lcd.initClass(projectName, NUM_MENU_ITEM);
	lcd.showMenu(menuList);
}


void loop()
{
	lcd.browseMenu(menuList, menuFunction);
}




void fct1(void)
{
	/* */
}


void fct2(void)
{
	/* */
}


void fct3(void)
{
	/* */
}


void fct4(void)
{
	/* */
}
