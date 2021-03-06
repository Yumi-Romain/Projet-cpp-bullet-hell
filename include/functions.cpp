void tickcounter(sf::Clock *tick_clock, int *ticks) {

    while (tick_clock->getElapsedTime().asSeconds() <= 0.00005) { /* wait */ }

    *ticks += 1;

    tick_clock->restart();

}


void eventPull(Bullet_hell *game, std::vector<Event> *events, int eventPullType) {

    for (int cpt = 0; cpt < game->events.size(); cpt++) {

        if (game->events[cpt].type == eventPullType) {

            events->push_back(game->events[cpt]);
            game->events.erase(game->events.begin() + cpt);

        }

    }

}


void move(Bullet_hell *game) {

    if (game->controler.joy == false) {

        sf::Vector2f pos = game->player.player_hit_box.getPosition();
        
        if (game->controler.shiftKey) {
            pos.x += (float) game->controler.keyDir.x / 180;
            pos.y += (float) game->controler.keyDir.y / 180;
        } else {
            pos.x += (float) game->controler.keyDir.x / 80;
            pos.y += (float) game->controler.keyDir.y / 80;
        }

        if (pos.x > 490 || pos.x < 0 || pos.y > 790 || pos.y < 0) {
            // sort de l'ecran
        } else {
            game->player.player_hit_box.setPosition(pos);
        }

    } else {

        sf::Vector2f move(
            (game->controler.axisX + game->controler.correctif.x),
            (game->controler.axisY + game->controler.correctif.y)
        );

        if (move.x < 0) {
            move.x += game->controler.deadzone.x;
            if (move.x > 0) move.x = 0;
        } else {
            move.x -= game->controler.deadzone.x;
            if (move.x < 0) move.x = 0;
        }

        if (move.y < 0) {
            move.y += game->controler.deadzone.y;
            if (move.y > 0) move.y = 0;
        } else {
            move.y -= game->controler.deadzone.y;
            if (move.y < 0) move.y = 0;
        }

        move.x /= 4000;
        move.y /= 4000;

        sf::Vector2f pos(game->player.player_hit_box.getPosition() + move);

        if (pos.x > 490 || pos.x < 0 || pos.y > 790 || pos.y < 0) {
            // sort de l'ecran
        } else {
            game->player.player_hit_box.setPosition(pos);
        }

    }

}


void move_bullet(Bullet_hell *game) {

    for (int cpt = 0; cpt < game->player.bullet_list.size(); cpt++) {

        game->player.bullet_list[cpt].move();

        if (game->player.bullet_list[cpt].out()) {
            game->player.bullet_list.erase(game->player.bullet_list.begin() + cpt);
        }

    }

    for (int cpt = 0; cpt < game->enemy_bullet_list.size(); cpt++) {

        game->enemy_bullet_list[cpt].move();

        if (game->enemy_bullet_list[cpt].out()) {
            game->enemy_bullet_list.erase(game->enemy_bullet_list.begin() + cpt);
        }

    }

}


bool collision(sf::Vector2f pos1, int rad1, sf::Vector2f pos2, int rad2) {

    if (sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= rad1 + rad2) {
        return true;
    } else {
        return false;
    }

}

std::string tostring(int score) {

    std::stringstream ss;

    ss << score;

    std::string sService = ss.str();

    return sService;

}

std::string tostring(Bullet_hell *game) {

    std::stringstream ss;

    ss << game->controler.axisX << " / " << game->controler.axisY;

    std::string sService = ss.str();

    return sService;

}

bool menu() {

    std::cout << "je rentre dans le menu" << std::endl;

    sf::RenderWindow window;

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Menu");
    window.setFramerateLimit(FRAMERATE);
    //window.setVerticalSyncEnabled(true);

    Menu menu(window.getSize().x, window.getSize().y);

    int show = 5;
    Background Background;
    bool play = false;
    bool key_pressed = false;

    while (play == false && window.isOpen()) {

        sf::Event event;

        //std::cout << menu.GetPressedItem() << std::endl;

        while(window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.key.code == sf::Keyboard::Escape) {
                if (show == 5) {
                    window.close();
                } else {
                    show = 5;
                    menu.SetPressedItem(0);
                }
            }

            if (event.type == sf::Event::KeyPressed) {

                if (show == 5) {

                    if (key_pressed == false) { 

                        key_pressed = true;

                        if (event.key.code == sf::Keyboard::Up) {
                            menu.MoveUp();
                        } else if (event.key.code == sf::Keyboard::Down) {
                            if (menu.GetPressedItem() != 2) {
                                menu.MoveDown();
                            }
                        } else if (event.key.code == sf::Keyboard::Return) {

                            if (menu.GetPressedItem() == 0) {
                                play = true;
                            } else if (menu.GetPressedItem() == 1) {
                                show = 2;
                                menu.SetPressedItem(3);
                            } else if (menu.GetPressedItem() == 2) {
                                window.close();
                            }

                        }

                    }

                } else if (show == 2) {

                    if (key_pressed == false) {

                        key_pressed = true;

                        if (event.key.code == sf::Keyboard::Up) {
                            if (menu.GetPressedItem() != 3) {
                                menu.MoveUp();
                            }
                        } else if (event.key.code == sf::Keyboard::Down) {
                            menu.MoveDown();
                        } else if (event.key.code == sf::Keyboard::Return) {

                            if (menu.GetPressedItem() == 5) {
                                show = 5;
                                menu.SetPressedItem(0);
                            }

                        }

                    }

                }

            } else if (event.type == sf::Event::KeyReleased) {
                    
                key_pressed = false;

            }

        }


        if (show == 2) {
            
            window.clear();
            Background.drawBackground(window);
            menu.drawOptions(window);
            window.display();

        } else if (show == 5) {

            window.clear();
            Background.drawBackground(window);
            menu.draw(window);
            window.display();

        }

    }

    return play;
 
}