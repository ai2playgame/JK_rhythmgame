#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include "SFML/Window/Event.hpp"
#include "ui-component.hpp"

namespace jk {
	//	error:exception
	template<class ...Args>
	using event_handler_t = std::function<std::uint32_t(const sf::Event &, Args...)>;

	template<class ...Args>
	class event_handlers {
		std::multimap<sf::Event::EventType, event_handler_t<Args...>> handlers_;

	public:
		using this_event_handler_t = event_handler_t<Args...>;
		event_handlers & operator << (std::pair<sf::Event::EventType, event_handler_t<Args...>> && pack) {
			handlers_.insert(pack);
			return *this;
		}
		event_handlers & operator -=(sf::Event::EventType key) {
			handlers_.erase(key);
			return *this;
		}
		result_t operator () (const sf::Event & e, Args ...args) {
			std::uint32_t ret{ 0 };
			result_t did{ 0 };
			did.fallthrough(jk::FALLTHROUGH::GO_FALLTHROUGH);
			if (handlers_.count(e.type)) {
				auto r = handlers_.equal_range(e.type);
				std::uint32_t buf;
				std::for_each(r.first, r.second, [&](auto & a) { ret = (buf = a.second(e, args...)) ? buf : ret; });
				did.processed();
				did.set_retv(ret);
				did.fallthrough();
				return did;
			}
			did.processed(false);
			return did;
		}

		void clear() noexcept { handlers_.clear(); }
	};
}
