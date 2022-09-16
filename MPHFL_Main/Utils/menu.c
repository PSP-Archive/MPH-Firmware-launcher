
/*

  Menu functions source

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include "menu.h"


// *** GLOBALS VARIABLES ***

FontType menuFont;						// Font of menu
u32 menuInitialized = 0;				// To know if init function has been called
u32 menuBgColor = 0;					// Current bgcolor for color function


// *** FUNCTIONS ***

u32 menuDrawBar (MenuBar *bar)

/*

  Draw a bar

  Parameters :	bar				->	Pointer of bar to draw

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuItem *pointer;
 FontOption fontOpt;
 u32 x, y;


 // Verify parameters
 if (!(bar)) return 1;

 // Quit if not showed
 if (!(bar->show)) return 0;

 // Draw bar depending location
 graphicDrawRectangle(MENU_BAR1_X,((bar->pos) ? (MENU_BASE_DOWN - MENU_BAR2_Y) : MENU_BAR1_Y),MENU_BAR1_W,MENU_BAR1_H,MENU_BAR1_COLOR,1);
 graphicDrawRectangle(MENU_BAR2_X,((bar->pos) ? (MENU_BASE_DOWN - MENU_BAR1_Y) : MENU_BAR2_Y),MENU_BAR2_W,MENU_BAR2_H,MENU_BAR2_COLOR,1);

 // Draw items
 pointer = bar->items;
 
 fontOpt.width = 10;
 fontOpt.height = 10;
 fontOpt.space = 0;

 x = MENU_BASE_ITEM;

 while (pointer)
 {
  // Draw selection rectangle
  if ((bar->parent->focus == MENU_FOCUS_BAR) && (pointer == bar->cur)) graphicDrawRectangle(x-1,((bar->pos) ? MENU_BASE_DOWN : MENU_BASE_UP),(strlen(pointer->name) * fontOpt.width) + 2,fontOpt.height,MENU_SELECTION_COLOR,1);

  // Draw text
  fontPrint(pointer->name,&fontOpt,x,((bar->pos) ? MENU_BASE_DOWN : MENU_BASE_UP));

  // If not used, print line in middle of text
  if (!(pointer->used))
  {
   y = (((bar->pos) ? MENU_BASE_DOWN : MENU_BASE_UP) + (fontOpt.height >> 1)) - 1;
   graphicDrawLine(x,y,x + (strlen(pointer->name) * fontOpt.width),y,MENU_SELECTION_COLOR);
  }

  // Find next position
  x += (strlen(pointer->name) * (fontOpt.width + fontOpt.space)) + MENU_ITEM_SPACE;

  // Go to the next item
  pointer = pointer->next;
 }

 return 0;
}

u32 menuDrawList (MenuList *list)

/*

  Draw a list

  Parameters :	list			->	Pointer of list to draw

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuOption *pointer;
 FontOption fontOpt;
 u32 y, y2, size = 0;
 u32 x;


 // Verify parameters
 if (!(list)) return 1;

 // Quit if not showed
 if (!(list->show)) return 0;

 // Draw option
 pointer = list->draw.cur;

 fontOpt.width = 8;
 fontOpt.height = 8;
 fontOpt.space = 0;

 y = MENU_BASE_OPTION;
 x = 0;

 while (pointer)
 {
  // Verify limit of list
  x++;
  if (x > MENU_LIST_LIMITDRAW)
  {
   list->draw.next = pointer;
   break;
  }

  // Find size for sub option (used many times)
  if (pointer->cur) size = strlen(pointer->cur->name) * fontOpt.width;

  // Draw selection rectangle
  if ((list->parent->focus == MENU_FOCUS_LIST) && (pointer == list->cur))
  {
   if (pointer->cur)
   {
	// Draw selection rectangle
	graphicDrawRectangle((MENU_BASE_RIGHT - 1) - size,y,size + 2,fontOpt.height,MENU_SELECTION_COLOR,1);
   }
   else
    graphicDrawRectangle((MENU_BASE_LEFT - 1),y,(strlen(pointer->name) * fontOpt.width) + 2,fontOpt.height,MENU_SELECTION_COLOR,1);
  }

  // Draw text
  fontPrint(pointer->name,&fontOpt,MENU_BASE_LEFT,y);

  // Draw sub option if exists
  if (pointer->cur)
  {
   fontPrint(pointer->cur->name,&fontOpt,MENU_BASE_RIGHT - size,y);

   // Draw the arrow if sub option previous or next exists
   sceGuLogicalOp(GU_INVERTED);
   sceGuEnable(GU_COLOR_LOGIC_OP);

   if (pointer->cur->prev) graphicDrawTriangle((MENU_BASE_RIGHT - 4) - size,y - 1,(MENU_BASE_RIGHT - 4) - size,y + 9,(MENU_BASE_RIGHT - 8) - size,y + 4,GRAPHIC_MAKE_COLOR(0x0,0x0,0x0,0xFF),1);
   if (pointer->cur->next) graphicDrawTriangle(MENU_BASE_RIGHT + 5,y - 1,MENU_BASE_RIGHT + 5,y + 9,MENU_BASE_RIGHT + 10,y + 4,GRAPHIC_MAKE_COLOR(0x0,0x0,0x0,0xFF),1);

   sceGuDisable(GU_COLOR_LOGIC_OP);
  }

  // If not used, print line in middle of text
  if (!(pointer->used))
  {
   y2 = (y + (fontOpt.height >> 1)) - 1;
   graphicDrawLine(MENU_BASE_LEFT,y2,((pointer->cur) ? MENU_BASE_RIGHT : (MENU_BASE_LEFT + (strlen(pointer->name) * fontOpt.width))),y2,MENU_SELECTION_COLOR);
  }

  // Find next position
  y += fontOpt.height + MENU_OPTION_SPACE;

  // Go to the next item
  pointer = pointer->next;
 }

 // Draw the arrow if not all option is drawed
 sceGuLogicalOp(GU_INVERTED);
 sceGuEnable(GU_COLOR_LOGIC_OP);

 if (list->draw.prev) graphicDrawTriangle(465,30,470,36,460,36,GRAPHIC_MAKE_COLOR(0x0,0x0,0x0,0xFF),1);
 if (list->draw.next) graphicDrawTriangle(465,256,470,250,460,250,GRAPHIC_MAKE_COLOR(0x0,0x0,0x0,0xFF),1);

 sceGuDisable(GU_COLOR_LOGIC_OP);

 return 0;
}

u32 menuDraw (Menu *menu)

/*

  Draw a menu

  Parameters :	menu			->	Pointer of menu to draw

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 FontOption fontOpt;


 // Verify parameters
 if (!(menu)) return 1;

 // Start render
 graphicStartRender();

 // Call the bgcolor function if exists
 menuBgColor = (menu->func) ? menu->func(menuBgColor) : menu->bgcolor;

 // Clear the screen with bgcolor
 graphicClearBuffer(menuBgColor);

 // Show image
 imageDraw(menu->bmp,0,0);

 // Draw title
 fontOpt.width = 16;
 fontOpt.height = 16;
 fontOpt.space = 0;

 if (menu->title)
 {
  fontPrint(menu->title,&fontOpt,MENU_TITLE_X,MENU_TITLE_Y);
  graphicDrawLine(MENU_TITLE_X,MENU_TITLE_Y + fontOpt.height,MENU_TITLE_X + ((strlen(menu->title) + fontOpt.space) * fontOpt.width),MENU_TITLE_Y + fontOpt.height,MENU_SELECTION_COLOR);
 }

 // Draw bar
 menuDrawBar(menu->bar);

 // Draw list
 menuDrawList(menu->list);

 // Stop render
 graphicStopRender();

 return 0;
}

u32 menuManageBar (MenuBar *bar, u32 button)

/*

  Manage a bar

  Parameters :	bar				->	Pointer of bar to manage
				button			->	Button pushed

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuItem *temp;


 // Verify parameters
 if (!(bar)) return 1;

 // If cur not initialized, exit
 if (!(bar->cur)) return 0;

 // If R button
 if (button & PSP_CTRL_RTRIGGER)
 {
  // Set the focus to the bar
  menuSetFocus(bar->parent,MENU_FOCUS_LIST);

  return 0;
 }

 // If up/down button
 if (((button & PSP_CTRL_UP) && (bar->pos)) || ((button & PSP_CTRL_DOWN) && (!(bar->pos))))
 {
  menuSetFocus(bar->parent,MENU_FOCUS_LIST);
  return 0;
 }

 // If previous button
 if (button & PSP_CTRL_LEFT)
 {
  // Search used previous item
  temp = bar->cur->prev;
  while (temp)
  {
   // Exit if good item
   if (temp->used == 1) break;

   // Go to the previous item
   temp = temp->prev;
  }

  if (temp) bar->cur = temp;

  return 0;
 }

 // If next button
 if (button & PSP_CTRL_RIGHT)
 {
  // Search used next item
  temp = bar->cur->next;
  while (temp)
  {
   // Exit if good item
   if (temp->used == 1) break;

   // Go to the next item
   temp = temp->next;
  }

  if (temp) bar->cur = temp;

  return 0;
 }

 // If validation button
 if ((button & bar->parent->button) && (bar->cur->func)) bar->cur->func(bar->cur);

 return 0;
}

u32 menuManageList (MenuList *list, u32 button)

/*

  Manage a list

  Parameters :	list			->	Pointer of list to manage
				button			->	Button pushed

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuOption *option;
 MenuSub *sub;
 int x;


 // Verify parameters
 if (!(list)) return 1;

 // If cur not initialized, exit
 if (!(list->cur)) return 0;

 // If R button
 if (button & PSP_CTRL_RTRIGGER)
 {
  // Set the focus to the bar
  menuSetFocus(list->parent,MENU_FOCUS_BAR);

  return 0;
 }

 // If up button
 if (button & PSP_CTRL_UP)
 {
  // Search used previous option
  option = list->cur->prev;
  while (option)
  {
   // Exit if good option
   if (option->used == 1) break;

   // Go to the previous option
   option = option->prev;
  }

  if (option)
  {
   list->cur = option;

   // For draw limit
   if (list->cur == list->draw.cur->prev)
   {
    list->draw.next = list->draw.cur;		// PAS OBLIGATOIRE, DRAW S'EN OCCUPE
	list->draw.cur = list->draw.prev;
	if (list->draw.prev->prev)
	{
	 for (x=0;x<MENU_LIST_LIMITDRAW;x++) list->draw.prev = list->draw.prev->prev;
	}
	else
	 list->draw.prev = NULL;
   }
  }
  else
   if (!(list->parent->bar->pos)) menuSetFocus(list->parent,MENU_FOCUS_BAR);

  return 0;
 }

 // If down button
 if (button & PSP_CTRL_DOWN)
 {
  // Search used next option
  option = list->cur->next;
  while (option)
  {
   // Exit if good option
   if (option->used == 1) break;

   // Go to the next option
   option = option->next;
  }

  if (option)
  {
   list->cur = option;

   // For draw limit
   if (list->cur == list->draw.next)
   {
    list->draw.prev = list->draw.cur;
	list->draw.cur = list->draw.next;
	list->draw.next = NULL;
   }
  }
  else
   if (list->parent->bar->pos) menuSetFocus(list->parent,MENU_FOCUS_BAR);

  return 0;
 }

 // If left button and current sub option exists
 if ((button & PSP_CTRL_LEFT) && (list->cur->cur))
 {
  // Search used previous sub option
  sub = list->cur->cur->prev;
  while (sub)
  {
   // Exit if good sub option
   if (sub->used == 1) break;

   // Go to the previous sub option
   sub = sub->prev;
  }


  if (sub) list->cur->cur = sub;

  return 0;
 }

 // If right button and current sub option exists
 if ((button & PSP_CTRL_RIGHT) && (list->cur->cur))
 {
  // Search used next sub option
  sub = list->cur->cur->next;
  while (sub)
  {
   // Exit if good sub option
   if (sub->used == 1) break;

   // Go to the next sub option
   sub = sub->next;
  }

  if (sub) list->cur->cur = sub;

  return 0;
 }

 // Call the option function if other key
 //if ((button & list->parent->button) && (list->cur->func)) list->cur->func(list->cur);
 if (button & list->parent->button)
 {
  // Call function of sub option if exists
  if (list->cur->cur)
  {
   if (list->cur->cur->func) list->cur->cur->func(list->cur->cur);
  }
  else
  // Call function of option
   if (list->cur->func) list->cur->func(list->cur);
 }

 return 0;
}

u32 menuInit (const char *fontfile, FontType *font)

/*

  Initialize the menu functions

  Parameters :	fontfile		->	Name of font image file to load
				font			->	Parameters of font to load

  Return :		0				->	OK
				1				->	Already initialized
				2				->	Bad parameters
				3				->	Unable to load image file
				4				->	Unable to initialize font

*/

