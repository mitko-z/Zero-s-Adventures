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
		int slotNumber,
		BUTTON_TYPE slot_type) :
		MenuButton(x, y, w, h, isAnimating, slot_type, false),
		m_slotNumber(slotNumber)
	{}

	void loadContent() override;
	void update() override;
private:
	void updateSlotText();
	std::string getSlotText();
	int m_slotNumber;
};