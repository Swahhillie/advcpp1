#pragma once
#include "menuhandler.h"
#include <iostream>

class Application
{
public:
    explicit Application(std::ostream& output):
        menu(MenuHandler(output)) {}
    bool running()
    {
        return menu.running();
    };

    void execute(std::string statement);

    MenuHandler menu;
private:

};