{
 // If already initialized
 if (menuInitialized) return 1;

 // Verify parameters
 if ((!(fontfile)) || (!(font))) return 2;

 // Copy font parameters
 memcpy(&menuFont,font,sizeof(FontType));

 // Load font
 if (imageLoad(&menuFont.font,fontfile,-1)) return 3;

 // Set font
 if (fontInit(&menuFont)) return 4;

 // Initialized
 menuInitialized = 1;

 return 0;
}

void menuTerm (void)

/*

  Terminate the menu functions

  Parameters :	None

  Return :		None

*/

{
 // If not initialized
 if (!(menuInitialized)) return;

 // Release image font
 imageDelete(menuFont.font);

 // Release font
 fontTerm();

 // Not initialized
 menuInitialized = 0;
}

u32 menuCreate (Menu **menu, const char *title, GRAPHIC_COLOR_TYPE bgcolor, const char *bmpfile, int blend, u32 button, u8 buttonBlock, MENU_COLOR_FUNC func, u8 show)

/*

  Create a new menu

  Parameters :	menu			->	Address of a menu pointer which contain new menu
				title			->	Title of menu
				bgcolor			->	Background color of menu
				bmpfile			->	Name of image file for background
				blend			->	Blend intensity
				button			->	Mask of action button
				buttonBlock		->	If != 0, must release button before continue
				func			->	if != NULL, func is called in the display loop
				show			->	if != 0 menu is showed

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for menu
				3				->	Unable to allocate memory for title

*/

