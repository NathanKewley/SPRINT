//set wether to output dev info
bool DevOut = true;

//check if player tile has been placed
bool PlayerStartPlaced = false;

//current mouse buttons pressed down
bool LeftMouseDown = false;
bool LeftMouseActive = false;
bool RightMouseDown = false;
bool RightMouseActive = true;

//map name and author
string MapName = "Unknown";
string MapAuthor = "Unknown";

//create control control
ControlState Control;

//create 2d vector to hold tile class
vector<TileClass> Map;

//vector of strings for the custom maps
vector<string> CustomMaps;
	
//vector of mapdisplayclass for each custom map found
vector<MapDisplay> CustomMapInfo;

//Tile width and height
int TileWidth=64; int TileHeight=64;
//Map width an height
int MapWidth; int MapHeight;

//declare a font
sf::Font MyFont;

//creates a frame clock
sf::Clock FrameClock;
sf::Clock Time;

//sf::VideoMode::GetDesktopMode 	
// Create the main window
sf::RenderWindow App(sf::VideoMode::getDesktopMode(), "'SPRINT' Level Editor");

// Create a view with the same size as the window, located somewhere in the center of the background
sf::Vector2f Center(App.getSize().x/2, App.getSize().y/2);
sf::Vector2f HalfSize(App.getSize().x/2, App.getSize().y/2);
sf::View View(Center, HalfSize);   

// Create images
sf::Texture CrossImage;
sf::Texture MapArrowImage;
sf::Texture TextEnterImage;
sf::Texture LoadButtonImage;
sf::Texture SaveButtonImage;
sf::Texture CancelImage;
sf::Texture CreateImage;
sf::Texture SetImage;
sf::Texture UpArrowImage;
sf::Texture DownArrowImage;
sf::Texture LeftArrowImage;
sf::Texture RightArrowImage;
sf::Texture TipSelectionImage;
sf::Texture ZoomInImage;
sf::Texture ZoomOutImage;
sf::Texture NewMapImage; 
sf::Texture MenuImage; 
sf::Texture TileDisplayImage; 
sf::Texture SaveDisplayImage; 
sf::Texture LoadDisplayImage;
sf::Texture SpecialDisplayImage; 
sf::Texture BackgroundImage; 
sf::Texture TileSelectionImage;
sf::Texture SpecialSelectionImage;
sf::Texture SpecialImageArray [9]; //0:start, 1:finnish, 2:blocked, 3:Portals, 4:Tips, 5-8:G-(up,down,left,right), 
vector<sf::Texture> TileImageVector;

//create sprites
sf::Sprite CrossSprite;
sf::Sprite MapArrowSprite;
sf::Sprite LoadMapButtonSprite[4];
sf::Sprite SaveMapButtonSprite[4];
sf::Sprite NewMapButtonSprite[6];
sf::Sprite TipSetSprite;
sf::Sprite TipSelectionSprite;
sf::Sprite ZoomOutSprite;
sf::Sprite ZoomInSprite;
sf::Sprite BackgroundSprite;
sf::Sprite MenuSprite;
sf::Sprite NewMapSprite;
sf::Sprite SaveDisplaySprite;
sf::Sprite LoadDisplaySprite;
sf::Sprite TileSelectionSprite;
sf::Sprite SpecialSelectionSprite;
sf::Sprite CurrentTileSprite;
sf::Sprite TileDisplaySprite;
sf::Sprite SpecialDisplaySprite;
sf::Sprite TipDownArrowSprite;
sf::Sprite TipUpArrowSprite;
//declrare strings
sf::Text StringMapWidth;
sf::Text StringMapHeight;
sf::Text StringMapNum;
sf::Text StringToolTip;
sf::Text StringMapName;
sf::Text StringMapAuthor;


void SetupFonts(){
	//load font
	MyFont.loadFromFile("font/andrios_font.ttf");
	//set font for strings
	StringMapWidth.setFont(MyFont);
	StringMapHeight.setFont(MyFont);
	StringMapNum.setFont(MyFont);
	StringToolTip.setFont(MyFont);
	StringMapName.setFont(MyFont);
	StringMapAuthor.setFont(MyFont);
	//set size of strings
	StringMapWidth.setCharacterSize(20);
	StringMapHeight.setCharacterSize(20);
	StringMapNum.setCharacterSize(35);
	StringToolTip.setCharacterSize(35);		
	StringMapWidth.move(370.f, 165.f);
	StringMapHeight.move(370.f, 210.f);
	StringMapNum.move(213.f, 231.f);
	StringToolTip.move(12.f, -5.f);
}

