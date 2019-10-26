#ifndef GRAPHICS_H
#define GRAPHICS_H

//forward declaring
struct SDL_Window;
struct SDL_Renderer;
 
class Graphics {
public:
	Graphics(); 
	~Graphics();	
private:
	
	SDL_Window* _window;
	SDL_Renderer* _renderer;	
};

#endif // !GRAPHICS_H