{
 // Verify parameters
 if (!(menu)) return 1;

 // Allocate new menu
 *menu = (Menu *) malloc(sizeof(Menu));
 if (!(*menu)) return 2;

 // Initialize menu
 memset((*menu),0,sizeof(Menu));

 menuSetTitle(*menu,title);

 (*menu)->bgcolor = bgcolor;
 imageLoad(&(*menu)->bmp,bmpfile,blend);

 (*menu)->button = button;
 (*menu)->buttonBlock = buttonBlock;
 (*menu)->func = func;

 (*menu)->show = show;

 return 0;
}

u32 menuDelete (Menu *menu)

/*

  Delete a menu

  Parameters :	menu			->	Pointer of menu to delete

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 // Verify parameters
 if (!(menu)) return 1;

 // Release title
 menuSetTitle(menu,NULL);

 // Release bar
 menuBarDelete(menu->bar);

 // Release list
 menuListDelete(menu->list);

 // Release image
 imageDelete(menu->bmp);

 // Free menu
 free(menu);

 return 0;
}

u32 menuBarCreate (MenuBar **bar, u8 position, u8 show)

/*

  Create a new bar

  Parameters :	bar				->	Address of a bar pointer which contain new bar
				position		->	if != 0, bar is drawed in bottom of screen
				show			->	if != 0 bar is showed

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for bar

*/

