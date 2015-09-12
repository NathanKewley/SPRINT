/*------------------------------------------------------------------------------
FILE                  ControlState.h
PROGRAM               Control class for Sprint Editor
PROGRAMMER            Nathan Kewley       
ENVIROMENT            Linux Mint 12 - g++ with SFML
------------------------------------------------------------------------------*/

using namespace std;
class ControlState{
	private:
	bool NewMap;
	bool Main;
	bool TileSelection;
	bool SpecialSelection;
	bool Menu;
	bool Save;
	bool Load;
	bool Portal;
	bool Tip;
	bool MapName;
	bool MapAuthor;
	public:
	//new map vars
	int NewMapWidth;
	int NewMapHeight;
	int NewMapTile;
	
	//wether to upload map or not
	bool UploadMap;
	
	//variable to hold just placed portal
	int CurrentPortal;
	
	//map offset
	float MapOffX;
	float MapOffY;
	
	float LastZoom;
	
	//zoom factor
	int ZoomFactor;
	
	//holds the last tile clicked
	int LastTileClicked;
	int LastBlocked;
	int LastUnblocked;

	//currently selectes (negative values mean they are not selected at all)
	int CurrentTile;
	int CurrentSpecial;
	int SaveNumber;
	int LoadNumber;
	int TipNumber;
	
	//map loading stuffs
	int CustomMapSelection;	
	
	string GetState();
	void SetState(string InState);
	ControlState();
};

ControlState::ControlState(){
	NewMap = false;
	Main = true;
	TileSelection = false;
	SpecialSelection = false;
	Menu = false;
	Save = false;
	Load = false;
	Portal = false;
	Tip = false;
	MapName = MapAuthor = false;
	UploadMap = true;
	NewMapWidth = 10;
	NewMapHeight = 10;
	NewMapTile = 0;
	CurrentTile = 0;
	CurrentSpecial = -1;
	MapOffX=0;
	MapOffY=0;
	LastTileClicked = 0;
	LastBlocked = 0;
	LastUnblocked = 0;
	SaveNumber = 0;
	LoadNumber = 0;
	TipNumber = 0;
	CurrentPortal = 0;
	ZoomFactor = 4;
	LastZoom = 0.f;
	CustomMapSelection = 0;
}

string ControlState::GetState(){
	if(NewMap == true){return "NewMap";}
	else if(Main == true){return "Main";}
	else if(TileSelection == true){return "TileSelection";}
	else if(SpecialSelection == true){return "SpecialSelection";}
	else if(Menu == true){return "Menu";}
	else if(Save == true){return "Save";}
	else if(Load == true){return "Load";}
	else if(Portal == true){return "Portal";}
	else if(Tip == true){return "Tip";}
	else if(MapName == true){return "TextEnterMapName";}
	else if(MapAuthor == true){return "TextEnterMapAuthor";}
	
	//return error is no game state applicable
	else {return "FATAL ERROR! (NO STATE RECOGNISED O.o)";}
}

void ControlState::SetState(string InState){
	string CurrentState = GetState();	
	NewMap = false;
	Main = false;	
	TileSelection = false;
	SpecialSelection = false;
	Menu = false;
	Save = false;
	Load = false;
	Portal = false;
	Tip = false;
	MapName = MapAuthor = false;
	
	if(InState == "NewMap"){NewMap = true; cout << "control state set to: 'NewMap'" << endl;}
	else if(InState == "Main"){Main = true; cout << "control state set to: 'Main'" << endl;}
	else if(InState == "TileSelection"){TileSelection = true; cout << "control state set to: 'TileSelection'" << endl;}
	else if(InState == "SpecialSelection"){SpecialSelection = true; cout << "control state set to: 'SpecialSelection'" << endl;}
	else if(InState == "Menu"){Menu = true; cout << "control state set to: 'Menu'" << endl;}		
	else if(InState == "Save"){Save = true; cout << "control state set to: 'Save'" << endl;}		
	else if(InState == "Load"){Load = true; cout << "control state set to: 'Load'" << endl;}		
	else if(InState == "Portal"){Portal = true; cout << "control state set to: 'Portal'" << endl;}
	else if(InState == "Tip"){Tip = true; cout << "control state set to: 'Tip'" << endl;}
	else if(InState == "TextEnterMapName"){MapName = true; cout << "control state set to: 'TextEnterMapName'" << endl;}
	else if(InState == "TextEnterMapAuthor"){MapAuthor = true; cout << "control state set to: 'TextEnterMapAuthor'" << endl;}
	
	//if state is not valid, reset to old state
	else {cout << "'ControlState::SetState', recieved: " << InState << "which is an invalid state. State not changed" << endl;SetState(CurrentState);}
	
}
