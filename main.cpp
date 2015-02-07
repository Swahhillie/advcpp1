#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <assert.h>
#include "application.h"
#include "menuhandler.h"
#include <conio.h>
#include <windows.h>
#include <fstream>

void check_statement(Application& app, std::stringstream& stream, const std::string& command, const std::string& expected_output)
{
    stream.str(std::string());
    app.execute(command);
    assert(stream.str() == expected_output);
}

void test_application()
{
    std::ofstream test_output("test_output.txt");
    std::stringstream stream;
    Application app(stream);

    std::ifstream test_script("test_script.txt");
    if(test_script)
    {
        std::string line;

        while(std::getline(test_script, line))
        {
            //execute line by line
            app.execute(line);
        }
        //make sure we got the expected output
        std::ifstream expected_output_file("expected_output.txt");
        std::string expected_output;

        expected_output_file.seekg(0, std::ios::end);
        expected_output.reserve(expected_output_file.tellg());
        expected_output_file.seekg(0, std::ios::beg);

        expected_output.assign((std::istreambuf_iterator<char>(expected_output_file)),
                               std::istreambuf_iterator<char>());

        test_output << stream.str();
        assert(expected_output == stream.str());
        std::cout << "test completed sucessfully" << std::endl; //otherwise the assertion failed and we wouldn't be here.
    }
    else
    {
        std::cout << "There is no test_script.txt" << std::endl;
    }

}

int main()
{

    //perform some tests
    test_application();


    //the actual application
    Application app(std::cout);
    std::cout << "Waiting for escape" << std::endl;
    while(app.running())
    {
        Sleep(200);
        while(_kbhit())
        {
            char c = _getch();
            if(c == 27)
            {
                std::cout << "\nEnter Command:" <<std::endl;
                std::string line;
                std::getline(std::cin, line);

                std::stringstream stream(line);
                std::string word;
                while(stream >> word)
                {
                    app.menu.buffer.push_back(word);
                }
                std::cout << std::endl;
                app.menu.execute();
                app.menu.buffer.clear();
            }
        }
    }


    return 0;
}
////check if we can bind to main.
//    app.execute("bind list main list_available_commands");
//    app.execute("list");
//    //check if we successfully bound the list available function
//    assert(sstream.str() == "bind -> bind\nlist -> list_available_commands\n");
//
//    //check if we can create a menu
//    app.execute("bind menu main menu");
//    app.execute("bind create_menu main create_menu");
//    app.execute("create_menu other_menu");
//    //give the other menu the menu function
//    check_statement(app, sstream, "bind menu other_menu menu", "in menu \"other_menu\"\n");
//    check_statement(app, sstream, "menu other_menu");
//    app.execute("menu other_menu");
//
//    //call a function that is not available in this context
//    check_statement(app, sstream, "list", "no function for commmand list\n");
//    //return to main menu
//    app.execute("menu main");
//    app.execute("bind list other_menu list_available_commands");
