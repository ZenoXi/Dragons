#include "App.h" // App.h must be included first
#include "EntryScene.h"

#include "Core/GameConstants.h"
#include "Helper/Functions.h"

#include <fstream>
#include <filesystem>

EntryScene::EntryScene(App* app)
    : Scene(app)
{}

void EntryScene::_Init(const SceneOptionsBase* options)
{
    EntrySceneOptions opt;
    if (options)
    {
        opt = *reinterpret_cast<const EntrySceneOptions*>(options);
    }

    core.InitState();

    _board = Create<zcom::Board>(&core);
    _board->SetParentSizePercent(1.0f, 1.0f);
    zcom::PROP_Shadow shadowProp;
    shadowProp.color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f);
    shadowProp.blurStandardDeviation = 5.0f;
    _board->SetProperty(shadowProp);
    _board->SetBackgroundColor(D2D1::ColorF(0, 0.0f));

    _canvas->AddComponent(_board.get());
    _canvas->SetBackgroundColor(D2D1::ColorF(0.2f, 0.12f, 0.06f));
}

void EntryScene::_Uninit()
{
    _canvas->ClearComponents();
}

void EntryScene::_Focus()
{

}

void EntryScene::_Unfocus()
{

}

void EntryScene::_Update()
{
    _canvas->Update();

    _ProcessNewInputRequests();
    _UpdateInputRequests();
}

void EntryScene::_Resize(int width, int height)
{

}

