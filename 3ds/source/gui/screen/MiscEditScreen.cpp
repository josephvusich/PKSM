/*
*   This file is part of PKSM
*   Copyright (C) 2016-2019 Bernardo Giordano, Admiral Fish, piepie62
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "MiscEditScreen.hpp"
#include "gui.hpp"
#include "ClickButton.hpp"
#include "ViewOverlay.hpp"
#include "VersionOverlay.hpp"
#include "LocationOverlay.hpp"
#include "AccelButton.hpp"
#include "PB7.hpp"

MiscEditScreen::MiscEditScreen(std::shared_ptr<PKX> pkm) : pkm(pkm)
{
    buttons.push_back(std::make_unique<ClickButton>(283, 211, 34, 28, [this](){ Gui::screenBack(); return true; }, ui_sheet_button_back_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<ClickButton>(204, 171, 108, 30, [this](){ otAndMet = !otAndMet; return true; }, ui_sheet_button_editor_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<ClickButton>(3, 211, 34, 28, [this](){ validate(); return true; }, ui_sheet_button_wireless_idx, "", 0.0f, 0));

    buttons.push_back(std::make_unique<AccelButton>(94, 34, 13, 13, [this](){ return this->changeMetLevel(false); }, ui_sheet_button_minus_small_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<Button>(109, 34, 31, 13, [this](){ Gui::setNextKeyboardFunc([this](){ setMetLevel(); }); return false; }, ui_sheet_res_null_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<AccelButton>(142, 34, 13, 13, [this](){ return this->changeMetLevel(true); }, ui_sheet_button_plus_small_idx, "", 0.0f, 0));

    buttons.push_back(std::make_unique<Button>(95, 54, 15, 12, [this](){ Gui::setNextKeyboardFunc([this](){ day(); }); return true; }, ui_sheet_button_info_detail_editor_dark_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<ClickButton>(95, 74, 15, 12, [this](){ Gui::setNextKeyboardFunc([this](){ month(); }); return true; }, ui_sheet_button_info_detail_editor_dark_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<Button>(95, 94, 15, 12, [this](){ Gui::setNextKeyboardFunc([this](){ year(); }); return true; }, ui_sheet_button_info_detail_editor_dark_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<Button>(95, 114, 15, 12, [this](){ currentOverlay = std::make_shared<LocationOverlay>(*this, this->pkm, this->otAndMet); return true; }, ui_sheet_button_info_detail_editor_dark_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<Button>(95, 134, 15, 12, [this](){ currentOverlay = std::make_shared<VersionOverlay>(*this, this->pkm); return true; }, ui_sheet_button_info_detail_editor_dark_idx, "", 0.0f, 0));

    if (pkm->generation() > Generation::FIVE)
    {
        buttons.push_back(std::make_unique<AccelButton>(94, 154, 13, 13, [this](){ return this->changeEnjoyment(false); }, ui_sheet_button_minus_small_idx, "", 0.0f, 0));
        buttons.push_back(std::make_unique<Button>(109, 154, 31, 13, [this](){ Gui::setNextKeyboardFunc([this](){ setEnjoyment(); }); return false; }, ui_sheet_res_null_idx, "", 0.0f, 0));
        buttons.push_back(std::make_unique<AccelButton>(142, 154, 13, 13, [this](){ return this->changeEnjoyment(true); }, ui_sheet_button_plus_small_idx, "", 0.0f, 0));
        
        buttons.push_back(std::make_unique<AccelButton>(94, 174, 13, 13, [this](){ return this->changeFullness(false); }, ui_sheet_button_minus_small_idx, "", 0.0f, 0));
        buttons.push_back(std::make_unique<Button>(109, 174, 31, 13, [this](){ Gui::setNextKeyboardFunc([this](){ setFullness(); }); return false; }, ui_sheet_res_null_idx, "", 0.0f, 0));
        buttons.push_back(std::make_unique<AccelButton>(142, 174, 13, 13, [this](){ return this->changeFullness(true); }, ui_sheet_button_plus_small_idx, "", 0.0f, 0));

        buttons.push_back(std::make_unique<AccelButton>(94, 194, 13, 13, [this](){ return this->changeAffection(false); }, ui_sheet_button_minus_small_idx, "", 0.0f, 0));
        buttons.push_back(std::make_unique<Button>(109, 194, 31, 13, [this](){ Gui::setNextKeyboardFunc([this](){ setAffection(); }); return false; }, ui_sheet_res_null_idx, "", 0.0f, 0));
        buttons.push_back(std::make_unique<AccelButton>(142, 194, 13, 13, [this](){ return this->changeAffection(true); }, ui_sheet_button_plus_small_idx, "", 0.0f, 0));
    }
}

void MiscEditScreen::draw() const
{
    C2D_SceneBegin(g_renderTargetBottom);
    Language lang = Configuration::getInstance().language();
    Gui::sprite(ui_sheet_emulated_bg_bottom_blue, 0, 0);
    Gui::sprite(ui_sheet_bg_style_bottom_idx, 0, 0);
    Gui::sprite(ui_sheet_bar_arc_bottom_blue_idx, 0, 206);
    Gui::backgroundAnimatedBottom();

    Gui::sprite(ui_sheet_textbox_name_bottom_idx, 0, 1);

    for (int i = 0; i < 5; i++)
    {
        Gui::sprite(ui_sheet_stripe_info_row_idx, 0, 30 + i * 40);
    }

    for (auto& button : buttons)
    {
        button->draw();
    }

    Gui::staticText(i18n::localize(otAndMet ? "HT_EGG" : "OT_MET"), 254, 186, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::CENTER, TextPosY::CENTER);
    Gui::staticText(i18n::localize("MET_LEVEL"), 5, 32, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::staticText(i18n::localize(otAndMet ? "MET_DAY" : "EGG_DAY"), 5, 52, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::staticText(i18n::localize(otAndMet ? "MET_MONTH" : "EGG_MONTH"), 5, 72, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::staticText(i18n::localize(otAndMet ? "MET_YEAR" : "EGG_YEAR"), 5, 92, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::staticText(i18n::localize(otAndMet ? "MET_LOCATION" : "EGG_LOCATION"), 5, 112, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::staticText(i18n::localize("ORIGIN_GAME"), 5, 132, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    if (pkm->generation() > Generation::FIVE)
    {
        Gui::staticText(i18n::localize("ENJOYMENT"), 5, 152, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
        Gui::staticText(i18n::localize("FULLNESS"), 5, 172, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
        Gui::staticText(i18n::localize(otAndMet ? "OT_AFFECTION" : "HT_AFFECTION"), 5, 192, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    }

    Gui::dynamicText(std::to_string((int)pkm->metLevel()), 107 + 35 / 2, 32, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::CENTER, TextPosY::TOP);
    Gui::dynamicText(std::to_string((int)(otAndMet ? pkm->metDay() : pkm->eggDay())), 115, 52, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::dynamicText(std::to_string((int)(otAndMet ? pkm->metMonth() : pkm->eggMonth())), 115, 72, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    int print = otAndMet ? pkm->metYear() : pkm->eggYear();
    if (print < 2000)
    {
        print += 2000;
    }
    Gui::dynamicText(std::to_string(print), 115, 92, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::dynamicText(i18n::location(lang, otAndMet ? pkm->metLocation() : pkm->eggLocation(), pkm->version()), 115, 112, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    Gui::dynamicText(i18n::game(lang, pkm->version()), 115, 132, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::LEFT, TextPosY::TOP);
    if (pkm->generation() > Generation::FIVE)
    {
        if (pkm->generation() == Generation::SIX)
        {
            print = ((PK6*)pkm.get())->enjoyment();
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            print = ((PK7*)pkm.get())->enjoyment();
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            print = ((PB7*)pkm.get())->enjoyment();
        }
        Gui::staticText(std::to_string(print), 107 + 35 / 2, 152, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::CENTER, TextPosY::TOP);
        if (pkm->generation() == Generation::SIX)
        {
            print = ((PK6*)pkm.get())->fullness();
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            print = ((PK7*)pkm.get())->fullness();
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            print = ((PB7*)pkm.get())->fullness();
        }
        Gui::staticText(std::to_string(print), 107 + 35 / 2, 172, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::CENTER, TextPosY::TOP);
        if (otAndMet)
        {
            if (pkm->generation() == Generation::SIX)
            {
                print = ((PK6*)pkm.get())->otAffection();
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                print = ((PK7*)pkm.get())->otAffection();
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                print = ((PB7*)pkm.get())->otAffection();
            }
        }
        else
        {
            if (pkm->generation() == Generation::SIX)
            {
                print = ((PK6*)pkm.get())->htAffection();
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                print = ((PK7*)pkm.get())->htAffection();
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                print = ((PB7*)pkm.get())->htAffection();
            }
        }
        Gui::staticText(std::to_string(print), 107 + 35 / 2, 192, FONT_SIZE_12, FONT_SIZE_12, COLOR_BLACK, TextPosX::CENTER, TextPosY::TOP);
    }
}

void MiscEditScreen::update(touchPosition* touch)
{
    if (!currentOverlay)
    {
        currentOverlay = std::make_shared<ViewOverlay>(*this, pkm, false);
    }

    u32 kDown = hidKeysDown();
    if (justSwitched)
    {
        if (hidKeysHeld() & KEY_TOUCH)
        {
            return;
        }
        else
        {
            justSwitched = false;
        }
    }

    if (kDown & KEY_B)
    {
        Gui::screenBack();
        return;
    }
    // else if (kDown & KEY_Y)
    // {
    //     coreConsole();
    //     return;
    // }

    for (auto& button : buttons)
    {
        if (button->update(touch))
        {
            return;
        }
    }
}

bool MiscEditScreen::changeAffection(bool up)
{
    if (otAndMet)
    {
        if (up)
        {
            if (pkm->generation() == Generation::SIX)
            {
                ((PK6*)pkm.get())->otAffection(((PK6*)pkm.get())->otAffection() + 1);
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                ((PK7*)pkm.get())->otAffection(((PK7*)pkm.get())->otAffection() + 1);
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                ((PB7*)pkm.get())->otAffection(((PB7*)pkm.get())->otAffection() + 1);
            }
        }
        else
        {
            if (pkm->generation() == Generation::SIX)
            {
                ((PK6*)pkm.get())->otAffection(((PK6*)pkm.get())->otAffection() - 1);
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                ((PK7*)pkm.get())->otAffection(((PK7*)pkm.get())->otAffection() - 1);
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                ((PB7*)pkm.get())->otAffection(((PB7*)pkm.get())->otAffection() - 1);
            }
        }
    }
    else
    {
        if (up)
        {
            if (pkm->generation() == Generation::SIX)
            {
                ((PK6*)pkm.get())->htAffection(((PK6*)pkm.get())->htAffection() + 1);
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                ((PK7*)pkm.get())->htAffection(((PK7*)pkm.get())->htAffection() + 1);
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                ((PB7*)pkm.get())->htAffection(((PB7*)pkm.get())->htAffection() + 1);
            }
        }
        else
        {
            if (pkm->generation() == Generation::SIX)
            {
                ((PK6*)pkm.get())->htAffection(((PK6*)pkm.get())->htAffection() - 1);
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                ((PK7*)pkm.get())->htAffection(((PK7*)pkm.get())->htAffection() - 1);
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                ((PB7*)pkm.get())->htAffection(((PB7*)pkm.get())->htAffection() - 1);
            }
        }
    }
    return false;
}

void MiscEditScreen::setAffection()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 3);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[4] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[3] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        u8 affection = (u8) std::min(std::stoi(input), 255);
        if (otAndMet)
        {
            if (pkm->generation() == Generation::SIX)
            {
                ((PK6*)pkm.get())->otAffection(affection);
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                ((PK7*)pkm.get())->otAffection(affection);
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                ((PB7*)pkm.get())->otAffection(affection);
            }
        }
        else
        {
            if (pkm->generation() == Generation::SIX)
            {
                ((PK6*)pkm.get())->htAffection(affection);
            }
            else if (pkm->generation() == Generation::SEVEN)
            {
                ((PK7*)pkm.get())->htAffection(affection);
            }
            else if (pkm->generation() == Generation::LGPE)
            {
                ((PB7*)pkm.get())->htAffection(affection);
            }
        }
    }
}

bool MiscEditScreen::changeFullness(bool up)
{
    if (up)
    {
        if (pkm->generation() == Generation::SIX)
        {
            ((PK6*)pkm.get())->fullness(((PK6*)pkm.get())->fullness() + 1);
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            ((PK7*)pkm.get())->fullness(((PK7*)pkm.get())->fullness() + 1);
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            ((PB7*)pkm.get())->fullness(((PB7*)pkm.get())->fullness() + 1);
        }
    }
    else
    {
        if (pkm->generation() == Generation::SIX)
        {
            ((PK6*)pkm.get())->fullness(((PK6*)pkm.get())->fullness() - 1);
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            ((PK7*)pkm.get())->fullness(((PK7*)pkm.get())->fullness() - 1);
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            ((PB7*)pkm.get())->fullness(((PB7*)pkm.get())->fullness() - 1);
        }
    }
    return false;
}

void MiscEditScreen::setFullness()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 3);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[4] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[3] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        u8 fullness = (u8) std::min(std::stoi(input), 255);
        if (pkm->generation() == Generation::SIX)
        {
            ((PK6*)pkm.get())->fullness(fullness);
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            ((PK7*)pkm.get())->fullness(fullness);
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            ((PB7*)pkm.get())->fullness(fullness);
        }
    }
}

bool MiscEditScreen::changeEnjoyment(bool up)
{
    if (up)
    {
        if (pkm->generation() == Generation::SIX)
        {
            ((PK6*)pkm.get())->enjoyment(((PK6*)pkm.get())->enjoyment() + 1);
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            ((PK7*)pkm.get())->enjoyment(((PK7*)pkm.get())->enjoyment() + 1);
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            ((PB7*)pkm.get())->enjoyment(((PB7*)pkm.get())->enjoyment() + 1);
        }
    }
    else
    {
        if (pkm->generation() == Generation::SIX)
        {
            ((PK6*)pkm.get())->enjoyment(((PK6*)pkm.get())->enjoyment() - 1);
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            ((PK7*)pkm.get())->enjoyment(((PK7*)pkm.get())->enjoyment() - 1);
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            ((PB7*)pkm.get())->enjoyment(((PB7*)pkm.get())->enjoyment() - 1);
        }
    }
    return false;
}

void MiscEditScreen::setEnjoyment()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 3);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[4] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[3] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        u8 enjoyment = (u8) std::min(std::stoi(input), 255);
        if (pkm->generation() == Generation::SIX)
        {
            ((PK6*)pkm.get())->enjoyment(enjoyment);
        }
        else if (pkm->generation() == Generation::SEVEN)
        {
            ((PK7*)pkm.get())->enjoyment(enjoyment);
        }
        else if (pkm->generation() == Generation::LGPE)
        {
            ((PB7*)pkm.get())->enjoyment(enjoyment);
        }
    }
}

bool MiscEditScreen::changeMetLevel(bool up)
{
    if (up)
    {
        pkm->metLevel(pkm->metLevel() + 1);
    }
    else
    {
        pkm->metLevel(pkm->metLevel() - 1);
    }
    return false;
}

void MiscEditScreen::setMetLevel()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 3);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[4] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[3] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        u8 level = (u8) std::min(std::stoi(input), 100);
        pkm->metLevel(level);
    }
}

void MiscEditScreen::day()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 2);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[3] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[2] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        u8 day = (u8) std::max(1, std::min(std::stoi(input), 31));
        if (otAndMet)
        {
            pkm->metDay(day);
        }
        else
        {
            pkm->eggDay(day);
        }
    }
}

void MiscEditScreen::month()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 2);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[3] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[2] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        u8 month = (u8) std::max(1, std::min(std::stoi(input), 12));
        if (otAndMet)
        {
            pkm->metMonth(month);
        }
        else
        {
            pkm->eggMonth(month);
        }
    }
}

void MiscEditScreen::year()
{
    static SwkbdState state;
    static bool first = true;
    if (first)
    {
        swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, 4);
        first = false;
    }
    swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
    swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
    char input[5] = {0};
    SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
    input[4] = '\0';
    if (ret == SWKBD_BUTTON_CONFIRM)
    {
        int year = std::stoi(input);
        if (year > 2000)
        {
            year -= 2000;
        }
        if (otAndMet)
        {
            pkm->metYear((u8)year);
        }
        else
        {
            pkm->eggYear((u8)year);
        }
    }
}

void MiscEditScreen::validate()
{
    return;
}