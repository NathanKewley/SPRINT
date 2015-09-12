/*------------------------------------------------------------------------------
FILE                  MapDisplayClass.h
PROGRAM               Map display class for the map browser inside sprint
PROGRAMMER            Nathan Kewley      
ENVIROMENT            Linux Mint 12 - g++ with SFML
------------------------------------------------------------------------------*/
using namespace std;

class MapDisplay{
	public:
		//strings to hold the map data
		sf::Text MapFilename;
		sf::Text MapName;
		sf::Text MapAuthor;

	MapDisplay(sf::Font &InFont);
};

MapDisplay::MapDisplay(sf::Font &InFont){
	MapFilename.setFont(InFont);
	MapAuthor.setFont(InFont);
	MapName.setFont(InFont);
}
