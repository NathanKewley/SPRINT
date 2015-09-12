using namespace std;

//set wether to output dev info
bool DevOut = true;
int FPS = 0;
int FPSLimit = 60;
float FPSTime = 0;
bool TileNum = false;
bool CollisionDev = false;
bool BoolDrawFPS = true;

//create control control
ControlState Control;

//create player based on player class
PlayerClass Player;

//current player image/character
int CurrentPlayerImage = 0;

//create 2d vector to hold tile class
vector<TileClass> Map;

//Map specific vars
int TileWidth=32; int TileHeight=32;
int MapWidth; int MapHeight;
	
//Constant control
int GRAVITY = 4;
int GravityY = 4;
int GravityX = 0;
	
float OptionsLastTime = 0;
	
// Create the main window
sf::RenderWindow App(sf::VideoMode::getDesktopMode(), "SPRINT - Alpha 11");

// Create a view with the same size as the window, located somewhere in the center of the background
sf::Vector2f Center(App.getSize().x/2, App.getSize().y/2);
sf::Vector2f HalfSize(App.getSize().x/2, App.getSize().y/2);
sf::View View(Center, HalfSize);  

//create a render texture for the map
sf::RenderTexture MapTexture;	

//vector of strings for the custom maps
vector<string> CustomMaps;
	
//vector of mapdisplayclass for each custom map found
vector<MapDisplay> CustomMapInfo;
	
//create a clock to keep time
sf::Clock Time;
sf::Clock FrameClock;

//delcare string for font
sf::Text MapIndexString;
sf::Text FPSString;
sf::Text FPSLimitString;
sf::Text RealTimeVarString;

//declare a font
sf::Font MyFont;

//define render images

//define images
sf::Texture WorkingImage;
sf::Texture WorkingImage2;
sf::Texture MapArrowImage;
sf::Texture MapSelectBackImage;
sf::Texture MenuSelectImage;
sf::Texture LevelCompleteImage;
sf::Texture LevelCompleteButtonImage;
sf::Texture SplashImage;
sf::Texture PausedImage;
sf::Texture CrossImage;
sf::Texture WinImage;
sf::Texture MenuImage;
sf::Texture CreditsImage;
sf::Texture OptionsImage; 
sf::Texture OptionsSelectImage;
sf::Texture LoadingImage;
sf::Texture SpecialImageArray [9]; //0:start, 1:finnish, 2:blocked, 3:Portals, 4:Tips, 5-8:G-(up,down,left,right), 
sf::Texture TipImageArray[4];
sf::Texture PausedMenuImages[3];
sf::Texture BackgroundImage;
sf::Texture MidImage;
vector<sf::Texture> TileImageVector;

//define sprites
sf::Sprite WorkingSprite;
sf::Sprite WorkingSprite2;
sf::Sprite MapArrowSprite;
sf::Sprite MapSelectBackSprite;
sf::Sprite MenuSelectSprite;
sf::Sprite LevelCompleteButtonSprite;
sf::Sprite LevelCompleteSprite;
sf::Sprite SplashSprite;
sf::Sprite PausedSprite;
sf::Sprite CrossSprite;
sf::Sprite WinSprite;
sf::Sprite MenuSprite; 
sf::Sprite CreditsSprite; 
sf::Sprite OptionsSprite; 
sf::Sprite OptionsSelectSprite;
sf::Sprite LoadingSprite; 
sf::Sprite BackgroundSprite; 
sf::Sprite MidSprite; 
sf::Sprite PausedMenuSprites[3];

//create ftp client
sf::Ftp ftp;

using namespace std;

void ClearGame(){
	Control.CurrentMap = 0;
	View.setCenter(Center);
	cout << "Game Veriables have been reset" << endl;
}

void LoadPlayerImage(){
	switch(CurrentPlayerImage){
		case 0:
			Player.PlayerImage.loadFromFile("gfx/Sprint/player/PlayerTileSheet0.png");
		break;
		case 1:
			Player.PlayerImage.loadFromFile("gfx/Sprint/player/PlayerTileSheet1.png");
		break;
		case 2:
			Player.PlayerImage.loadFromFile("gfx/Sprint/player/PlayerTileSheet2.png");
		break;
	}
	Player.JetPackImage.loadFromFile("gfx/Sprint/player/JetPack.png");
}

