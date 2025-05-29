#include "Player.hpp"
#include <Enemy.hpp>
#include <Combat.hpp>

Player::Player() : currentDirection(Direction::RIGHT), currentState(State::IDLE), lives(3)
{
    isInBound = {false,false,false,false};
}

void Player::setRenderHelper(HelperPtr value){helper = value;}

void Player::setDeltaTime(DeltaTime value){deltaTime = value;}

void Player::handleWindowResize(const double& factorX, const double& factorY)
{
    Measures measures = helper.get()->getMeasuresRoom();
    double widthTile = static_cast<int>(std::get<0>(measures));
    double heightTile = static_cast<int>(std::get<1>(measures));
    destRect.w = static_cast<int>(widthTile*0.5);
    destRect.h = static_cast<int>(heightTile*0.85);
    destRect.x = std::get<2>(measures)+factorX*widthTile;
    destRect.y = std::get<3>(measures)+factorY*heightTile;
}

void Player::initAnimation(SDL_Renderer* renderer) 
{
    healthBar.init(renderer, helper.get()->getTexture("assets/sprites/Health.png",renderer));

    int textureWidth, textureHeight;

    int screenWidth, screenHeight;
    SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);
    healthBar.setBaseSize(screenWidth, screenHeight);

    SDL_QueryTexture(helper.get()->getTexture("assets/sprites/dwarf.png",renderer), NULL, NULL, &textureWidth, &textureHeight);

    frameWidth = (textureWidth/5); // para los otros sprites hay que dividirlo dependiendo de la cantidad de columnas
    frameHeight = (textureHeight/9); // lo mismo aca pero de filas
    srcRect = {0, 0, 16, 18};
    destRect = {0, 0, static_cast<int>(std::get<0>(helper.get()->getMeasuresRoom())*0.5), static_cast<int>(std::get<1>(helper.get()->getMeasuresRoom())*0.85)};
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
    if(currentState == State::DEAD ) 
    {
        return;
    }
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
        }

        if (!isMoving && currentState == State::RUNNING) 
        {
            currentState = State::IDLE;
        }
    }
}

void Player::update() 
{
    if (currentState == State::TAKING_DAMAGE) 
    {
        damageCooldown += *deltaTime;
        if (damageCooldown >= DAMAGE_COOLDOWN_TIME) 
        {
            damageCooldown = 0.0f;
            setState(State::IDLE);
        }
    }
    else if (isMoving) 
    {
        float moveAmount = -1.0f;
        
        if(currentDirection==Direction::UP||currentDirection==Direction::DOWN)
        {
            moveAmount = std::get<1>(helper.get()->getMeasuresRoom()) * (*deltaTime);
        }
        else
        {
            moveAmount = std::get<0>(helper.get()->getMeasuresRoom()) * (*deltaTime);
        }
        
        SDL_Rect bounds = getBounds();
        
        double borderX = std::get<2>(helper.get()->getMeasuresRoom());
        double borderY = std::get<3>(helper.get()->getMeasuresRoom());
        int width = helper.get()->getWindowWitdth();
        int height = helper.get()->getWindowHeight();

        switch (currentDirection) 
        {
            case Direction::UP:    
            {
                //deduccion: se necesita de unos pixeles vacios por encima de cada sprite
                //y cuando se reescala se multiplica dicho vacio
                //para evitar ese error visual se le impide que suba mucho al multiplicar por 0.85
                if(static_cast<float>(borderY)<=bounds.y+bounds.h*0.85-moveAmount)
                {
                    destRect.y -= moveAmount;
                    isInBound[static_cast<int>(Direction::UP)] = false;
                }
                else
                {
                    destRect.y = static_cast<float>(borderY)-destRect.h*0.85;
                    isInBound[static_cast<int>(Direction::UP)] = true;
                }
            } 
            break;

            case Direction::DOWN:  
            {
                if(height-static_cast<float>(borderY)-bounds.h>=bounds.y+moveAmount)
                {
                    destRect.y += moveAmount;
                    isInBound[static_cast<int>(Direction::DOWN)] = false;
                }
                else
                {
                    destRect.y = height-static_cast<float>(borderY)-destRect.h+(bounds.y-destRect.y);
                    isInBound[static_cast<int>(Direction::DOWN)] = true;
                }
            } 
            break;

            case Direction::LEFT:  
            {
                if(static_cast<float>(borderX)<=bounds.x-moveAmount)
                {
                    destRect.x -= moveAmount;
                    isInBound[static_cast<int>(Direction::LEFT)] = false;
                }
                else
                {
                    destRect.x = static_cast<float>(borderX)-(bounds.x-destRect.x);
                    isInBound[static_cast<int>(Direction::LEFT)] = true;
                }
            }
            break;

            case Direction::RIGHT: 
            {
                if(width-static_cast<float>(borderX)>=bounds.x+bounds.w+moveAmount)
                {
                    destRect.x += moveAmount;
                    isInBound[static_cast<int>(Direction::RIGHT)] = false;
                }
                else
                {
                    destRect.x = width-static_cast<float>(borderX)-bounds.w;
                    isInBound[static_cast<int>(Direction::RIGHT)] = true;
                }
            }
            break;
        }
    }
    updateAnimation();
    healthBar.update(lives, *deltaTime);
}