{
 // Verify parameters
 if (!(bar)) return 1;

 // Allocate new bar
 *bar = (MenuBar *) malloc(sizeof(MenuBar));
 if (!(*bar)) return 2;

 // Initialize bar
 memset((*bar),0,sizeof(MenuBar));
 
 (*bar)->pos = position;
 (*bar)->show = show;

 return 0;
}

u32 menuBarDelete (MenuBar *bar)

/*

  Delete a bar

  Parameters :	bar				->	Pointer of bar to delete

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 // Verify parameters
 if (!(bar)) return 1;

 // If parent, correct pointer
 if (bar->parent) bar->parent->bar = NULL;

 // Release items
 while (bar->items) menuBarDelItem(bar,bar->items);

 // Free bar
 free(bar);

 return 0;
}

u32 menuBarAttach (Menu *menu, MenuBar *bar)

/*

  Attach a bar to a menu

  Parameters :	menu			->	Pointer of menu which contain bar
				bar				->	Pointer of bar to attach

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 // Verify parameters
 if ((!(menu)) || (!(bar))) return 1;

 // Attach bar to menu
 menu->bar = bar;
 bar->parent = menu;

 return 0;
}

u32 menuListCreate (MenuList **list, u8 show)

/*

  Create a new list

  Parameters :	list			->	Address of a list pointer which contain new list
				show			->	if != 0 list is showed

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for list

*/

{
 // Verify parameters
 if (!(list)) return 1;

 // Allocate new list
 *list = (MenuList *) malloc(sizeof(MenuList));
 if (!(*list)) return 2;

 // Initialize list
 memset((*list),0,sizeof(MenuList));
 
 (*list)->show = show;

 return 0;
}

u32 menuListDelete (MenuList *list)

/*

  Delete a list

  Parameters :	list			->	Pointer of list to delete

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 // Verify parameters
 if (!(list)) return 1;

 // If parent, correct pointer
 if (list->parent) list->parent->list = NULL;

 // Release options
 while (list->options) menuListDelOption(list,list->options);

 // Free list
 free(list);

 return 0;
}

u32 menuListAttach (Menu *menu, MenuList *list)

/*

  Attach a list to a menu

  Parameters :	menu			->	Pointer of menu which contain list
				list			->	Pointer of list to attach

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 // Verify parameters
 if ((!(menu)) || (!(list))) return 1;

 // Attach list to menu
 menu->list = list;
 list->parent = menu;

 return 0;
}

u32 menuBarAddItem (MenuBar *bar, MenuItem **ret, const char *name, u8 used, MENU_ITEM_FUNC func)

/*

  Add an item to a bar

  Parameters :	bar				->	Pointer of bar to add item
				ret				->	if != NULL, he will contains address of item created
				name			->	Name of the item
				used			->	if != 1, he is used
				func			->	if != NULL, func is called in the dispatch loop if action button is pushed

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for item
				3				->	Unable to allocate memory for name

*/

