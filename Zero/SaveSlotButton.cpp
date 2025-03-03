#include <fstream>
#include <regex>
#include <algorithm>

#include "SaveSlotButton.h"
#include "StateMachine.h"

void SaveSlotButton::loadContent()
{
    updateSlotText();
	MenuButton::loadContent();
}

void SaveSlotButton::update()
{
    if (m_isPressed)
    {
        std::shared_ptr<StateMachine> stateMachine = StateMachine::getInstnce();
        stateMachine->setSaveSlotTo(m_slotNumber);
    }
	MenuButton::update();
    updateSlotText();
}

void SaveSlotButton::updateSlotText()
{
    m_textToDisplay = "Slot " + std::to_string(m_slotNumber) + ": " + getSlotText();
    m_text.setString(m_textToDisplay);
}

std::string SaveSlotButton::getSlotText()
{
    // Construct the file name
    std::string fileName = "saveslot" + std::to_string(m_slotNumber) + ".sav";
    std::string slotText = "   ...EMPTY...";
    // Open the file
    std::ifstream file(fileName);
    if (file.is_open())
    {
        // Read the first line from the file
        std::string firstLine;
        if (std::getline(file, firstLine))
        {
            // Define a regex pattern for the "YYYY-MM-DD HH:MM:SS" format
            std::regex dateTimePattern("^\\d{2}.\\d{2}.\\d{4} \\d{2}:\\d{2}:\\d{2}$");

            // Check if the first line matches the pattern
            if (std::regex_match(firstLine, dateTimePattern))
            {
                // replace the '-' symbol with '.' because the font does not presents correctly the '-' symbol
                std::replace(firstLine.begin(), firstLine.end(), '-', '.');
                slotText = "   " + firstLine;
            }
        }
    }
    return std::string(slotText);
}
