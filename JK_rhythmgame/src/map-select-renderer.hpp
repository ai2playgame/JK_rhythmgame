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
		// commented by ouchiminh
		// buttonはstd::enable_shared_from_thisを継承しているのでテンプレート引数変えました。
		std::vector<std::shared_ptr<musicButton>> musicButtons_;						
		std::vector<std::shared_ptr<musicButton>>::iterator musicButtonsItr_;
		std::vector<std::shared_ptr<beatmap_directory>>  beatDirectories_;

		static const sf::Vector2f MAINBUTTON_MARGIN;		//曲選択ボタンのmargin
		SCENEFLAG sceneflag_;
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

		std::uint32_t on_keyboard_down(const sf::Event &e);		//キーボード入力イベント
		void iteratorUp();
		void iteratorDown();
		
	};
}
