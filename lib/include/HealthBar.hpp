#pragma once
#include <Utilities.hpp>

class HealthBar 
{
    public:

        HealthBar();

        void init(SDL_Renderer* renderer, SDL_Texture* texture);
        void update(int currentLives, float deltaTime);
        void render(SDL_Renderer* renderer, SDL_Texture* texture);
        void takeDamage();
        void handleWindowResize(int screenWidth, int screenHeight);
        void setBaseSize(int referenceWidth, int referenceHeight);
        
    private:

        SDL_Rect srcRect;
        SDL_Rect destRect;
        
        int frameWidth;
        int frameHeight;
        int baseScreenWidth;
        int baseScreenHeight;
        int currentLives;
        int targetLives;

        float scaleFactor;
        float animationTimer;
        float animationDuration;

        bool isAnimating;
        
        std::vector<std::pair<int, float>> animationSequence;
        const std::vector<int> livesToRow = {6, 4, 2, 0};
};