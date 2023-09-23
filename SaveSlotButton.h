#pragma once

#include "MenuButton.h"


class SaveSlotButton : public MenuButton
{
public:
	SaveSlotButton() = default;
	SaveSlotButton(
		double x,
		double y,
		double w,
		double h,
		bool isAnimating,
		int slotNumber) :
		MenuButton(x, y, w, h, isAnimating, BUTTON_TYPE::SAVE_SLOT, false),
		m_slotNumber(slotNumber)
	{}

	void loadContent() override;
	void update() override;
private:
	void updateSlotText();
	std::string getSlotText();
	int m_slotNumber;
};