#pragma once
#include <Utilities.hpp>

class HealthBar 
{
    public:
        HealthBar();
        void init(SDL_Renderer* renderer, const std::string& texturePath);
        void update(int currentLives, float deltaTime);
        void render(SDL_Renderer* renderer);
        void takeDamage();
        void handleWindowResize(int screenWidth, int screenHeight);
        void setBaseSize(int referenceWidth, int referenceHeight);
        
    private:
        SDL_Texture* texture;
        SDL_Rect srcRect;
        SDL_Rect destRect;
        int frameWidth;
        int frameHeight;
        int baseScreenWidth;
        int baseScreenHeight;
        float scaleFactor;
        bool isAnimating;
        float animationTimer;
        float animationDuration;
        int currentLives;
        int targetLives;
        std::vector<std::pair<int, float>> animationSequence;
        const std::vector<int> livesToRow = {6, 4, 2, 0};
};