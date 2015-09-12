//include for C/C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <sys/stat.h>

//include needed SFML files
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "MapDisplayClass.h"
#include "TileClass.h"
#include "EditorControl.h"
#include "SystemFunctions.h"
#include "Editor.h"

using namespace std;

void NewMap(int InWidth, int InHeight, sf::Texture& InDefaultImage){
	int i=0;
	int XCount=0;
	int YCount=0;
	int TotalSize = (InWidth*InHeight);
	TileClass TempTile(0);
	
	//delete old map
	Map.clear();
	PlayerStartPlaced = false;

	//sets the size of the backfround
	BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (InWidth*TileWidth)/4,(InHeight*TileHeight)/4));

	//create new map based on input
	for (i = 0; i < TotalSize; i++ ) {
		Map.push_back(TempTile);
		SetSprite(Map[i].Picture, InDefaultImage, 4);
		//Map[i].Picture.SetImage(InDefaultImage);
		Map[i].Picture.setPosition((TileWidth*XCount),(TileHeight*YCount));
		Map[i].SpecialPicture.setPosition((TileWidth*XCount),(TileHeight*YCount));
		XCount++;
		if((i+1)%InWidth == 0){XCount=0;YCount++;}
	}
}
void ShowMap(int InWidth, int InHeight){
	int i=0;
	int TotalSize = (InWidth*InHeight);
	for (i = 0; i < TotalSize; i++ ) {
		//draw the map
		App.draw(Map[i].Picture);
		if(Map[i].Special != "None"){
			App.draw(Map[i].SpecialPicture);
		}
		//draw the destination of portals if neccasary
		if((Map[i].Special == "Portal") and (Map[i].PortalX + Map[i].PortalY > 0)){
			//works out where to draw line depending on zoom factor			
			//if(Control.ZoomFactor == 4){App.draw(sf::Shape::Line(Map[i].Picture.GetPosition().x+(TileWidth/2), Map[i].Picture.GetPosition().y+(TileHeight/2), Map[i].PortalX, Map[i].PortalY, 1.0, sf::Color::Blue));}
			//else if(Control.ZoomFactor == 2){App.draw(sf::Shape::Line(Map[i].Picture.GetPosition().x+(TileWidth/2), Map[i].Picture.GetPosition().y+(TileHeight/2), Map[i].PortalX/2, Map[i].PortalY/2, 1.0, sf::Color::Blue));}
		}
	}
}
void NewMapEvent(int InX, int InY){
	//top left arrow
	if(MouseClickSprite(InX, InY, NewMapButtonSprite[0]) == true){
		if(Control.NewMapWidth > 1){Control.NewMapWidth--;}
		if(DevOut == true){cout << "top left arrow" << endl;}
	}
	//top right arrow
	if(MouseClickSprite(InX, InY, NewMapButtonSprite[1]) == true){
		if(Control.NewMapWidth < 99){Control.NewMapWidth++;}
		if(DevOut == true){cout << "top Right arrow" << endl;}
	}
	//middle left arrow
	if(MouseClickSprite(InX, InY, NewMapButtonSprite[2]) == true){
		if(Control.NewMapHeight > 1){Control.NewMapHeight--;}
		if(DevOut == true){cout << "middle left arrow" << endl;}
	}
	//middle right arrow
	if(MouseClickSprite(InX, InY, NewMapButtonSprite[3]) == true){
		if(Control.NewMapHeight < 99){Control.NewMapHeight++;}
		if(DevOut == true){cout << "middle right arrow" << endl;}
	}
	//detect cancel button pressed
	if(MouseClickSprite(InX, InY, NewMapButtonSprite[4]) == true){
		Control.SetState("Main");
		if(DevOut == true){cout << "New map creation cancelled" << endl;}
		//sets the mouse so it dosent put selected special on the map strainght away
		LeftMouseActive = false;
		LeftMouseDown = false;
	}
	//if new map is pressed create a new map
	if(MouseClickSprite(InX, InY, NewMapButtonSprite[5]) == true){
		MapWidth = Control.NewMapWidth;
		MapHeight = Control.NewMapHeight;
		NewMap(MapWidth,MapHeight,TileImageVector[Control.NewMapTile]);
		Control.SetState("Main");
		if(DevOut == true){cout << "New map created with width: " << MapWidth << " | Height: " << MapHeight << endl;}
	}
}
void NewMapOptions(){
	//move the newMapSprite, arrows, text and buttons to appropriate locations
	NewMapSprite.setPosition((View.getCenter().x-NewMapSprite.getGlobalBounds().width/2), (View.getCenter().y-NewMapSprite.getGlobalBounds().height/2));
	NewMapButtonSprite[0].setPosition(View.getCenter().x-6, View.getCenter().y-43);
	NewMapButtonSprite[1].setPosition(View.getCenter().x+115, View.getCenter().y-43);
	NewMapButtonSprite[2].setPosition(View.getCenter().x-6, View.getCenter().y+15);
	NewMapButtonSprite[3].setPosition(View.getCenter().x+115, View.getCenter().y+15);
	NewMapButtonSprite[4].setPosition(View.getCenter().x+55, View.getCenter().y+70);
	NewMapButtonSprite[5].setPosition(View.getCenter().x-154, View.getCenter().y+70);
	StringMapWidth.setPosition(View.getCenter().x+60, View.getCenter().y-44);
	StringMapHeight.setPosition(View.getCenter().x+60, View.getCenter().y+12);
	
	//draw the NewMapSprite, arrows and buttons to the screen
	App.draw(NewMapSprite);
	App.draw(NewMapButtonSprite[0]);
	App.draw(NewMapButtonSprite[1]);
	App.draw(NewMapButtonSprite[2]);
	App.draw(NewMapButtonSprite[3]);
	App.draw(NewMapButtonSprite[4]);
	App.draw(NewMapButtonSprite[5]);

	//render the new map width variable	
	stringstream Width;
    Width << Control.NewMapWidth;
    StringMapWidth.setString(Width.str());
    App.draw(StringMapWidth);
    
    //Render the new map height variable
	stringstream Height;
    Height << Control.NewMapHeight;
    StringMapHeight.setString(Height.str());
    App.draw(StringMapHeight);
}
void DrawTileSelection(){
//TipSelectionSprite.setPosition((View.getCenter().x-TipSelectionSprite.getGlobalBounds().width/2), (View.getCenter().y-TipSelectionSprite.getGlobalBounds().height/2));
	int i;
	int x = View.getCenter().x-(TileSelectionSprite.getGlobalBounds().width/2)+41;
	int y = View.getCenter().y-(TileSelectionSprite.getGlobalBounds().height/2)+43;
	int XCount=0;
	int YCount=0;

	//create temp sprite for tile images
	sf::Sprite TempTile;
	
	//move the temp tile to thier starting point
	TempTile.move(x,y);
	
	//Move the TileSelectionSprite to the appropriate location
	TileSelectionSprite.setPosition((View.getCenter().x-TileSelectionSprite.getGlobalBounds().width/2), (View.getCenter().y-TileSelectionSprite.getGlobalBounds().height/2));
	
	App.draw(TileSelectionSprite);
	for (i = 0; i < TileImageVector.size(); i++ ){
	    SetSprite(TempTile, TileImageVector[i], 4);
		//TempTile.SetImage(TileImageVector[i]);
		TempTile.setPosition(((66)*XCount+x),((66)*YCount+y));
		XCount++;
		if((i+1)%6 == 0){XCount=0;YCount++;}
		App.draw(TempTile);
	}
}
void SelectTile(int InX, int InY){
	int TileSelected;
	int x = (View.getCenter().x-(TileSelectionSprite.getGlobalBounds().width/2))+41;
	int y = (View.getCenter().y-(TileSelectionSprite.getGlobalBounds().height/2))+43; //67
	cout << "y: " << y << endl;
	//check if user clicked within the tile selection box
	if((InX >= x) && (InX <= 395+x) && (InY >= y) && (InY <= 240+y)){
		InX -= x; InY -= y;
		InX = InX/(66); 
		InY = InY/(66);
		TileSelected = (InY*6)+InX;
		//see if tile exists
		if(TileSelected <= (TileImageVector.size()-1)){
			Control.CurrentTile = TileSelected;
			Control.CurrentSpecial = -1;
			Control.SetState("Main");
			if(DevOut == true){cout << "Tile Selected: " << TileSelected << endl;}
			//sets the mouse so it dont put selected tile on map straight away
			LeftMouseActive = false;
			LeftMouseDown = false;
		}
	}
}
void DrawSpecialSelection(){
	int i;
	int x = View.getCenter().x-(SpecialSelectionSprite.getGlobalBounds().width/2)+41;
	int y = View.getCenter().y-(SpecialSelectionSprite.getGlobalBounds().height/2)+43;
	int XCount=0;
	int YCount=0;
	
	//create temp sprite for drawing the special tiles
	sf::Sprite TempSpecial;
	
	//move the special tiles to thier offset position
	TempSpecial.move(x,y);
	
	//move the specialselectionsprite to the appropriate position
	SpecialSelectionSprite.setPosition((View.getCenter().x-SpecialSelectionSprite.getGlobalBounds().width/2), (View.getCenter().y-SpecialSelectionSprite.getGlobalBounds().height/2));
	
	//draw the special selection sprite	
	App.draw(SpecialSelectionSprite);
	
	for (i = 0; i < 9; i++ ){
	    SetSprite(TempSpecial, SpecialImageArray[i], 4);
		//TempSpecial.SetImage(SpecialImageArray[i]);
		TempSpecial.setPosition(((66)*XCount+x),((66)*YCount+y));
		XCount++;
		if((i+1)%6 == 0){XCount=0;YCount++;}
		App.draw(TempSpecial);
	}
}
void SelectSpecial(int InX, int InY){
	int SpecialSelected;
	int x = View.getCenter().x-(SpecialSelectionSprite.getGlobalBounds().width/2)+41;
	int y = View.getCenter().y-(SpecialSelectionSprite.getGlobalBounds().height/2)+43;
	
	//check if user clicked within the tile selection box
	if((InX >= x) && (InX <= 396+x) && (InY >= y) && (InY <= 264+y)){
		InX -= x; InY -= y;
		InX = InX/(66); 
		InY = InY/(66);
		SpecialSelected = (InY*6)+InX;
		//see if tile exists
		if(SpecialSelected <= (8)){
			Control.CurrentSpecial = SpecialSelected;
			Control.CurrentTile = -1;
			Control.SetState("Main");
			if(DevOut == true){cout << "Special Selected: " << SpecialSelected << endl;}
			//sets the mouse so it dosent put selected special on the map strainght away
			LeftMouseActive = false;
			LeftMouseDown = false;
		}
	}
}

