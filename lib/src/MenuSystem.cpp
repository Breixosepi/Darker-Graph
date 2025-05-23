#include "MenuSystem.hpp"
#include <iostream>
#include <Player.hpp>

MenuSystem::MenuSystem()
{
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    helper = std::make_shared<RenderHelper>();
}

MenuSystem::MenuSystem(SDL_Renderer* render, TTF_Font* fonts, SDL_Window* wind)
{
    renderer = render;
    font = fonts;
    window = wind;
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    helper = std::make_shared<RenderHelper>();
    setMainMenu(this);
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
}

void MenuSystem::addWidget(const std::string& name, const std::string& label, std::function<void()> action) 
{
    const int startY = 150;    
    const int stepY = 50;      
    const int centerX = 400;  

    std::unique_ptr<Widget> widget = std::make_unique<Widget>(name, centerX, startY + 2*(widgets.size() * stepY), label);
    widget->setAction(action);

    SDL_Rect rect;
    rect.x = centerX - (label.length() * 10); 
    rect.y = startY + 2*(widgets.size() * stepY) - 25;
    rect.w = label.length() * 20;
    rect.h = 50;
    widget->setRect(rect);

    if (!head) 
    {
        head = widget.get();
        current = head;
    } 
    else 
    {
        tail->setNext(widget.get());
        widget->setPrev(tail);
    }

    tail = widget.get();
    widgets.push_back(std::move(widget));

    if (widgets.size() > 1) 
    {
        tail->setNext(head);
        head->setPrev(tail);
    }
}

void MenuSystem::handleEvent(const SDL_Event& event) 
{
    if (event.type == SDL_KEYDOWN) 
    {
        switch (event.key.keysym.sym) 
        {
            case SDLK_UP: navigateUp(); break;
            case SDLK_DOWN: navigateDown(); break;
            case SDLK_RETURN: executeCurrent(); break;
            case SDLK_f:
            {
                static bool isFullscreen = false;
                isFullscreen = !isFullscreen;
                SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
                if (window) 
                {
                    SDL_SetWindowFullscreen(window, isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                    if (!isFullscreen) 
                    {
                        SDL_SetWindowSize(window, 800, 800); 
                    }
                }
                break;
            }
        }
    }
    if (event.type == SDL_MOUSEMOTION) 
    {
        for (auto& widget : widgets) 
        {
            if (widget->containsPoint(event.motion.x, event.motion.y)) 
            {
                current = widget.get();
                break;
            }
        }
    }
    
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) 
    {
        for (auto& widget : widgets) 
        {
            if (widget->containsPoint(event.button.x, event.button.y)) 
            {
                current = widget.get();
                executeCurrent();
                break;
            }
        }
    }
}

void MenuSystem::render()  
{
    if (!renderer) return;
    SDL_RenderCopy(renderer,helper.get()->getTexture("assets/screenshots/background-title.png",renderer),NULL,NULL);
    if (font && !widgets.empty()) 
    {
        for (const auto& widget : widgets) 
        {
            widget->render(renderer, font, widget.get() == current);
        }
    }     
}
void MenuSystem::navigateUp() 
{
    if (!current) 
    {
        return; 
    }

    current = current->getPrev() ? current->getPrev() : tail;
}

void MenuSystem::navigateDown() 
{
    if (!current) 
    {
        return; 
    }

    current = current->getNext() ? current->getNext() : head;
}

void MenuSystem::executeCurrent() 
{
    if (current) 
    {
        current->execute();
    }
}

