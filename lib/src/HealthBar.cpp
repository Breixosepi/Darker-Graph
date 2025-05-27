#include "HealthBar.hpp"
#include <SDL2/SDL_image.h>
#include <algorithm>

HealthBar::HealthBar() : frameWidth(0),frameHeight(0),baseScreenWidth(1280),  baseScreenHeight(720),scaleFactor(1.0f),isAnimating(false),animationTimer(0.0f),animationDuration(0.6f),currentLives(3),targetLives(3)
{}


void HealthBar::init(SDL_Renderer* renderer, SDL_Texture* texture) 
{
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
    frameWidth = textureWidth / 3;
    frameHeight = textureHeight / 7;
    destRect.w = frameWidth * 6;
    destRect.h = frameHeight * 2;
    destRect.x = 40;
    destRect.y = 20;
    srcRect = {0, 0, frameWidth * 3, frameHeight};
}

void HealthBar::setBaseSize(int referenceWidth, int referenceHeight) 
{
    baseScreenWidth = referenceWidth;
    baseScreenHeight = referenceHeight;
    scaleFactor = 1.0f; 
}

void HealthBar::handleWindowResize(int screenWidth, int screenHeight) 
{
    float newScaleX = static_cast<float>(screenWidth) / baseScreenWidth;
    float newScaleY = static_cast<float>(screenHeight) / baseScreenHeight;
    scaleFactor = std::min(newScaleX, newScaleY);
    destRect.w = static_cast<int>((frameWidth * 3 * 2) * scaleFactor);
    destRect.h = static_cast<int>((frameHeight * 2) * scaleFactor);
    destRect.x = static_cast<int>(20 * scaleFactor);
    destRect.y = static_cast<int>(20 * scaleFactor);
}

void HealthBar::takeDamage() 
{
    if (isAnimating || currentLives <= 0) return;
    targetLives = currentLives - 1;
    isAnimating = true;
    animationTimer = 0.0f;
    animationSequence.clear();
    if (currentLives == 3) 
    {
        animationSequence.push_back({1, 0.2f}); // Fila 1 (2.5 vidas)
        animationSequence.push_back({2, 0.4f}); // Fila 2 (2 vidas)
    } 
    else if (currentLives == 2) 
    {
        animationSequence.push_back({3, 0.2f}); // Fila 3 (1.5 vidas)
        animationSequence.push_back({4, 0.4f}); // Fila 4 (1 vida)
    }
    else if (currentLives == 1) 
    {
        animationSequence.push_back({5, 0.2f}); // Fila 5 (0.5 vidas)
        animationSequence.push_back({6, 0.4f}); // Fila 6 (0 vidas)
    }
}

void HealthBar::update(int currentLives, float deltaTime) 
{
    if (!isAnimating) 
    {
        this->currentLives = currentLives;
        int row = livesToRow[currentLives];
        srcRect = {0, row * frameHeight, frameWidth * 3, frameHeight};
        return;
    }

    animationTimer += deltaTime;
    float accumulatedTime = 0.0f;
    int currentRow = livesToRow[this->currentLives];
    
    for (const auto& [row, duration] : animationSequence) 
    {
        accumulatedTime += duration;
        if (animationTimer <= accumulatedTime) 
        {
            currentRow = row;
            break;
        }
    }

    if (animationTimer >= animationDuration) 
    {
        isAnimating = false;
        this->currentLives = targetLives;
        currentRow = livesToRow[targetLives];
    }
    srcRect = {0, currentRow * frameHeight, frameWidth * 3, frameHeight};
}

void HealthBar::render(SDL_Renderer* renderer, SDL_Texture* texture) 
{
    if (!renderer) 
    {
        return;
    }
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}