void DrawMenu(){
	//set position of menu
	MenuSprite.setPosition((View.getCenter().x-MenuSprite.getGlobalBounds().width/2), (View.getCenter().y-MenuSprite.getGlobalBounds().height/2));
	
	App.draw(MenuSprite);
}
void DrawOverlay(){
	//sets the tile/special preview
	if(Control.CurrentTile > -1){
		SetSprite(CurrentTileSprite, TileImageVector[Control.CurrentTile], 4);
	}
	else if(Control.CurrentSpecial > -1){
		SetSprite(CurrentTileSprite, SpecialImageArray[Control.CurrentSpecial], 4);		
	}

	//gets the rect of the view
	sf::FloatRect ViewRect((View.getCenter().x-(View.getSize().x/2)), (View.getCenter().y-(View.getSize().y/2)), View.getCenter().x+(View.getSize().x/2), View.getCenter().y+(View.getSize().y/2));

	//moves the sprites to the appropriate position
	TileDisplaySprite.setPosition(ViewRect.width-80, ViewRect.height-80);
	CurrentTileSprite.setPosition(ViewRect.width-80, ViewRect.height-80);
	ZoomInSprite.setPosition(ViewRect.width-80, ViewRect.height-160);
	ZoomOutSprite.setPosition(ViewRect.width-80, ViewRect.height-230);
	StringToolTip.setPosition(ViewRect.left+5, ViewRect.top+2);

	//sets sprite for the tile display image
	SetSprite(TileDisplaySprite, TileDisplayImage, 4);			
	
	//draws the sprites to the screen
	App.draw(TileDisplaySprite);
	App.draw(CurrentTileSprite);
	App.draw(ZoomInSprite);
	App.draw(ZoomOutSprite);
	
	//draw portal text if a portal is being placed
	if(Control.GetState() == "Portal"){
		StringToolTip.setString("Right click on map to place Portal Destination");
		App.draw(StringToolTip);
	}
}
void Zoom(string Zoom){
	int XCount, YCount, i;
//	float CurrentTime = Time.getElapsedTime().asSeconds();
	XCount = YCount = 0;
	if(Control.LastZoom+0.25 < Time.getElapsedTime().asSeconds()){
		Control.LastZoom = Time.getElapsedTime().asSeconds();
		if((Zoom == "In") and (Control.ZoomFactor < 4)){
			Control.ZoomFactor = Control.ZoomFactor*2;
			cout << "Zoom Factor set to: " << Control.ZoomFactor << endl;
			TileWidth = Control.ZoomFactor*16;
			TileHeight = Control.ZoomFactor*16;
		}	
		if((Zoom == "Out") and (Control.ZoomFactor > 1)){
			if(Control.ZoomFactor > 2){Control.ZoomFactor = Control.ZoomFactor/2;}
			else if(Control.ZoomFactor == 2){Control.ZoomFactor = 1;}
			cout << "Zoom Factor set to: " << Control.ZoomFactor << endl;
			TileWidth = Control.ZoomFactor*16;
			TileHeight = Control.ZoomFactor*16;	
		}	
		//resizes and moves all the currently places tile accordingly
		int TotalSize = (MapWidth*MapHeight);
		for (i = 0; i < TotalSize; i++ ) {
			Map[i].Picture.setPosition((XCount*TileWidth), (YCount*TileHeight));
			Map[i].SpecialPicture.setPosition((XCount*TileWidth), (YCount*TileHeight));
			
			//sets the position of the images to fit the new scale factor
			//Map[i].Picture.SetX(XCount*TileWidth);
			
			//scales the images to fit the new zoom factor
			Map[i].Picture.setScale(Control.ZoomFactor, Control.ZoomFactor);	
			Map[i].SpecialPicture.setScale(Control.ZoomFactor, Control.ZoomFactor);
		
			//logic to work out the map size thinggy
			XCount++;	
			if((i+1)%MapWidth == 0){XCount=0;YCount++;}
		}
		//sets the size of the backfround
		BackgroundSprite.setTextureRect(sf::IntRect(0, 0, (MapWidth*TileWidth)/4,(MapHeight*TileHeight)/4));
	}
}
void MainMouseClick(int InX, int InY, string Button = "left"){
	int TileClicked;
	InX += Control.MapOffX;
	InY += Control.MapOffY;
	//Tile Selection
	if(MouseClickSprite(InX, InY, TileDisplaySprite) == true){
		//if(Control.NewMapTile > 0){Control.NewMapTile--;}
		//Control.SetState("Menu");
		cout << "clicked on tile display sprite" << endl;
	}
	//Zoom in pressed
	else if(MouseClickSprite(InX, InY, ZoomInSprite) == true){
		cout << "Zooming in" << endl;
		Zoom("In");
		cout << "clicked zoom in" << endl;
	}
	//Zoom out pressed
	else if(MouseClickSprite(InX, InY, ZoomOutSprite) == true){
		cout << "Zooming out" << endl;
		Zoom("Out");
		cout << "clicked zoom out" << endl;
	}
	//set tile clicked on to the currently selected tile
	else{
		InX = InX/TileWidth; InY = InY/TileHeight;
		TileClicked = (InY*MapWidth)+InX;
		//see if tile exists
		if(TileClicked <= (Map.size()-1)){
			//set the last tile clicked to this tile
			Control.LastTileClicked = TileClicked;
			//if left mouse was pressed place appropritate tile
			if(Button == "Left"){
				if(Control.CurrentTile > -1){
					//set the teml sprite to the selected tile
					SetSprite(Map[TileClicked].Picture, TileImageVector[Control.CurrentTile], Control.ZoomFactor);				
					
					//set the image value for the tile
					Map[TileClicked].IntPicture = Control.CurrentTile;
					
					//place an ! "blocked" by default for new tile (if tile image != 0)
					if(Control.CurrentTile != 0){
						Map[TileClicked].Special = "Blocked";
						SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[2], Control.ZoomFactor);
					}
					
					//tell me stuffs
					if(DevOut == true){cout << "Tile Picture Modified: " << TileClicked << endl;}
				}
				else if(Control.CurrentSpecial > -1){
					if(Map[TileClicked].Special == "Start"){PlayerStartPlaced = false;}	
					if(Control.CurrentSpecial == 0){ 
						if(PlayerStartPlaced == false){
							Map[TileClicked].Special = "Start"; 
							SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[0], Control.ZoomFactor);
							PlayerStartPlaced = true;
						}
					}
					if(Control.CurrentSpecial == 1){Map[TileClicked].Special = "End"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[1], Control.ZoomFactor);}
					if((Control.CurrentSpecial == 2) and (Map[TileClicked].Special != "Blocked")){Map[TileClicked].Special = "Blocked"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[2], Control.ZoomFactor); LeftMouseActive = LeftMouseDown = false;}
					else if((Control.CurrentSpecial == 2) and (Map[TileClicked].Special == "Blocked"))	{Map[TileClicked].Special = "None"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[0], Control.ZoomFactor); LeftMouseActive = LeftMouseDown = false;}
					if(Control.CurrentSpecial == 3){Map[TileClicked].Special = "Portal"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[3], Control.ZoomFactor); Control.SetState("Portal"); Control.CurrentPortal = TileClicked;}
					if(Control.CurrentSpecial == 4){Map[TileClicked].Special = "Tip"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[4], Control.ZoomFactor); Control.SetState("Tip");}
					if(Control.CurrentSpecial == 5){Map[TileClicked].Special = "GravityUp"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[5], Control.ZoomFactor);}
					if(Control.CurrentSpecial == 6){Map[TileClicked].Special = "GravityDown"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[6], Control.ZoomFactor);}
					if(Control.CurrentSpecial == 7){Map[TileClicked].Special = "GravityLeft"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[7], Control.ZoomFactor);}
					if(Control.CurrentSpecial == 8){Map[TileClicked].Special = "GravityRight"; SetSprite(Map[TileClicked].SpecialPicture, SpecialImageArray[8], Control.ZoomFactor);}
					if(DevOut == true){cout << "Tile Special Modified: " << TileClicked << endl;}
				}				
			}
			//if right mouse was pressed clear the tile
			else if(Button == "Right"){
				SetSprite(Map[TileClicked].Picture, TileImageVector[0], 4);							
				//Map[TileClicked].Picture.SetImage(TileImageVector[0]);
				Map[TileClicked].IntPicture = 0;	
				Map[TileClicked].PortalX = 0;	
				Map[TileClicked].PortalY = 0;	
				if(Map[TileClicked].Special == "Start"){PlayerStartPlaced = false;}	
				Map[TileClicked].Special = "None"; 
				SetSprite(Map[TileClicked].SpecialPicture, TileImageVector[0], 4);
			}
		}
	}
}
void DrawMapPreview(int InMapNum){
	//images and sprites for the map preview
	sf::Texture PreviewImage;
	sf::Sprite PreviewSprite;
	
	//sets a string for the loading of the image
	ostringstream PreviewFilename;
	PreviewFilename << "maps/map" << InMapNum << ".png";
	string strPreviewFileName = PreviewFilename.str();
	
	//Makes sure the map and image exist
	if(MapExists(InMapNum)){
		//if the map exists load the image for the map
		PreviewImage.loadFromFile(strPreviewFileName.c_str());
	}
	//else load the new map image
	else {PreviewImage.loadFromFile("gfx/editor/CreateNewMap.png");}
	
	//sets the sprite based on the image
	SetSprite(PreviewSprite, PreviewImage, 1);
	
	//resizes the sprite to 240x240 so it fits properly
	//PreviewSprite.setSize(240,240);
	
	//moves the sprite to the proper location
	PreviewSprite.setPosition(View.getCenter().x-4, View.getCenter().y-120);
	
	//draw the image to the screen
	//App.draw(PreviewSprite);
}
void Save(int InNumber){
	int i, BackWidth, BackHeight;
	int First, Seccond;
	sf::Image MapPreview;
	
	//filename stuff for the map file
	ostringstream Filename;
	Filename << "CustomMaps/map" << InNumber << ".map";
	string strFileName = Filename.str();
	ofstream File(strFileName.c_str());
	
	//filename for the image file
	ostringstream PreviewFilename;
	PreviewFilename << "CustomMaps/map" << InNumber << ".png";
	string strPreviewFileName = PreviewFilename.str();
	
	First = Seccond = 0;
	
	//save all the map data
	File << MapAuthor.c_str() << endl;
	File << MapName.c_str() << endl;
	File << MapWidth << endl;
	File << MapHeight << endl;
	for(i=0; i<Map.size(); i++)	
		//cout << "Saving: Map[" << i << "] | " << "IntPicture: " << Map[i].IntPicture << " | Special: " << Map[i].Special << " | PortalX: " << Map[i].PortalX << " | PortalY: " << Map[i].PortalY << " | TipNum: " << Map[i].TipNumber << endl;
		//put the data into the file
		File << Map[i].IntPicture << endl << Map[i].Special.c_str() << endl << Map[i].PortalX << endl << Map[i].PortalY << endl << Map[i].TipNumber << endl;		
	File.close();
	cout << "map saved as: '" << strFileName.c_str() << "'" << endl;
	
	//save an image of the map for the map preview
	//BackWidth = BackgroundSprite.getGlobalBounds().width;
	//BackHeight = BackgroundSprite.getGlobalBounds().height;
	//BackgroundSprite.Resize(640,480);
	App.clear(sf::Color(32, 26, 47));
	App.draw(BackgroundSprite);
	ShowMap(MapWidth, MapHeight); //Draws the map so that the capture is of the map
	MapPreview = App.capture();
	cout << "Saving map preview image" << endl;
	MapPreview.saveToFile(strPreviewFileName.c_str());
	
	//reset the background
	//BackgroundSprite.Resize(BackWidth, BackHeight);
	
	Control.SetState("Main");
}

