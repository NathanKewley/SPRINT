//sets the sprite based on an image and scale factor
void SetSprite(sf::Sprite& InSprite, sf::Texture& InTexture, int ScaleAmount = 1){
	int InImageWidth, InImageHeight;
	
	//sets the image "smooth" property to false to prevent blurring of images
	InTexture.setSmooth(false);	

	//set the sprite to the image
	InSprite.setTexture(InTexture);

	//scale the image by ScaleAmount*x the original width	
	InSprite.setScale(ScaleAmount,ScaleAmount);
}

//checks wether a map exists
bool MapExists(int InMapNum)
{
	//makes the filename of the map
	stringstream InternalMapFile;
    InternalMapFile << "maps/map" << InMapNum << ".map";
    string strInternalMapFile = InternalMapFile.str();

    ifstream ifile(strInternalMapFile.c_str());
    return ifile;
}

//check if moused clicked on given sprite
bool MouseClickSprite(int InMouseX, int InMouseY, sf::Sprite& InSprite){
	//gets the position of the given sprite
	int SpriteX1 = InSprite.getPosition().x;
	int SpriteY1 = InSprite.getPosition().y;
	
	//gets height and width of given sprite
	int SpriteX2 = (SpriteX1 + (InSprite.getGlobalBounds().width));
	int SpriteY2 = (SpriteY1 + (InSprite.getGlobalBounds().height));
	
	cout << "X1: " << SpriteX1 << " | Y1: " << SpriteY1 << " | X2: " << SpriteX2 << " | Y2: " << SpriteY2 << endl;
	
	//creates a rect from the sprite
	sf::IntRect InSpriteRect(SpriteX1, SpriteY1, SpriteX2, SpriteY2);
	//sf::IntRect InSpriteRect(InSprite.getGlobalBounds());
	
	//checks if the mouse click is inside the rect, return accordingly
	//if(InSpriteRect.contains(InMouseX, InMouseY)){return(true);} else {return(false);}	
	if((InMouseX >= SpriteX1) and (InMouseY >= SpriteY1) and (InMouseX <= SpriteX2) and (InMouseY <= SpriteY2)){return(true);} else {return(false);} 
}

//check if moused clicked on given sprite
bool CheckSpriteCollision(int InX, int InY, sf::Sprite& InSprite){
	//gets the position of the given sprite
	int SpriteX1 = InSprite.getPosition().x;
	int SpriteY1 = InSprite.getPosition().y;
	
	//gets height and width of given sprite
	int SpriteX2 = (SpriteX1 + (InSprite.getGlobalBounds().width));
	int SpriteY2 = (SpriteY1 + (InSprite.getGlobalBounds().height));
	
	//creates a rect from the sprite
	sf::IntRect InSpriteRect(SpriteX1, SpriteY1, SpriteX2, SpriteY2);
	//sf::IntRect InSpriteRect(InSprite.getGlobalBounds());
	
	//checks if the mouse click is inside the rect, return accordingly
	//if(InSpriteRect.contains(InMouseX, InMouseY)){return(true);} else {return(false);}	
	if((InX >= SpriteX1) and (InY >= SpriteY1) and (InX <= SpriteX2) and (InY <= SpriteY2)){return(true);} else {return(false);} 
}

//Scans the user map dir for map files
int ScanDir(vector<string> &InMapNames, string dir, string Extention)
{
	int i = 0;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
	//add file's in directory to vector
    while ((dirp = readdir(dp)) != NULL) {
		InMapNames.push_back(string(dirp->d_name));
    }
    closedir(dp);

	//remove the non ".map" files
	for(i=0;i<InMapNames.size();i++){
		if(InMapNames[i].size() > 4){
	 		if(InMapNames[i].compare((InMapNames[i].size()-4),4,Extention) != 0){
	    		InMapNames.erase(InMapNames.begin()+i); 
	    		i--;
			}
		}
		else {
	    	InMapNames.erase(InMapNames.begin()+i); 
	    	i--;
	    }
	}
}

void LoadTileImages(vector<sf::Texture> &InTileImageVector){
	int num = 1;
	string prefix = "gfx/Tiles/";
	string suffix = ".png";
	ostringstream Filename;
	string strFilename;
	sf::Texture TempImage;
	struct stat stFileInfo;
	int intStat = 0;

	while (intStat == 0)
	{
		Filename << prefix << num << suffix;
		strFilename = Filename.str();
		intStat = stat(strFilename.c_str(),&stFileInfo);
		if(intStat == 0) 
		{
			cout << "Loading Tile: " << num << endl;
			TempImage.loadFromFile(strFilename.c_str());
			//TempImage.SetSmooth(false);
			InTileImageVector.push_back(TempImage);
			num++;
			Filename.str("");
		}
	} 
}

//loads the author and map name from the map name file
string LoadMapInfo(string InFile, string Choice){
	int BeforeTileData = 0; //used to grab mapwidth and height before processing tiles
	string ReturnData = "NULL";

	//int MapWidth, MapHeight;
	string ReadData;

	ifstream MapFile(InFile.c_str());
	
	//load data from file (INSANITY!!!)
	if (MapFile.is_open()){
		while ( BeforeTileData < 2){
			getline (MapFile,ReadData);
			if((BeforeTileData == 0) and (Choice == "Author")){ //loads map width
				ReturnData = "By: ";
				ReturnData += ReadData;
			}
			else if((BeforeTileData == 1) and (Choice == "MapName")){ //loads map height and sets the new map up via size
				ReturnData = ReadData;
			}
			BeforeTileData++;
		}
	} else {cout << "Map file not opend" << endl;}
	return(ReturnData);
}

//gets the filename of a map
string GetMapFilename(string InMapLabel){
	string FinalMapName = "CustomMaps/map0.map";
	string Prefix = "CustomMaps/";
	string Suffix = ".map";
	
	InMapLabel.replace(InMapLabel.size()-4,4,Suffix);
	FinalMapName = Prefix.append(InMapLabel);
	
	return(FinalMapName);
}