void SetupImages(){
	//load image files into images
	MapArrowImage.loadFromFile("gfx/Sprint/MapBrowser/Arrow.png");	
	WorkingImage.loadFromFile("gfx/Sprint/LoadMapList.png");	
	WorkingImage2.loadFromFile("gfx/Sprint/DownloadingMap.png");	
	MapSelectBackImage.loadFromFile("gfx/Sprint/MapBrowser/Back.png");	
	WinImage.loadFromFile("gfx/Sprint/menu/Win.png");
	LevelCompleteButtonImage.loadFromFile("gfx/Sprint/LevelCompleteButton.png");
	MenuImage.loadFromFile("gfx/Sprint/menu/SprintMenu.png");
	CreditsImage.loadFromFile("gfx/Sprint/menu/Credits.png");
	OptionsImage.loadFromFile("gfx/Sprint/menu/Options.png");
	LoadingImage.loadFromFile("gfx/Sprint/menu/Loading.png");
	CrossImage.loadFromFile("gfx/Cross.png");
	OptionsSelectImage.loadFromFile("gfx/Sprint/options/OptionsSelect.png");
	PausedImage.loadFromFile("gfx/Sprint/menu/PausedMenu.png");
	SplashImage.loadFromFile("gfx/Sprint/Splash.png");
	LevelCompleteImage.loadFromFile("gfx/Sprint/LevelComplete.png");
	MenuSelectImage.loadFromFile("gfx/Sprint/menu/MenuSelect.png");
	BackgroundImage.loadFromFile("gfx/backgrounds/back1.png");
	MidImage.loadFromFile("gfx/backgrounds/mid1.png");
	
	//special images
	SpecialImageArray[0].loadFromFile("gfx/Special/PlayerStart.png");
	SpecialImageArray[1].loadFromFile("gfx/Special/PlayerEnd.png");
	SpecialImageArray[2].loadFromFile("gfx/Special/Blocked.png");
	SpecialImageArray[3].loadFromFile("gfx/Special/Portal.png");
	SpecialImageArray[4].loadFromFile("gfx/Special/Tip.png");
	SpecialImageArray[5].loadFromFile("gfx/Special/GravityUp.png");
	SpecialImageArray[6].loadFromFile("gfx/Special/GravityDown.png");
	SpecialImageArray[7].loadFromFile("gfx/Special/GravityLeft.png");
	SpecialImageArray[8].loadFromFile("gfx/Special/GravityRight.png");
	
	//load the tips
	TipImageArray[0].loadFromFile("gfx/tips/Tip0.png");
	TipImageArray[1].loadFromFile("gfx/tips/Tip1.png");
	TipImageArray[2].loadFromFile("gfx/tips/Tip2.png");
	TipImageArray[3].loadFromFile("gfx/tips/Tip3.png");
	
	//load paused menu images
	PausedMenuImages[0].loadFromFile("gfx/Sprint/menu/ResumeButton.png");
	PausedMenuImages[1].loadFromFile("gfx/Sprint/menu/OptionsButton.png");
	PausedMenuImages[2].loadFromFile("gfx/Sprint/menu/ExitButton.png");
	
	//loads the initial player image
	LoadPlayerImage();
	
	//load all of the tile images
	LoadTileImages(TileImageVector);
	
	//Set sprites to images
	SetSprite(MapArrowSprite, MapArrowImage, 1);	
	SetSprite(WorkingSprite, WorkingImage, 4);	
	SetSprite(WorkingSprite2, WorkingImage2, 4);	
	SetSprite(MapSelectBackSprite, MapSelectBackImage, 2);
	SetSprite(WinSprite, WinImage, 4);
	SetSprite(MenuSprite, MenuImage, 2);
	SetSprite(CreditsSprite, CreditsImage, 2);
	SetSprite(OptionsSprite, OptionsImage, 4);
	SetSprite(OptionsSelectSprite, OptionsSelectImage, 4);
	SetSprite(LoadingSprite, LoadingImage, 4);
	SetSprite(CrossSprite, CrossImage, 4);
	SetSprite(PausedSprite, PausedImage, 4);
	SetSprite(SplashSprite, SplashImage, 4);
	SetSprite(LevelCompleteSprite, LevelCompleteImage, 4);
	SetSprite(LevelCompleteButtonSprite, LevelCompleteButtonImage, 4);
	SetSprite(MenuSelectSprite, MenuSelectImage, 2);
	SetSprite(BackgroundSprite, BackgroundImage, 4);
	SetSprite(MidSprite, MidImage, 4);
	
	SetSprite(PausedMenuSprites[0], PausedMenuImages[0], 4);
	SetSprite(PausedMenuSprites[1], PausedMenuImages[1], 4);
	SetSprite(PausedMenuSprites[2], PausedMenuImages[2], 4);
	
	//set up the player sprite
	SetSprite(Player.PlayerSprite, Player.PlayerImage, 2);
	SetSprite(Player.JetPackSprite, Player.JetPackImage, 2);

	//sets the sprite center to the center so rotating work properly
	Player.PlayerSprite.setOrigin(6.5,7.5);

	//set background and intermediate layer to repeat
	BackgroundImage.setRepeated(true);
	MidImage.setRepeated(true);
	
	//setup the font(s)
	FPSLimitString.setFont(MyFont);
	FPSLimitString.setCharacterSize(50);
	FPSLimitString.move(505,205);

}