void Save(string InName){
	int i, BackWidth, BackHeight;
	int First, Seccond;
	sf::Image MapPreview;
	
	//filename stuff for the map file
	ostringstream Filename;
	Filename << "CustomMaps/" << InName << ".map";
	string strFileName = Filename.str();
	ofstream File(strFileName.c_str());
	
	//filename for the image file
	ostringstream PreviewFilename;
	PreviewFilename << "CustomMaps/" << InName << ".png";
	string strPreviewFileName = PreviewFilename.str();
	
	First = Seccond = 0;
	
	//save all the map data
	File << MapAuthor.c_str() << endl;
	File << MapName.c_str() << endl;
	File << MapWidth << endl;
	File << MapHeight << endl;
	for(i=0; i<Map.size(); i++)	
		//cout << "Saving: Map[" << i << "] | " << "IntPicture: " << Map[i].IntPicture << " | Special: " << Map[i].Special << " | PortalX: " << Map[i].PortalX << " | PortalY: " << Map[i].PortalY << " | TipNum: " << Map[i].TipNumber << endl;
		//put the data into the file
		File << Map[i].IntPicture << endl << Map[i].Special.c_str() << endl << Map[i].PortalX << endl << Map[i].PortalY << endl << Map[i].TipNumber << endl;		
	File.close();
	cout << "map saved as: '" << strFileName.c_str() << "'" << endl;
	
	if(Control.UploadMap == true){
		//upload the map if neccasary
		sf::Ftp ftp;
		int i = 0;	

		// Connect to the server
		sf::Ftp::Response response = ftp.connect("175.107.158.147");
		if (response.isOk())
		    std::cout << "Connected" << std::endl;	

		// Log in
		response = ftp.login("sprint", "S17mypAZ583");
		if (response.isOk())
		    std::cout << "Logged in" << std::endl;

		response = ftp.upload(strFileName.c_str(), "CustomMaps");
		if (response.isOk())
			std::cout << "Map uploaded" << std::endl;	
			
		ftp.disconnect();
	}

	App.clear(sf::Color(32, 26, 47));
	App.draw(BackgroundSprite);
	ShowMap(MapWidth, MapHeight); //Draws the map so that the capture is of the map
	MapPreview = App.capture();
	cout << "Saving map preview image" << endl;
	MapPreview.saveToFile(strPreviewFileName.c_str());
	
	//reset the background
	//BackgroundSprite.Resize(BackWidth, BackHeight);
	
	Control.SetState("Main");
}
void SaveOptions(){
	//move the sprites and text to the approprioate location on screen
	SaveDisplaySprite.setPosition((View.getCenter().x-SaveDisplaySprite.getGlobalBounds().width/2), (View.getCenter().y-SaveDisplaySprite.getGlobalBounds().height/2));
	SaveMapButtonSprite[0].setPosition(View.getCenter().x-235, View.getCenter().y-55); //text enter 1 map name
	SaveMapButtonSprite[1].setPosition(View.getCenter().x-235, View.getCenter().y+15); //text enter 2 author
	SaveMapButtonSprite[2].setPosition(View.getCenter().x-230, View.getCenter().y+92);
	SaveMapButtonSprite[3].setPosition(View.getCenter().x-120, View.getCenter().y+92);
	CrossSprite.setPosition(View.getCenter().x-47, View.getCenter().y+52);
	StringMapName.setPosition(View.getCenter().x-229, View.getCenter().y-60);	
	StringMapAuthor.setPosition(View.getCenter().x-229, View.getCenter().y+8);
	
	//draw the sprites to screen
	App.draw(SaveDisplaySprite);
	App.draw(SaveMapButtonSprite[0]);
	App.draw(SaveMapButtonSprite[1]);
	App.draw(SaveMapButtonSprite[2]);
	App.draw(SaveMapButtonSprite[3]);
	if(Control.UploadMap == true){App.draw(CrossSprite);}

	//update and draw the map name and author strings
	stringstream MapInfo;
    MapInfo << Control.SaveNumber;
    StringMapName.setString(MapName.c_str());
    StringMapAuthor.setString(MapAuthor.c_str());
    App.draw(StringMapName);
    App.draw(StringMapAuthor);

    //draw the map preview
    DrawMapPreview(Control.SaveNumber);
    
    //warn player that they will overwrite the selected map (if not saving as new map)
    if(MapExists(Control.SaveNumber)){
    	StringToolTip.setPosition(View.getCenter().x+2, View.getCenter().y+80);
    	StringToolTip.setString("Will Replace Map!!");
    	App.draw(StringToolTip);
	}
}
void SaveEvent(int InX, int InY){
	//text enter 1 map name
	if(MouseClickSprite(InX, InY, SaveMapButtonSprite[0]) == true){
		Control.SetState("TextEnterMapName");
	}
	//text enter 2 map author
	if(MouseClickSprite(InX, InY, SaveMapButtonSprite[1]) == true){
		Control.SetState("TextEnterMapAuthor");
	}
	//Upload map checkbox
	if(MouseClickSprite(InX, InY, CrossSprite) == true){
		if(Control.UploadMap == true){Control.UploadMap = false;} else {Control.UploadMap = true;}
	}	
	//save button
	if(MouseClickSprite(InX, InY, SaveMapButtonSprite[2]) == true){
		if(DevOut == true){cout << "Save button pressed" << endl;}
		Save(MapName);
		//sets the mouse so it dont put selected tile on map straight away
		LeftMouseActive = false;
		LeftMouseDown = false;
	}
	//cancel button
	if(MouseClickSprite(InX, InY, SaveMapButtonSprite[3]) == true){
		if(DevOut == true){cout << "Cancel button pressed" << endl;}
		Control.SetState("Main");
		//sets the mouse so it dosent put selected special on the map strainght away
		LeftMouseActive = false;
		LeftMouseDown = false;		
	}
}
void Load(int InNumber){
	int BeforeTileData = 0; //used to grab mapwidth and height before processing tiles
	int StringToNum;
	int Switch = 0; //keeps track of which map variable is current
	int i = 0;
	int LineCount = 0;
	int SpecialPic = -1;
	string ReadData;
	
	ostringstream Filename;
	Filename << "CustomMaps/map" << InNumber << ".map";
	string strFileName = Filename.str();
	ifstream MapFile(strFileName.c_str());
	
	//load data from file (INSANITY!!!)
	if (MapFile.is_open()){
		while ( MapFile.good() ){
			LineCount++;
			//cout << "current line of file: " << LineCount << endl;
			getline (MapFile,ReadData);
			BeforeTileData++;
			if(BeforeTileData == 1){ //loads map width
				MapName = ReadData;
			}			
			if(BeforeTileData == 2){ //loads map width
				MapAuthor = ReadData;
			}			
			if(BeforeTileData == 3){ //loads map width
				StringToNum = atoi(ReadData.c_str()); 
				Control.NewMapWidth = StringToNum;
				cout << "loaded map width: " << Control.NewMapWidth << endl;
			}
			else if(BeforeTileData == 4){ //loads map height and sets the new map up via size
				StringToNum = atoi(ReadData.c_str()); 
				Control.NewMapHeight = StringToNum;
				MapWidth = Control.NewMapWidth;
				MapHeight = Control.NewMapHeight;
				NewMap(MapWidth,MapHeight,TileImageVector[0]);
				cout << "loaded map Height: " << Control.NewMapHeight << endl;
			}
			if(i < (MapWidth*MapHeight)){
				if(BeforeTileData > 4){
					//cout << "loading tile: " << i << endl;
					if(Switch == 0){
						StringToNum = atoi(ReadData.c_str()); 
						Map[i].IntPicture = StringToNum;
						SetSprite(Map[i].Picture, TileImageVector[StringToNum], Control.ZoomFactor);						
						//Map[i].Picture.SetImage(TileImageVector[StringToNum]); 
						cout << "Map[" << i << "].IntPicture set to: " << StringToNum << endl;
					} 
					if(Switch == 1){
						Map[i].Special = ReadData; 
						//find out which picture to give it
						if(Map[i].Special == "Start"){SpecialPic = 0; PlayerStartPlaced = true;}
						if(Map[i].Special == "End"){SpecialPic = 1;}
						if(Map[i].Special == "Blocked"){SpecialPic = 2;}
						if(Map[i].Special == "Portal"){SpecialPic = 3;}	
						if(Map[i].Special == "Tip"){SpecialPic = 4;}			
						if(Map[i].Special == "GravityUp"){SpecialPic = 5;}	
						if(Map[i].Special == "GravityDown"){SpecialPic = 6;}	
						if(Map[i].Special == "GravityLeft"){SpecialPic = 7;}	
						if(Map[i].Special == "GravityRight"){SpecialPic = 8;}	
						if(SpecialPic != -1){SetSprite(Map[i].SpecialPicture, SpecialImageArray[SpecialPic], Control.ZoomFactor);}
						cout << "Map[" << i << "].Special set to: " << ReadData << endl;
					}
					if(Switch == 2){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalX = StringToNum; 
						cout << "Portal X set to: " << Map[i].PortalX << endl;
					}
					if(Switch == 3){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalY = StringToNum;
						cout << "Portal Y set to: " << Map[i].PortalY << endl;				 				
					}
					if(Switch == 4){
						StringToNum = atoi(ReadData.c_str());
						Map[i].TipNumber = StringToNum;
						cout << "TipNumber set to: " << Map[i].TipNumber << endl;				 				
						i++; 
					}				
					Switch++; if(Switch == 5){Switch = 0;}
				}
				//cout << ReadData << endl;
			}
		}
		cout << "attempted to load map.........." << endl;
		MapFile.close();
		Control.SetState("Main");
	}
	//load data into the tile class
}
void Load(string InFilename){
	int BeforeTileData = 0; //used to grab mapwidth and height before processing tiles
	int StringToNum;
	int Switch = 0; //keeps track of which map variable is current
	int i = 0;
	int LineCount = 0;
	int SpecialPic = -1;
	string ReadData;
	
	//ostringstream Filename;
	//Filename << "CustomMaps/" << InFilename << ".map";
	//string strFileName = Filename.str();
	ifstream MapFile(InFilename.c_str());
	cout << "loading string map" << endl;
	//load data from file (INSANITY!!!)
	if (MapFile.is_open()){
		while ( MapFile.good() ){
			LineCount++;
			//cout << "current line of file: " << LineCount << endl;
			getline (MapFile,ReadData);
			BeforeTileData++;
			if(BeforeTileData == 1){ //loads map width
				MapName = ReadData;
			}			
			if(BeforeTileData == 2){ //loads map width
				MapAuthor = ReadData;
			}			
			if(BeforeTileData == 3){ //loads map width
				StringToNum = atoi(ReadData.c_str()); 
				Control.NewMapWidth = StringToNum;
				cout << "loaded map width: " << Control.NewMapWidth << endl;
			}
			else if(BeforeTileData == 4){ //loads map height and sets the new map up via size
				StringToNum = atoi(ReadData.c_str()); 
				Control.NewMapHeight = StringToNum;
				MapWidth = Control.NewMapWidth;
				MapHeight = Control.NewMapHeight;
				NewMap(MapWidth,MapHeight,TileImageVector[0]);
				cout << "loaded map Height: " << Control.NewMapHeight << endl;
			}
			if(i < (MapWidth*MapHeight)){
				if(BeforeTileData > 4){
					//cout << "loading tile: " << i << endl;
					if(Switch == 0){
						StringToNum = atoi(ReadData.c_str()); 
						Map[i].IntPicture = StringToNum;
						SetSprite(Map[i].Picture, TileImageVector[StringToNum], Control.ZoomFactor);						
						//Map[i].Picture.SetImage(TileImageVector[StringToNum]); 
						cout << "Map[" << i << "].IntPicture set to: " << StringToNum << endl;
					} 
					if(Switch == 1){
						Map[i].Special = ReadData; 
						//find out which picture to give it
						if(Map[i].Special == "Start"){SpecialPic = 0; PlayerStartPlaced = true;}
						if(Map[i].Special == "End"){SpecialPic = 1;}
						if(Map[i].Special == "Blocked"){SpecialPic = 2;}
						if(Map[i].Special == "Portal"){SpecialPic = 3;}	
						if(Map[i].Special == "Tip"){SpecialPic = 4;}			
						if(Map[i].Special == "GravityUp"){SpecialPic = 5;}	
						if(Map[i].Special == "GravityDown"){SpecialPic = 6;}	
						if(Map[i].Special == "GravityLeft"){SpecialPic = 7;}	
						if(Map[i].Special == "GravityRight"){SpecialPic = 8;}	
						if(SpecialPic != -1){SetSprite(Map[i].SpecialPicture, SpecialImageArray[SpecialPic], Control.ZoomFactor);}
						cout << "Map[" << i << "].Special set to: " << ReadData << endl;
					}
					if(Switch == 2){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalX = StringToNum; 
						cout << "Portal X set to: " << Map[i].PortalX << endl;
					}
					if(Switch == 3){
						StringToNum = atoi(ReadData.c_str());
						Map[i].PortalY = StringToNum;
						cout << "Portal Y set to: " << Map[i].PortalY << endl;				 				
					}
					if(Switch == 4){
						StringToNum = atoi(ReadData.c_str());
						Map[i].TipNumber = StringToNum;
						cout << "TipNumber set to: " << Map[i].TipNumber << endl;				 				
						i++; 
					}				
					Switch++; if(Switch == 5){Switch = 0;}
				}
				//cout << ReadData << endl;
			}
		}
		cout << "attempted to load map.........." << endl;
		MapFile.close();
		Control.SetState("Main");
	}
	//load data into the tile class
}
void LoadOptions(){
	//keeps track of which maps to draw
	int i = Control.CustomMapSelection; 
	
	//move the menu sprite to the correct location
	LoadDisplaySprite.setPosition((View.getCenter().x-LoadDisplaySprite.getGlobalBounds().width/2), (View.getCenter().y-LoadDisplaySprite.getGlobalBounds().height/2));

	//move the arrow to the appropriate location
	MapArrowSprite.setPosition((View.getCenter().x-300), ((View.getCenter().y-170)));	

	//draw the MenuSprite and menu select sprite
	App.draw(LoadDisplaySprite);
	App.draw(MapArrowSprite);
	
	//postions and render all the text
	for(i=Control.CustomMapSelection;i<Control.CustomMapSelection+7;i++){
		if(i < CustomMapInfo.size()){
			//draws the map filename
			//CustomMapInfo[i].MapFilename.setPosition((View.getCenter().x-190), ((View.getCenter().y-130)+(98*(i-Control.CustomMapSelection))));
			//App.draw(CustomMapInfo[i].MapFilename);
			
			//draws the map name
			CustomMapInfo[i].MapName.setPosition((View.getCenter().x-240), ((View.getCenter().y-187)+(52*(i-Control.CustomMapSelection))));
			App.draw(CustomMapInfo[i].MapName);

			//draws the map author
			CustomMapInfo[i].MapAuthor.setPosition((View.getCenter().x+40), ((View.getCenter().y-187)+(52*(i-Control.CustomMapSelection))));
			App.draw(CustomMapInfo[i].MapAuthor);
		}
	}
}
void LoadEvent(string InKey){
	if(InKey == "Z"){Control.SetState("Main");}
	if(InKey == "Up"){if(Control.CustomMapSelection > 0){Control.CustomMapSelection--;}}
	if(InKey == "Down"){if(Control.CustomMapSelection < CustomMapInfo.size()-1){Control.CustomMapSelection++;}}
	if(InKey == "Return"){
		if(CustomMapInfo.size() > 0){
			sf::String InternalTempString = CustomMapInfo[Control.CustomMapSelection].MapFilename.getString();
			Load(InternalTempString);
		}
	}

}
void DrawTipPreview(int InTipNum){
	//images and sprites for the map preview
	sf::Texture PreviewImage;
	sf::Sprite PreviewSprite;
	
	//sets a string for the loading of the image
	ostringstream PreviewFilename;
	PreviewFilename << "gfx/tips/Tip" << InTipNum << ".png";
	string strPreviewFileName = PreviewFilename.str();

	//Load the image
	PreviewImage.loadFromFile(strPreviewFileName.c_str());
			
	//sets the sprite based on the image
	SetSprite(PreviewSprite, PreviewImage, 1);
	
	//moves the sprite to the proper location
	PreviewSprite.setPosition(View.getCenter().x-2, View.getCenter().y-117);
	
	//draw the image to the screen
	App.draw(PreviewSprite);
}
void TipSelectionEvent(int InX, int InY){
	//TipUpArrowSprite.setPosition(View.getCenter().x-107, View.getCenter().y-41);
	//TipDownArrowSprite.setPosition(View.getCenter().x-107, View.getCenter().y+31);	
	//TipSetSprite.setPosition(View.getCenter().x-232, View.getCenter().y+92);
	//up arrow
	if(MouseClickSprite(InX, InY, TipUpArrowSprite) == true){
		if(Control.TipNumber < 3){Control.TipNumber++;}
		cout << "Increased tip number" << endl;
	}
	//down arrow
	if(MouseClickSprite(InX, InY, TipDownArrowSprite) == true){
		if(Control.TipNumber > 0){Control.TipNumber--;}
		cout << "decresed tip number" << endl;
	}
	//Set/Confirm button
	if(MouseClickSprite(InX, InY, TipSetSprite) == true){
		//sets the tip variable for the tile
		Map[Control.LastTileClicked].TipNumber = Control.TipNumber;
		
		//sets the mouse to inactive so it dont auto place another tip tile
		LeftMouseActive = false;
		LeftMouseDown = false;
		
		//set state back to main
		Control.SetState("Main");
	}
}
void DrawTipSelection(){
	//set position of options, text, set button, and arrow sprites
	TipSelectionSprite.setPosition((View.getCenter().x-TipSelectionSprite.getGlobalBounds().width/2), (View.getCenter().y-TipSelectionSprite.getGlobalBounds().height/2));
	StringMapNum.setPosition(View.getCenter().x-100, View.getCenter().y-15);
	TipUpArrowSprite.setPosition(View.getCenter().x-107, View.getCenter().y-41);
	TipDownArrowSprite.setPosition(View.getCenter().x-107, View.getCenter().y+31);	
	TipSetSprite.setPosition(View.getCenter().x-232, View.getCenter().y+92);	
	
	//draw the sprites to "App"
	App.draw(TipSelectionSprite);
	App.draw(TipUpArrowSprite);
	App.draw(TipDownArrowSprite);
	App.draw(TipSetSprite);
		
	//draw the current map number text
	stringstream TipNum;
    TipNum << Control.TipNumber;
    StringMapNum.setString(TipNum.str());
    App.draw(StringMapNum);
    
    //draw the tip preview image
    DrawTipPreview(Control.TipNumber);
}
void PlacePortalDestination(int InX, int InY){
	InX += Control.MapOffX;
	InY += Control.MapOffY;

	InX = InX/TileWidth;
	InY = InY/TileHeight;

	if(Control.ZoomFactor == 4){
		Map[Control.CurrentPortal].PortalX = (InX*TileWidth)+(TileWidth/2);
		Map[Control.CurrentPortal].PortalY = (InY*TileHeight)+(TileHeight/2);
	}
	else if(Control.ZoomFactor == 2){
		Map[Control.CurrentPortal].PortalX = (InX*(TileWidth*2))+((TileWidth*2)/2);
		Map[Control.CurrentPortal].PortalY = (InY*(TileHeight*2))+((TileHeight*2)/2);
	}
	
	Control.SetState("Main");
}
void HandleMousePress(int InX, int InY, string Button = "Left"){
   	cout << Button << " Click At - X: " << InX << " | Y: " << InY << endl;
  	if(Control.GetState() == "NewMap"){NewMapEvent(InX+Control.MapOffX,InY+Control.MapOffY);}
   	else if(Control.GetState() == "Main"){MainMouseClick(InX,InY, Button);} 	
   	else if(Control.GetState() == "TileSelection"){SelectTile(InX+Control.MapOffX,InY+Control.MapOffY);}
   	else if(Control.GetState() == "SpecialSelection"){SelectSpecial(InX+Control.MapOffX,InY+Control.MapOffY);}
   	else if((Control.GetState() == "Save") or (Control.GetState() == "TextEnterMapName") or (Control.GetState() == "TextEnterMapAuthor")){SaveEvent(InX+Control.MapOffX,InY+Control.MapOffY);}
   	//else if(Control.GetState() == "Load"){LoadEvent();}
   	//else if(Control.GetState() == "Menu"){MenuClick(InX,InY);}
   	else if(Control.GetState() == "Tip"){TipSelectionEvent(InX+Control.MapOffX, InY+Control.MapOffY);}
   	else if((Control.GetState() == "Portal") and (Button == "Right")){PlacePortalDestination(InX, InY);}
}