void EntryScene::_ProcessNewInputRequests()
{
    UserInputRequest* request = _board->GetUserInputRequest();
    if (!_gettingUserInput && request)
    {
        switch (request->inputType)
        {
        case UserInputType::WAIT_FOR_CONFIRMATION:
        {
            _waitingForConfirmation = true;
            _gettingUserInput = true;

            _confirmationButton = Create<zcom::Button>(request->inputPrompt);
            _confirmationButton->Text()->SetFont(L"Arial");
            _confirmationButton->Text()->SetFontSize(50.0f);
            _confirmationButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            float textWidth = _confirmationButton->Text()->GetTextWidth();
            _confirmationButton->SetBaseSize(std::max(150.0f, textWidth + 80.0f), 80.0f);
            _confirmationButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
            _confirmationButton->SetVerticalOffsetPixels(-100.0f);
            _confirmationButton->SetZIndex(10);
            _confirmationButton->SetBorderVisibility(true);
            _confirmationButton->SetBorderWidth(4.0f);
            _confirmationButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
            _confirmationButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            zcom::PROP_Shadow shadowProp;
            shadowProp.color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
            shadowProp.blurStandardDeviation = 25.0f;
            _confirmationButton->SetProperty(shadowProp);
            _confirmationButton->SetActivation(zcom::ButtonActivation::RELEASE);
            _confirmationButton->SetOnActivated([&]()
            {
                _waitingForConfirmation = false;
                _gettingUserInput = false;
                _board->SendUserInputResponse();

                _canvas->RemoveComponent(_confirmationButton.get());
            });

            _canvas->AddComponent(_confirmationButton.get());

            break;
        }
        case UserInputType::YES_OR_NO:
        {
            _waitingForYesOrNoChoice = true;
            _gettingUserInput = true;

            auto params = reinterpret_cast<UserInputParams_YesOrNo*>(request->inputParams.get());

            _yesOrNoPanel = Create<zcom::Panel>();
            _yesOrNoPanel->SetBaseSize(600, 400);
            _yesOrNoPanel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _yesOrNoPanel->SetZIndex(10);
            _yesOrNoPanel->SetBackgroundColor(D2D1::ColorF(0, 0.8f));

            _yesOrNoPromptLabel = Create<zcom::Label>(request->inputPrompt);
            _yesOrNoPromptLabel->SetBaseSize(540, 260);
            _yesOrNoPromptLabel->SetHorizontalAlignment(zcom::Alignment::CENTER);
            _yesOrNoPromptLabel->SetVerticalOffsetPixels(40);
            _yesOrNoPromptLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _yesOrNoPromptLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _yesOrNoPromptLabel->SetFont(L"Arial");
            _yesOrNoPromptLabel->SetFontSize(36.0f);
            _yesOrNoPromptLabel->SetWordWrap(true);

            _yesButton = Create<zcom::Button>(params->yesText);
            _yesButton->Text()->SetFont(L"Arial");
            _yesButton->Text()->SetFontSize(36.0f);
            _yesButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _yesButton->Text()->SetFontColor(D2D1::ColorF(0.5f, 0.8f, 0.5f));
            _yesButton->SetBaseSize(120.0f, 60.0f);
            _yesButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
            _yesButton->SetOffsetPixels(-80.0f, -40.0f);
            _yesButton->SetBorderVisibility(true);
            _yesButton->SetBorderWidth(4.0f);
            _yesButton->SetBorderColor(D2D1::ColorF(0.5f, 0.8f, 0.5f));
            _yesButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _yesButton->SetOnActivated([&, request]()
            {
                _waitingForYesOrNoChoice = false;
                _gettingUserInput = false;
                auto params = reinterpret_cast<UserInputParams_YesOrNo*>(request->inputParams.get());
                params->yes = true;
                _board->SendUserInputResponse();

                _canvas->RemoveComponent(_yesOrNoPanel.get());
                _yesOrNoPanel->ClearItems();
            });

            _noButton = Create<zcom::Button>(params->noText);
            _noButton->Text()->SetFont(L"Arial");
            _noButton->Text()->SetFontSize(36.0f);
            _noButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _noButton->Text()->SetFontColor(D2D1::ColorF(0.8f, 0.5f, 0.5f));
            _noButton->SetBaseSize(120.0f, 60.0f);
            _noButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
            _noButton->SetOffsetPixels(80.0f, -40.0f);
            _noButton->SetBorderVisibility(true);
            _noButton->SetBorderWidth(4.0f);
            _noButton->SetBorderColor(D2D1::ColorF(0.8f, 0.5f, 0.5f));
            _noButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _noButton->SetOnActivated([&, request]()
            {
                _waitingForYesOrNoChoice = false;
                _gettingUserInput = false;
                auto params = reinterpret_cast<UserInputParams_YesOrNo*>(request->inputParams.get());
                params->yes = false;
                _board->SendUserInputResponse();

                _canvas->RemoveComponent(_yesOrNoPanel.get());
                _yesOrNoPanel->ClearItems();
            });

            _yesOrNoPanel->AddItem(_yesOrNoPromptLabel.get());
            _yesOrNoPanel->AddItem(_yesButton.get());
            _yesOrNoPanel->AddItem(_noButton.get());
            _canvas->AddComponent(_yesOrNoPanel.get());

            break;
        }
        case UserInputType::CHOOSE_NUMBER:
        {
            _waitingForNumberChoice = true;
            _gettingUserInput = true;

            auto params = reinterpret_cast<UserInputParams_ChooseNumber*>(request->inputParams.get());
            int minNumber = params->lowerBound;
            int maxNumber = params->upperBound;
            _chosenNumber = minNumber;

            _numberChoicePanel = Create<zcom::Panel>();
            _numberChoicePanel->SetBaseSize(700, 300);
            _numberChoicePanel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _numberChoicePanel->SetZIndex(10);
            _numberChoicePanel->SetBackgroundColor(D2D1::ColorF(0, 0.8f));

            _numberChoicePromptLabel = Create<zcom::Label>(request->inputPrompt);
            _numberChoicePromptLabel->SetBaseSize(640, 100);
            _numberChoicePromptLabel->SetHorizontalAlignment(zcom::Alignment::CENTER);
            _numberChoicePromptLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _numberChoicePromptLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _numberChoicePromptLabel->SetFont(L"Arial");
            _numberChoicePromptLabel->SetFontSize(36.0f);
            _numberChoicePromptLabel->SetWordWrap(true);

            _chosenNumberLabel = Create<zcom::Label>(string_to_wstring(int_to_str(_chosenNumber)));
            _chosenNumberLabel->SetBaseSize(100, 100);
            _chosenNumberLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _chosenNumberLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _chosenNumberLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _chosenNumberLabel->SetFont(L"Arial");
            _chosenNumberLabel->SetFontSize(64.0f);
            _chosenNumberLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _plus1Button = Create<zcom::Button>(L"+1");
            _plus1Button->SetBaseSize(60, 60);
            _plus1Button->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _plus1Button->SetHorizontalOffsetPixels(130);
            _plus1Button->Text()->SetFont(L"Arial");
            _plus1Button->Text()->SetFontSize(36.0f);
            _plus1Button->Text()->SetFontColor(D2D1::ColorF(0.5f, 0.8f, 0.5f));
            _plus1Button->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _plus1Button->SetCornerRounding(8.0f);
            _plus1Button->SetSelectedBorderColor(D2D1::ColorF(0, 0.0f));
            _plus1Button->SetOnActivated([&, maxNumber]()
            {
                _chosenNumber += 1;
                if (_chosenNumber > maxNumber)
                    _chosenNumber = maxNumber;
                _chosenNumberLabel->SetText(string_to_wstring(int_to_str(_chosenNumber)));
            });

            _plus2Button = Create<zcom::Button>(L"+2");
            _plus2Button->SetBaseSize(60, 60);
            _plus2Button->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _plus2Button->SetHorizontalOffsetPixels(210);
            _plus2Button->Text()->SetFont(L"Arial");
            _plus2Button->Text()->SetFontSize(36.0f);
            _plus2Button->Text()->SetFontColor(D2D1::ColorF(0.5f, 0.8f, 0.5f));
            _plus2Button->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _plus2Button->SetCornerRounding(8.0f);
            _plus2Button->SetSelectedBorderColor(D2D1::ColorF(0, 0.0f));
            _plus2Button->SetOnActivated([&, maxNumber]()
            {
                _chosenNumber += 2;
                if (_chosenNumber > maxNumber)
                    _chosenNumber = maxNumber;
                _chosenNumberLabel->SetText(string_to_wstring(int_to_str(_chosenNumber)));
            });

            _plus5Button = Create<zcom::Button>(L"+5");
            _plus5Button->SetBaseSize(60, 60);
            _plus5Button->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _plus5Button->SetHorizontalOffsetPixels(290);
            _plus5Button->Text()->SetFont(L"Arial");
            _plus5Button->Text()->SetFontSize(36.0f);
            _plus5Button->Text()->SetFontColor(D2D1::ColorF(0.5f, 0.8f, 0.5f));
            _plus5Button->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _plus5Button->SetCornerRounding(8.0f);
            _plus5Button->SetSelectedBorderColor(D2D1::ColorF(0, 0.0f));
            _plus5Button->SetOnActivated([&, maxNumber]()
            {
                _chosenNumber += 5;
                if (_chosenNumber > maxNumber)
                    _chosenNumber = maxNumber;
                _chosenNumberLabel->SetText(string_to_wstring(int_to_str(_chosenNumber)));
            });

            _minus1Button = Create<zcom::Button>(L"-1");
            _minus1Button->SetBaseSize(60, 60);
            _minus1Button->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _minus1Button->SetHorizontalOffsetPixels(-130);
            _minus1Button->Text()->SetFont(L"Arial");
            _minus1Button->Text()->SetFontSize(36.0f);
            _minus1Button->Text()->SetFontColor(D2D1::ColorF(0.8f, 0.5f, 0.5f));
            _minus1Button->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _minus1Button->SetCornerRounding(8.0f);
            _minus1Button->SetSelectedBorderColor(D2D1::ColorF(0, 0.0f));
            _minus1Button->SetOnActivated([&, minNumber]()
            {
                _chosenNumber -= 1;
                if (_chosenNumber < minNumber)
                    _chosenNumber = minNumber;
                _chosenNumberLabel->SetText(string_to_wstring(int_to_str(_chosenNumber)));
            });

            _minus2Button = Create<zcom::Button>(L"-2");
            _minus2Button->SetBaseSize(60, 60);
            _minus2Button->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _minus2Button->SetHorizontalOffsetPixels(-210);
            _minus2Button->Text()->SetFont(L"Arial");
            _minus2Button->Text()->SetFontSize(36.0f);
            _minus2Button->Text()->SetFontColor(D2D1::ColorF(0.8f, 0.5f, 0.5f));
            _minus2Button->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _minus2Button->SetCornerRounding(8.0f);
            _minus2Button->SetSelectedBorderColor(D2D1::ColorF(0, 0.0f));
            _minus2Button->SetOnActivated([&, minNumber]()
            {
                _chosenNumber -= 2;
                if (_chosenNumber < minNumber)
                    _chosenNumber = minNumber;
                _chosenNumberLabel->SetText(string_to_wstring(int_to_str(_chosenNumber)));
            });

            _minus5Button = Create<zcom::Button>(L"-5");
            _minus5Button->SetBaseSize(60, 60);
            _minus5Button->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _minus5Button->SetHorizontalOffsetPixels(-290);
            _minus5Button->Text()->SetFont(L"Arial");
            _minus5Button->Text()->SetFontSize(36.0f);
            _minus5Button->Text()->SetFontColor(D2D1::ColorF(0.8f, 0.5f, 0.5f));
            _minus5Button->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _minus5Button->SetCornerRounding(8.0f);
            _minus5Button->SetSelectedBorderColor(D2D1::ColorF(0, 0.0f));
            _minus5Button->SetOnActivated([&, minNumber]()
            {
                _chosenNumber -= 5;
                if (_chosenNumber < minNumber)
                    _chosenNumber = minNumber;
                _chosenNumberLabel->SetText(string_to_wstring(int_to_str(_chosenNumber)));
            });

            _numberChoiceConfirmButton = Create<zcom::Button>(L"Confirm");
            _numberChoiceConfirmButton->Text()->SetFont(L"Arial");
            _numberChoiceConfirmButton->Text()->SetFontSize(36.0f);
            _numberChoiceConfirmButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _numberChoiceConfirmButton->SetBaseSize(200.0f, 60.0f);
            _numberChoiceConfirmButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
            _numberChoiceConfirmButton->SetVerticalOffsetPixels(-20.0f);
            _numberChoiceConfirmButton->SetBorderVisibility(true);
            _numberChoiceConfirmButton->SetBorderWidth(4.0f);
            _numberChoiceConfirmButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
            _numberChoiceConfirmButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _numberChoiceConfirmButton->SetOnActivated([&, request]()
            {
                _waitingForNumberChoice = false;
                _gettingUserInput = false;
                auto params = reinterpret_cast<UserInputParams_ChooseNumber*>(request->inputParams.get());
                params->chosenNumber = _chosenNumber;
                _board->SendUserInputResponse();
                _board->DisableNoClickMode();

                _canvas->RemoveComponent(_numberChoicePanel.get());
                _numberChoicePanel->ClearItems();
            });

            _numberChoicePanel->AddItem(_numberChoicePromptLabel.get());
            _numberChoicePanel->AddItem(_chosenNumberLabel.get());
            _numberChoicePanel->AddItem(_plus1Button.get());
            _numberChoicePanel->AddItem(_plus2Button.get());
            _numberChoicePanel->AddItem(_plus5Button.get());
            _numberChoicePanel->AddItem(_minus1Button.get());
            _numberChoicePanel->AddItem(_minus2Button.get());
            _numberChoicePanel->AddItem(_minus5Button.get());
            _numberChoicePanel->AddItem(_numberChoiceConfirmButton.get());
            _canvas->AddComponent(_numberChoicePanel.get());

            _board->EnableNoClickMode();

            break;
        }
        case UserInputType::CHOOSE_OPTION:
        {
            _waitingForChoice = true;
            _gettingUserInput = true;

            auto params = reinterpret_cast<UserInputParams_ChooseOption*>(request->inputParams.get());
            int optionCount = params->options.size();

            _choicePanel = Create<zcom::Panel>();
            _choicePanel->SetBaseSize(900, 120 + optionCount * 80 + 30);
            _choicePanel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _choicePanel->SetZIndex(10);
            _choicePanel->SetBackgroundColor(D2D1::ColorF(0, 0.8f));

            _choicePromptLabel = Create<zcom::Label>(request->inputPrompt);
            _choicePromptLabel->SetBaseSize(840, 120);
            _choicePromptLabel->SetHorizontalAlignment(zcom::Alignment::CENTER);
            _choicePromptLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _choicePromptLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _choicePromptLabel->SetFont(L"Arial");
            _choicePromptLabel->SetFontSize(48.0f);
            _choicePromptLabel->SetWordWrap(true);

            for (int i = 0; i < optionCount; i++)
            {
                auto choiceButton = Create<zcom::Button>(params->options[i]);
                choiceButton->Text()->SetFont(L"Arial");
                choiceButton->Text()->SetFontSize(28.0f);
                choiceButton->SetBaseSize(840.0f, 80.0f);
                choiceButton->SetHorizontalAlignment(zcom::Alignment::CENTER);
                choiceButton->SetVerticalOffsetPixels(120.0f + i * 80);
                choiceButton->Text()->SetWordWrap(true);
                choiceButton->SetOnActivated([&, request, i]()
                {
                    _waitingForChoice = false;
                    _gettingUserInput = false;

                    auto params = reinterpret_cast<UserInputParams_ChooseOption*>(request->inputParams.get());
                    params->chosenOptionIndex = i;
                    _board->SendUserInputResponse();
                    _board->DisableNoClickMode();

                    _CleanUpAfterChoice();
                });

                _choicePanel->AddItem(choiceButton.release(), true);
            }

            _choicePanel->AddItem(_choicePromptLabel.get());
            _canvas->AddComponent(_choicePanel.get());

            _board->EnableNoClickMode();

            break;
        }
        case UserInputType::CHOOSE_CARD_FROM_HAND:
        {
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(request->inputParams.get());

            if (params->maxCardCount <= 0)
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingToChooseCardsFromHand = true;
            _gettingUserInput = true;

            _chooseCardsFromHandLabel = Create<zcom::Label>(request->inputPrompt);
            _chooseCardsFromHandLabel->SetBaseSize(600, 180);
            _chooseCardsFromHandLabel->SetVerticalOffsetPixels(60);
            _chooseCardsFromHandLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
            _chooseCardsFromHandLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _chooseCardsFromHandLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _chooseCardsFromHandLabel->SetFont(L"Arial");
            _chooseCardsFromHandLabel->SetFontSize(36.0f);
            _chooseCardsFromHandLabel->SetWordWrap(true);
            _chooseCardsFromHandLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
            _chooseCardsFromHandLabel->Resize(_chooseCardsFromHandLabel->GetBaseWidth(), 100);
            _chooseCardsFromHandLabel->SetBaseHeight(_chooseCardsFromHandLabel->GetTextHeight() + 100);

            _chooseCardsFromHandDoneButton = Create<zcom::Button>(L"Done");
            _chooseCardsFromHandDoneButton->Text()->SetFont(L"Arial");
            _chooseCardsFromHandDoneButton->Text()->SetFontSize(36.0f);
            _chooseCardsFromHandDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _chooseCardsFromHandDoneButton->SetBaseSize(150.0f, 60.0f);
            _chooseCardsFromHandDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _chooseCardsFromHandDoneButton->SetZIndex(10);
            _chooseCardsFromHandDoneButton->SetBorderVisibility(true);
            _chooseCardsFromHandDoneButton->SetBorderWidth(4.0f);
            _chooseCardsFromHandDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
            _chooseCardsFromHandDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _chooseCardsFromHandDoneButton->SetOnActivated([&, request]()
            {
                _waitingToChooseCardsFromHand = false;
                _gettingUserInput = false;

                auto params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(request->inputParams.get());
                params->chosenCards.clear();
                for (auto card : _chosenCardsFromHand)
                    params->chosenCards.push_back(card);
                _board->SendUserInputResponse();
                _board->DisableChooseCardFromHandMode();

                _canvas->RemoveComponent(_chooseCardsFromHandLabel.get());
                _canvas->RemoveComponent(_chooseCardsFromHandDoneButton.get());
            });
            _chooseCardsFromHandDoneButton->SetVisible(params->minCardCount <= 0);

            _canvas->AddComponent(_chooseCardsFromHandLabel.get());
            _canvas->AddComponent(_chooseCardsFromHandDoneButton.get());

            _chosenCardsFromHand.clear();
            _board->EnableChooseCardFromHandMode(
                params->choosingPlayerIndex,
                params->handPlayerIndex,
                params->maxCardCount,
                params->allowedTypes,
                [&, request](cards::Card* selectedCard)
                {
                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(request->inputParams.get());

                    _chosenCardsFromHand.insert(selectedCard);
                    if (_chosenCardsFromHand.size() >= params->minCardCount)
                        _chooseCardsFromHandDoneButton->SetVisible(true);
                },
                [&, request](cards::Card* deselectedCard)
                {
                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(request->inputParams.get());

                    _chosenCardsFromHand.erase(deselectedCard);
                    if (_chosenCardsFromHand.size() < params->minCardCount)
                        _chooseCardsFromHandDoneButton->SetVisible(false);
                }
            );

            break;
        }
        case UserInputType::CHOOSE_CARD_FROM_ACTIVE_CARDS:
        {
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(request->inputParams.get());

            if (params->maxCardCount <= 0)
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingToChooseCardsFromActiveCards = true;
            _gettingUserInput = true;

            _chooseCardsFromActiveCardsLabel = Create<zcom::Label>(request->inputPrompt);
            _chooseCardsFromActiveCardsLabel->SetBaseSize(600, 180);
            _chooseCardsFromActiveCardsLabel->SetVerticalOffsetPixels(60);
            _chooseCardsFromActiveCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
            _chooseCardsFromActiveCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _chooseCardsFromActiveCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _chooseCardsFromActiveCardsLabel->SetFont(L"Arial");
            _chooseCardsFromActiveCardsLabel->SetFontSize(36.0f);
            _chooseCardsFromActiveCardsLabel->SetWordWrap(true);
            _chooseCardsFromActiveCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
            _chooseCardsFromActiveCardsLabel->Resize(_chooseCardsFromActiveCardsLabel->GetBaseWidth(), 100);
            _chooseCardsFromActiveCardsLabel->SetBaseHeight(_chooseCardsFromActiveCardsLabel->GetTextHeight() + 100);

            _chooseCardsFromActiveCardsDoneButton = Create<zcom::Button>(L"Done");
            _chooseCardsFromActiveCardsDoneButton->Text()->SetFont(L"Arial");
            _chooseCardsFromActiveCardsDoneButton->Text()->SetFontSize(36.0f);
            _chooseCardsFromActiveCardsDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _chooseCardsFromActiveCardsDoneButton->SetBaseSize(150.0f, 60.0f);
            _chooseCardsFromActiveCardsDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::CENTER);
            _chooseCardsFromActiveCardsDoneButton->SetZIndex(10);
            _chooseCardsFromActiveCardsDoneButton->SetBorderVisibility(true);
            _chooseCardsFromActiveCardsDoneButton->SetBorderWidth(4.0f);
            _chooseCardsFromActiveCardsDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
            _chooseCardsFromActiveCardsDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _chooseCardsFromActiveCardsDoneButton->SetOnActivated([&, request]()
            {
                _waitingToChooseCardsFromActiveCards = false;
                _gettingUserInput = false;

                auto params = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(request->inputParams.get());
                params->chosenCards.clear();
                for (auto card : _chosenCardsFromActiveCards)
                    params->chosenCards.push_back(card);
                _board->SendUserInputResponse();
                _board->DisableChooseCardFromActiveCardsMode();

                _canvas->RemoveComponent(_chooseCardsFromActiveCardsLabel.get());
                _canvas->RemoveComponent(_chooseCardsFromActiveCardsDoneButton.get());
            });
            _chooseCardsFromActiveCardsDoneButton->SetVisible(params->minCardCount <= 0);

            _canvas->AddComponent(_chooseCardsFromActiveCardsLabel.get());
            _canvas->AddComponent(_chooseCardsFromActiveCardsDoneButton.get());

            _chosenCardsFromActiveCards.clear();
            _board->EnableChooseCardFromActiveCardsMode(
                params->choosingPlayerIndex,
                params->targetActivesPlayerIndex,
                params->maxCardCount,
                params->allowedTypes,
                [&, request](cards::Card* selectedCard)
                {
                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(request->inputParams.get());

                    _chosenCardsFromActiveCards.insert(selectedCard);
                    if (_chosenCardsFromActiveCards.size() >= params->minCardCount)
                        _chooseCardsFromActiveCardsDoneButton->SetVisible(true);
                },
                [&, request](cards::Card* deselectedCard)
                {
                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(request->inputParams.get());

                    _chosenCardsFromActiveCards.erase(deselectedCard);
                    if (_chosenCardsFromActiveCards.size() < params->minCardCount)
                        _chooseCardsFromActiveCardsDoneButton->SetVisible(false);
                }
            );

            break;
        }
        case UserInputType::CHOOSE_CARD_FROM_DISPLAYED_CARDS:
        {
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(request->inputParams.get());

            if (params->maxCardCount <= 0)
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingToChooseCardsFromDisplayedCards = true;
            _gettingUserInput = true;

            _chooseCardsFromDisplayedCardsLabel = Create<zcom::Label>(request->inputPrompt);
            _chooseCardsFromDisplayedCardsLabel->SetBaseSize(600, 180);
            _chooseCardsFromDisplayedCardsLabel->SetVerticalOffsetPixels(60);
            _chooseCardsFromDisplayedCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
            _chooseCardsFromDisplayedCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _chooseCardsFromDisplayedCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _chooseCardsFromDisplayedCardsLabel->SetFont(L"Arial");
            _chooseCardsFromDisplayedCardsLabel->SetFontSize(36.0f);
            _chooseCardsFromDisplayedCardsLabel->SetWordWrap(true);
            _chooseCardsFromDisplayedCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
            _chooseCardsFromDisplayedCardsLabel->Resize(_chooseCardsFromDisplayedCardsLabel->GetBaseWidth(), 100);
            _chooseCardsFromDisplayedCardsLabel->SetBaseHeight(_chooseCardsFromDisplayedCardsLabel->GetTextHeight() + 100);

            _chooseCardsFromDisplayedCardsDoneButton = Create<zcom::Button>(L"Done");
            _chooseCardsFromDisplayedCardsDoneButton->Text()->SetFont(L"Arial");
            _chooseCardsFromDisplayedCardsDoneButton->Text()->SetFontSize(36.0f);
            _chooseCardsFromDisplayedCardsDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _chooseCardsFromDisplayedCardsDoneButton->SetBaseSize(150.0f, -60.0f);
            _chooseCardsFromDisplayedCardsDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
            _chooseCardsFromDisplayedCardsDoneButton->SetZIndex(10);
            _chooseCardsFromDisplayedCardsDoneButton->SetBorderVisibility(true);
            _chooseCardsFromDisplayedCardsDoneButton->SetBorderWidth(4.0f);
            _chooseCardsFromDisplayedCardsDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
            _chooseCardsFromDisplayedCardsDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _chooseCardsFromDisplayedCardsDoneButton->SetOnActivated([&, request]()
            {
                _waitingToChooseCardsFromDisplayedCards = false;
                _gettingUserInput = false;

                auto params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(request->inputParams.get());
                params->chosenCards.clear();
                for (auto card : _chosenCardsFromDisplayedCards)
                    params->chosenCards.push_back(card);
                _board->SendUserInputResponse();
                _board->DisableChooseCardFromDisplayedCardsMode();

                _canvas->RemoveComponent(_chooseCardsFromDisplayedCardsLabel.get());
                _canvas->RemoveComponent(_chooseCardsFromDisplayedCardsDoneButton.get());
            });
            _chooseCardsFromDisplayedCardsDoneButton->SetVisible(params->minCardCount <= 0);

            _canvas->AddComponent(_chooseCardsFromDisplayedCardsLabel.get());
            _canvas->AddComponent(_chooseCardsFromDisplayedCardsDoneButton.get());

            _chosenCardsFromDisplayedCards.clear();
            _board->EnableChooseCardFromDisplayedCardsMode(
                params->playerIndex,
                params->maxCardCount,
                params->allowedTypes,
                [&, request](cards::Card* selectedCard)
                {
                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(request->inputParams.get());

                    _chosenCardsFromDisplayedCards.insert(selectedCard);
                    if (_chosenCardsFromDisplayedCards.size() >= params->minCardCount)
                        _chooseCardsFromDisplayedCardsDoneButton->SetVisible(true);
                },
                [&, request](cards::Card* deselectedCard)
                {
                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(request->inputParams.get());

                    _chosenCardsFromDisplayedCards.erase(deselectedCard);
                    if (_chosenCardsFromDisplayedCards.size() < params->minCardCount)
                        _chooseCardsFromDisplayedCardsDoneButton->SetVisible(false);
                }
            );

            break;
        }
        case UserInputType::DRAW_CARD:
        {
            auto params = reinterpret_cast<UserInputParams_DrawCard*>(request->inputParams.get());
            params->drawnCards.clear();

            if (_DrawEndStateReached(params))
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingForCardDraw = true;
            _gettingUserInput = true;

            _drawCardsLabel = Create<zcom::Label>(request->inputPrompt);
            _drawCardsLabel->SetBaseSize(600, 180);
            _drawCardsLabel->SetVerticalOffsetPixels(60);
            _drawCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
            _drawCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _drawCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _drawCardsLabel->SetFont(L"Arial");
            _drawCardsLabel->SetFontSize(36.0f);
            _drawCardsLabel->SetWordWrap(true);
            _drawCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
            _drawCardsLabel->Resize(_drawCardsLabel->GetBaseWidth(), 100);
            _drawCardsLabel->SetBaseHeight(_drawCardsLabel->GetTextHeight() + 100);

            _drawCardsDoneButton = Create<zcom::Button>(L"Done");
            _drawCardsDoneButton->Text()->SetFont(L"Arial");
            _drawCardsDoneButton->Text()->SetFontSize(36.0f);
            _drawCardsDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _drawCardsDoneButton->SetBaseSize(150.0f, 60.0f);
            _drawCardsDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
            _drawCardsDoneButton->SetZIndex(10);
            _drawCardsDoneButton->SetVerticalOffsetPixels(-20.0f);
            _drawCardsDoneButton->SetBorderVisibility(true);
            _drawCardsDoneButton->SetBorderWidth(4.0f);
            _drawCardsDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
            _drawCardsDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
            _drawCardsDoneButton->SetOnActivated([&, request]()
            {
                _waitingForCardDraw = false;
                _gettingUserInput = false;

                _board->SendUserInputResponse();
                _board->DisableDrawCardMode();

                _canvas->RemoveComponent(_drawCardsLabel.get());
                _canvas->RemoveComponent(_drawCardsDoneButton.get());
            });
            _drawCardsDoneButton->SetVisible(params->minCardCount <= 0);

            _canvas->AddComponent(_drawCardsLabel.get());
            _canvas->AddComponent(_drawCardsDoneButton.get());

            _board->EnableDrawCardMode(params->allowedTypes, [&, request](cards::Card* drawnCard)
            {
                auto params = reinterpret_cast<UserInputParams_DrawCard*>(request->inputParams.get());
                params->drawnCards.push_back(drawnCard);

                if (params->drawnCards.size() >= params->minCardCount)
                    _drawCardsDoneButton->SetVisible(true);

                if (_DrawEndStateReached(params))
                {
                    _waitingForCardDraw = false;
                    _gettingUserInput = false;

                    _board->SendUserInputResponse();
                    _board->DisableDrawCardMode();

                    _canvas->RemoveComponent(_drawCardsLabel.get());
                    _canvas->RemoveComponent(_drawCardsDoneButton.get());
                }
            });

            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void EntryScene::_UpdateInputRequests()
{
    UserInputRequest* request = _board->GetUserInputRequest();
    if (!request || !_gettingUserInput)
        return;

    if (_waitingForNumberChoice)
    {
        auto params = reinterpret_cast<UserInputParams_ChooseNumber*>(request->inputParams.get());
        if (_chosenNumber == params->lowerBound)
        {
            _minus1Button->SetActive(false);
            _minus2Button->SetActive(false);
            _minus5Button->SetActive(false);
        }
        else
        {
            _minus1Button->SetActive(true);
            _minus2Button->SetActive(true);
            _minus5Button->SetActive(true);
        }
        if (_chosenNumber == params->upperBound)
        {
            _plus1Button->SetActive(false);
            _plus2Button->SetActive(false);
            _plus5Button->SetActive(false);
        }
        else
        {
            _plus1Button->SetActive(true);
            _plus2Button->SetActive(true);
            _plus5Button->SetActive(true);
        }
    }
}

void EntryScene::_CleanUpAfterChoice()
{
    _canvas->RemoveComponent(_choicePanel.get());
    _choicePanel->ClearItems();
}

bool EntryScene::_DrawEndStateReached(UserInputParams_DrawCard* params)
{
    if (params->drawnCards.size() >= params->maxCardCount)
        return true;
    if (!params->ignoreHandSize && core.GetState().players[params->playerIndex].hand.size() >= GAME_HAND_SIZE)
        return true;
    bool allDecksEmpty = core.GetState().offenseDeck.empty() && core.GetState().defenseDeck.empty() && core.GetState().utilityDeck.empty() && core.GetState().comboDeck.empty();
    if (allDecksEmpty)
        return true;
    return false;
}