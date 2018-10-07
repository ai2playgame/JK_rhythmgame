#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include "color-manager.hpp"
#include <iostream>

const sf::Vector2f jk::map_select_renderer::MAINBUTTON_MARGIN = { 0.f, 0.040f };

void jk::map_select_renderer::init(sf::RenderWindow* window) {
	//�ϐ�������
	window_ = window;

	//�C�x���g�o�^
	handlers_ << std::make_pair < sf::Event::EventType, jk::event_handler_t<> >(
		sf::Event::EventType::KeyPressed,
		[this](sf::Event const& e)->std::uint32_t {return this->on_key_down(e); }
	);
	
	namespace fs = std::filesystem;	
	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		try {
			// ����vector�̃f�[�^�^��shared_ptr<>�Ȃ̂ŁA�����A���ڍ\�z����Ƃ����shared_ptr���\�z���Ȃ��Ƃ����Ȃ��B
			beatDirectories_.push_back(std::make_shared<jk::beatmap_directory>(p.path()));		
		}catch (fs::filesystem_error & e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	makeButton();
	f_.loadFromFile(".\\res\\fonts\\Perfograma.otf");

	musicButtonsItr_ = std::begin(musicButtons_);
	sceneflag_ = RUNNING;
}

void jk::map_select_renderer::addButton(std::shared_ptr<jk::beatmap_directory> bd) {
	for (auto & b : *bd) {
		// commented by ouchiminh
		// jk::ui_component����h������N���X��jk::ui_mng::create�Ő�������Ȃ���΂Ȃ�Ȃ��B
		// ����ui_mng����shared_ptr��ێ����Ă��邽�߁A�ʂŔz������K�v�͂Ȃ����A�ق��̃R�[�h�̏C�������Ȃ��ςނ悤�Ɋ��ɒ�`���Ă���ϐ��͏����Ă��Ȃ��B
		musicButtons_.push_back(components_.create<jk::musicButton>(&b,makeButtonName(b)));
	}
}

void jk::map_select_renderer::makeButton()
{
	for (auto bd : beatDirectories_) {
		addButton(bd);
	}
	initButtonPos();
}

const sf::Text jk::map_select_renderer::makeButtonName(const jk::beatmap& b) const {
	sf::Text name;
	auto sname = b.get_path().stem().generic_string();
	name.setString(sname);

	name.setFillColor(jk::color::color_mng::get("Data.str_color").value_or(jk::color::str_color));
	name.setFont(f_);

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

std::uint32_t jk::map_select_renderer::on_key_down(const sf::Event & e)
{
	assert(e.type == sf::Event::EventType::KeyPressed);
	switch (e.key.code)
	{
	case sf::Keyboard::Up:
		buttonSelect_up();
		break;

	case sf::Keyboard::Down:
		buttonSelect_down();
		break;

	case sf::Keyboard::Enter:
		executeClicked();
		break;

	case sf::Keyboard::Escape:
		backTitle();
		break;
	}
	return 0;
}

void jk::map_select_renderer::buttonSelect_up()
{


	if (musicButtonsItr_ == musicButtons_.begin()) {
		return;
	}
	else {
		(*musicButtonsItr_)->setState(musicButtonState::NOT_SELECTED);			//���I�𒆂̃{�^�����I���ɕύX
		--musicButtonsItr_;
		(*musicButtonsItr_)->setState(musicButtonState::SELECTED);				//�I����ύX
	}

}

void jk::map_select_renderer::buttonSelect_down()
{
	if (musicButtonsItr_ == musicButtons_.end() || musicButtonsItr_+1 == musicButtons_.end()) {
		return;
	}
	else {
		(*musicButtonsItr_)->setState(musicButtonState::NOT_SELECTED);			//���I�𒆂̃{�^�����I���ɕύX
		++musicButtonsItr_;
		(*musicButtonsItr_)->setState(musicButtonState::SELECTED);				//�I����ύX
	}


}

void jk::map_select_renderer::executeClicked()
{
	(*musicButtonsItr_)->setState(musicButtonState::CLICKED);				//���I�𒆂̃{�^�����N���b�N��ԂɕύX
	sceneflag_ = FINISHED;					//�I���t���O�𗧂Ă�
}

void jk::map_select_renderer::backTitle()
{
	sceneflag_ = FINISHED;					//�I���t���O�𗧂Ă�
}

std::optional<jk::beatmap> jk::map_select_renderer::get_selected() const {
	if (musicButtonsItr_ == musicButtons_.end()) return std::nullopt;
	auto bptr = (*musicButtonsItr_)->get_beatmap();
	
	return bptr ? std::optional<jk::beatmap>(*bptr) : std::optional<jk::beatmap>(std::nullopt);
}

jk::SCENEFLAG jk::map_select_renderer::operator() () {
	window_->clear();
	components_.draw(*window_);
	window_->display();
	return sceneflag_;
}

std::uint32_t jk::map_select_renderer::input(const sf::Event & e) noexcept
{
	handlers_(e);
	return components_.event_procedure(e);
}

void jk::map_select_renderer::free_resource() noexcept {}