void HandleKey(string InKey){
	if(Control.GetState() == "Load"){LoadEvent(InKey);}
	else{
	    if(InKey == "Return"){
			if(Control.GetState() == "Main"){Control.SetState("Save");}              
		}
   		if(InKey == "Z"){				
			if(Control.GetState() == "Main"){Control.SetState("NewMap");}
		}
	}
}

void MousePressed(int InX, int InY, string Button){
	if(Button == "Left"){
		HandleMousePress(InX, InY, Button);
		if(LeftMouseDown == true){LeftMouseActive = true;}
		LeftMouseDown = true; 	
	}
	if(Button == "Right"){
		HandleMousePress(InX, InY, Button);
		if(RightMouseDown == true){RightMouseActive = true;}
		RightMouseDown = true; 	
	}
}
void MouseReleased(string Button){
	if(Button == "Left"){
		if(LeftMouseActive = false){LeftMouseDown = false;}
		if(LeftMouseDown = true){LeftMouseActive = false;}
		LeftMouseActive = false;
	}
	if(Button == "Right"){
		if(RightMouseActive = false){RightMouseDown = false;}
		if(RightMouseDown = true){RightMouseActive = false;}
		RightMouseActive = false;
	}
}
void HandleWindowResize(){
	//sets the view
	View.setSize((App.getSize().x), (App.getSize().y));
	View.setCenter((App.getSize().x/2), (App.getSize().y/2));
	
	//resets the offset
	Control.MapOffX = 0;
	Control.MapOffY = 0;
}
int main(){
	//Setup Sprites and images
	SetupImages();
	
    //set up the fonts and stuff
    SetupFonts();
	
	//fix bug on windows maybe?
	HandleWindowResize();
	
	//Creates the initial/new map
	MapHeight = 10; MapWidth = 10;	
	NewMap(MapWidth,MapHeight,TileImageVector[0]);

    //sets a framerate limit
    App.setFramerateLimit(60);
    
    // Start game loop
    while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
        	// handle resizing of window
            if (Event.type == sf::Event::Resized)
                HandleWindowResize();
        	
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();
			
            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();
                              
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::C))
                if(Control.GetState() == "Main"){LoadCustomMaps(); Control.SetState("Load");}                           

			//player pressed up key
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up))
				HandleKey("Up");	
				
			//player pressed up key
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Z))
				HandleKey("Z");				
				
			//player pressed down key
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down))
				HandleKey("Down");	
				
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::X))
                HandleKey("Return");
            
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::R))
                if(Control.GetState() == "Main"){Control.SetState("SpecialSelection");} 

            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::E))
                if(Control.GetState() == "Main"){Control.SetState("TileSelection");} 
               
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Q))
                if(Control.GetState() == "Main"){Control.SetState("Menu");} else if(Control.GetState() == "Menu") {Control.SetState("Main");}              
            
            //puts text into the map name
            if(( Event.type == sf::Event::TextEntered ) and (Event.text.unicode != 8) and (Event.text.unicode < 126) and (Event.text.unicode > 47) or (Event.text.unicode == 32)){
            	if((Event.text.unicode != 58) and (Event.text.unicode != 59)){
            		if(Control.GetState() == "TextEnterMapName"){if(MapName.size() < 20){MapName += static_cast<char>(Event.text.unicode);}}
            		if(Control.GetState() == "TextEnterMapAuthor"){if(MapAuthor.size() < 20){MapAuthor += static_cast<char>(Event.text.unicode);}}
            	}
            }
            
            //erases text
            if(( Event.type == sf::Event::TextEntered ) and (Event.text.unicode == 8)){        
            	if(Control.GetState() == "TextEnterMapName"){if(MapName.size() > 0){MapName.erase( MapName.size( ) - 1 );}}
            	if(Control.GetState() == "TextEnterMapAuthor"){if(MapAuthor.size() > 0){MapAuthor.erase( MapAuthor.size( ) - 1 );}}
            }
            
            //detect mouse button pressed
            if(Event.type == sf::Event::MouseButtonPressed){
	            if(Event.mouseButton.button == sf::Mouse::Left){
    	        	MousePressed(Event.mouseButton.x, Event.mouseButton.y, "Left");
    	        }
    	        if(Event.mouseButton.button == sf::Mouse::Right){ 	
	    	        MousePressed(Event.mouseButton.x, Event.mouseButton.y, "Right");
	    	    }
            }  
            //detect mouse button released
            if(Event.type == sf::Event::MouseButtonReleased){
	            if(Event.mouseButton.button == sf::Mouse::Left){
    	        	MouseReleased("Left");
    	        }
	            if(Event.mouseButton.button == sf::Mouse::Right){
    	        	MouseReleased("Right");
    	        }    	        
            }
        }
        //if left mouse if down act accordingly
        if((LeftMouseDown == true) and (LeftMouseActive == true)){      	
			if(Control.GetState() == "Main"){HandleMousePress(sf::Mouse::getPosition(App).x, sf::Mouse::getPosition(App).y, "Left");}
		}
        //if right mouse if down act accordingly
        if((RightMouseDown == true) and (RightMouseActive == true)){      	
			if(Control.GetState() == "Main"){HandleMousePress(sf::Mouse::getPosition(App).x, sf::Mouse::getPosition(App).y, "Right");}
		}			
		
		if((Control.GetState() == "Main") or (Control.GetState() == "Portal")){
        	// Move the view using arrow keys
        	float Offset = 400.f * (FrameClock.restart().asSeconds());
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){ View.move(-Offset, 0); Control.MapOffX-=Offset;}
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){ View.move(Offset, 0); Control.MapOffX+=Offset;}
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ View.move(0, -Offset); Control.MapOffY-=Offset;}
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ View.move(0, Offset); Control.MapOffY+=Offset;}
        	
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ View.move(-Offset, 0); Control.MapOffX-=Offset;}
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ View.move(Offset, 0); Control.MapOffX+=Offset;}
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ View.move(0, -Offset); Control.MapOffY-=Offset;}
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ View.move(0, Offset); Control.MapOffY+=Offset;}        	
		}
		
        // Set the view
        App.setView(View);

        // Clear screen
        App.clear();

        //draw static overlay
        App.draw(BackgroundSprite);
       
        //draw the map
        ShowMap(MapWidth,MapHeight);
        
        // Reset to the default view to draw the interface
        //App.setView(App.getDefaultView());
        
        //draw static overlay
        DrawOverlay();
        
        //show the new map dialogue if nessasary
		if(Control.GetState() == "NewMap"){NewMapOptions();}
		else if(Control.GetState() == "TileSelection"){DrawTileSelection();}
		else if(Control.GetState() == "SpecialSelection"){DrawSpecialSelection();}
		else if(Control.GetState() == "Menu"){DrawMenu();}
		else if((Control.GetState() == "Save") or (Control.GetState() == "TextEnterMapName") or (Control.GetState() == "TextEnterMapAuthor")){SaveOptions();}
		else if(Control.GetState() == "Load"){LoadOptions();}
		else if(Control.GetState() == "Tip"){DrawTipSelection();}

        // Finally, display rendered frame on screen
        App.display();
    }
	if(DevOut == true){cout << "exiting application" << endl;}
    return EXIT_SUCCESS;
}	