void DrawRealTimeVar(){
	stringstream VarTempString;
	switch(Control.RealTimeVar){
		case 1:
    		VarTempString << "GRAVITY: " << GRAVITY;
		break;
		case 2:
    		VarTempString << "Player.JumpHeight: " << Player.JumpHeight;
		break;
		case 3:
    		VarTempString << "Player.AnimationTime: " << Player.AnimationTime;
		break;
		case 4:
    		VarTempString << "Player.MoveSpeed: " << Player.MoveSpeed;
		break;		
	}
	//set the string
	RealTimeVarString.setString(VarTempString.str());
	
	//draw the string to the screen
	App.draw(RealTimeVarString);
}

void HandleRealTimeVar(string InKey){
	//toggle the variable being modified
	if(InKey == "F12"){if(Control.RealTimeVar < 4){Control.RealTimeVar++;} else {Control.RealTimeVar = 0;}}

	//if a variable is selected for modification
	else if(Control.RealTimeVar != 0){ 	
		//increase the varibale
		if(InKey == "+"){
			switch(Control.RealTimeVar){
				case 1:
    				GRAVITY++;
				break;
				case 2:
    				Player.JumpHeight++;
				break;
				case 3:
    				Player.AnimationTime+=0.01;
				break;
				case 4:
    				Player.MoveSpeed+=0.1;
				break;				
			}	
		}		

		//decrease the varibale
		if(InKey == "-"){
			switch(Control.RealTimeVar){
				case 1:
    				GRAVITY--;
				break;
				case 2:
    				Player.JumpHeight--;
				break;
				case 3:
    				Player.AnimationTime-=0.01;
				break;
				case 4:
    				Player.MoveSpeed-=0.1;
				break;				
			}
		}
	}
	else {cout << "use F12 to select a variable for modification before using +/-" << endl;}
}

void LoadServerMapList(vector<string> &MapNames)
{
	// Create a new FTP client
	int i = 0;

	// Connect to the server
	sf::Ftp::Response response = ftp.connect("175.107.158.147");
	if (response.isOk())
	    cout << "Connected" << std::endl;

	// Log in
	response = ftp.login("sprint", "S17mypAZ583");
	if (response.isOk()){
	    cout << "Logged in" << std::endl;
	} else {cout << "Failed to log in" << endl;}

	// Print the working directory
	sf::Ftp::DirectoryResponse directory = ftp.getWorkingDirectory();
	if (directory.isOk())
	    cout << "Working directory: " << directory.getDirectory() << std::endl;

	sf::Ftp::ListingResponse Resp = ftp.getDirectoryListing("CustomMaps");
	if (Resp.isOk())
	{
		cout << "Feching Filenames" << endl;
		MapNames =  Resp.getFilenames(); 
		//ftp.download("/home/nathan/Desktop/SPRINTMAPS/map0.map", ".", sf::Ftp::Ascii);
	}
}

