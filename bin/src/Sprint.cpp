/*------------------------------------------------------------------------------
FILE                  Sprint.cpp
PROGRAM               main file for Sprint game
PROGRAMMER            Nathan Kewley         
ENVIROMENT            Linux Mint 12 - g++ with SFML
------------------------------------------------------------------------------*/

//include for C/C++
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
//include needed SFML files
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

//my includes
#include "TileClass.h"
#include "SprintPlayerClass.h"
#include "SprintControl.h"
#include "SystemFunctions.h"
#include "MapDisplayClass.h"
#include "Sprint.h"

void DrawFPS(){
	//draw the fps to the screen
	stringstream CurrentFPS;
    CurrentFPS << "FPS: " << FPS;
    FPSString.setString(CurrentFPS.str());
	App.draw(FPSString);
}
void ShowMap(int InWidth, int InHeight){
	int XCount = 0;
	int YCount = 0;
	int i=0;
	int TotalSize = (InWidth*InHeight);
	
	//create a sprite from the MapTexture
	//sf::Sprite TempMapSprite(MapTexture.getTexture());
	
	//draw the map
	//App.draw(TempMapSprite); 
	
	for (i = 0; i < TotalSize; i++ ) {
		//draw the map
		App.draw(Map[i].Picture);
		if(Map[i].Special != "None"){
			if(Map[i].Special != "Blocked"){
				App.draw(Map[i].SpecialPicture);
			}
		}
	}
	
	//draw dev overlay if nessasary
	if(TileNum == true){
		for (i = 0; i < TotalSize; i++ ) {
			//put tile numbers on the screen
			stringstream Num;
		    Num << i;
		    
			MapIndexString.setString(Num.str());
			MapIndexString.setPosition((TileWidth*XCount),(TileHeight*YCount));
			XCount++;
			if((i+1)%Control.MapWidth == 0){XCount=0;YCount++;}
			App.draw(MapIndexString);
			
			//put the blocked symbold onto the map
			if(Map[i].Special == "Blocked"){App.draw(Map[i].SpecialPicture);}
		}
	}
}
void DrawTip(int InMapPos){
	sf::Sprite TempSprite;
	
	//sets the temp sprite to appropriate image
	SetSprite(TempSprite, TipImageArray[Map[InMapPos].TipNumber], 1);
	
	//sets the position to just above the player/tip
	TempSprite.setPosition(Map[InMapPos].Picture.getPosition().x-100,Map[InMapPos].Picture.getPosition().y-125);
	
	App.draw(TempSprite);
}
void SetGravity(int InGravity, string Dir){
	if((Dir == "Up") or (Dir == "Down")){
		GravityY = InGravity; 
		GravityX = 0;
		
		//set the player rotation to 0
		Player.PlayerSprite.setRotation(0);
		
		//flip the player to look proper
		if(Dir == "Down"){Player.PlayerSprite.setScale(Player.PlayerSprite.getScale().x,2.f);} else {Player.PlayerSprite.setScale(Player.PlayerSprite.getScale().x,-2.f);}
	}
	if((Dir == "Left") or (Dir == "Right")){
		GravityX = InGravity;
		GravityY = 0;
		
		//set player flip to false
		Player.PlayerSprite.setScale(Player.PlayerSprite.getScale().x,-2.f);
		
		//rotate the player so it looks proper
		if(Dir == "Left"){Player.PlayerSprite.setRotation(270);} else {Player.PlayerSprite.setRotation(90);}
	}
	Player.SetJump(0);
}
void MoveBack(int X, int Y){
	if(X > 0){X = -Control.BackScrollSpeed;}
	else if(X < 0){X = Control.BackScrollSpeed;}
	if(Y > 0){Y = -Control.BackScrollSpeed;}
	else if(Y < 0){Y = Control.BackScrollSpeed;}	
	BackgroundSprite.move(X,Y);
	MidSprite.move(X*2,Y*2);
}
void UpdateView(){
	//gets the old view center postions
	int OldX = View.getCenter().x;
	int OldY = View.getCenter().y;
	
	//sets the view
	View.setCenter(Player.PlayerSprite.getPosition().x,Player.PlayerSprite.getPosition().y);
	
	//gets the new view center postions
	int NewX = View.getCenter().x;
	int NewY = View.getCenter().y;
	
	//sets the map offset based on the movement of the view
	Control.MapOffX -= OldX-NewX;
	Control.MapOffY -= OldY-NewY;
}
void UpdatePlayer(){
	sf::CircleShape Circle(2,30);
	Circle.setFillColor(sf::Color::Yellow);	
	bool PlayerInMap = true;
	int FinalNewX, FinalNewY, FinalMapPos, FinalMapPos2, XOff;
	bool DelX, DelY;
	XOff = 0;
	
	//update the player gravity effect
	Player.UpdateGravity(GravityX,GravityY);
	
	//check if the player is in the map ---------------------------------
	if(Player.PlayerSprite.getPosition().x < 0){PlayerInMap = false;}
	if(Player.PlayerSprite.getPosition().x+Player.PlayerWidth > (Control.MapWidth*TileWidth)){PlayerInMap = false;}
	if(Player.PlayerSprite.getPosition().y < 0){PlayerInMap = false;}
	if(Player.PlayerSprite.getPosition().y+Player.PlayerHeight > (Control.MapHeight*TileWidth)){PlayerInMap = false;}
	
	//if the player is in the map check if the new position of the player is blocked or otherwise impassable
	if(PlayerInMap == true){	
	
		//check if the player is colliding with any "Special" tiles
		FinalNewX = Player.PlayerSprite.getPosition().x;
		FinalNewY = Player.PlayerSprite.getPosition().y;
		FinalMapPos = (((FinalNewY/TileHeight)*Control.MapWidth)+FinalNewX/TileWidth);
		
		//if the player is on a special tile
		if(Map[FinalMapPos].Special != "None"){
			if(Map[FinalMapPos].Special == "GravityUp"){SetGravity(-GRAVITY, "Up");}
			if(Map[FinalMapPos].Special == "GravityDown"){SetGravity(GRAVITY, "Down");}
			if(Map[FinalMapPos].Special == "GravityLeft"){SetGravity(-GRAVITY, "Left");}
			if(Map[FinalMapPos].Special == "GravityRight"){SetGravity(GRAVITY, "Right");}
			if(Map[FinalMapPos].Special == "End"){Control.SetState("LevelComplete");}
			if(Map[FinalMapPos].Special == "Portal"){
				cout << "Player has been teleported" << endl;
				Player.SetPos((Map[FinalMapPos].PortalX/2)-(TileWidth/2)+7.5, ((Map[FinalMapPos].PortalY/2)-(TileHeight/2)+6.5));
				Player.SetJump(0);
			}
			if(Map[FinalMapPos].Special == "Tip"){
				DrawTip(FinalMapPos);
			}	
		}
		
		//adds the player newX to the players current position to get a final destination
		FinalNewX += Player.NewX;
		FinalNewY += Player.NewY;
		
		//handle the collision on the X axis ----------------------------
		FinalNewY -= Player.NewY;
		
		//set offset based on gravity and direction
		if((GravityY != 0) and (GravityX == 0)){
			if(Player.NewX > 0){FinalNewX+=8; XOff+=8;} else if(Player.NewX < 0){FinalNewX -= 11.5; XOff-=11.5;}
		}
		else {
			if(Player.NewX > 0){FinalNewX+=12; XOff+=12;} else if(Player.NewX < 0){FinalNewX -= 14.5; XOff-=14.5;}
		}
	
		//put in the y offset
		FinalNewY -= 8.5; 
		FinalMapPos = (((FinalNewY/TileHeight)*Control.MapWidth)+FinalNewX/TileWidth);
		if(CollisionDev == true){Circle.setPosition(FinalNewX, FinalNewY); App.draw(Circle);}
			
		FinalNewY += 16; 
		FinalMapPos2 = (((FinalNewY/TileHeight)*Control.MapWidth)+FinalNewX/TileWidth);
		if(CollisionDev == true){Circle.setPosition(FinalNewX, FinalNewY); App.draw(Circle);}
		
		//check if the player can move to the desired location
		if((Map[FinalMapPos].Special == "Blocked") or (Map[FinalMapPos2].Special == "Blocked")){
			Player.NewX = 0;
			if(GravityX != 0){Player.PlayerCanJump = true;} else {Player.PlayerCanJump = false;}
		}
		
		//put FinalNewX back based on XOff
		FinalNewX -= XOff;
		FinalNewY -= 6.5;
		
		//handle collision on Y axis ----------------------------------
		FinalNewY += Player.NewY;
		FinalNewX -= Player.NewX;
		
		//set offset based on gravity and directoion
		if((GravityY != 0) and (GravityX == 0)){
			if(Player.NewY > 0){FinalNewY+=12;}	else if(Player.NewY < 0){FinalNewY -= 12;}		
		}
		else {
			if(Player.NewY > 0){FinalNewY+=8;} else if(Player.NewY < 0){FinalNewY -= 12;}
		}
		
		//put the x offset
		FinalNewX -= 8.5; 
		FinalMapPos = (((FinalNewY/TileHeight)*Control.MapWidth)+FinalNewX/TileWidth);
		if(CollisionDev == true){Circle.setPosition(FinalNewX, FinalNewY); App.draw(Circle);}
			
		FinalNewX += 16; 
		FinalMapPos2 = (((FinalNewY/TileHeight)*Control.MapWidth)+FinalNewX/TileWidth);
		if(CollisionDev == true){Circle.setPosition(FinalNewX, FinalNewY); App.draw(Circle);}
		
		//check if the player can move to the desired location
		if((Map[FinalMapPos].Special == "Blocked") or (Map[FinalMapPos2].Special == "Blocked")){
			Player.NewY = 0;
			if(GravityY != 0){Player.PlayerCanJump = true;} else {Player.PlayerCanJump = false;}
		}
	}	
	
	//if the player far enough outside of map reset the level
	if(PlayerInMap == false){
		if(Control.CustomMap == false){
			if(Player.PlayerSprite.getPosition().x < -200){Load(Control.CurrentMap);}
			if(Player.PlayerSprite.getPosition().x+Player.PlayerWidth > (Control.MapWidth*TileWidth+200)){Load(Control.CurrentMap);}
			if(Player.PlayerSprite.getPosition().y < -200){Load(Control.CurrentMap);}
			if(Player.PlayerSprite.getPosition().y+Player.PlayerHeight > (Control.MapHeight*TileWidth+200)){Load(Control.CurrentMap);}
		}
		else{
			sf::String InternalTempString = CustomMapInfo[Control.CustomMapSelection].MapFilename.getString();
			if(Player.PlayerSprite.getPosition().x < -200){Load(InternalTempString);}
			if(Player.PlayerSprite.getPosition().x+Player.PlayerWidth > (Control.MapWidth*TileWidth+200)){Load(InternalTempString);}
			if(Player.PlayerSprite.getPosition().y < -200){Load(InternalTempString);}
			if(Player.PlayerSprite.getPosition().y+Player.PlayerHeight > (Control.MapHeight*TileWidth+200)){Load(InternalTempString);}
		}		
	}
	
	//set the player animation to 'still' if player is not moving
	if(Player.NewX + Player.NewY == 0){Player.PlayerSprite.setTextureRect(sf::IntRect(0, 0, Player.PlayerImageWidth, Player.PlayerImageHeight));}
	
	//move the player
	Player.PlayerMove(Player.NewX, Player.NewY);
	
	//Move the background
	MoveBack(Player.NewX, Player.NewY);
	
	//set the player new x and y to 0
	Player.NewX = Player.NewY = 0;
	
	//update the view to center on the player
	UpdateView();
	
	//draw player to screen
	App.draw(Player.PlayerSprite);
}
void MenuEvent(){
	//Start
	if(Control.MenuSelection == 0){
		if(DevOut == true){cout << "Start Game Pressed" << endl;}
		Control.CustomMap = false;
		Load(Control.CurrentMap);
		//Control.MenuSelection = 0;
	}
	//custom maps
	if(Control.MenuSelection == 1){
		if(DevOut == true){cout << "You wants to playt custom map :D" << endl;}
		LoadCustomMaps();
		Control.SetState("MapSelection");
		//Control.MenuSelection = 0;
	}
	//Download maps
	if(Control.MenuSelection == 2){
		if(DevOut == true){cout << "Download more Mappy's's's" << endl;}
		//draw the working image to screen
		WorkingSprite.setPosition((View.getCenter().x-WorkingSprite.getGlobalBounds().width/2), (View.getCenter().y-WorkingSprite.getGlobalBounds().height/2));
		App.draw(WorkingSprite);
		App.display();
		
		//load the custom map lists		
		QueueMapList();
		Control.SetState("DownloadMaps");
		
		//Control.MenuSelection = 0;
	}
	//Options
	if(Control.MenuSelection == 3){
		if(DevOut == true){cout << "Options Pressed" << endl;}
		Control.SetState("Options");
		//Control.MenuSelection = 0;
	}
	//Credits
	if(Control.MenuSelection == 4){
		if(DevOut == true){cout << "Credits Pressed" << endl;}
		Control.SetState("Credits");
		//Control.MenuSelection = 0;
	}
	//Exit
	if(Control.MenuSelection == 5){
		if(DevOut == true){cout << "Exit Pressed" << endl;}
		App.close();
	}
}
void RunMenu(){
	//move the menu sprite to the correct location
	MenuSprite.setPosition((View.getCenter().x-MenuSprite.getGlobalBounds().width/2), (View.getCenter().y-MenuSprite.getGlobalBounds().height/2));
	
	//calculate to appropriate location of the menuselectsprite and place it in that location
	MenuSelectSprite.setPosition((View.getCenter().x+105), ((View.getCenter().y-57)+(27*Control.MenuSelection)));	
	
	//draw the MenuSprite and menu select sprite
	App.draw(MenuSprite);
	App.draw(MenuSelectSprite);
}
void DrawMapSelection(){
	//keeps track of which maps to draw
	int i = Control.CustomMapSelection; 
	
	//move the menu sprite to the correct location
	MapSelectBackSprite.setPosition((View.getCenter().x-MenuSprite.getGlobalBounds().width/2), (View.getCenter().y-MenuSprite.getGlobalBounds().height/2));

	//move the arrow to the appropriate location
	MapArrowSprite.setPosition((View.getCenter().x-300), ((View.getCenter().y-170)));	

	//draw the MenuSprite and menu select sprite
	App.draw(MapSelectBackSprite);
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
void MapSelectionEvent(string InKey){
	if(InKey == "Z"){Control.SetState("Menu"); Control.CustomMapSelection = 0;}
	if(InKey == "Up"){if(Control.CustomMapSelection > 0){Control.CustomMapSelection--;}}
	if(InKey == "Down"){if(Control.CustomMapSelection < CustomMapInfo.size()-1){Control.CustomMapSelection++;}}
	if(InKey == "Return"){
		if(CustomMapInfo.size() > 0){
			Control.CustomMap = true;
			sf::String InternalTempString = CustomMapInfo[Control.CustomMapSelection].MapFilename.getString();
			Load(InternalTempString);
		}
	}

}
void MapDownloadEvent(string InKey){
	if(InKey == "Z"){Control.SetState("Menu"); Control.CustomMapSelection = 0;}
	if(InKey == "Up"){if(Control.CustomMapSelection > 0){Control.CustomMapSelection--;}}
	if(InKey == "Down"){if(Control.CustomMapSelection < CustomMapInfo.size()-1){Control.CustomMapSelection++;}}
	if(InKey == "Return"){
		//draw the working/loading image to screen
		WorkingSprite2.setPosition((View.getCenter().x-WorkingSprite2.getGlobalBounds().width/2), (View.getCenter().y-WorkingSprite2.getGlobalBounds().height/2));
		App.draw(WorkingSprite2);
		App.display();
	
		//download the map
		sf::String InternalTempString = CustomMapInfo[Control.CustomMapSelection].MapFilename.getString();
		DownloadMap(InternalTempString);
	}

}
void LoadNextLevel(){
	Control.CurrentMap++; 
	Load(Control.CurrentMap);
}
void LevelCompleteEvent(int InX, int InY){
	if(MouseClickSprite(InX, InY, LevelCompleteButtonSprite) == true){
		if(Control.CustomMap == false){Control.SetState("Main"); LoadNextLevel();} else {Control.SetState("MapSelection");}
	}
}
void DrawLevelComplete(){
	//move images to the correct locations
	LevelCompleteSprite.setPosition((View.getCenter().x-LevelCompleteSprite.getGlobalBounds().width/2), (View.getCenter().y-LevelCompleteSprite.getGlobalBounds().height/2));
	LevelCompleteButtonSprite.setPosition((View.getCenter().x-156), (View.getCenter().y+100));
	
	//draw the images to the screen
	App.draw(LevelCompleteSprite);
	App.draw(LevelCompleteButtonSprite);
}
void DrawPausedMenu(){
	//place all relevant sprites in appropriate locations
	PausedSprite.setPosition((View.getCenter().x-PausedSprite.getGlobalBounds().width/2), (View.getCenter().y-PausedSprite.getGlobalBounds().height/2));
	PausedMenuSprites[0].setPosition((View.getCenter().x-148), (View.getCenter().y-68));
	PausedMenuSprites[1].setPosition((View.getCenter().x-148), (View.getCenter().y+10));
	PausedMenuSprites[2].setPosition((View.getCenter().x-148), (View.getCenter().y+88));

	//draw all a dem to the screen to the screen
	App.draw(PausedSprite);
	App.draw(PausedMenuSprites[0]);
	App.draw(PausedMenuSprites[1]);
	App.draw(PausedMenuSprites[2]);
	
}
void PausedMenuEvent(int InX, int InY){
	//color the background appropriatly
	App.clear(sf::Color(32, 26, 47));
	
	//if the player selects resume
	if(MouseClickSprite(InX, InY, PausedMenuSprites[0]) == true){
		Control.SetState("Main");
	}
	//if the player selects options
	//if(MouseClickSprite(InX, InY, PausedMenuSprites[1]) == true){
		//stops options from being acceses from the first level, needs to be fixed
		//if(Control.CurrentMap > 0){Control.SetState("Options");}
	//}
	//if the player selects exit
	if(MouseClickSprite(InX, InY, PausedMenuSprites[2]) == true){
		ClearGame();
		Control.SetState("Menu");
	}
}
void DrawOptions(){
	//creates temp sptite for the player on options screen
	sf::Sprite TempPlayer;
	
	//sets the temp player
	SetSprite(TempPlayer, Player.PlayerImage, 4);
	TempPlayer.setTextureRect(sf::IntRect(0, 0, Player.PlayerImageWidth, Player.PlayerImageHeight));

	//sets the positions on the items
	OptionsSprite.setPosition((View.getCenter().x-OptionsSprite.getGlobalBounds().width/2), (View.getCenter().y-OptionsSprite.getGlobalBounds().height/2));
	TempPlayer.setPosition(View.getCenter().x+178,View.getCenter().y-135);	
	CrossSprite.setPosition(View.getCenter().x+184,View.getCenter().y-68);
	OptionsSelectSprite.setPosition(View.getCenter().x-310,(View.getCenter().y-132)+Control.OptionSelection*50);

	//draws stuffs to the screen
	App.draw(OptionsSprite);
	App.draw(OptionsSelectSprite);
	App.draw(TempPlayer);
	if(BoolDrawFPS == true){App.draw(CrossSprite);}
}
void OptionsEvent(string InKey){
	//player select - left arrow
	if(Time.getElapsedTime().asSeconds() > OptionsLastTime+0.2){
		if((InKey == "Left") and (Control.OptionSelection == 0)){
			CurrentPlayerImage--;
			if(CurrentPlayerImage < 0){CurrentPlayerImage = 2;}
			LoadPlayerImage();
		}
		//player select - right arrow
		if((InKey == "Right") and (Control.OptionSelection == 0)){
			CurrentPlayerImage++;
			if(CurrentPlayerImage > 2){CurrentPlayerImage = 0;}
			LoadPlayerImage();
		}
		OptionsLastTime = Time.getElapsedTime().asSeconds();
	}
	//if the player Clicks on the show FPS box
	if((InKey == "Return") and (Control.OptionSelection == 1)){
		if(BoolDrawFPS == true){BoolDrawFPS = false;} else {BoolDrawFPS = true;}
	}
	//save option and return to menu
	if((InKey == "Return") and (Control.OptionSelection == 2)){
		SaveOptions();
		if(Control.CurrentMap == 0){Control.SetState("Menu");} else {Control.SetState("Paused");}
		Control.OptionSelection = 0;
	}
}
void MovePlayer(string InDir, int InOffset){
	//animate the player
    Player.Animate(Time.getElapsedTime().asSeconds());
    
    //set player new x and y based on keys pressed and gravity
	if(InDir == "Left"){
		if(GravityY != 0){
        	Player.NewX -= Player.MoveSpeed*InOffset;
        	Player.PlayerSprite.setScale(-2.f,Player.PlayerSprite.getScale().y);
        }
        if(GravityX != 0){
        	if(GravityX > 0){
           		Player.NewY += Player.MoveSpeed*InOffset;
           	} 
           	else {
           		Player.NewY -= Player.MoveSpeed*InOffset;
	        }
		    Player.PlayerSprite.setScale(2.f,Player.PlayerSprite.getScale().y);
        }		
    }
	if(InDir == "Right"){
		if(GravityY != 0){
        	Player.NewX += Player.MoveSpeed*InOffset;
       	    Player.PlayerSprite.setScale(2.f,Player.PlayerSprite.getScale().y);		
        }
        if(GravityX != 0){
        	if(GravityX > 0){
           		Player.NewY -= Player.MoveSpeed*InOffset;
           	} 
           	else {
           		Player.NewY += Player.MoveSpeed*InOffset;
	        }
		    Player.PlayerSprite.setScale(-2.f,Player.PlayerSprite.getScale().y);		
        }
    }
}
void HandleMousePress(int InX, int InY){
	cout << "Left Click At - X: " << InX << " | Y: " << InY << endl;
	cout << "Control.MapOff - X: " << Control.MapOffX << " | Y: " << Control.MapOffY << endl;
	//if(Control.GetState() == "Menu"){MenuEvent(InX, InY);}
	if(Control.GetState() == "Paused"){PausedMenuEvent(InX+Control.MapOffX, InY+Control.MapOffY);}
	else if(Control.GetState() == "Credits"){Control.SetState("Menu");}
	else if(Control.GetState() == "Win"){Control.SetState("Menu");}
	else if(Control.GetState() == "Splash"){Control.SetState("Menu");}
	else if(Control.GetState() == "LevelComplete"){LevelCompleteEvent(InX+Control.MapOffX, InY+Control.MapOffY);}
}
void HandleKey(string InKey, float InOffset = 0){
	//send the key to MapSelectionEvent if its currently running
	if(Control.GetState() == "MapSelection"){MapSelectionEvent(InKey);}
	else if(Control.GetState() == "DownloadMaps"){MapDownloadEvent(InKey);}
	
	//handle all jey presses accordingly
	else if(InKey == "Up"){
		if(Control.GetState() == "Main"){PlayerJump();}
		else if(Control.GetState() == "Menu"){if(Control.MenuSelection > 0){Control.MenuSelection--;}}
		else if(Control.GetState() == "Options"){if(Control.OptionSelection > 0){Control.OptionSelection--;}}
	}
	else if(InKey == "Down"){
		if(Control.GetState() == "Menu"){if(Control.MenuSelection < 5){Control.MenuSelection++;}}
		else if(Control.GetState() == "Options"){if(Control.OptionSelection < 2){Control.OptionSelection++;}}
	}
	else if(InKey == "Left"){
		if(Control.GetState() == "Main"){MovePlayer("Left", InOffset);}
		else if(Control.GetState() == "Options"){OptionsEvent("Left");}
	}
	else if(InKey == "Right"){
		if(Control.GetState() == "Main"){MovePlayer("Right", InOffset);}
		else if(Control.GetState() == "Options"){OptionsEvent("Right");}		
	}
	else if(InKey == "Return"){
		if(Control.GetState() == "LevelComplete"){if(Control.CustomMap == false){Control.SetState("Main"); LoadNextLevel();} else {Control.SetState("MapSelection");}}
		else if(Control.GetState() == "Menu"){MenuEvent();}
		else if(Control.GetState() == "Options"){OptionsEvent("Return");}
		else if(Control.GetState() == "Credits"){Control.SetState("Menu");}		
		else if(Control.GetState() == "Win"){Control.SetState("Menu");}		
	}
	else if(InKey == "Z"){
		if(Control.GetState() == "Main"){PlayerJump();}
	}
	else if((InKey == "F12") or (InKey == "+") or (InKey == "-")){HandleRealTimeVar(InKey);}
}
void DrawBack(){
	//colors the area outside of the map
	App.clear(sf::Color(32, 26, 47));
	
	//draws the background
	App.draw(BackgroundSprite);
	
	//draws the mid layer
	App.draw(MidSprite);
}
void DrawSplash(){
	//set the alpha for time
	float Alpha = Time.getElapsedTime().asSeconds()*100;
	
	//colors the background
	App.clear(sf::Color(32, 26, 47));	
	
	//set the position of the splash screen
	SplashSprite.setPosition((View.getCenter().x-SplashSprite.getGlobalBounds().width/2), (View.getCenter().y-SplashSprite.getGlobalBounds().height/2));
	
	//make it fade after going big
	if(Alpha > 220){Alpha = Alpha - Time.getElapsedTime().asSeconds()*160;}

	//update the alpha value
	SplashSprite.setColor(sf::Color(255, 255, 255, Alpha));

	//draw the spash screen to screen
	if(Time.getElapsedTime().asSeconds() < 4){App.draw(SplashSprite);} else {Control.SetState("Menu");}
}
void HandleWindowResize(){
	//sets the view
	View.setSize((App.getSize().x), (App.getSize().y));
	View.setCenter((App.getSize().x/2), (App.getSize().y/2));
	
	//resets the offset
	Control.MapOffX = 0;
	Control.MapOffY = 0;
}
void DrawCredits(){
	CreditsSprite.setPosition((View.getCenter().x-CreditsSprite.getGlobalBounds().width/2), (View.getCenter().y-CreditsSprite.getGlobalBounds().height/2));
	App.draw(CreditsSprite);
}
int main(){
	//holds the FPScount 
	int FPSCount = 0;

	//set up the font....
	MyFont.loadFromFile("font/andrios_font.ttf");
	MapIndexString.setFont(MyFont);
	MapIndexString.setCharacterSize(25);

	RealTimeVarString.setFont(MyFont);
	RealTimeVarString.setCharacterSize(40);	
	RealTimeVarString.setPosition(5,20);
	//loads the options from disk
	LoadOptions();
	
	//setup the images
	SetupImages();
	
	//animate a player frame (this somewhat magically fixes a bug so just leave it here)
	Player.Animate(0.1);
	
	//fix bug on windows maybe?
	HandleWindowResize();	
	
	//set frame rate limit
	App.setFramerateLimit(FPSLimit);
	
	//set FPSTime for FPS counting
	FPSTime = Time.getElapsedTime().asSeconds();
    
    // Start game loop
    while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event)){
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();
                
        	// handle resizing of window
            if (Event.type == sf::Event::Resized)
                HandleWindowResize();                
                
            // Escape key : set state to paused if game is currently running
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                if(Control.GetState() == "Main"){Control.SetState("Paused");}   
      
            // F1 show player collision spheres
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F1))
				if(CollisionDev == false){CollisionDev = true;} else {CollisionDev = false;}
                
            // F2 key load map for testing
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F2))
				Load(Control.CurrentMap);
			
            // f3 key change map to laod
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F3))
				Control.CurrentMap++;		
				
			// f4 key change map to load
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F4))
				Control.CurrentMap--;	
				
			// F5 temp to toggle gravity
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F5))
				GravityY -= GravityY*2;
				
			// F6 toggle tile num
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F6))
				if(TileNum == false){TileNum = true;} else {TileNum = false;}	
				
			// F7 GIANT PLAYER
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F7))
				if(Player.PlayerSprite.getScale().x == 2){Player.PlayerSprite.setScale(10,10);} else {Player.PlayerSprite.setScale(2,2);}
				
			// F12 to toggle real time variable minipulation
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::F12))
				HandleKey("F12");	
				
			// + to increase selected variable
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Equal))
				HandleKey("+");
				
			// - to decrease selected variabe;
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Dash))
				HandleKey("-");														
				
			// S to start game
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S))
				if(Control.GetState() == "Menu"){Load(Control.CurrentMap);}
				
			//player pressed up key
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up))
				HandleKey("Up");	
				
			//player pressed up key
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Z))
				HandleKey("Z");				
				
			//player pressed down key
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down))
				HandleKey("Down");															
				
			//continue on to the next level
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Return))
                HandleKey("Return");
                
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::X))
                HandleKey("Return");             
            
            //detect mouse press    
            if(Event.type == sf::Event::MouseButtonPressed){
            	if(Event.mouseButton.button == sf::Mouse::Left){HandleMousePress(Event.mouseButton.x, Event.mouseButton.y);}
            }
            	//HandleMousePress(Event.mouseButton.x, Event.mouseButton.y);     		
        }
        
		//if(Control.GetState() == "Main"){
        	// Move the player
        	float Offset = 150.f * (FrameClock.restart().asSeconds());
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        		HandleKey("Left", Offset);
        		//Player.PlayerSprite.FlipX(true);
        	}        
        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){ 
				HandleKey("Right", Offset);
        		//Player.PlayerSprite.FlipX(false);
        	}       
        	//if (App.getInput().IsKeyDown(sf::Keyboard::Space)){ 
				//Player.NewY -= GravityY*Offset*0.6;
				//Player.JetPackActive = true;
				//if(Control.GetState() == "Splash"){Control.SetState("Menu");}
        	//}     	
    
   		// Set the view
    	App.setView(View);

    	// Clear screen
    	App.clear();
    	
    	//determine what to do based on current game state
    	if(Control.GetState() == "Menu"){RunMenu();}
    	else if(Control.GetState() == "Main"){DrawBack(); ShowMap(Control.MapWidth, Control.MapHeight);UpdatePlayer();}
    	else if(Control.GetState() == "Credits"){DrawCredits();}
    	else if(Control.GetState() == "Win"){WinSprite.setPosition((View.getCenter().x-WinSprite.getGlobalBounds().width/2), (View.getCenter().y-WinSprite.getGlobalBounds().height/2));App.draw(WinSprite);}
    	else if(Control.GetState() == "Options"){DrawOptions();}
    	else if(Control.GetState() == "Splash"){DrawSplash();}
    	else if(Control.GetState() == "LevelComplete"){DrawLevelComplete();}
    	else if(Control.GetState() == "Paused"){App.draw(BackgroundSprite); ShowMap(Control.MapWidth, Control.MapHeight);DrawPausedMenu();}
    	else if(Control.GetState() == "MapSelection"){DrawMapSelection();}
    	else if(Control.GetState() == "DownloadMaps"){DrawMapSelection();}    	
    
    	// Reset to the default view to draw the interface
    	App.setView(App.getDefaultView());
        
        //draws FPS to the screen
        if(BoolDrawFPS == true){DrawFPS();}
        if(Control.RealTimeVar != 0){DrawRealTimeVar();}
        
        //render the App display to screen
        App.display();
        
        //add a count to the fps
        FPSCount++;
        
        //check if a seccond has passed and fps needs to be updated
        if((FPSTime+1) < Time.getElapsedTime().asSeconds()){FPS = FPSCount; FPSCount = 0; FPSTime = Time.getElapsedTime().asSeconds();}
        
    	//draw static overlay
    	//DrawOverlay();
    }
}