{
 MenuItem *item, *temp;


 // Verify parameters
 if ((!(bar)) || (!(name))) return 1;

 // Allocate new item
 item = (MenuItem *) malloc(sizeof(MenuItem));
 if (!(item)) return 2;

 // Initialize item
 memset(item,0,sizeof(MenuItem));
 
 item->name = (char *) malloc(strlen(name) + 1);
 if (!(item->name))
 {
  free(item);
  return 3;
 }

 strcpy(item->name,name);
 item->used = used;
 item->func = func;

 // Link item to bar
 item->parent = bar;

 if (!(bar->items))
  bar->items = item;
 else
 {
  temp = bar->items;
  while (temp->next) temp = temp->next;

  temp->next = item;
  item->prev = temp;
 }

 // Save pointer in ret if != NULL
 if (ret) *ret = item;

 return 0;
}

u32 menuBarDelItem (MenuBar *bar, MenuItem *item)

/*

  Delete an item

  Parameters :	bar				->	Pointer of bar which contains item to delete
				item			->	Pointer of item to delete

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Item not found in bar

*/

{
 MenuItem *temp;


 // Verify parameters
 if ((!(bar)) || (!(item))) return 1;

 // Search if item is in bar
 temp = bar->items;
 while (temp != item) temp = temp->next;

 // If not found
 if (!(temp)) return 2;

 // Unlink item to bar
 if (temp == bar->items)
 {
  bar->items = bar->items->next;
  if (bar->items) bar->items->prev = NULL;
 }
 else
 {
  if (temp->prev) temp->prev->next = temp->next;
  if (temp->next) temp->next->prev = temp->prev;
 }

 // Delete name
 if (temp->name) free(temp->name);

 // Delete item
 free(temp);

 return 0;
}

u32 menuListAddOption (MenuList *list, MenuOption **ret, const char *name, u8 used, MENU_OPTION_FUNC func)

/*

  Add an option to a list

  Parameters :	list			->	Pointer of list to add option
				ret				->	if != NULL, he will contains address of option created
				name			->	Name of the option
				used			->	if != 1, he is used
				func			->	if != NULL, func is called in the dispatch loop if action button is pushed

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for option
				3				->	Unable to allocate memory for name

*/

{
 MenuOption *option, *temp;


 // Verify parameters
 if ((!(list)) || (!(name))) return 1;

 // Allocate new option
 option = (MenuOption *) malloc(sizeof(MenuOption));
 if (!(option)) return 2;

 // Initialize option
 memset(option,0,sizeof(MenuOption));
 
 option->name = (char *) malloc(strlen(name) + 1);
 if (!(option->name))
 {
  free(option);
  return 3;
 }

 strcpy(option->name,name);
 option->used = used;
 option->func = func;

 // Link option to list
 option->parent = list;

 if (!(list->options))
  list->options = option;
 else
 {
  temp = list->options;
  while (temp->next) temp = temp->next;

  temp->next = option;
  option->prev = temp;
 }

 // Save pointer in ret if != NULL
 if (ret) *ret = option;

 return 0;
}

u32 menuListDelOption (MenuList *list, MenuOption *option)

/*

  Delete an option

  Parameters :	list			->	Pointer of list which contains option to delete
				option			->	Pointer of option to delete

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Option not found in list

*/

{
 MenuOption *temp;


 // Verify parameters
 if ((!(list)) || (!(option))) return 1;

 // Search if option is in list
 temp = list->options;
 while (temp != option) temp = temp->next;

 // If not found
 if (!(temp)) return 2;

 // Unlink option to list
 if (temp == list->options)
 {
  list->options = list->options->next;
  if (list->options) list->options->prev = NULL;
 }
 else
 {
  if (temp->prev) temp->prev->next = temp->next;
  if (temp->next) temp->next->prev = temp->prev;
 }

 // Delete all sub options
 while (temp->subs) menuOptionDelSub(temp,temp->subs);

 // Delete name
 if (temp->name) free(temp->name);

 // Delete option
 free(temp);

 return 0;
}

u32 menuOptionAddSub (MenuOption *option, MenuSub **ret, const char *name, u8 used, MENU_SUB_FUNC func)

/*

  Add a sub option to an option

  Parameters :	option			->	Pointer of option to add sub option
				ret				->	if != NULL, he will contains address of sub option created
				name			->	Name of the sub option
				used			->	if != 1, he is used
				func			->	if != NULL, func is called in the dispatch loop if action button is pushed

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for sub option
				3				->	Unable to allocate memory for name

*/

