#ifndef __PANELS_H__
#define __PANELS_H__

#include "GameLevel.hpp"
#include <Graphics/Panel.hpp>

enum PanelTypes : uint16_t {
	StaticView,
	DynamicView,
	Hud,
	WinScreen,
	LoseScreen,
};

struct PanelStaticView : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void panelUpdate() final;

	// the texture of this panel, ungrayscaled, needed for some stuff as this panel is drawn grayscaled
	sf::Texture textureUngrayscaled;

private:
	void checkModeChange();

	void backgroundDraw(GameLevel* levelActive);
	void charactersDraw(GameLevel* levelActive);
	// applies a grayscale to this texture, saves the ungrayscaled version to textureUngrayscaled
	void panelApplyGrayscale();
};

struct PanelDynamicView : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:

	sf::RenderTexture viewMaskTexture;
	sf::ConvexShape viewMaskShapeCreate(float radius, float coneSize, uint16_t pointsCount);
	void viewMaskCreate();
	bool isFirstUpdate = true;

	void checkModeChange();

	// draws the static panel's textureUngrayscaled on this panel, we do this so that we can get a non-grayscaled version of the static stuff
	void staticDraw();
	void charactersDraw(GameLevel* levelActive);
	void viewMaskApply();
};
struct PanelHud : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
		PathsRender,
		ObjectsRender,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:
	void checkModeChange();
};

struct PanelWinScreen : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:
	void checkModeChange();

	// draws background of win screen
	void backgroundDraw();
	// draws win text
	void textDraw();
};

struct PanelLoseScreen : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:
	void checkModeChange();

	// draws background of win screen
	void backgroundDraw();
	// draws win text
	void textDraw();
};

#endif