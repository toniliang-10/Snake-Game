#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 750
#define PILE_SIZE 50

int generate_fruit();

SDL_Window* window;
SDL_Renderer* renderer;

struct tile{
    int x;
    int y;
    int width;
    int height;
};

struct tile snake[226];   //225 max length of snake,  +1 extra space for null terminator

int snake_size = 0;
int fruit_generated = 0;
int pos = -1;
int game_is_running;

int delta_x = 1;
int delta_y = 0;

int initialize_window(void){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "error initializing SDL \n");
        return 0;
    }
    window = SDL_CreateWindow(
        "Snake Game", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH,   //750
        WINDOW_HEIGHT,  //750
        0
        );
    if (!window){
        fprintf(stderr, "Failed to create a window");
        return 0;
    }

    renderer = SDL_CreateRenderer(
        window, 
        -1, 
        0
    );
    if (!renderer){
        fprintf(stderr, "Failed to create a renderer");
        return 0;
    };

    return 1;
}

void setup(void){
    snake[0].x = 350;
    snake[0].y = 350;
    snake[0].width = PILE_SIZE;        //50, pile size W
    snake[0].height = PILE_SIZE;       //50, pile size H

    snake_size = 1;
}

void draw(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 255 && snake[i].height != 0; i++){
        SDL_Rect snake_part_rect = {
            (int)snake[i].x,
            (int)snake[i].y,
            (int)snake[i].width,
            (int)snake[i].height,
        };

        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
        SDL_RenderFillRect(renderer, &snake_part_rect);
    }
    
    if(snake_size == 225){
        game_is_running = 0;
        return;
    }
    
    if(!fruit_generated){
        pos = generate_fruit();
    }
    
    //draw the fruit

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    struct tile fruit;
    SDL_Rect fruit_rect = {
        fruit.x = (pos / 15) * PILE_SIZE,
        fruit.y = (pos % 15) * PILE_SIZE,
        fruit.height = 50,
        fruit.width = 50
    };

    printf("fruitX pos: %d \n", fruit.x);
    printf("fruitY pos: %d \n", fruit.y);

    SDL_RenderFillRect(renderer, &fruit_rect);
    SDL_RenderPresent(renderer);
}

int generate_fruit(){

    int board[226] = 
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        };

    for(int i = 0; i < snake_size; i++){
        int row = snake[i].x / 50;
        int col = snake[i].y / 50;

        board[row * 15 + col] = -1;         //snake is here so dont generate a fruit here
    }

    //store the parts of the board that are not filled
    int not_filled[225] = {0};
    int size = 0;
    for(int i = 0; i < 225; i++){
        if(board[i] == 1){
            not_filled[size] = i;
            size++;
        }
    }

    //get a random index from the places in the board that arent occupied
    srand(time(NULL));
    int random_index = rand() % size;
    printf("random index: %d \n", random_index);

    fruit_generated = 1;

    //this is the position that will be used to get x(row) and the y(col) to draw the fruit
    int res = not_filled[random_index];
    return res;

}

void process_input(){
    SDL_Event event;
    

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                game_is_running = 0;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    game_is_running = 0;
                }
                if(event.key.keysym.sym == SDLK_w && delta_y != 1){
                    delta_x = 0;
                    delta_y = -1;
                }
                if(event.key.keysym.sym == SDLK_s && delta_y != -1){ 
                    delta_x = 0;
                    delta_y = 1;
                }
                if(event.key.keysym.sym == SDLK_a && delta_x != 1){
                    delta_x = -1;
                    delta_y = 0;
                }
                if(event.key.keysym.sym == SDLK_d && delta_x != -1){
                    delta_x = 1;
                    delta_y = 0;
                }
                break;
        }
    }
}

void update(){
    //move head first
    snake[0].x = snake[0].x + (delta_x * PILE_SIZE);
    snake[0].y = snake[0].y + (delta_y * PILE_SIZE);

    //have a loop that will move the prev snake body part to the next one


    //Then this should check if the snake head hit the border or itself (if it did game is over)
    if(snake[0].x < 0 || snake[0].x >= (15 * PILE_SIZE) || snake[0].y < 0 || snake[0].y >= (15 * PILE_SIZE)){
        game_is_running = 0;
        return;
    }

    //check if snake ate the fruit
}

void destroy_renderer(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(){
    printf("Game is currently running...");

    srand(time(NULL));

    game_is_running = initialize_window();  //creates window and renderer, returns 1 success, 0 failed
    setup();    // initializes the snake

    while(game_is_running){
        process_input();
        update();
        draw();


        SDL_Delay(1000);
    }

    destroy_renderer();

    return 0;

    

}

