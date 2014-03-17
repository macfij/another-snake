#include "SDL/SDL.h"
#include "constants.h"
#include "snake.h"
#include "globals.h"
#include <iostream>

/*
  Function Snake() creates a new snake.

  Snake is the length of SNAKE_LENGTH and has velocity of SNAKE_VELOCITY
  which can be found in file 'constants.h'. On default snake's mouth is
  closed. Snake starts moving from the center of screen and goes to the
  right.
*/
Snake::Snake() {
    w = 5;
    h = 5;
    open = false;
    faceVar = -1;
    length = SNAKE_LENGTH;
    velocity = SNAKE_VELOCITY;
    xPos = new int[length];
    yPos = new int[length];
    for (int i = 0; i < length; i++) {
        *(xPos + i) = 318;
        *(yPos + i) = 240;
    }
    xVel = velocity;
    yVel = 0;
    direction = RIGHT;
}

bool Snake::check_collision() {
    for (int j = 1; j < length; j++) {
        if (xPos[0] == xPos[j] && yPos[0] == yPos[j]) {
            return true;
        }
    }
    return false;
}

/*
  Function handle_input translates keys pressed by user into changes of
  horizontal and vertical velocity of snake.

  Pressing 'f' button opens and closes snakes mouth.
*/
void Snake::handle_input() {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            direction = UP;
            if (yVel == 0) {
                yVel = -velocity;
            }
            xVel = 0;
            break;
        case SDLK_s:
            direction = DOWN;
            if (yVel == 0) {
                yVel = +velocity;
            }
            xVel = 0;
            break;
        case SDLK_a:
            direction = LEFT;
            if (xVel == 0) {
                xVel = -velocity;
            }
            yVel = 0;
            break;
        case SDLK_d:
            direction = RIGHT;
            if (xVel == 0) {
                xVel = +velocity;
            }
            yVel = 0;
            break;
        case SDLK_f:
            open = !open;
        }
    }
}

/*
  Function move changes the coordinates of each block of snake's body.

  First the tail blocks are moved into previous position of the next
  block. Then the position of head is changed accordingly to the
  direction of movement.

  Function also handles reaching the end of the screen. If snakes head
  should move out of the screen it's moved to the opposite edge of the
  screen.
*/
void Snake::move() {
    for (int i = length - 1; i > 0; i--) {
        *(xPos + i) = *(xPos + i - 1);
        *(yPos + i) = *(yPos + i - 1);
    }
    
    if (*xPos >= SCREEN_WIDTH)
        *xPos = 0;
    else if (*xPos < 0)
        *xPos = SCREEN_WIDTH - 4;
    else
        *xPos += xVel;
    
    if (*yPos >= SCREEN_HEIGHT)
        *yPos = 0;
    else if (*yPos < 0)
        *yPos = SCREEN_HEIGHT + yVel;
    else
        *yPos += yVel;
}

