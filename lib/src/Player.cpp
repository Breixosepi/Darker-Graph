#include "Player.hpp"

Player::Player() : texture(nullptr), currentDirection(Direction::RIGHT), currentState(State::IDLE) 
{

}

void Player::initAnimation(const RendererPtr &renderer, const TexturePtr &texture) 
{
    this->texture = texture.get();
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture.get(), NULL, NULL, &textureWidth, &textureHeight);
    
    frameWidth = textureWidth / 5; // para los otros sprites hay que dividirlo dependiendo de la cantidad de columnas
    frameHeight = textureHeight / 9; // lo mismo aca pero de filas
    srcRect = {0, 0, frameWidth, frameHeight};
    destRect = {0, 0, frameWidth * 6, frameHeight * 6};
}

int Player::getAnimationRow() const 
{
    //esto es por como esta el sprite, la primera fila es IDLE, desde la segunda hasta la 4 es RUNNING y etc 
    //(no hay sprite hacia la izquierda asi que esta reflejado el de la izquierda)
    switch(currentState) 
    {
        case State::IDLE: 
        {
            return 0;
        } 
        case State::RUNNING:
            switch(currentDirection) 
            {
                case Direction::RIGHT: 
                case Direction::LEFT: 
                {
                    return 1;
                }
                case Direction::UP:    
                {
                    return 2;
                } 
                case Direction::DOWN: 
                {
                    return 3;
                } 
            }
        case State::ATTACKING: 
        {
            if(currentDirection == Direction::RIGHT || currentDirection == Direction::LEFT) 
            {
                return 4;
            } 
            else if (currentDirection == Direction::DOWN) 
            {
                return 5;
            } 
            else if (currentDirection == Direction::UP) 
            {
                return 6;
            }
        } 
        case State::TAKING_DAMAGE: 
        {
            return 7;
        } 
        case State::DEAD: 
        {
            return 8;
        } 
    }
    return 0;
}

void Player::handleImput(const SDL_Event &event) 
{
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) 
    {
        bool keyPressed = (event.type == SDL_KEYDOWN);

        switch (event.key.keysym.sym) 
        {
            case SDLK_w:
            case SDLK_UP:
                if (keyPressed) 
                {
                    currentDirection = Direction::UP;
                    isMoving = true;
                    currentState = State::RUNNING;
                } 
                else 
                {
                    if (currentDirection == Direction::UP) 
                    {
                        isMoving = false;
                    }
                }
                    break;

            case SDLK_s:
            case SDLK_DOWN:
                if (keyPressed) 
                {
                    currentDirection = Direction::DOWN;
                    isMoving = true;
                    currentState = State::RUNNING;
                } 
                else 
                {
                    if (currentDirection == Direction::DOWN) 
                    {
                        isMoving = false;
                    }
                }
                    break;

            case SDLK_a:
            case SDLK_LEFT:
                if (keyPressed) 
                {
                    currentDirection = Direction::LEFT;
                    isMoving = true;
                    currentState = State::RUNNING;
                } 
                else 
                {
                    if (currentDirection == Direction::LEFT) 
                    {
                        isMoving = false;
                    }
                }
                    break;

            case SDLK_d:
            case SDLK_RIGHT:
                if (keyPressed) 
                {
                    currentDirection = Direction::RIGHT;
                    isMoving = true;
                    currentState = State::RUNNING;
                } 
                else 
                {
                    if (currentDirection == Direction::RIGHT) 
                    {
                        isMoving = false;
                    }
                }
                    break;

            case SDLK_SPACE:
                if (keyPressed) 
                {
                    currentState = State::ATTACKING;
                    currentFrame = 0;
                    animTimer = 0.0f;
                }
                    break;

            case SDLK_e:
                if (keyPressed) 
                {
                    currentState = State::DEAD;
                }
                    break;
        }

        if (!isMoving && currentState == State::RUNNING) 
        {
            currentState = State::IDLE;
        }
    }
}

void Player::update(float deltaTime) 
{
    if (isMoving) 
    {
        float moveAmount = 200.0f * deltaTime;
        switch (currentDirection) 
        {
            case Direction::UP:    
            {
                destRect.y -= moveAmount;
            } 
                break;

            case Direction::DOWN:  
            {
                destRect.y += moveAmount;
            } 
                break;

            case Direction::LEFT:  
            {
                destRect.x -= moveAmount;
            }
                break;

            case Direction::RIGHT: 
            {
                destRect.x += moveAmount;
            }
                break;
        }
    }
    updateAnimation(deltaTime);
}

void Player::updateAnimation(float deltaTime) 
{
    float animSpeed = 0.1f; 
    animTimer += deltaTime;

    if (animTimer >= animSpeed) 
    {
        animTimer = 0.0f;
        
        switch(currentState) 
        {
            case State::RUNNING:
            {
                currentFrame = (currentFrame + 1) % 4;
            }
                break;
            case State::ATTACKING:
                if (currentFrame < 3) 
                {
                    currentFrame++;
                }
                    break;
            case State::TAKING_DAMAGE:
                if (currentFrame < 1) 
                {
                    currentFrame++;
                } 
                    break;
            case State::DEAD:
            {
                if (currentFrame < 3) 
                {
                    currentFrame++;
                }
            }
                break;
            default: 
                currentFrame = 0;
        }
    }
    
    int row = getAnimationRow();
    srcRect.x = currentFrame * frameWidth;
    srcRect.y = row * frameHeight;
}

void Player::renderPlayer(const RendererPtr &renderer) 
{
    SDL_RendererFlip flip = (currentDirection == Direction::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer.get(), texture, &srcRect, &destRect, 0, nullptr, flip);
}

void Player::setPosition(int x, int y) 
{
    destRect.x = x;
    destRect.y = y;
}

void Player::setState(State newState) 
{
    if (currentState != newState) 
    {
        currentState = newState;
        currentFrame = 0; 
        animTimer = 0.0f;
    }
}

