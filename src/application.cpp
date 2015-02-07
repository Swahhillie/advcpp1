#include "application.h"
#include <sstream>

void Application::execute(std::string statement)
{
    std::stringstream stream;
    stream << statement;
    std::string word;
    while(stream >> word)
    {
        menu.buffer.push_back(word);
    }
    menu.execute();
    menu.buffer.clear();
}
