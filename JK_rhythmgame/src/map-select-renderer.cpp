#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include <iostream>
#include <cassert>

const sf::Vector2f jk::map_select_renderer::MAINBUTTON_MARGIN = { 0.f, 0.040f };

void jk::map_select_renderer::init(sf::RenderWindow* window) {

	musicButtonsItr_ = std::end(musicButtons_);		//変数初期化
	
	//イベントハンドラ登録
	handlers_ << std::make_pair<sf::Event::EventType, jk::event_handler_t<>>(
		sf::Event::EventType::KeyPressed,
		[this](sf::Event const &e)->std::uint32_t {return this->on_keyboard_down(e); }
	);

	namespace fs = std::filesystem;	
	window_ = window;

	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		try {
			// このvectorのデータ型はshared_ptr<>なので、もし、直接構築するとすればshared_ptrを構築しないといけない。
			beatDirectories_.push_back(std::make_shared<jk::beatmap_directory>(p));		
		}catch (fs::filesystem_error & e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	makeButton();

	sceneflag_ = RUNNING;
}

void jk::map_select_renderer::addButton(std::shared_ptr<jk::beatmap_directory> bd) {
	for (auto & b : *bd) {
		// commented by ouchiminh
		// jk::ui_componentから派生するクラスはjk::ui_mng::createで生成されなければならない。
		// 実はui_mng内でshared_ptrを保持しているため、別で配列を作る必要はないが、ほかのコードの修正が少なく済むように既に定義してある変数は消していない。
		musicButtons_.push_back(components_.create<jk::musicButton>(&b,makeButtonName(b)));
	}
}

void jk::map_select_renderer::makeButton()
{
	for (auto bd : beatDirectories_) {
		addButton(bd);
	}
}

const sf::Text jk::map_select_renderer::makeButtonName(const jk::beatmap& b) const {
	sf::Text name;
	auto sname = b.get_path().stem().generic_string();
	name.setString(sname);

	return name;
}

void jk::map_select_renderer::initButtonPos() {
	sf::Vector2f pivotRe = { 0.578f, 0.216f};		//相対位置(ボタン左上をピボットポイントと想定)
	auto size = window_->getSize();
	for (auto button : this->musicButtons_) {
		// commented by ouchiminh
		// エラーをなくすためにmusicButtons_のテンプレート引数を変更したため、このコードも変更した。
		button->set_position({ size.x * pivotRe.x, size.y * pivotRe.y});
		pivotRe += MAINBUTTON_MARGIN;	
	}
}

std::uint32_t jk::map_select_renderer::on_keyboard_down(const sf::Event & e)
{
	assert(e.type == sf::Event::EventType::KeyPressed);

	switch (e.key.code)
	{
	case sf::Keyboard::Up:
		(*musicButtonsItr_)->updateState(musicButtonState::NOT_SELECTED);			//今選択中のボタンを非選択に変更

		if (musicButtonsItr_ == musicButtons_.begin()) {
			musicButtonsItr_ = musicButtons_.end();
		}
		else {
			--musicButtonsItr_;
		}
		(*musicButtonsItr_)->updateState(musicButtonState::SELECTED);				//今選択中のボタンを選択に変更
		break;

	case sf::Keyboard::Down:
		(*musicButtonsItr_)->updateState(musicButtonState::NOT_SELECTED);			//今選択中のボタンを非選択に変更
		if (musicButtonsItr_ == musicButtons_.end()) {
			musicButtonsItr_ = musicButtons_.begin();
		}
		else {
			++musicButtonsItr_;
		}
		(*musicButtonsItr_)->updateState(musicButtonState::SELECTED);				//今選択中のボタンを選択に変更
		break;

	case sf::Keyboard::Enter:
		(*musicButtonsItr_)->updateState(musicButtonState::CLICKED);				//今選択中のボタンをクリック状態に変更
		sceneflag_ = FINISHED;
		break;

	case sf::Keyboard::Escape:
		sceneflag_ = FINISHED;
		break;

	default:
		return 0;
	}
	return 0;
}

std::optional<jk::beatmap> jk::map_select_renderer::get_selected() const {
	if (musicButtonsItr_ == musicButtons_.end()) return std::nullopt;
	auto bptr = (*musicButtonsItr_)->get_beatmap();
	
	return bptr ? std::optional<jk::beatmap>(*bptr) : std::optional<jk::beatmap>(std::nullopt);
}

jk::SCENEFLAG jk::map_select_renderer::operator() () {
	components_.draw(*window_);
	return sceneflag_;
}

std::uint32_t jk::map_select_renderer::input(const sf::Event & e) noexcept
{
	if (e.type == sf::Event::KeyPressed) {
		switch (e.key.code) {
		case sf::Keyboard::Up:
			(*musicButtonsItr_)->updateState(musicButtonState::NOT_SELECTED);			//今選択中のボタンを非選択に変更
			
			if (musicButtonsItr_ == musicButtons_.begin()) {
				musicButtonsItr_ = musicButtons_.end();
			}else {
				--musicButtonsItr_;
			}
			(*musicButtonsItr_)->updateState(musicButtonState::SELECTED);				//今選択中のボタンを選択に変更
			break;

		case sf::Keyboard::Down:
			(*musicButtonsItr_)->updateState(musicButtonState::NOT_SELECTED);			//今選択中のボタンを非選択に変更
			if (musicButtonsItr_ == musicButtons_.end()) {
				musicButtonsItr_ = musicButtons_.begin();
			}
			else {
				++musicButtonsItr_;
			}
			(*musicButtonsItr_)->updateState(musicButtonState::SELECTED);				//今選択中のボタンを選択に変更
			break;

		case sf::Keyboard::Enter:
			(*musicButtonsItr_)->updateState(musicButtonState::CLICKED);				//今選択中のボタンをクリック状態に変更
			sceneflag_ = FINISHED;
			break;
		
		case sf::Keyboard::Escape:
			sceneflag_ = FINISHED;
			break;
		}	
	}
	return components_.event_procedure(e);
}

void jk::map_select_renderer::free_resource() noexcept
{}