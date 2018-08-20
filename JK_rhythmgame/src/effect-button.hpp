#pragma once
#include "sfml-button.hpp"

namespace jk {
	class effect_button;

	template<class ...Arg>
	std::uint32_t place_holder(Arg...args) { return 0; }
	
	// effect starter��effect procedure�͂������̎q�Ƃ�������
	enum effect_type{};

	// effect���͂��߂�ׂ��Ȃ�true��Ԃ��B����ȊO��false
	// �G�t�F�N�g���n�܂������ǂ������f����B(�n�܂��Ă�����K�v�ȏ���������)
	using effect_starter = std::function<bool(const sf::Event &, sf::Sprite &, sf::Text &, jk::effect_button &)>;	
	using effect_procedure = std::function<std::uint32_t(sf::Sprite &, sf::Text &, jk::effect_button &)>;
	
	class effect_button : public button {
		
	};

}
