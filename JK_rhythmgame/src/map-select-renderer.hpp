#pragma once

#include <optional>
#include <filesystem>
#include "SFML/Graphics.hpp"
#include "scene.hpp"
#include "ui-component.hpp"
#include "aes/aes-utl.hpp"
#include "beatmap-directory.hpp"
#include "musicButton.hpp"

namespace jk {

	class map_select_renderer : public renderer {
		enc::aes_utl encoder_;
		sf::RenderWindow* window_;
		ui_mng components_;
		sf::Font f_;
		// commented by ouchiminh
		// button��std::enable_shared_from_this���p�����Ă���̂Ńe���v���[�g�����ς��܂����B
		std::vector<std::shared_ptr<musicButton>> musicButtons_;
		std::vector<std::shared_ptr<musicButton>>::iterator musicButtonsItr_;
		std::vector<std::shared_ptr<beatmap_directory>>  beatDirectories_;

		static const sf::Vector2f MAINBUTTON_MARGIN;		//�ȑI���{�^����margin
		jk::SCENEFLAG sceneflag_;

		event_handlers<> handlers_;
				
	public:
		void init(sf::RenderWindow* window);
		[[nodiscard]] SCENEFLAG operator() () override;
		std::uint32_t input(const sf::Event & e) noexcept override;
		void free_resource() noexcept override;

		[[nodiscard]] std::optional<jk::beatmap> get_selected() const;
	
	private:
		void addButton(std::shared_ptr<beatmap_directory> bd);
		void makeButton();
		const sf::Text makeButtonName(const jk::beatmap& b) const;
		void initButtonPos();
		std::uint32_t on_key_down(const sf::Event &e);	//�L�[�{�[�h���̓C�x���g

		void buttonSelect_up();
		void buttonSelect_down();
		void executeClicked();
		void backTitle();
		
	};
}