{
 MenuSub *sub, *temp;


 // Verify parameters
 if ((!(option)) || (!(name))) return 1;

 // Allocate new sub
 sub = (MenuSub *) malloc(sizeof(MenuSub));
 if (!(sub)) return 2;

 // Initialize sub
 memset(sub,0,sizeof(MenuSub));
 
 sub->name = (char *) malloc(strlen(name) + 1);
 if (!(sub->name))
 {
  free(sub);
  return 3;
 }

 strcpy(sub->name,name);
 sub->used = used;
 sub->func = func;

 // Link sub to option
 sub->parent = option;

 if (!(option->subs))
  option->subs = sub;
 else
 {
  temp = option->subs;
  while (temp->next) temp = temp->next;

  temp->next = sub;
  sub->prev = temp;
 }

 // Save pointer in ret if != NULL
 if (ret) *ret = sub;

 return 0;
}

u32 menuOptionDelSub (MenuOption *option, MenuSub *sub)

/*

  Delete a sub option

  Parameters :	option			->	Pointer of option which contains sub option to delete
				sub				->	Pointer of sub option to delete

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Sub option not found in option

*/

{
 MenuSub *temp;


 // Verify parameters
 if ((!(option)) || (!(sub))) return 1;

 // Search if sub is in option
 temp = option->subs;
 while (temp != sub) temp = temp->next;

 // If not found
 if (!(temp)) return 2;

 // Unlink sub to option
 if (temp == option->subs)
 {
  option->subs = option->subs->next;
  if (option->subs) option->subs->prev = NULL;
 }
 else
 {
  if (temp->prev) temp->prev->next = temp->next;
  if (temp->next) temp->next->prev = temp->prev;
 }

 // Delete name
 if (temp->name) free(temp->name);

 // Delete sub
 free(temp);

 return 0;
}

u32 menuBarInitPointer (MenuBar *bar)

/*

  Init all pointer of a bar

  Parameters :	bar				->	Pointer of bar to initialize

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuItem *item;


 // Verify parameters
 if (!(bar)) return 1;

 // Find good item
 item = bar->items;

 while (item)
 {
  // Exit if good item
  if (item->used == 1) break;

  // Go to the next item
  item = item->next;
 }

 // Set cur pointer
 bar->cur = item;

 return 0;
}

u32 menuListInitPointer (MenuList *list)

/*

  Init all pointer of a list

  Parameters :	list			->	Pointer of list to initialize

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuOption *option, *temp;


 // Verify parameters
 if (!(list)) return 1;

 // Find good option
 temp = NULL;
 option = list->options;

 while (option)
 {
  // Exit if good option
  if ((option->used == 1) && (!(temp))) temp = option;

  // Init cur pointer of option
  menuOptionInitPointer(option);

  // Go to the next option
  option = option->next;
 }

 // Set cur pointer
 list->cur = temp;

 // Set draw pointer
 list->draw.prev = NULL;
 list->draw.cur = list->options;

 return 0;
}

u32 menuOptionInitPointer (MenuOption *option)

/*

  Init all pointer of an option

  Parameters :	option			->	Pointer of option to initialize

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 MenuSub *sub;


 // Verify parameters
 if (!(option)) return 1;

 // Find good sub option
 sub = option->subs;

 while (sub)
 {
  // Exit if good sub option
  if (sub->used == 1) break;

  // Go to the next sub option
  sub = sub->next;
 }

 // Set cur pointer
 option->cur = sub;

 return 0;
}

u32 menuInitPointer (Menu *menu)

/*

  Init all pointer of a menu

  Parameters :	menu			->	Pointer of menu to initialize

  Return :		0				->	OK
				1				->	Bad parameters

*/

{
 // Verify parameters
 if (!(menu)) return 1;

 // Initialize bar
 menuBarInitPointer(menu->bar);

 // Initialize list and options
 menuListInitPointer(menu->list);

 return 0;
}

u32 menuSetTitle (Menu *menu, const char *title)

/*

  Set a title in a menu

  Parameters :	menu			->	Pointer of menu to set title
				title			->	Title to put in menu (can be NULL)

  Return :		0				->	OK
				1				->	Bad parameters
				2				->	Unable to allocate memory for title

*/

{
 // Verify parameters
 if (!(menu)) return 1;

 // Delete old title if exists
 if (menu->title)
 {
  free(menu->title);
  menu->title = NULL;
 }

 // Create new title if != NULL
 if (title)
 {
  // Allocate memory for string
  menu->title = (char *) malloc(strlen(title) + 1);
  if (!(menu->title)) return 2;

  // Copy string
  strcpy(menu->title,title);
 }

 return 0;
}