void DownloadMap(string InMapName)
{
	// Create a new FTP client
	//sf::Ftp DownloadFtp;
	//int i = 0;

	// Connect to the server
	sf::Ftp::Response response;
	//if (response.isOk())
	 //   cout << "Connected" << std::endl;

	// Log in
	//response = DownloadFtp.login("sprint", "S17mypAZ583");
	//if (response.isOk())
	//    cout << "Logged in" << std::endl;

	cout << "Downloading Map: " << InMapName << endl;
	response = ftp.download(InMapName, "CustomMaps/");
	if(response.isOk()){
		cout << "Finnished Downloading Map" << endl;
	}
	else {cout << "failed to download map" << endl;}
}

void QueueMapList(){
	int i = 0;
	CustomMaps.clear();
	CustomMapInfo.clear();
	
	LoadServerMapList(CustomMaps);
	//ScanDir(CustomMaps, "CustomMaps/", ".map");
	
	for(i=0;i<CustomMaps.size();i++){
		MapDisplay TempCustomMap(MyFont);
		
		//CustomMaps[i] = GetMapFilename(CustomMaps[i]);
		cout << "Found Map: " << CustomMaps[i] << endl;
		
		TempCustomMap.MapFilename.setString(CustomMaps[i]);
		TempCustomMap.MapName.setString(CustomMaps[i]);
		//TempCustomMap.MapAuthor.setString(LoadMapInfo(CustomMaps[i], "Author"));
		
		CustomMapInfo.push_back(TempCustomMap);
	}
	cout << "Custom Map info size: " << CustomMapInfo.size() << endl;
}

void LoadCustomMaps(){
	int i = 0;
	CustomMaps.clear();
	CustomMapInfo.clear();
	ScanDir(CustomMaps, "CustomMaps/", ".map");
	
	for(i=0;i<CustomMaps.size();i++){
		MapDisplay TempCustomMap(MyFont);
		
		CustomMaps[i] = GetMapFilename(CustomMaps[i]);
		cout << "Found Map: " << CustomMaps[i] << endl;
		
		TempCustomMap.MapFilename.setString(CustomMaps[i]);
		TempCustomMap.MapName.setString(LoadMapInfo(CustomMaps[i], "MapName"));
		TempCustomMap.MapAuthor.setString(LoadMapInfo(CustomMaps[i], "Author"));
		
		CustomMapInfo.push_back(TempCustomMap);
	}
	cout << "Custom Map info size: " << CustomMapInfo.size() << endl;
}

