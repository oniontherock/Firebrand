#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include <ECS.hpp>

namespace ECSRegistry {
	void ECSInitialize();
	void ECSTerminate();
}

// avoid having undefined constructor arguments for events or components,
// as it's more convenient to not have to define every event/component all the time
// whenever you create a new type, ensure you register it in the implementation file of this header

namespace EntityEvents {
	struct EventExample final : public Event {

		EventExample(uint16_t _var = 0) : var(_var) {};

		uint16_t var;

		void clear() final {
			var = 0;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventExample());
		};
	};
}
namespace EntityComponents {
	struct ComponentExample final : public Component {

		void system(Entity& entity) final;

		ComponentExample(uint16_t _var = 0) : var(_var) {
			hasSystem = true;
		};

		uint16_t var;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentExample(var));
		};
		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
}

#endif