/*------------------------------------------------------------------------------
FILE                  SprintPlayerClass.h
PROGRAM               PlayerClass for SPRINT
PROGRAMMER            Nathan Kewley      
ENVIROMENT            Linux Mint 12 - g++ with SFML
------------------------------------------------------------------------------*/

using namespace std;

class PlayerClass{		
	private:
		int AnimationState;
		int JetPackAnimationState;
		float StoredTime;
		float JetPackTime;
	public:
		int CheckedCollision, NewX, NewY, JumpTime, PlayerImageWidth, PlayerImageHeight, JumpHeight;
		float GravityTime, PlayerHeight, PlayerWidth, LastNewX, LastNewY, AnimationTime, MoveSpeed;
		bool Falling, PlayerCanJump, JetPackActive;
		//create sprite container
		sf::Texture PlayerImage;
		sf::Sprite PlayerSprite;
		
		//create jetpack flame container
		sf::Texture JetPackImage;	
		sf::Sprite JetPackSprite;
		
		void SetPos(int InX, int InY);
		void Animate(float InTime);
		void AnimateJetPack(float InTime);
		void Fall(bool IsFalling);
		void UpdateGravity(int InGravitX, int InGravityY);
		void SetJump(int InVel);
		void PlayerMove(int InX, int InY);
		
		PlayerClass();
};

PlayerClass::PlayerClass(){
	CheckedCollision = JumpTime = 0;
	Falling = PlayerCanJump = false;
	StoredTime = 0;
	PlayerHeight = 22;
	PlayerWidth = 26;
	PlayerImageHeight = 15;
	PlayerImageWidth = 13;
	AnimationState = JetPackAnimationState = 0;
	GravityTime = LastNewX = LastNewY = 0;
	NewX = NewY = 0;
	JumpHeight = 30;
	AnimationTime = 0.04;
	MoveSpeed = 1.6;
	
	//sets the JetPack sprite
	JetPackSprite.setPosition(150, 140);
	PlayerSprite.setTextureRect(sf::IntRect(JetPackAnimationState*10, 0, 10+JetPackAnimationState*10, PlayerImageHeight));	
	
	//sets the player sprite
	PlayerSprite.setPosition(150, 125);
	PlayerSprite.setTextureRect(sf::IntRect(AnimationState*PlayerImageWidth, 0, PlayerImageWidth+AnimationState*PlayerImageWidth, PlayerImageHeight));
}

void PlayerClass::PlayerMove(int InX, int InY){
	PlayerSprite.move(InX, InY);
}

void PlayerClass::UpdateGravity(int InGravityX = 0, int InGravityY = 0){
	NewX += InGravityX;
	NewY += InGravityY;

	//if((Falling == true) && (JumpTime < 1)){NewY += InGravityY;}
	if(JumpTime > 0){
		if(InGravityX > 0){NewX -= (InGravityX*2); JumpTime--;}
		if(InGravityX < 0){NewX -= (InGravityX*2); JumpTime--;}
		if(InGravityY > 0){NewY -= (InGravityY*2); JumpTime--;}
		if(InGravityY < 0){NewY -= (InGravityY*2); JumpTime--;}
	}

}

void PlayerClass::Animate(float InTime){
	if(InTime > (StoredTime+AnimationTime)){
		if(AnimationState < 11){AnimationState++;} else {AnimationState = 0;}
		PlayerSprite.setTextureRect(sf::IntRect(AnimationState*PlayerImageWidth, 0, PlayerImageWidth, PlayerImageHeight));
		StoredTime = InTime;
		CheckedCollision = 0;
	}
}

void PlayerClass::AnimateJetPack(float InTime){
	if(InTime > (JetPackTime+0.05)){
		if(JetPackAnimationState < 5){JetPackAnimationState++;} else {JetPackAnimationState = 0;}
		JetPackSprite.setTextureRect(sf::IntRect(JetPackAnimationState*10, 0, 10+JetPackAnimationState*10, 15));
		JetPackTime = InTime;
	}
}

void PlayerClass::SetPos(int InX, int InY){
	PlayerSprite.setPosition(InX, InY);
	CheckedCollision = 0;
}

void PlayerClass::SetJump(int InVel){
	JumpTime = InVel;
}

void PlayerClass::Fall(bool IsFalling){
	Falling = IsFalling;
	CheckedCollision = 0;
}
