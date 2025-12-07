#include <SDL2/SDL.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int FROG_SIZE = 35;
const int LANES = 11;

enum LaneType { SAFE, ROAD, WATER, WIN };

struct Vehicle {
    float x, y;
    int width, height;
    float speed;
    SDL_Color color;
};

struct Log {
    float x, y;
    int width, height;
    float speed;
};

class Frogger {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    int frogX, frogY;
    int lives;
    int score;
    int level;
    bool onLog;
    float logSpeed;
    
    std::vector<LaneType> lanes;
    std::vector<std::vector<Vehicle>> vehicles;
    std::vector<std::vector<Log>> logs;
    
    Uint32 lastMoveTime;
    const Uint32 MOVE_DELAY = 150;

public:
    Frogger() : window(nullptr), renderer(nullptr), running(true), 
                lives(3), score(0), level(1), onLog(false), logSpeed(0),
                lastMoveTime(0) {
        frogX = SCREEN_WIDTH / 2 - FROG_SIZE / 2;
        frogY = SCREEN_HEIGHT - TILE_SIZE - 5;
        
        initLanes();
        initVehicles();
        initLogs();
    }
    
    ~Frogger() {
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }
    
    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
        
        window = SDL_CreateWindow("FROGGER - Proyecto Final", 
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) return false;
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) return false;
        
        srand(static_cast<unsigned>(time(0)));
        return true;
    }
    
    void initLanes() {
        lanes = {
            WIN,   // 0 - Goal
            SAFE,  // 1
            WATER, // 2
            WATER, // 3
            WATER, // 4
            SAFE,  // 5 - Middle safe zone
            ROAD,  // 6
            ROAD,  // 7
            ROAD,  // 8
            SAFE,  // 9
            SAFE   // 10 - Start
        };
    }
    
    void initVehicles() {
        vehicles.clear();
        vehicles.resize(LANES);
        
        // Lane 6 - Cars (right)
        for (int i = 0; i < 3; i++) {
            vehicles[6].push_back({i * 250.0f, 6 * TILE_SIZE + 5.0f, 60, 30, 
                                  2.0f * level, {255, 0, 0, 255}});
        }
        
        // Lane 7 - Trucks (left)
        for (int i = 0; i < 2; i++) {
            vehicles[7].push_back({i * 350.0f, 7 * TILE_SIZE + 5.0f, 90, 30, 
                                  -1.5f * level, {0, 100, 255, 255}});
        }
        
        // Lane 8 - Fast cars (right)
        for (int i = 0; i < 4; i++) {
            vehicles[8].push_back({i * 200.0f, 8 * TILE_SIZE + 5.0f, 50, 30, 
                                  3.0f * level, {255, 255, 0, 255}});
        }
    }
    
    void initLogs() {
        logs.clear();
        logs.resize(LANES);
        
        // Lane 2 - Short logs (right)
        for (int i = 0; i < 3; i++) {
            logs[2].push_back({i * 280.0f, 2 * TILE_SIZE + 10.0f, 100, 25, 
                              1.2f * level});
        }
        
        // Lane 3 - Medium logs (left)
        for (int i = 0; i < 2; i++) {
            logs[3].push_back({i * 400.0f, 3 * TILE_SIZE + 10.0f, 150, 25, 
                              -1.0f * level});
        }
        
        // Lane 4 - Long logs (right)
        for (int i = 0; i < 2; i++) {
            logs[4].push_back({i * 450.0f, 4 * TILE_SIZE + 10.0f, 180, 25, 
                              0.8f * level});
        }
    }
    
    void handleInput() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            
            if (e.type == SDL_KEYDOWN) {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - lastMoveTime < MOVE_DELAY) continue;
                lastMoveTime = currentTime;
                
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_w:
                        if (frogY > 0) {
                            frogY -= TILE_SIZE;
                            score += 10;
                        }
                        break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        if (frogY < SCREEN_HEIGHT - TILE_SIZE) frogY += TILE_SIZE;
                        break;
                    case SDLK_LEFT:
                    case SDLK_a:
                        if (frogX > 0) frogX -= TILE_SIZE;
                        break;
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (frogX < SCREEN_WIDTH - TILE_SIZE) frogX += TILE_SIZE;
                        break;
                    case SDLK_r:
                        resetFrog();
                        break;
                }
            }
        }
    }
    
    void update() {
        // Update vehicles
        for (int lane = 0; lane < LANES; lane++) {
            for (auto& v : vehicles[lane]) {
                v.x += v.speed;
                
                if (v.speed > 0 && v.x > SCREEN_WIDTH) {
                    v.x = -v.width;
                } else if (v.speed < 0 && v.x < -v.width) {
                    v.x = SCREEN_WIDTH;
                }
            }
        }
        
        // Update logs
        for (int lane = 0; lane < LANES; lane++) {
            for (auto& log : logs[lane]) {
                log.x += log.speed;
                
                if (log.speed > 0 && log.x > SCREEN_WIDTH) {
                    log.x = -log.width;
                } else if (log.speed < 0 && log.x < -log.width) {
                    log.x = SCREEN_WIDTH;
                }
            }
        }
        
        checkCollisions();
        checkWin();
    }
    
    void checkCollisions() {
        int currentLane = frogY / TILE_SIZE;
        
        // Check road collisions
        if (lanes[currentLane] == ROAD) {
            for (const auto& v : vehicles[currentLane]) {
                if (frogX + FROG_SIZE > v.x && frogX < v.x + v.width &&
                    frogY + FROG_SIZE > v.y && frogY < v.y + v.height) {
                    die();
                    return;
                }
            }
        }
        
        // Check water
        if (lanes[currentLane] == WATER) {
            onLog = false;
            for (const auto& log : logs[currentLane]) {
                if (frogX + FROG_SIZE > log.x && frogX < log.x + log.width &&
                    frogY + FROG_SIZE > log.y && frogY < log.y + log.height) {
                    onLog = true;
                    logSpeed = log.speed;
                    break;
                }
            }
            
            if (onLog) {
                frogX += static_cast<int>(logSpeed);
                if (frogX < 0 || frogX > SCREEN_WIDTH - FROG_SIZE) {
                    die();
                }
            } else {
                die();
            }
        }
    }
    
    void checkWin() {
        int currentLane = frogY / TILE_SIZE;
        if (lanes[currentLane] == WIN) {
            score += 100 * level;
            level++;
            initVehicles();
            initLogs();
            resetFrog();
        }
    }
    
    void die() {
        lives--;
        if (lives <= 0) {
            // Game Over
            lives = 3;
            score = 0;
            level = 1;
            initVehicles();
            initLogs();
        }
        resetFrog();
    }
    
    void resetFrog() {
        frogX = SCREEN_WIDTH / 2 - FROG_SIZE / 2;
        frogY = SCREEN_HEIGHT - TILE_SIZE - 5;
        onLog = false;
    }
    
    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw lanes
        for (int i = 0; i < LANES; i++) {
            SDL_Rect lane = {0, i * TILE_SIZE, SCREEN_WIDTH, TILE_SIZE};
            
            switch (lanes[i]) {
                case WIN:
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case SAFE:
                    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
                    break;
                case ROAD:
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                    break;
                case WATER:
                    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
                    break;
            }
            SDL_RenderFillRect(renderer, &lane);
            
            // Draw lane lines for roads
            if (lanes[i] == ROAD) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                for (int x = 0; x < SCREEN_WIDTH; x += 40) {
                    SDL_Rect line = {x, i * TILE_SIZE + TILE_SIZE / 2 - 2, 20, 4};
                    SDL_RenderFillRect(renderer, &line);
                }
            }
        }
        
        // Draw vehicles
        for (const auto& laneVehicles : vehicles) {
            for (const auto& v : laneVehicles) {
                SDL_Rect rect = {static_cast<int>(v.x), static_cast<int>(v.y), 
                                v.width, v.height};
                SDL_SetRenderDrawColor(renderer, v.color.r, v.color.g, v.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        
        // Draw logs
        for (const auto& laneLogs : logs) {
            for (const auto& log : laneLogs) {
                SDL_Rect rect = {static_cast<int>(log.x), static_cast<int>(log.y), 
                                log.width, log.height};
                SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        
        // Draw frog
        SDL_Rect frog = {frogX, frogY, FROG_SIZE, FROG_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &frog);
        
        // Draw eyes
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect eye1 = {frogX + 8, frogY + 8, 8, 8};
        SDL_Rect eye2 = {frogX + FROG_SIZE - 16, frogY + 8, 8, 8};
        SDL_RenderFillRect(renderer, &eye1);
        SDL_RenderFillRect(renderer, &eye2);
        
        // Draw HUD
        drawHUD();
        
        SDL_RenderPresent(renderer);
    }
    
    void drawHUD() {
        // Lives
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int i = 0; i < lives; i++) {
            SDL_Rect heart = {10 + i * 30, 10, 20, 20};
            SDL_RenderFillRect(renderer, &heart);
        }
        
        // Score and Level indicator (simple bars)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect scoreBar = {SCREEN_WIDTH - 150, 10, score % 200, 10};
        SDL_RenderFillRect(renderer, &scoreBar);
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_Rect levelBar = {SCREEN_WIDTH - 150, 25, level * 20, 10};
        SDL_RenderFillRect(renderer, &levelBar);
    }
    
    void run() {
        while (running) {
            handleInput();
            update();
            render();
            SDL_Delay(16); // ~60 FPS
        }
    }
};

int main(int argc, char* argv[]) {
    Frogger game;
    
    if (!game.init()) {
        SDL_Log("Failed to initialize: %s", SDL_GetError());
        return 1;
    }
    
    game.run();
    return 0;
}