void SetPlayer(int InIndex){
	//set the player position
	Player.SetPos(Map[InIndex].Picture.getPosition().x+12.5, (Map[InIndex].Picture.getPosition().y+9));
	
	//re-check player collision in the new map
	Player.Fall(true);
	
	//set the player to not being able to jump
	Player.PlayerCanJump = false;
	
	//reset the player rotation and flip value
	//Player.PlayerSprite.scale(1.f,1.f);
	Player.PlayerSprite.setRotation(0);
	
	if(DevOut == true){
		cout << "player has been set for this level" << endl;
		cout << "player X: " << Map[InIndex].Picture.getPosition().x << " | Y: " << Map[InIndex].Picture.getPosition().y << endl;
	}
}
void PlayerJump(){
	if(Player.PlayerCanJump == true){
		cout << "player jumping" << endl;
		Player.SetJump(Player.JumpHeight);
		Player.PlayerCanJump = false;
	}
}
void NewMap(int InWidth, int InHeight, sf::Texture& InDefaultTexture){
	int i=0;
	int XCount=0;
	int YCount=0;
	int TotalSize;
	TileClass TempTile(0);
	
	TotalSize = (InWidth*InHeight);
	//delete old map
	Map.clear();

	//output size of map made
	if(DevOut == true){cout << "Map Made with. Width: " << InWidth << " | Height: "<< InHeight << endl;}
	if(DevOut == true){cout << "map total size is: " << TotalSize << endl;}

	//create new map based on input
	for (i = 0; i < TotalSize; i++ ) {
		Map.push_back(TempTile);
		SetSprite(Map[i].Picture, InDefaultTexture, 2);
		Map[i].Picture.setPosition((TileWidth*XCount),(TileHeight*YCount));
		Map[i].SpecialPicture.setPosition((TileWidth*XCount),(TileHeight*YCount));
		XCount++;
		if((i+1)%InWidth == 0){XCount=0;YCount++;}
	}
	if(DevOut == true){cout << "Final map index: " << i << endl; cout << Map[i].IntPicture << endl;}
	//resizes the background to fit the map size
	//BackgroundSprite.scale(InWidth*TileWidth,InHeight*TileHeight);
}
void SaveOptions(){
	ostringstream Filename;
	Filename << "bin/Options.dat";
	string strFileName = Filename.str();
	ofstream File(strFileName.c_str());
	
	//save the chosen player to the options file
	File << CurrentPlayerImage << endl;
	
	//save the value of show FPS
	File << BoolDrawFPS << endl;
	
	//save the value of FPS Limit
	//File << FPSLimit << endl;
	
	//close the file
	File.close();
	
	cout << "Options saved to disk" << endl;
}
void LoadOptions(){
	int LineCount = 0;
	string ReadData;

	ostringstream Filename;
	Filename << "bin/Options.dat";
	string strFileName = Filename.str();
	ifstream Options(strFileName.c_str());
	
	//load data from file (INSANITY!!!)
	if (Options.is_open()){
		while ( Options.good() ){
			//reads a line of data from file
			getline (Options,ReadData);	
			
			//loads the player image
			if(LineCount == 0){
				CurrentPlayerImage = atoi(ReadData.c_str()); 
				cout << "PlayerImage = "  << CurrentPlayerImage << endl;
			}
			//loads wether to show fps
			if(LineCount == 1){
				BoolDrawFPS = atoi(ReadData.c_str()); 
				cout << "ShowFPS = "  << BoolDrawFPS << endl;
			}
			//loads current fps limit
			//if(LineCount == 2){
			//	FPSLimit = atoi(ReadData.c_str()); 
			//	cout << "FPSLimit = "  << FPSLimit << endl;
			//}			
			LineCount++;			
		}
	}
	
	cout << "Options have been loaded from disk" << endl;
}
void MakeMap(int InWidth, int InHeight){
	int XCount = 0;
	int YCount = 0;
	int i=0;
	int TotalSize = (InWidth*InHeight);
	
	//make the map texture setting size
	//MapTexture.create(TileWidth*InWidth, TileHeight*InHeight);
	
	//clear the map texture
	//MapTexture.clear(sf::Color::Red);
	
	//draw tiles and special to the map texture
	//for (i = 0; i < TotalSize; i++ ) {
		//draw the map
	//	MapTexture.draw(Map[i].Picture);
	//	if(Map[i].Special != "None"){
	//		if(Map[i].Special == "Blocked"){
	//			MapTexture.draw(Map[i].SpecialPicture);
	//		}
	//	}
	//}
	//update the texture display
	//MapTexture.display();
	
	//scale the backround and set its postions
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (InWidth*TileWidth+View.getSize().x),(InHeight*TileHeight+View.getSize().y)));
	BackgroundSprite.setPosition(-View.getSize().x, -View.getSize().y);
	
	MidSprite.setTextureRect(sf::IntRect(0, 0, InWidth*TileWidth+View.getSize().x,MidImage.getSize().y));
	MidSprite.setPosition(-View.getSize().x, InHeight*TileHeight);
}

