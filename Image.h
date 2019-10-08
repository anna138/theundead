#pragma once

class Image 
{
	public:
		int width, height;
		unsigned char *data;
		/* Function Prototypes*/
		~Image();
		Image(const char*);
};