u32 menuSetFocus (Menu *menu, u32 focus)
{
 MenuItem *item;
 MenuOption *option;


 // Verify parameters
 if (!(menu)) return 1;

 // Verify with focus of ok
 switch (focus)
 {
  case MENU_FOCUS_BAR :

   // Verify if bar exists
   if (!(menu->bar)) return 1;

   // Verify if at least one item valid
   item = menu->bar->items;
   while (item)
   {
    if (item->used == 1) break;
	item = item->next;
   }

   // If not item valid found, exit
   if (!(item)) return 1;

  break;

  case MENU_FOCUS_LIST :

   // Verify if list exists
   if (!(menu->list)) return 1;

   // Verify if at least one option valid
   option = menu->list->options;
   while (option)
   {
    if (option->used == 1) break;
	option = option->next;
   }

   // If not option valid found, exit
   if (!(option)) return 1;

  break;
 }

 // Set focus
 menu->focus = focus;

 return 0;
}

u32 menuManage (Menu *menu)

/*

  Manage a menu

  Parameters :	menu			->	Pointer of menu to manage
				button			->	Button pushed

  Return :		0				->	OK
				1				->	Not initialized
				2				->	Bad parameters

*/

{
 SceCtrlData pad;
 u32 button = 0;
 

 // If not initialized
 if (!(menuInitialized)) return 1;

 // Verify parameters
 if (!(menu)) return 2;

 // Init variable
 menu->run = 1;

 // Set focus
 menuSetFocus(menu,MENU_FOCUS_LIST);
 menuSetFocus(menu,MENU_FOCUS_BAR);

 // Menu loop
 while (menu->run)
 {
  // Draw menu if showed
  if (menu->show) menuDraw(menu);

  // Read key
  sceCtrlReadBufferPositive(&pad,1);

  // Compare
  if ((button != pad.Buttons) || (!(menu->buttonBlock)))
  {
   // Save the button
   button = pad.Buttons;

   // Pass control to bar or option depending focus
   if (menu->focus == MENU_FOCUS_LIST)
    menuManageList(menu->list,button);
   else
    if (menu->focus == MENU_FOCUS_BAR) menuManageBar(menu->bar,button);
  }
 }

 return 0;
}

/*
int menuBarGetCurrent (MenuBar *bar)
{
 MenuItem *temp;
 int current;


 // Verify parameters
 if (!(bar)) return -1;

 // Initialize variables
 temp = bar->items;
 current = 0;

 // Find the current item
 while (temp)
 {
  // If current item found
  if (temp == bar->cur) return current;

  // Update variables
  temp = temp->next;
  current++;
 }

 // Not found
 return -1;
}

u32 menuBarSetCurrent (MenuBar *bar, int current)
{
 MenuItem *temp;


 // Verify parameters
 if (!(bar)) return 1;

 // Initialize variables
 temp = bar->items;

 // Find the current item
 while (temp)
 {
  // If current item found
  if (!(current))
  {
   bar->cur = temp;
   return 0;
  }

  // Update variables
  temp = temp->next;
  current--;
 }

 // Not found
 return 2;
}

int menuListGetCurrent (MenuList *list)
{
 MenuOption *temp;
 int current;


 // Verify parameters
 if (!(list)) return -1;

 // Initialize variables
 temp = list->options;
 current = 0;

 // Find the current option
 while (temp)
 {
  // If current option found
  if (temp == list->cur) return current;

  // Update variables
  temp = temp->next;
  current++;
 }

 // Not found
 return -1;
}

u32 menuListSetCurrent (MenuList *list, int current)
{
 MenuOption *temp;


 // Verify parameters
 if (!(list)) return 1;

 // Initialize variables
 temp = list->options;

 // Find the current option
 while (temp)
 {
  // If current option found
  if (!(current))
  {
   list->cur = temp;
   return 0;
  }

  // Update variables
  temp = temp->next;
  current--;
 }

 // Not found
 return 2;
}

int menuOptionGetCurrent (MenuOption *option)
{
 MenuSub *temp;
 int current;


 // Verify parameters
 if (!(option)) return -1;

 // Initialize variables
 temp = option->subs;
 current = 0;

 // Find the current sub
 while (temp)
 {
  // If current sub found
  if (temp == option->cur) return current;

  // Update variables
  temp = temp->next;
  current++;
 }

 // Not found
 return -1;
}

u32 menuOptionSetCurrent (MenuOption *option, int current)
{
 MenuSub *temp;


 // Verify parameters
 if (!(option)) return 1;

 // Initialize variables
 temp = option->subs;

 // Find the current sub
 while (temp)
 {
  // If current sub found
  if (!(current))
  {
   option->cur = temp;
   return 0;
  }

  // Update variables
  temp = temp->next;
  current--;
 }

 // Not found
 return 2;
}
*/

char *menuConvertWideToAscii (u16 *widestr)

/*

  Transform a wide string to an ascii string

  Parameters :	widestr			->	Wide string to convert

  Return :		Converted string

*/

{
 static char string[MENU_OSK_SIZE_STRING];
 int x;


 // Verify parameters
 if (!(widestr)) return NULL;

 // Convert
 x = 0;
 while (*widestr)
 {
  string[x] = (*widestr) & 0xFF;

  widestr++;
  x++;
 }

 // Terminate caracter
 string[x] = 0;

 return string;
}

