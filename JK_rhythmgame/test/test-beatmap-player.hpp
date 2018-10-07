#pragma once
#include "test.hpp"
#include "../src/beatmap-player.hpp"
#include "../src/ui-component.hpp"
#include "SFML/Graphics.hpp"

namespace jk::test {
	DEFINE_TEST(beatmap_player_test) {
		sf::RenderWindow window(sf::VideoMode(1280,720), "test", sf::Style::Titlebar | sf::Style::Close);
		sf::Event event;
		
		jk::ui_mng mng;
		jk::beatmap b;
		auto player = mng.create<jk::beatmap_player>(b, sf::Vector2i(1280, 720));

		while (window.isOpen()) {
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				else mng.event_procedure(event);
			}
			player->update();
			mng.draw(window);
		}
	}
}