void Load(int InNumber){
	int BeforeTileData = 0; //used to grab mapwidth and height before processing tiles
	int StringToNum;
	int Switch = 0; //keeps track of which map variable is current
	int i = 0;
	int LineCount = 0;
	int SpecialPic = -1;
	
	//reset gravity and player
	GravityY = GRAVITY;
	GravityX = 0;
	Player.PlayerSprite.setScale(2.f,2.f);
	
	//reset position of background and mid sprites
	BackgroundSprite.setPosition(0,0);
	MidSprite.setPosition(0,0);
	
	//int MapWidth, MapHeight;
	string ReadData;
	
	App.setView(App.getDefaultView());
	App.draw(LoadingSprite);
	App.display();
	
	ostringstream Filename;
	Filename << "maps/map" << InNumber << ".map";
	string strFileName = Filename.str();
	ifstream MapFile(strFileName.c_str());
	
	//load data from file (INSANITY!!!)
	if (MapFile.is_open()){
		while ( MapFile.good() ){
			LineCount++;
			//cout << "current line of file: " << LineCount << endl;
			getline (MapFile,ReadData);
			BeforeTileData++;
			if(BeforeTileData == 3){ //loads map width
				StringToNum = atoi(ReadData.c_str()); 
				//MapWidth = StringToNum;
				Control.MapWidth = StringToNum;
				cout << "loaded map width: " << Control.MapWidth << endl;
			}
			else if(BeforeTileData == 4){ //loads map height and sets the new map up via size
				StringToNum = atoi(ReadData.c_str()); 
				//MapHeight = StringToNum;
				Control.MapHeight = StringToNum;
				cout << "loaded map Height: " << Control.MapHeight << endl;
				NewMap(Control.MapWidth,Control.MapHeight,TileImageVector[0]);				
			}
			if(i < (Control.MapWidth*Control.MapHeight)){
				if(BeforeTileData > 4){
					//cout << "loading tile: " << i << endl;
					if(Switch == 0){
						StringToNum = atoi(ReadData.c_str()); 
						Map[i].IntPicture = StringToNum;
						SetSprite(Map[i].Picture, TileImageVector[StringToNum], 2);
						//Map[i].Picture.SetImage(TileImageVector[StringToNum]); 
						cout << "Map[" << i << "].IntPicture set to: " << StringToNum << endl;
					} 
					if(Switch == 1){
						Map[i].Special = ReadData; 
						//find out which picture to give it *facepalm*
						if(Map[i].Special == "Start"){SpecialPic = 0; SetPlayer(i);}
						if(Map[i].Special == "End"){SpecialPic = 1;}
						if(Map[i].Special == "Blocked"){SpecialPic = 2;}
						if(Map[i].Special == "Portal"){SpecialPic = 3;}	
						if(Map[i].Special == "Tip"){SpecialPic = 4;}			
						if(Map[i].Special == "GravityUp"){SpecialPic = 5;}	
						if(Map[i].Special == "GravityDown"){SpecialPic = 6;}	
						if(Map[i].Special == "GravityLeft"){SpecialPic = 7;}	
						if(Map[i].Special == "GravityRight"){SpecialPic = 8;}
						if(SpecialPic != -1){
							SetSprite(Map[i].SpecialPicture, SpecialImageArray[SpecialPic], 2);
							//Map[i].SpecialPicture.SetImage(SpecialImageArray[SpecialPic]);
						}
						cout << "Map[" << i << "].Special set to: " << ReadData << endl;
					}
					if(Switch == 2){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalX = StringToNum; 
						if(Map[i].PortalX > 0){cout << "Portal X set to: " << Map[i].PortalX << endl;}
					}
					if(Switch == 3){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalY = StringToNum;
						if(Map[i].PortalY > 0){cout << "Portal Y set to: " << Map[i].PortalY << endl;}					 				 
					}
					if(Switch == 4){
						StringToNum = atoi(ReadData.c_str());
						Map[i].TipNumber = StringToNum;
						i++; 
					}			
					Switch++; if(Switch == 5){Switch = 0;}
				}
			}
			//cout << ReadData << endl;
		}
		cout << "attempted to load map.........." << endl;
		MapFile.close();
		Control.SetState("Main");
	}
	else{
		cout << "Could not find map, Assuming last level completed, showing win screen" << endl;
		ClearGame();
		Control.SetState("Win");
	}
	//Makes the map Texture based on the loaded data
	MakeMap(Control.MapWidth, Control.MapHeight);
}