/*
  Function show draws each block of snake on the screen.

  First it draws the tail and later the opened or closed mouth of snake.
*/
void Snake::show() {
    
    for (int i = 1; i < length; i++) {
        snakeBody.x = *(xPos + i);
        snakeBody.y = *(yPos + i);
        snakeBody.w = w;
        snakeBody.h = h;
        SDL_FillRect(screen, &snakeBody,
		     SDL_MapRGB(screen->format, (*(xPos + i) % 255),
				(*(yPos + i) % 255), 128));
    }
    if (xVel != 0) {
        if (xVel >= 0)
            faceVar = -1;
        else
            faceVar = 1;
        if (open) {
	    
            snakeBody.x = *xPos;
            snakeBody.y = *yPos - 2;
            snakeBody.w = 8;
            snakeBody.h = 7;
            SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, (*xPos % 255), (*yPos % 255), 128));
	    
            //Bigger piece of snakes head.
            snakeBody.x = *xPos + 3 + faceVar * 3;
            snakeBody.y = *yPos - 2;
            snakeBody.w = 2;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Smaller piece of snakes head.
            snakeBody.x = *xPos + 3 + faceVar * 1;
            snakeBody.y = *yPos - 2;
            snakeBody.w = 2;
            snakeBody.h = 1;
            SDL_FillRect(screen, &snakeBody, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Snakes eye.
            snakeBody.x = *xPos + 3 + faceVar * 2;
            snakeBody.y = *yPos + 1;
            snakeBody.w = 2;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Snakes mouth.
            snakeBody.x = *xPos + 2 - faceVar * 2;
            snakeBody.y = *yPos;
            snakeBody.w = 4;
            snakeBody.h = 4;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
        } else {
            snakeBody.x = *xPos;
            snakeBody.y = *yPos;
            snakeBody.w = 8;
            snakeBody.h = 5;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, (*xPos % 255), (*yPos % 255),
				    128));
	    
            //Snakes eye.
            snakeBody.x = *xPos + 3 + faceVar * 2;
            snakeBody.y = *yPos + 1;
            snakeBody.w = 2;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Snakes mouth.
            snakeBody.x = *xPos + 2 - faceVar * 2;
            snakeBody.y = *yPos + 3;
            snakeBody.w = 4;
            snakeBody.h = 1;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
        }
    } else {
        if (yVel >= 0)
            faceVar = -1;
        else
            faceVar = 1;
        if (open) {
            snakeBody.x = *xPos - 2;
            snakeBody.y = *yPos - 3;
            snakeBody.w = 7;
            snakeBody.h = 8;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, (*xPos % 255), (*yPos % 255),
				    128));

            //Bigger piece of snakes head.
	    snakeBody.x = *xPos - 2;
            snakeBody.y = *yPos + faceVar * 2;
            snakeBody.w = 2;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

	    //Smaller piece of snakes head.
	    snakeBody.x = *xPos - 2;
            snakeBody.y = *yPos + faceVar;
            snakeBody.w = 1;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Snakes eye.
            snakeBody.x = *xPos + 1;
            snakeBody.y = *yPos + faceVar * 2;
            snakeBody.w = 2;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Snakes mouth.
            snakeBody.x = *xPos;
            snakeBody.y = *yPos - 1 - faceVar * 2;
            snakeBody.w = 4;
            snakeBody.h = 4;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
        } else {
	    
            snakeBody.x = *xPos;
            snakeBody.y = *yPos - 3;
            snakeBody.w = 5;
            snakeBody.h = 8;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, (*xPos % 255), (*yPos % 255),
				    128));
	    
            //Snakes eye.
            snakeBody.x = *xPos + 1;
            snakeBody.y = *yPos + faceVar * 2;
            snakeBody.w = 2;
            snakeBody.h = 2;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	    
            //Snakes mouth.
            snakeBody.x = *xPos + 3;
            snakeBody.y = *yPos - 1 - faceVar * 2;
            snakeBody.w = 1;
            snakeBody.h = 4;
            SDL_FillRect(screen, &snakeBody,
			 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
        }
    }
    
}

void Snake::get_position(int* x, int* y) {
    *x = *xPos;
    *y = *yPos;
}

/*
  Function grow handles the event of eating a block of food.

  First, memory for a longer snake is allocated, then the old positions
  are copied there. The new block of snake gets the same position as
  the last block of old snake.

  Finally, the old snake is deleted.
*/
void Snake::grow() {
    length++;
    
    int* xOld = xPos;
    int* yOld = yPos;
    
    int* xNew = new int[length];
    int* yNew = new int[length];
    
    for (int i = 0; i < length - 1; i++) {
        *(xNew + i) = *(xPos + i);
        *(yNew + i) = *(yPos + i);
    }
    *(xNew + length - 1) = *(xNew + length - 2);
    *(yNew + length - 1) = *(yNew + length - 2);
    
    xPos = xNew;
    yPos = yNew;
    
    delete[] xOld;
    delete[] yOld;
}

void Snake::mouth_change_state(bool isOpen) {
    this->open = isOpen;
}

int Snake::get_dir() {
    return this->direction;
}

/*
  Function mouth_open decides when to open snakes mouth. It opens when
  snakes head approaches a piece of food.
*/
void Snake::mouth_open(int direction, int xSnake, int ySnake, int xFood,
		       int yFood) {
    switch (direction) {
    case UP:
        if (xFood - 1 == xSnake) {
            if ((ySnake - yFood >= 0) && (ySnake - yFood <= 50)) {
                mouth_change_state(true);
            }
        }
        break;
    case DOWN:
        if (xFood - 1 == xSnake) {
            if ((yFood - ySnake >= 0) && (yFood - ySnake <= 50)) {
                mouth_change_state(true);
            }
        }
        break;
    case LEFT:
        if (yFood - 1 == ySnake) {
            if ((xSnake - xFood >= 0) && (xSnake - xFood <= 50)) {
                mouth_change_state(true);
            }
        }
        break;
    case RIGHT:
        if (yFood - 1 == ySnake) {
            if ((xFood - xSnake >= 0) && (xFood - xSnake <= 50)) {
                mouth_change_state(true);
            }
        }
    }
}