void SetupImages(){
	MapArrowImage.loadFromFile("gfx/Sprint/MapBrowser/Arrow.png");	
	//NewMapImage
	NewMapImage.loadFromFile("gfx/editor/NewMap.png");
	MenuImage.loadFromFile("gfx/editor/Menu.png");
	CrossImage.loadFromFile("gfx/Cross.png");
	//Tile Display Image
	TileDisplayImage.loadFromFile("gfx/Tiles/TileDisplay.png");
	
	//TipSelection image
	TipSelectionImage.loadFromFile("gfx/editor/TipSelect.png");
	SetImage.loadFromFile("gfx/editor/Set.png");
	
	//save image
	SaveDisplayImage.loadFromFile("gfx/editor/Save.png");
	LoadDisplayImage.loadFromFile("gfx/Sprint/MapBrowser/Back.png");
	SaveButtonImage.loadFromFile("gfx/editor/SaveButton.png");
	LoadButtonImage.loadFromFile("gfx/editor/LoadButton.png");
	TextEnterImage.loadFromFile("gfx/editor/TextEnter.png");
	
	//TileSelectuin image
	TileSelectionImage.loadFromFile("gfx/editor/TileSelection.png");
	
	//Special selcetion image
	SpecialSelectionImage.loadFromFile("gfx/editor/SpecialSelection.png");
	
	//background image
	BackgroundImage.loadFromFile("gfx/backgrounds/back1.png");
	
	//Zoomb in/out imags
	ZoomInImage.loadFromFile("gfx/editor/ZoomIn.png");
	ZoomOutImage.loadFromFile("gfx/editor/ZoomOut.png");
	
	//arrow images
	UpArrowImage.loadFromFile("gfx/UpArrow.png");
	DownArrowImage.loadFromFile("gfx/DownArrow.png");
	LeftArrowImage.loadFromFile("gfx/LeftArrow.png");
	RightArrowImage.loadFromFile("gfx/RightArrow.png");
	
	//new map images
	CreateImage.loadFromFile("gfx/editor/Create.png");
	CancelImage.loadFromFile("gfx/editor/Cancel.png");
	
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
	
	//Set Sprites if neccasary
	SetSprite(MapArrowSprite, MapArrowImage, 1);	
	SetSprite(NewMapButtonSprite[0], LeftArrowImage, 1);
	SetSprite(NewMapButtonSprite[1], RightArrowImage, 1);
	SetSprite(NewMapButtonSprite[2], LeftArrowImage, 1);
	SetSprite(NewMapButtonSprite[3], RightArrowImage, 1);
	SetSprite(NewMapButtonSprite[4], CancelImage, 1);
	SetSprite(NewMapButtonSprite[5], CreateImage, 1);
	SetSprite(SaveMapButtonSprite[0], TextEnterImage, 1);
	SetSprite(SaveMapButtonSprite[1], TextEnterImage, 1);
	SetSprite(SaveMapButtonSprite[2], SaveButtonImage, 1);
	SetSprite(SaveMapButtonSprite[3], CancelImage, 1);
	SetSprite(LoadMapButtonSprite[0], UpArrowImage, 1);
	SetSprite(LoadMapButtonSprite[1], DownArrowImage, 1);
	SetSprite(LoadMapButtonSprite[2], LoadButtonImage, 1);
	SetSprite(LoadMapButtonSprite[3], CancelImage, 1);
	SetSprite(TipSetSprite, SetImage, 1);
	SetSprite(TipUpArrowSprite, UpArrowImage, 1);
	SetSprite(TipDownArrowSprite, DownArrowImage, 1);
	SetSprite(TipSelectionSprite, TipSelectionImage, 1);
	SetSprite(NewMapSprite, NewMapImage, 1);
	SetSprite(LoadDisplaySprite, LoadDisplayImage, 2);
	SetSprite(MenuSprite, MenuImage, 2);
	SetSprite(TileDisplaySprite, TileDisplayImage, 1);
	SetSprite(SaveDisplaySprite, SaveDisplayImage, 1);
	SetSprite(BackgroundSprite, BackgroundImage, 4);
	SetSprite(TileSelectionSprite, TileSelectionImage, 1);
	SetSprite(SpecialSelectionSprite, SpecialSelectionImage, 1);
	SetSprite(ZoomInSprite, ZoomInImage, 4);
	SetSprite(ZoomOutSprite, ZoomOutImage, 4);
	SetSprite(CrossSprite, CrossImage, 4);
			
	//sets the backgound to repeat
	BackgroundImage.setRepeated(true);
			
	//set position of sprites where neccasary
	TipSelectionSprite.setPosition(80,120);
	NewMapSprite.setPosition(160,120);
	LoadDisplaySprite.setPosition(80,120);
	SaveDisplaySprite.setPosition(80,120);
	//MenuSprite.setPosition(80,80);
	TileSelectionSprite.setPosition(80,80);
	SpecialSelectionSprite.setPosition(80,80);
	SpecialDisplaySprite.setPosition(571.f, 401.f);
	TileDisplaySprite.setPosition(571.f, 401.f);
	CurrentTileSprite.setPosition(571.f, 401.f);	
	ZoomInSprite.setPosition(570.f,250.f);
	ZoomOutSprite.setPosition(570.f,320.f);
	
	//load all of the tile images
	LoadTileImages(TileImageVector);
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




