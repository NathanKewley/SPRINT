/*------------------------------------------------------------------------------
FILE                  TileClass.h
PROGRAM               TileClass header for Sprint Editor
PROGRAMMER            Nathan Kewley      
ENVIROMENT            Linux Mint 12 - g++ with SFML
------------------------------------------------------------------------------*/

using namespace std;

class TileClass{		
	public:
		//ID of the Tile
		int IntPicture;
		string Special;
		
		//information for the portal tile
		int PortalX;
		int PortalY;
		
		//info for the tip tile
		int TipNumber;
		
		//create sprite container
		sf::Sprite Picture;
		sf::Sprite SpecialPicture;
		
		//Constructor (Tells the tile What it is)
		TileClass(int InIntPicture, string InSpecial);
		
		//destructor
		~TileClass();
};

TileClass::TileClass(int InIntPicture, string InSpecial = "None"){
	IntPicture = InIntPicture;
	Special = InSpecial;
	PortalX = 0;
	PortalY = 0;
	TipNumber = 0;
}
TileClass::~TileClass(){
	//cout << "killed tile :(" << endl;
}
