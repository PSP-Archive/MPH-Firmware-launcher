
#ifndef MENU_INCLUDED
#define MENU_INCLUDED

/*

  Menu functions header  

  By MPH (mphtheone@hotmail.com)

*/


// *** INCLUDES ***

#include <pspctrl.h>
#include "../2D/font.h"


// *** DEFINES ***

#define MENU_TITLE_X			10
#define MENU_TITLE_Y			30

#define MENU_FOCUS_LIST			1
#define MENU_FOCUS_BAR			2

#define MENU_ITEM_SPACE			10
#define MENU_OPTION_SPACE		10

#define MENU_BASE_LEFT			10
#define MENU_BASE_RIGHT			450
#define MENU_BASE_UP			3
#define MENU_BASE_DOWN			259
#define MENU_BASE_ITEM			4
#define MENU_BASE_OPTION		80

#define MENU_BAR1_X				1
#define MENU_BAR1_Y				1
#define MENU_BAR1_W				478
#define MENU_BAR1_H				14
#define MENU_BAR1_COLOR			GRAPHIC_MAKE_COLOR(0x0,0x0,0x0,0xFF)

#define MENU_BAR2_X				2
#define MENU_BAR2_Y				2
#define MENU_BAR2_W				476
#define MENU_BAR2_H				12
#define MENU_BAR2_COLOR			GRAPHIC_MAKE_COLOR(0x23,0x5C,0xDB,0xFF)
//#define MENU_BAR2_COLOR			GRAPHIC_MAKE_COLOR(0x80,0xC0,0xFF,0xFF)

#define MENU_SELECTION_X		

#define MENU_SELECTION_COLOR	GRAPHIC_MAKE_COLOR(0xFF,0x9E,0x05,0xFF)
//#define MENU_SELECTION_COLOR	GRAPHIC_MAKE_COLOR(0x70,0x70,0x70,0xFF)

#define MENU_LIST_LIMITDRAW		10

// FOR OSK

#define	MENU_OSK_BLIND_SPEED			1
#define	MENU_OSK_SIZE_STRING			128

#define	MENU_OSK_STATUS_NONE			0
#define	MENU_OSK_STATUS_INIT			1
#define	MENU_OSK_STATUS_VISIBLE			2
#define	MENU_OSK_STATUS_QUIT			3
#define	MENU_OSK_STATUS_FINISHED		4


// *** STRUCTURES ***

typedef struct MenuItem *MenuItemP;
typedef struct MenuOption *MenuOptionP;
typedef struct MenuSub *MenuSubP;

typedef u32 (*MENU_COLOR_FUNC) (u32);
typedef u32 (*MENU_ITEM_FUNC) (MenuItemP);
typedef u32 (*MENU_OPTION_FUNC) (MenuOptionP);
typedef u32 (*MENU_SUB_FUNC) (MenuSubP);

typedef struct MenuItem			// Menu item
{
 char *name;					// Name of item
 u8 used;						// Use flag (0 = not used with line, 1 = used, 2 = not used without line (for text))
 MENU_ITEM_FUNC func;			// Function called when item selected
 void *user;					// User variable
 struct MenuItem *prev;			// Previous item pointer
 struct MenuItem *next;			// Next item pointer
 struct MenuBar *parent;		// Parent of item
} MenuItem;

typedef struct MenuBar			// Menu bar
{
 u8 pos;						// Position flag (0 = up, 1, down)
 u8 show;						// Show flag
 MenuItem *items;				// Pointer to item chained list
 MenuItem *cur;					// Pointer to current item
 struct Menu *parent;			// Parent of bar
} MenuBar;

typedef struct MenuSub			// Menu sub option
{
 char *name;					// Name of sub option
 u8 used;						// Use flag
 MENU_SUB_FUNC func;			// Function called when sub option selected
 void *user;					// User variable
 struct MenuSub *prev;			// Previous sub option pointer
 struct MenuSub *next;			// Next sub option pointer
 struct MenuOption *parent;		// Parent of sub option
} MenuSub;

typedef struct MenuOption		// Menu option
{
 char *name;					// Name of option
 u8 used;						// Use flag
 MENU_OPTION_FUNC func;			// Function called when option selected
 void *user;					// User variable
 MenuSub *subs;					// Pointer to list of sub options
 MenuSub *cur;					// Pointer to current sub option
 struct MenuOption *prev;		// Previous option pointer
 struct MenuOption *next;		// Next option pointer
 struct MenuList *parent;		// Parent of option
} MenuOption;

typedef struct MenuListDraw		// Menu list draw
{
 MenuOption *prev;				// Previous option to show
 MenuOption *cur;				// Current option to show
 MenuOption *next;				// Next option to show
} MenuListDraw;

