#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <cstring>
#include <atlstr.h>
#include <cwchar>
#include <vector>
#include <ctime>
#include <cmath>
#include <random>
#include <queue>
#include <thread>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "Zengine.h"
using namespace std;
int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 6;
    sf::RenderWindow window(sf::VideoMode(1240, 800), "Map Maker", sf::Style::Default, settings);
    font.loadFromFile("STXIHEI.TTF");
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    Z_View V = { 0,0,1,1,0,0 };
    Z_Shape s1;
    //cout << "s";
    Z_Line L[100] = {
        Z_Line{{-100,-50},{0,0} },
        Z_Line{{0,0},{-100,-200} },
        Z_Line{{-100,-200},{100,100} },
        Z_Line{{100,100},{-100,-50} }
    };
    s1.Divide(L, 4);
    while (window.isOpen()) {
        window.clear(sf::Color::White);
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Window title");
        ImGui::Text("window 1");
        ImGui::Text("window 2");
        ImGui::Text("window 3");
        ImGui::End();
        s1.Render(&window, V);


 
        ImGui::SFML::Render(window);
        window.display();
        
    }
    ImGui::SFML::Shutdown();
    return 0;
}