void Load(string InMap){
	int BeforeTileData = 0; //used to grab mapwidth and height before processing tiles
	int StringToNum;
	int Switch = 0; //keeps track of which map variable is current
	int i = 0;
	int LineCount = 0;
	int SpecialPic = -1;
	
	//reset gravity and player
	GravityY = GRAVITY;
	GravityX = 0;
	Player.PlayerSprite.setScale(2.f,2.f);
	
	//reset position of background and mid sprites
	BackgroundSprite.setPosition(0,0);
	MidSprite.setPosition(0,0);
	
	//int MapWidth, MapHeight;
	string ReadData;
	
	App.setView(App.getDefaultView());
	App.draw(LoadingSprite);
	App.display();
	
	ifstream MapFile(InMap.c_str());
	
	//load data from file (INSANITY!!!)
	if (MapFile.is_open()){
		while ( MapFile.good() ){
			LineCount++;
			//cout << "current line of file: " << LineCount << endl;
			getline (MapFile,ReadData);
			BeforeTileData++;
			if(BeforeTileData == 3){ //loads map width
				StringToNum = atoi(ReadData.c_str()); 
				//MapWidth = StringToNum;
				Control.MapWidth = StringToNum;
				cout << "loaded map width: " << Control.MapWidth << endl;
			}
			else if(BeforeTileData == 4){ //loads map height and sets the new map up via size
				StringToNum = atoi(ReadData.c_str()); 
				//MapHeight = StringToNum;
				Control.MapHeight = StringToNum;
				cout << "loaded map Height: " << Control.MapHeight << endl;
				NewMap(Control.MapWidth,Control.MapHeight,TileImageVector[0]);				
			}
			if(i < (Control.MapWidth*Control.MapHeight)){
				if(BeforeTileData > 4){
					//cout << "loading tile: " << i << endl;
					if(Switch == 0){
						StringToNum = atoi(ReadData.c_str()); 
						Map[i].IntPicture = StringToNum;
						SetSprite(Map[i].Picture, TileImageVector[StringToNum], 2);
						//Map[i].Picture.SetImage(TileImageVector[StringToNum]); 
						cout << "Map[" << i << "].IntPicture set to: " << StringToNum << endl;
					} 
					if(Switch == 1){
						Map[i].Special = ReadData; 
						//find out which picture to give it *facepalm*
						if(Map[i].Special == "Start"){SpecialPic = 0; SetPlayer(i);}
						if(Map[i].Special == "End"){SpecialPic = 1;}
						if(Map[i].Special == "Blocked"){SpecialPic = 2;}
						if(Map[i].Special == "Portal"){SpecialPic = 3;}	
						if(Map[i].Special == "Tip"){SpecialPic = 4;}			
						if(Map[i].Special == "GravityUp"){SpecialPic = 5;}	
						if(Map[i].Special == "GravityDown"){SpecialPic = 6;}	
						if(Map[i].Special == "GravityLeft"){SpecialPic = 7;}	
						if(Map[i].Special == "GravityRight"){SpecialPic = 8;}
						if(SpecialPic != -1){
							SetSprite(Map[i].SpecialPicture, SpecialImageArray[SpecialPic], 2);
							//Map[i].SpecialPicture.SetImage(SpecialImageArray[SpecialPic]);
						}
						cout << "Map[" << i << "].Special set to: " << ReadData << endl;
					}
					if(Switch == 2){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalX = StringToNum; 
						if(Map[i].PortalX > 0){cout << "Portal X set to: " << Map[i].PortalX << endl;}
					}
					if(Switch == 3){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalY = StringToNum;
						if(Map[i].PortalY > 0){cout << "Portal Y set to: " << Map[i].PortalY << endl;}					 				 
					}
					if(Switch == 4){
						StringToNum = atoi(ReadData.c_str());
						Map[i].TipNumber = StringToNum;
						i++; 
					}			
					Switch++; if(Switch == 5){Switch = 0;}
				}
			}
			//cout << ReadData << endl;
		}
		cout << "attempted to load map.........." << endl;
		MapFile.close();
		Control.SetState("Main");
	}
	else{
		cout << "Could not find map, Assuming last level completed, showing win screen" << endl;
		ClearGame();
		Control.SetState("Win");
	}
	//Makes the map Texture based on the loaded data
	MakeMap(Control.MapWidth, Control.MapHeight);
}


