void MenuSystem::setMainMenu(MenuSystem* menu) 
{ 
    menu->addWidget("start", "Iniciar Juego", [&]()
    {   
        creator.restart();
        bool gameOver = false;
        while(!gameOver)
        {
      // room setup
        Level level = Level(creator.createMap(false));
        
        SDL_GetWindowSize(window,&windowWidth,&windowHeight);
        helper.get()->handleWindowResize(windowWidth,windowHeight,level.getMatrixSize());
        level.setRenderHelper(helper);

      // Player setup
        SDL_Surface* playerSurface = IMG_Load("assets/sprites/dwarf.png");
        TexturePtr playerTexture(SDL_CreateTextureFromSurface(renderer, playerSurface));
        SDL_FreeSurface(playerSurface);
        Player player;
        player.initAnimation(renderer, playerTexture);
        player.setPosition(400, 300); 
        player.setRenderHelper(helper);

      // Enemy setup 
        EnemyManager enemies;
        enemies.init(renderer, "assets/sprites/Orc.png");
        auto* dungeonGraph = level.getMap();
        if (dungeonGraph) 
        {
        dungeonGraph->enum_for_each_node([&](Designar::nat_t index, auto* node) 
        {
            if (node) 
            {
                int roomIndex = index;
                
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> countDist(1, 5);
                int enemyCount = countDist(gen);
                
                for (int i = 0; i < enemyCount; ++i) 
                {
                    std::uniform_int_distribution<> xDist(0, 500);
                    std::uniform_int_distribution<> yDist(-150, 400);
                    enemies.addEnemy(roomIndex, xDist(gen), yDist(gen));
                }
            }
            return true;
        });
    }

        bool running = true;
        bool showMap = false;
        SDL_Event event;
        Uint32 lastTime = SDL_GetTicks();

        while (running) 
        {
            Uint32 currentTime = SDL_GetTicks();
            float deltaTime = (currentTime - lastTime) / 1000.0f; 
            lastTime = currentTime;

            while (SDL_PollEvent(&event)) 
            {
                player.handleImput(event);
                if (event.type == SDL_QUIT) 
                {
                    running = false;
                    gameOver = true;
                    SDL_PushEvent(&event); 
                }
                else if (event.type == SDL_KEYDOWN) 
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        if(showMap){showMap = false;}
                        else
                        {
                            running = false;
                            gameOver = true;
                            
                        } 
                        break;
                    case SDLK_m: //Con la tecla 'm' se muestra el mapa de la dungeon
                        if(showMap){showMap = false;}
                        else{showMap = true;}
                        break;
                    }  
                }
                else if(event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
                    {
                        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                        helper.get()->handleWindowResize(windowWidth,windowHeight,level.getMatrixSize());
                        level.handleResizeWindow();
                    } 
                }  
            }
            if(!showMap)
            {
                int currentRoomIndex = *level.getCurrentRoom()->getIndex();
                enemies.setCurrentRoom(currentRoomIndex);
                // Actualización de objetos
                player.update(deltaTime);
                enemies.update(deltaTime, player);
                enemies.handlePlayerAttack(player);

                //Comprueba si despues de moverse se encuentra en los limites del cuarto actual
                if(player.getIsInBound()&&player.getState()==State::RUNNING)
                {
                    int index = *level.getCurrentRoom()->getIndex();
                    player.setPosition(level.verifyPassRoom(static_cast<int>(player.getDirection()),player.getBounds()));
                    if(index!=*level.getCurrentRoom()->getIndex())
                    {
                        std::cout<<"you advanced to room number: "<<*level.getCurrentRoom()->getIndex()<<" you come from room number: "<<index<<std::endl;
                    }
                }
                //Si esta en la Ultima sala, comprueba que este sobre la escalera para avanzar de nivel
                else if(*level.getCurrentRoom()->getIndex()==level.getMap()->get_num_nodes())
                {
                    int middleX = helper.get()->getMiddlePointInX();
                    int middleY = helper.get()->getMiddlePointInY();
                    SDL_Rect dest = player.getBounds();
                    if(std::abs(middleX-(dest.x+dest.w/2))<=dest.w/2&&std::abs(middleY-(dest.y+dest.h))<=dest.h/4)
                    {
                        bool result = level.IsEulerianPath(); //retorna true si se hizo correctamente un camino euleriano
                        if(result)
                        {
                            // std::cout<<"score x2, score actual:" <<enemies.getScore() <<std::endl;
                            enemies.setScore(enemies.getScore()*2);
                            // std::cout<<"nuevo score:" <<enemies.getScore() <<std::endl;

                        }
                        std::cout<<"Loading New Level!"<<std::endl;
                        running = false;
                        creator.levelUp();
                        SDL_Delay(650);
                    }
                }
                // Renderizado
                SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255); 
                SDL_RenderClear(renderer);
                level.renderRoom(renderer,deltaTime);
                player.renderPlayer(renderer);
                enemies.render();
                level.renderRoomLastFrame(renderer,deltaTime);
                SDL_RenderPresent(renderer);
                if(!player.isAlive())
                {
                    showGameOverScreen(enemies.getScore());
                    running = false;
                    gameOver = true;
                }
                SDL_Delay(16); 
            }
            else{level.renderMap(renderer);}
        }
        }     
    }); 

    menu->addWidget("load", "Puntuaciones", [&]() 
    {
        
        std::cout << "Puntuaciones seleccionadas\n";
        showHighScores();
        
    });

    
    menu->addWidget("options", "Opciones", []() 
    {
        std::cout << "Opciones seleccionadas\n";
    });

    menu->addWidget("credits", "Creditos", []() 
    {
        std::cout << "todo: Eugenio El musculoso!\n" << "Nada : Francisco el negro\n" ;
    });
    
    menu->addWidget("exit", "Salir", []() 
    {
        SDL_Event quitEvent{SDL_QUIT};
        SDL_PushEvent(&quitEvent);
    });
} 

