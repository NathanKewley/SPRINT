/*------------------------------------------------------------------------------
FILE                  ControlState.h
PROGRAM               Control class for Sprint
PROGRAMMER            Nathan Kewley      
ENVIROMENT            Linux Mint 12 - g++ with SFML
------------------------------------------------------------------------------*/

using namespace std;
class ControlState{
	private:
	bool Menu;
	bool Main;
	bool Credits;
	bool Options;
	bool Win;
	bool Paused;
	bool Splash;
	bool LevelComplete;
	bool MapSelection;
	bool DownloadMaps;
	public:
	int MapWidth, MapHeight, CurrentMap;
	int MapOffX, MapOffY;
	int MenuSelection;
	int RealTimeVar;
	int BackScrollSpeed;
	int OptionSelection;
	int CustomMapSelection;
	bool CustomMap;
	
	string GetState();
	void SetState(string InState);
	ControlState();
};

ControlState::ControlState(){
	Main = false;
	Menu = true;
	Splash = false;
	Credits = false;
	Options = false;
	Win = false;
	Paused = false;
	LevelComplete = false;
	MapSelection = false;
	CustomMap = false;
	DownloadMaps = false;
	MapWidth = MapHeight = CurrentMap = MapOffX = MapOffY = MenuSelection = RealTimeVar = OptionSelection = CustomMapSelection = 0;
	BackScrollSpeed = 1;
}

string ControlState::GetState(){
	if(Menu == true){return "Menu";}
	else if(Main == true){return "Main";}
	else if(Credits == true){return "Credits";}
	else if(Options == true){return "Options";}
	else if(Win == true){return "Win";}
	else if(Paused == true){return "Paused";}
	else if(Splash == true){return "Splash";}
	else if(LevelComplete == true){return "LevelComplete";}
	else if(MapSelection == true){return "MapSelection";}
	else if(DownloadMaps == true){return "DownloadMaps";}
	
	//return error is no game state applicable
	else {return "FATAL ERROR! (NO STATE RECOGNISED O.o)";}
}

void ControlState::SetState(string InState){
	string CurrentState = GetState();	
	Main = false;	
	Menu = false;
	Credits = false;
	Options = false;
	Win = false;
	Paused = false;
	Splash = false;
	LevelComplete = false;
	MapSelection = false;
	DownloadMaps = false;
	
	if(InState == "Menu"){Menu = true; cout << "control state set to: 'Menu'" << endl;}
	else if(InState == "Main"){Main = true; cout << "control state set to: 'Main'" << endl;}	
	else if(InState == "Credits"){Credits = true; cout << "control state set to: 'Credits'" << endl;}
	else if(InState == "Options"){Options = true; cout << "control state set to: 'Options'" << endl;}
	else if(InState == "Win"){Win = true; cout << "control state set to: 'Win'" << endl;}
	else if(InState == "Paused"){Paused = true; cout << "control state set to: 'Paused'" << endl;}
	else if(InState == "Splash"){Splash = true; cout << "control state set to: 'Splash'" << endl;}
	else if(InState == "LevelComplete"){LevelComplete = true; cout << "control state set to: 'LevelComplete'" << endl;}
	else if(InState == "MapSelection"){MapSelection = true; cout << "control state set to: 'MapSelection'" << endl;}
	else if(InState == "DownloadMaps"){DownloadMaps = true; cout << "control state set to: 'DownloadMaps'" << endl;}
	
	//if state is not valid, reset to old state
	else {cout << "'ControlState::SetState', recieved: " << InState << "which is an invalid state. State not changed" << endl;SetState(CurrentState);}
	
}