u16 *menuConvertAsciiToWide (char *string)

/*

  Transform an ascii string to a wide string

  Parameters :	string			->	Ascii string to convert

  Return :		Converted string

*/

{
 static u16 widestr[MENU_OSK_SIZE_STRING];
 int x;


 // Verify parameters
 if (!(string)) return NULL;

 // Convert
 x = 0;
 while (*string)
 {
  widestr[x] = *string;

  string++;
  x++;
 }

 // Terminate caracter
 widestr[x] = 0;

 return widestr;
}

char *menuGetOskText (Menu *menu, char *title, char *defstr, u32 language, u32 limit)

/*

  Get a string with On Screen Keyboard

  Parameters :	menu			->	Pointer to the menu currently drawed
				title			->	Title of osk
				defstr			->	Default string put in osk
				language		->	Language id of osk
				limit			->	Maximum number of caracters allowed

  Return :		Osk string or NULL if error

*/

{
 FontOption fontOpt;
 OskData data;
 OskBase osk;
 u16 titleW[MENU_OSK_SIZE_STRING];
 u16 defstrW[MENU_OSK_SIZE_STRING];
 u16 stringW[MENU_OSK_SIZE_STRING];
 u8 run;


 // Verify parameters
 if (!(menu)) return NULL;

 // Initialize strings
 if (title)
  memcpy(titleW,menuConvertAsciiToWide(title),sizeof(u16) * MENU_OSK_SIZE_STRING);
 else
  titleW[0] = 0;

 if (defstr)
  memcpy(defstrW,menuConvertAsciiToWide(defstr),sizeof(u16) * MENU_OSK_SIZE_STRING);
 else
  defstrW[0] = 0;

 stringW[0] = 0;

 // Initialize structure
 memset(&data,0,sizeof(OskData));

 data.language = 2;
 data.nLine = 1;
 data.unk3 = 1;
 data.title = titleW;
 data.defstr = defstrW;
 data.string = stringW;
 data.strmaxlen = MENU_OSK_SIZE_STRING;
 data.limit = limit;
 //data.charset = 0;

 memset(&osk,0,sizeof(OskBase));

 osk.size = sizeof(OskBase);
 osk.language = language;
 osk.ctrl = (menu->button & PSP_CTRL_CROSS) ? 1 : 0;
 osk.unk1[0] = 17;
 osk.unk1[1] = 19;
 osk.unk1[2] = 18;
 osk.unk1[3] = 16;
 osk.nData = 1;
 osk.data = &data;

 // Initialize osk
 if (sceUtilityOskInitStart(&osk)) return NULL;

 // Osk loop
 run = 1;
 while (run)
 {
  // Start render
  graphicStartRender();

  // Call the bgcolor function if exists
  menuBgColor = (menu->func) ? menu->func(menuBgColor) : menu->bgcolor;

  // Clear the screen with bgcolor
  graphicClearBuffer(menuBgColor);

  if (menu->show)
  {
   // Show image
   imageDraw(menu->bmp,0,0);

   // Draw title
   fontOpt.width = 16;
   fontOpt.height = 16;
   fontOpt.space = 0;

   if (menu->title)
   {
    fontPrint(menu->title,&fontOpt,MENU_TITLE_X,MENU_TITLE_Y);
    graphicDrawLine(MENU_TITLE_X,MENU_TITLE_Y + fontOpt.height,MENU_TITLE_X + ((strlen(menu->title) + fontOpt.space) * fontOpt.width),MENU_TITLE_Y + fontOpt.height,GRAPHIC_MAKE_COLOR(0xFF,0xFF,0xFF,0xFF));
   }

   // Draw bar
   menuDrawBar(menu->bar);

   // Draw list
   menuDrawList(menu->list);
  }

  // Stop render
  sceGuFinish();

  // Wait until display list has finished executing
  sceGuSync(0,0);

  // Show OSK
  switch(sceUtilityOskGetStatus())
  {
   case MENU_OSK_STATUS_INIT :
   break;

   case MENU_OSK_STATUS_VISIBLE :
	sceUtilityOskUpdate(MENU_OSK_BLIND_SPEED);
   break;

   case MENU_OSK_STATUS_QUIT :
    sceUtilityOskShutdownStart();
   break;

   case MENU_OSK_STATUS_FINISHED :
	run = 0;
   break;

   case MENU_OSK_STATUS_NONE :
   default :
   break;
  }

  // Wait vblank start
  sceDisplayWaitVblankStart();

  // Swap the buffers and save draw buffer
  sceGuSwapBuffers();
 }

 // Return ascii string
 return menuConvertWideToAscii(stringW);
}