void MenuSystem::showGameOverScreen(int score)
{
    bool nameEntered = false;
    std::string playerName;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* screenScore = IMG_Load("assets/screenshots/caveBack.jpg");
    TexturePtr background(SDL_CreateTextureFromSurface(renderer, screenScore));
    SDL_FreeSurface(screenScore);

    SDL_StartTextInput();
    bool running = true;
    while (running && !nameEntered)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_TEXTINPUT:
                    if (playerName.length() < 20)
                    {
                        playerName += event.text.text;
                        updateInputTexture(playerName, textColor);
                    }
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !playerName.empty())
                    {
                        playerName.pop_back();
                        updateInputTexture(playerName, textColor);
                    }
                    else if (event.key.keysym.sym == SDLK_RETURN && !playerName.empty())
                    {
                        nameEntered = true;
                        saveHighScore(playerName, score);
                    }
                    else if (event.key.keysym.sym == SDLK_ESCAPE) 
                    {
                        running = false;
                        nameEntered = true;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background.get(), NULL, NULL);

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Ingresa tu nombre:", textColor);
        TexturePtr textTexture(SDL_CreateTextureFromSurface(renderer, textSurface));
        SDL_Rect textRect;
        textRect.w = static_cast<int>(textSurface->w * (windowWidth / 800.0)); 
        textRect.h = static_cast<int>(textSurface->h * (windowHeight / 800.0)); 
        textRect.x = (windowWidth / 2) - (textRect.w / 2);
        textRect.y = static_cast<int>(windowHeight * 0.25); 
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture.get(), NULL, &textRect);

        SDL_Rect inputRect;
        inputRect.w = static_cast<int>(300 * (windowWidth / 800.0)); 
        inputRect.h = static_cast<int>(40 * (windowHeight / 800.0)); 
        inputRect.x = (windowWidth / 2) - (inputRect.w / 2);
        inputRect.y = static_cast<int>(windowHeight * 0.35); 

        if (!playerName.empty())
        {
            updateInputTexture(playerName, textColor);
            SDL_Rect playerNameRect;
            playerNameRect.w = static_cast<int>(playerName.length() * 20 * (windowWidth / 800.0)); 
            playerNameRect.h = static_cast<int>(50 * (windowHeight / 800.0)); 
            playerNameRect.x = (windowWidth / 2) - (playerNameRect.w / 2);
            playerNameRect.y = inputRect.y + (inputRect.h / 2) - (playerNameRect.h / 2); 
            SDL_RenderCopy(renderer, inputTexture.get(), NULL, &playerNameRect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &inputRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_StopTextInput();
}

void MenuSystem::updateInputTexture(const std::string& text, SDL_Color color) 
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    inputTexture.reset(SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);
}

void MenuSystem::saveHighScore(const std::string& name, int score) 
{
    std::ofstream file("highscores.txt", std::ios::app); 
    if (file.is_open()) 
    {
        file << name << " " << score << "\n";
        file.close();
        std::cout << "Puntaje guardado para " << name << ": " << score << std::endl;
    } 
}

void MenuSystem::showHighScores()
{
    highScores.clear();
    std::ifstream file("highscores.txt");
    if (file.is_open())
    {
        std::string name;
        int score;
        while (file >> name >> score)
        {
            size_t firstChar = name.find_first_not_of(" \t");
            if (std::string::npos != firstChar)
            {
                name = name.substr(firstChar);
            }
            size_t lastChar = name.find_last_not_of(" \t");
            if (std::string::npos != lastChar)
            {
                name = name.substr(0, lastChar + 1);
            }
            highScores.push_back({name, score});
        }
        file.close();

        std::sort(highScores.begin(), highScores.end(), [](const auto& a, const auto& b)
        {
            return a.second > b.second;
        });
    }

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color bgColor = {30, 30, 50, 255};

    bool showingScores = true;
    SDL_Event event;

    while (showingScores)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                showingScores = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_RETURN)
                {
                    showingScores = false;
                }
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);

        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Puntuaciones Altas", textColor);
        if (titleSurface)
        {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            SDL_Rect titleRect;
            titleRect.w = static_cast<int>(titleSurface->w * (windowWidth / 800.0)); 
            titleRect.h = static_cast<int>(titleSurface->h * (windowHeight / 800.0)); 
            titleRect.x = (windowWidth / 2) - (titleRect.w / 2);
            titleRect.y = static_cast<int>(windowHeight * 0.0625); 
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
            SDL_FreeSurface(titleSurface);
            SDL_DestroyTexture(titleTexture);
        }

        int startY = static_cast<int>(windowHeight * 0.1875); 
        int maxScoresToShow = 10;
        int scoreLineHeight = static_cast<int>(40 * (windowHeight / 800.0)); 

        for (size_t i = 0; i < highScores.size() && i < maxScoresToShow; ++i)
        {
            std::string scoreText = std::to_string(i + 1) + ". " + highScores[i].first + ": " + std::to_string(highScores[i].second);
            SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
            if (scoreSurface)
            {
                SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
                SDL_Rect scoreRect;
                scoreRect.w = static_cast<int>(scoreSurface->w * (windowWidth / 800.0)); 
                scoreRect.h = static_cast<int>(scoreSurface->h * (windowHeight / 800.0)); 
                scoreRect.x = (windowWidth / 2) - (scoreRect.w / 2);
                scoreRect.y = startY + (int)(i * scoreLineHeight);
                SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
                SDL_FreeSurface(scoreSurface);
                SDL_DestroyTexture(scoreTexture);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}