void Player::updateAnimation() 
{
    float animSpeed = 0.1f; 
    animTimer += *deltaTime;

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
    srcRect.x = currentFrame * frameWidth + 8; //Se borran 8 pixeles horizontales entre frame y frame
    srcRect.y = getAnimationRow() * frameHeight + 14; //Se borran 14 pixeles horizontales entre frame y frame
}

void Player::renderPlayer(SDL_Renderer* renderer) 
{
    SDL_RendererFlip flip = (currentDirection == Direction::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    if(currentState!=State::ATTACKING)
    {
        SDL_RenderCopyEx(renderer, helper.get()->getTexture("assets/sprites/dwarf.png",renderer), &srcRect, &destRect, 0, nullptr, flip);
    }
    else
    {
        SDL_Rect newSrc = srcRect;
        newSrc.y -= 6;
        newSrc.h += 6;
        newSrc.w += 6;
        SDL_Rect newDest = destRect;
        newDest.w *= static_cast<double>(newSrc.w)/srcRect.w;
        newDest.h *= static_cast<double>(newSrc.h)/srcRect.h;
        newDest.y -= (newDest.h - destRect.h);
        if(currentDirection==Direction::LEFT)
        {
            newDest.x -= (newDest.w - destRect.w);
        }
        SDL_RenderCopyEx(renderer, helper.get()->getTexture("assets/sprites/dwarf.png",renderer), &newSrc, &newDest, 0, nullptr, flip);
    }
    healthBar.render(renderer, helper.get()->getTexture("assets/sprites/Health.png",renderer));
}

void Player::setPosition(int x, int y) 
{
    destRect.x = x;
    destRect.y = y;
}

void Player::setPosition(std::pair<int,int> value) 
{
    destRect.x = value.first;
    destRect.y = value.second;
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

SDL_Rect Player::getDest() const{return destRect;}

SDL_Rect Player::getBounds() const
{
    SDL_Rect bounds = destRect;
    double difInX = destRect.w*0.1;
    double difInY = destRect.h*0.225;
    bounds.x += difInX;
    bounds.w -= difInX*1.5;
    bounds.y += difInY;
    bounds.h -= difInY*1.5;
    return bounds;
}

SDL_Rect Player::getAttackHitbox() const 
{
    if (currentState != State::ATTACKING) 
    {
        return {0, 0, 0, 0};
    }
    
    SDL_Rect attackHitbox = {0, 0, 0, 0};
    const SDL_Rect bounds = getBounds();
    const int hitboxWidth = bounds.w * 0.5f;
    const int hitboxHeight = bounds.h * 1.0f;
    const int attackRange = hitboxHeight * 0.75f; 

    switch (currentDirection) 
    {
        case Direction::RIGHT:
            attackHitbox = { bounds.x + bounds.w, bounds.y, hitboxWidth, hitboxHeight};
            break;

        case Direction::LEFT:
            attackHitbox = { bounds.x - hitboxWidth, bounds.y, hitboxWidth, hitboxHeight};
            break;

        case Direction::UP:
            attackHitbox = { bounds.x, bounds.y - attackRange, hitboxWidth, attackRange};
            break;

        case Direction::DOWN:
            attackHitbox = { bounds.x + bounds.w/2, bounds.y + bounds.h, hitboxWidth, attackRange};
            break;
    }
    return attackHitbox;
}

void Player::attack(Enemy& enemy)
{
    Combat::playerAttack(*this, enemy);
}

int Player::getCurrentFrame() const
{
    return currentFrame; 
}

void Player::renderAttackHitbox(SDL_Renderer* renderer) const
{
    if (currentState != State::ATTACKING)
    { 
        return;
    }

    SDL_Rect hitbox = getAttackHitbox();
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_RenderFillRect(renderer, &hitbox);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hitbox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Player::renderDebugBounds(SDL_Renderer* renderer)
{
    SDL_Rect bounds = getBounds();
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
    SDL_RenderFillRect(renderer, &bounds);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawRect(renderer, &bounds);
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

bool Player::getIsInBound() const
{
    switch (currentDirection)
    {
    case Direction::RIGHT:
        return isInBound[static_cast<int>(Direction::RIGHT)];
    case Direction::UP:
        return isInBound[static_cast<int>(Direction::UP)];
    case Direction::DOWN:
        return isInBound[static_cast<int>(Direction::DOWN)];
    case Direction::LEFT:
        return isInBound[static_cast<int>(Direction::LEFT)];
    }
    return false;
}

int Player::getHealth() const 
{
    return lives;
}

void Player::setHealth(int lives) 
{
    this->lives = lives;
}

bool Player::getHasHit() const 
{
    return hasHit;
}

void Player::setHasHit(bool hit) 
{
    hasHit = hit;
}

void Player::takeDamage(int amount) 
{
    if (lives <= 0) return;
    
    int newLives = std::max(lives - amount, 0);
    if (newLives < lives) 
    {
        healthBar.takeDamage(); 
    }
    lives = newLives;

    if(lives==0)
    {
        currentState = State::DEAD;
    }
}

void Player::handleWindowResize(int newWidth, int newHeight) 
{
    healthBar.handleWindowResize(newWidth, newHeight);
}

bool Player::isAlive() const
{
    return lives > 0;
}