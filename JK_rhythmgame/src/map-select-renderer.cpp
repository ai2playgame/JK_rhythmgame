#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include <iostream>

const sf::Vector2f jk::map_select_renderer::MAINBUTTON_MARGIN = { 0.f, 0.040f };

void jk::map_select_renderer::init(sf::RenderWindow* window) {
	namespace fs = std::filesystem;	
	window_ = window;

	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		try {
			beatDirectories_.emplace_back(p);		
		}catch (fs::filesystem_error & e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	makeButton();

	sceneflag_ = RUNNING;
}

void jk::map_select_renderer::addButton(std::shared_ptr<jk::beatmap_directory> bd) {
	for (const auto b : *bd) {
		// commented by ouchiminh
		// jk::ui_component����h������N���X��jk::ui_mng::create�Ő�������Ȃ���΂Ȃ�Ȃ��B
		// ����ui_mng����shared_ptr��ێ����Ă��邽�߁A�ʂŔz������K�v�͂Ȃ����A�ق��̃R�[�h�̏C�������Ȃ��ςނ悤�Ɋ��ɒ�`���Ă���ϐ��͏����Ă��Ȃ��B
		musicButtons_.push_back(components_.create<jk::musicButton>(b,makeButtonName(b)));
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
	sf::Vector2f pivotRe = { 0.578f, 0.216f};		//���Έʒu(�{�^��������s�{�b�g�|�C���g�Ƒz��)
	auto size = window_->getSize();
	for (auto button : this->musicButtons_) {
		// commented by ouchiminh
		// �G���[���Ȃ������߂�musicButtons_�̃e���v���[�g������ύX�������߁A���̃R�[�h���ύX�����B
		button->set_position({ size.x * pivotRe.x, size.y * pivotRe.y});
		pivotRe += MAINBUTTON_MARGIN;	
	}
}

std::optional<jk::beatmap> jk::map_select_renderer::get_selected() const {
	auto wPtr = (*musicButtonsItr_)->get_beatmap();
	auto sPtr = wPtr.lock();
	return *sPtr;
}

jk::SCENEFLAG jk::map_select_renderer::operator() () {
	components_.draw(*window_);
	return sceneflag_;
}

std::uint32_t jk::map_select_renderer::input(const sf::Event & e) noexcept
{
	return 1; 
}

void jk::map_select_renderer::free_resource() noexcept{}