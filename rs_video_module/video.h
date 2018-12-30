struct screen_attributes
{
	int XRes;
  int YRes;
  char rbpp;		// requested
  char ubpp;			// how many we're using on actual screen
	char Active;  // whether we have a current mode
  uint32 Status;
	uint32 Width; 	// size of our window
	uint32 Height;
};