typedef struct MenuList			// Menu list
{
 u8 show;						// Show flag
 MenuOption *options;			// Pointer to options
 MenuOption *cur;				// Pointer to current option
 MenuListDraw draw;				// Contains the draw information
 struct Menu *parent;			// Parent of list
} MenuList;

typedef struct Menu				// Menu
{
 GRAPHIC_COLOR_TYPE bgcolor;	// Background color
 MENU_COLOR_FUNC func;			// Color function called if != NULL
 Image *bmp;					// Menu wallpaper
 char *title;					// Menu title
 MenuBar *bar;					// Pointer to a menu bar
 MenuList *list;				// Pointer to a menu list
 u32 button;					// Button mask
 u8 buttonBlock;				// If = 1, wait release button before read
 u32 focus;						// Focus variable
 u8 show;						// Show menu if show = 1
 u8 run;						// Exit menu dispatch if run = 0
} Menu;

// FOR OSK

typedef struct OskData						// Osk data structure
{
 u32 unk1[2];								// Unknow 1 ?
 u32 language;								// Language of messages
 u32 unk2;									// Unknow 2 ?
 u32 charset;								// Characters set
 u32 nLine;									// Number of lines
 u32 unk3;									// Unknow 3 ?
 u16 *title;								// Title
 u16 *defstr;								// Default string input
 u32 strmaxlen;								// Max size for output string
 u16 *string;								// Output string
 u32 result;								// Result value
 u32 limit;									// Max characters limit
} OskData;

typedef struct OskBase						// Osk base structure
{
 u32 size;									// Size of the structure
 u32 language;								// Language of message
 u32 ctrl;									// Control type
 u32 unk1[4];								// Unknow 1 ?
 u32 result;								// Result value
 u32 unk2[4];								// Unknow 2 ?
 u32 nData;									// Number of data
 OskData *data;								// Array of data
 u32 status;								// Status of osk
 u32 unk3;									// Unknow 3 ?
} OskBase;


// *** FUNCTIONS DECLARATIONS ***

u32 menuInit (const char *, FontType *);												// Initialize functions menu
void menuTerm (void);																	// Terminate functions menu

u32 menuCreate (Menu **, const char *, GRAPHIC_COLOR_TYPE, const char *, int, u32, u8, MENU_COLOR_FUNC, u8);	// Create a menu
u32 menuDelete (Menu *);																// Delete a menu

u32 menuBarCreate (MenuBar **, u8, u8);													// Create a menu bar
u32 menuBarDelete (MenuBar *);															// Delete a menu bar
u32 menuBarAttach (Menu *, MenuBar *);													// Attach a menu bar to a menu

u32 menuListCreate (MenuList **, u8);													// Create a menu list
u32 menuListDelete (MenuList *);														// Delete a menu list
u32 menuListAttach (Menu *, MenuList *);												// Attach a menu list to a menu

u32 menuBarAddItem (MenuBar *, MenuItem **, const char *, u8, MENU_ITEM_FUNC);			// Add an item to a menu bar
u32 menuBarDelItem (MenuBar *, MenuItem *);												// Delete an item to a menu bar

u32 menuListAddOption (MenuList *, MenuOption **, const char *, u8, MENU_OPTION_FUNC);	// Add an option to a menu list
u32 menuListDelOption (MenuList *, MenuOption *);										// Delete an option to a menu list

u32 menuOptionAddSub (MenuOption *, MenuSub **, const char *, u8, MENU_SUB_FUNC);		// Add a sub option to an option
u32 menuOptionDelSub (MenuOption *, MenuSub *);											// Delete a sub option to an option

u32 menuBarInitPointer (MenuBar *);														// Init all pointer in bar (points to first valid variable)
u32 menuListInitPointer (MenuList *);													// Init all pointer in list (points to first valid variable)
u32 menuOptionInitPointer (MenuOption *);												// Init all pointer in option (points to first valid variable)
u32 menuInitPointer (Menu *);															// Init all pointer in menu (points to first valid variable)

u32 menuSetTitle (Menu *, const char *);												// Set title of a menu

u32 menuSetFocus (Menu *, u32);															// Set focus to a menu
u32 menuManage (Menu *);																// Show and manage a menu

u32 menuDraw (Menu *);																	// Draw a menu

// FOR OSK

char *menuGetOskText (Menu *, char *, char *, u32, u32);								// Get a string with On Screen Keyboard

extern int sceUtilityOskInitStart (OskBase *);											// Initialize osk functions
extern int sceUtilityOskShutdownStart (void);											// Shutdown the osk
extern int sceUtilityOskUpdate (u32);													// Update the osk display
extern int sceUtilityOskGetStatus (void);												// Get the status of osk

#endif
