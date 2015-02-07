#pragma once
#include <map>
#include <vector>
#include <string>

class MenuHandler
{

public:
    explicit MenuHandler(std::ostream& output);

    typedef void (MenuHandler::*MenuItem)();
    std::vector<std::string> buffer;
    void execute();
    bool running()
    {
        return running_;
    }

private:
    std::ostream& output_;
    //name of the currently active menu
    std::string activeMenu_;
    bool running_;
    bool verbose_;
    typedef std::map<std::string, MenuItem> MenuMapping;
    typedef std::map<std::string, std::string> AdministrationMapping;

    //all current menus
    std::map<std::string, MenuMapping> menus_;

    //get the table of commands for the currently active menu
    MenuMapping& menuItems()
    {
        return menus_[activeMenu_];
    }
    //for storing which functions were mapped to a command
    std::map<std::string, AdministrationMapping> administrations_;

    //get the administration for the currently active menu
    AdministrationMapping& administration()
    {
        return administrations_[activeMenu_];
    }

    //!exposed functions

    //bind a function to a command. bind [command] [menu_name] [function_name]
    void bind();
    //close the program
    void exit();
    //print parameters echo [parameters...]
    void echo();
    //list available commands.
    void list_available_commands();
    //list available menus.
    void list_available_menus();
    //save binds to a file. save [filename]
    void save();
    //load binds from a file load [filename]
    void load();
    //create a named menu
    void create_menu();
    //goto a named menu
    void menu();
    //remove a named menu
    void remove_menu();
    //if set to 1, will print the statements before they are executed. verbose [1|0]
    void verbose();

    //!implementation
    void create_menu(std::string name);
    void menu(std::string name);
    void remove_menu(std::string name);

    void save_binds(std::ostream& out);
    void load_binds(std::istream& in);
    MenuItem get_function(const std::string& func_name);
    void bind(const std::string& menu_name, const std::string& key, const std::string& func);
};
