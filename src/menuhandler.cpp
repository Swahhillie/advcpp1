#include "menuhandler.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <assert.h>
#include <fstream>

MenuHandler::MenuHandler(std::ostream& output_):
    output_(output_),
    running_(true),
    verbose_(false)
{
    create_menu("main");
    menu("main");
    //give the main menu the bind function. the command for bind is "bind"
    bind("main", "bind", "bind");
}
void MenuHandler::execute()
{
    if(!running_)return;

    if(verbose_)
    {
        output_ << "execute(";
        for(unsigned int i = 0; i < buffer.size(); ++i)
        {
            output_ << buffer[i];
            if(i != buffer.size() -1)
            {
                output_ << ", ";
            }
        }
        output_ << ")\n";
    }
    if(!buffer.empty())
    {
        MenuMapping::iterator it = menuItems().find(buffer.front());
        if(it != menuItems().end())
        {
            MenuItem m = it->second;
            if(m)
            {
                (this->*m)();
            }
            else
            {
                output_ << "nop\n";
            }
        }
        else
        {
            output_ << "no function for command " << buffer.front() << "\n";
        }
    }
}
void MenuHandler::bind()
{
    if(buffer.size() >= 4)
    {
        bind(buffer[2], buffer[1], buffer[3]);
    }

}
void MenuHandler::bind(const std::string& menu, const std::string& key, const std::string& func)
{
    MenuItem m = get_function(func);
    if(m && menus_.find(menu) != menus_.end())
    {
        menus_[menu][key] = m;
        administrations_[menu][key] = func;
        output_ << "bound function " << func << " to command " << key << " in menu " << menu << "\n";
    }
    else
    {
        output_ << "could not bind " << func << " to command " << key << " in menu " << menu << "\n";
    }

}
void MenuHandler::exit()
{
    running_ = false;
}

void MenuHandler::echo()
{
    for(unsigned int i = 1; i < buffer.size(); i++)
    {
        output_ << buffer[i] << " ";
    }
    output_ << "\n";
}

void MenuHandler::save()
{
    if(buffer.size() >= 2)
    {
        std::ofstream stream(buffer[1].c_str());
        if(stream)
        {
            output_ << "saving binds to file " << buffer[1] << std::endl;
            save_binds(stream);
        }
    }
}
void MenuHandler::load()
{
    if(buffer.size() >= 2)
    {
        std::ifstream stream(buffer[1].c_str());
        if(stream)
        {
            output_ << "loading binds from file " << buffer[1] << std::endl;
            load_binds(stream);
        }
    }
}
void MenuHandler::list_available_commands()
{
    std::map<std::string, std::string>::const_iterator it;
    for(it = administration().begin(); it != administration().end(); ++it)
    {
        output_ << it->first << " -> " << it->second << "\n";
    }
}
void MenuHandler::list_available_menus()
{
    std::map<std::string, MenuMapping>::iterator it;
    for(it = menus_.begin(); it != menus_.end(); ++it)
    {
        output_ << it->first << "\n";
    }
}
void MenuHandler::verbose()
{
    if(buffer.size() >= 2)
    {
        if(buffer[1] == "1")
        {
            output_ << "verbose on\n";
            verbose_ = true;
        }
        else if(buffer[1] == "0")
        {
            output_ << "verbose off\n";
            verbose_ = false;
        }
        else
        {
            output_ << "usage: verbose [1|0]\n";
        }
    }
}
MenuHandler::MenuItem MenuHandler::get_function(const std::string& func_name)
{
    //could put this in a map if needed.
    if(func_name == "create_menu")return &MenuHandler::create_menu;
    if(func_name == "menu")return &MenuHandler::menu;
    if(func_name == "remove_menu")return &MenuHandler::remove_menu;
    if(func_name == "exit")return &MenuHandler::exit;
    if(func_name == "bind")return &MenuHandler::bind;
    if(func_name == "echo")return &MenuHandler::echo;
    if(func_name == "save")return &MenuHandler::save;
    if(func_name == "load")return &MenuHandler::load;
    if(func_name == "list_available_commands")return &MenuHandler::list_available_commands;
    if(func_name == "list_available_menus")return &MenuHandler::list_available_menus;
    if(func_name == "verbose")return &MenuHandler::verbose;
    return NULL;
}

void MenuHandler::save_binds(std::ostream& out)
{
    std::map<std::string, AdministrationMapping>::iterator adminis;
    for(adminis = administrations_.begin(); adminis != administrations_.end(); ++adminis)
    {
        AdministrationMapping::iterator it;
        for(it= adminis->second.begin(); it != adminis->second.end(); ++it)
        {
            out << adminis->first << " " << it->first << " " << it->second << "\n";
        }
    }
    if(verbose_)
    {
        //set verbose to false to avoid a stackoverflow
        verbose_ = false;
        output_ << "save file{\n";
        save_binds(output_);
        output_ << "}\n";
        //turn verbose back on
        verbose_ = true;
    }
}

void MenuHandler::load_binds(std::istream& in)
{
    std::string menu_name, command, function_name;
    while(in >> menu_name >> command >> function_name)
    {
        //create the menu if it doesn't exist yet
        if(menus_.find(menu_name) == menus_.end())
        {
            create_menu(menu_name);
        }
        //bind the command
        bind(menu_name, command, function_name);
    }

}
void MenuHandler::create_menu()
{
    if(buffer.size() >= 2 && buffer[1] != "main")
    {
        create_menu(buffer[1]);
    }
}

void MenuHandler::menu()
{
    if(buffer.size() >= 2)
    {
        menu(buffer[1]);
    }
}

void MenuHandler::remove_menu()
{
    if(buffer.size() >= 2 && buffer[1] != "main")
    {
        remove_menu(buffer[1]);
    }
}

void MenuHandler::create_menu(std::string name)
{
    menus_[name] = MenuMapping();
    administrations_[name] = AdministrationMapping();

    output_ << "created menu \"" << name << "\"" << std::endl;
}

void MenuHandler::menu(std::string name)
{
    std::map<std::string,MenuMapping>::iterator it = menus_.find(name);
    if(it != menus_.end())
    {
        activeMenu_ = name;
        output_ << "in menu \"" << name << "\"" << std::endl;
    }
    else
    {
        output_ << "no menu \"" << name << "\"" << std::endl;
    }
}

void MenuHandler::remove_menu(std::string name)
{
    std::map<std::string, MenuMapping>::iterator it = menus_.find(name);
    if(it != menus_.end())
    {
        menus_.erase(it);
        administrations_.erase(administrations_.find(name));

        output_ << "removed menu \"" << name << "\"" << std::endl;
    }
    else
    {
        output_ << "no menu \"" << name << "\"" << std::endl;
    }
}
