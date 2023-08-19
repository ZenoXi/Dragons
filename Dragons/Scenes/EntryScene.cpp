#include "App.h" // App.h must be included first
#include "EntryScene.h"

#include "Core/GameConstants.h"
#include "Helper/Functions.h"
#include "Helper/ListUtils.h"
#include "Helper/PacketTypes.h"

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
        _connection = opt.connection;
        _gameSeed = opt.gameSeed;
        _playerIndex = opt.playerIndex;
        _opponentIndex = opt.opponentIndex;
    }

    core.InitState(_gameSeed);

    _board = Create<zcom::Board>(&core, _playerIndex, _opponentIndex);
    _board->SetParentSizePercent(1.0f, 1.0f);
    zcom::attr::Shadow shadowAttr;
    shadowAttr.color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f);
    shadowAttr.blurStandardDeviation = 5.0f;
    _board->SetAttribute(shadowAttr);
    _board->SetBackgroundColor(D2D1::ColorF(0, 0.0f));

    _board->OnCardPlay([&](cards::Card* card) {
        _connection->Send(znet::Packet(10).From(card->GetCardSessionId()));
    });
    _board->OnCardDraw([&](cards::CardType type) {
        _connection->Send(znet::Packet(11).From(type));
    });
    _board->OnCardDiscard([&](cards::Card* card) {
        _connection->Send(znet::Packet(12).From(card->GetCardSessionId()));
    });

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

    _ProcessOpponentActions();
    _ProcessNewInputRequests();
    _UpdateInputRequests();
}

void EntryScene::_Resize(int width, int height)
{

}

void EntryScene::_ProcessOpponentActions()
{
    if (_board->UIState().currentPlayer == _playerIndex)
        return;

    while (_connection->PacketCount() > 0)
    {
        znet::PacketView view = _connection->PeekPacket();
        if (view.id == (int)PacketType::CARD_PLAY)
        {
            if (!_board->CanPlay(_opponentIndex))
                return;

            uint32_t cardSessionId = _connection->GetPacket().Cast<uint32_t>();
            _board->PlayCard(cardSessionId);
        }
        else if (view.id == (int)PacketType::CARD_DRAW)
        {
            if (!_board->CanDraw(_opponentIndex))
                return;

            cards::CardType cardType = _connection->GetPacket().Cast<cards::CardType>();
            _board->DrawCard(cardType);
        }
        else if (view.id == (int)PacketType::CARD_DISCARD)
        {
            if (!_board->CanDiscard(_opponentIndex))
                return;

            uint32_t cardSessionId = _connection->GetPacket().Cast<uint32_t>();
            _board->DiscardCard(cardSessionId);
        }
        else if (view.id == (int)PacketType::CONFIRMATION)
        {
            if (!_waitingForConfirmation)
                return;

            _connection->GetPacket();

            _waitingForConfirmation = false;
            _gettingUserInput = false;
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentConfirmationLabel.get());
        }
        else if (view.id == (int)PacketType::YES_OR_NO)
        {
            if (!_waitingForYesOrNoChoice)
                return;

            bool choice = _connection->GetPacket().Cast<bool>();

            _waitingForYesOrNoChoice = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_YesOrNo*>(_board->GetUserInputRequest()->inputParams.get());
            params->yes = choice;
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentYesOrNoLabel.get());
        }
        else if (view.id == (int)PacketType::NUMBER_CHOICE)
        {
            if (!_waitingForNumberChoice)
                return;

            int chosenNumber = _connection->GetPacket().Cast<int>();

            _waitingForNumberChoice = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseNumber*>(_board->GetUserInputRequest()->inputParams.get());
            params->chosenNumber = chosenNumber;
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentNumberChoiceLabel.get());
        }
        else if (view.id == (int)PacketType::OPTION_CHOICE)
        {
            if (!_waitingForChoice)
                return;

            int chosenOptionIndex = _connection->GetPacket().Cast<int>();

            _waitingForChoice = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseOption*>(_board->GetUserInputRequest()->inputParams.get());
            params->chosenOptionIndex = chosenOptionIndex;
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentChoiceLabel.get());
        }
        else if (view.id == (int)PacketType::CHOOSE_CARD_FROM_HAND)
        {
            if (!_waitingToChooseCardsFromHand)
                return;

            _waitingToChooseCardsFromHand = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromHand*>(_board->GetUserInputRequest()->inputParams.get());
            znet::Packet packet = _connection->GetPacket();
            size_t cardCount = packet.size / sizeof(uint32_t);
            for (int i = 0; i < cardCount; i++)
            {
                uint32_t cardSessionId = ((uint32_t*)packet.Bytes())[i];
                auto& hand = _board->UIState().players[params->handPlayerIndex].hand;
                for (auto& card : hand)
                {
                    if (card->GetCardSessionId() == cardSessionId)
                    {
                        params->chosenCards.push_back(card);
                        break;
                    }
                }
            }
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentChooseCardsFromHandLabel.get());
        }
        else if (view.id == (int)PacketType::CHOOSE_CARD_FROM_ACTIVE_CARDS)
        {
            if (!_waitingToChooseCardsFromActiveCards)
                return;

            _waitingToChooseCardsFromActiveCards = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromActiveCards*>(_board->GetUserInputRequest()->inputParams.get());
            znet::Packet packet = _connection->GetPacket();
            size_t cardCount = packet.size / sizeof(uint32_t);
            for (int i = 0; i < cardCount; i++)
            {
                uint32_t cardSessionId = ((uint32_t*)packet.Bytes())[i];
                auto& activeCards = _board->UIState().players[params->targetActivesPlayerIndex].activeCards;
                for (auto& card : activeCards)
                {
                    if (card->GetCardSessionId() == cardSessionId)
                    {
                        params->chosenCards.push_back(card);
                        break;
                    }
                }
            }
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentChooseCardsFromActiveCardsLabel.get());
        }
        else if (view.id == (int)PacketType::CHOOSE_CARD_FROM_DISPLAYED_CARDS)
        {
            if (!_waitingToChooseCardsFromDisplayedCards)
                return;

            _waitingToChooseCardsFromDisplayedCards = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromDisplayedCards*>(_board->GetUserInputRequest()->inputParams.get());
            znet::Packet packet = _connection->GetPacket();
            size_t cardCount = packet.size / sizeof(uint32_t);
            for (int i = 0; i < cardCount; i++)
            {
                uint32_t cardSessionId = ((uint32_t*)packet.Bytes())[i];
                for (auto& displayInfo : _board->UIState().displayedCards)
                {
                    if (displayInfo.card->GetCardSessionId() == cardSessionId)
                    {
                        params->chosenCards.push_back(displayInfo.card);
                        break;
                    }
                }
            }
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentChooseCardsFromDisplayedCardsLabel.get());
        }
        else if (view.id == (int)PacketType::CHOOSE_CARD_FROM_GRAVEYARD)
        {
            if (!_waitingToChooseCardsFromGraveyard)
                return;

            _waitingToChooseCardsFromGraveyard = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(_board->GetUserInputRequest()->inputParams.get());
            znet::Packet packet = _connection->GetPacket();
            size_t cardCount = packet.size / sizeof(uint32_t);
            for (int i = 0; i < cardCount; i++)
            {
                uint32_t cardSessionId = ((uint32_t*)packet.Bytes())[i];
                for (auto& card : _board->UIState().graveyard)
                {
                    if (card->GetCardSessionId() == cardSessionId)
                    {
                        params->chosenCards.push_back(card);
                        break;
                    }
                }
            }
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentChooseCardsFromGraveyardLabel.get());
        }
        else if (view.id == (int)PacketType::CHOOSE_DECK)
        {
            if (!_waitingToChooseDecks)
                return;

            _waitingToChooseDecks = false;
            _gettingUserInput = false;
            auto params = reinterpret_cast<UserInputParams_ChooseDeck*>(_board->GetUserInputRequest()->inputParams.get());
            znet::Packet packet = _connection->GetPacket();
            size_t cardCount = packet.size / sizeof(cards::CardType);
            for (int i = 0; i < cardCount; i++)
            {
                cards::CardType cardType = ((cards::CardType*)packet.Bytes())[i];
                params->chosenDecks.push_back(cardType);
            }
            _board->SendUserInputResponse();
            _canvas->RemoveComponent(_opponentChooseDecksLabel.get());
        }
        else if (view.id == (int)PacketType::PLAY_CARDS_DONE)
        {
            if (!_waitingForCardPlay)
                return;

            _connection->GetPacket();

            _waitingForCardPlay = false;
            _gettingUserInput = false;

            _board->SendUserInputResponse();
            _board->DisablePlayCardMode();

            _canvas->RemoveComponent(_playCardsLabel.get());
            _canvas->RemoveComponent(_playCardsDoneButton.get());
        }
        else if (view.id == (int)PacketType::DRAW_CARDS_DONE)
        {
            if (!_waitingForCardDraw)
                return;

            _connection->GetPacket();

            _waitingForCardDraw = false;
            _gettingUserInput = false;

            _board->SendUserInputResponse();
            _board->DisablePlayCardMode();

            _canvas->RemoveComponent(_drawCardsLabel.get());
            _canvas->RemoveComponent(_drawCardsDoneButton.get());
        }
        else if (view.id == (int)PacketType::DISCARD_CARDS_DONE)
        {
            if (!_waitingForCardDiscard)
                return;

            _connection->GetPacket();

            _waitingForCardDiscard = false;
            _gettingUserInput = false;

            _board->SendUserInputResponse();
            _board->DisableDiscardCardMode();

            _canvas->RemoveComponent(_discardCardsLabel.get());
            _canvas->RemoveComponent(_discardCardsDoneButton.get());
        }
    }
}

void EntryScene::_ProcessNewInputRequests()
{
    UserInputRequest* request = _board->GetUserInputRequest();
    int counter = _board->GetRequestCounter();
    if (request && (!_gettingUserInput || counter != _currentInputRequestId))
    {
        // Getting extra user input requests is only possible while playing cards
        if (_gettingUserInput)
        {
            _canvas->RemoveComponent(_playCardsLabel.get());
            _canvas->RemoveComponent(_playCardsDoneButton.get());
            _board->DisablePlayCardMode();
        }

        _currentInputRequestId = counter;
        switch (request->inputType)
        {
        case UserInputType::WAIT_FOR_CONFIRMATION:
        {
            _waitingForConfirmation = true;
            _gettingUserInput = true;

            auto params = reinterpret_cast<UserInputParams_WaitForConfirmation*>(request->inputParams.get());

            if (params->playerIndex != _playerIndex)
            {
                _opponentConfirmationLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentConfirmationLabel->SetBaseSize(600, 180);
                _opponentConfirmationLabel->SetVerticalOffsetPixels(60);
                _opponentConfirmationLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentConfirmationLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentConfirmationLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentConfirmationLabel->SetFont(L"Arial");
                _opponentConfirmationLabel->SetFontSize(36.0f);
                _opponentConfirmationLabel->SetWordWrap(true);
                _opponentConfirmationLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentConfirmationLabel->Resize(_opponentConfirmationLabel->GetBaseWidth(), 100);
                _opponentConfirmationLabel->SetBaseHeight(_opponentConfirmationLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentConfirmationLabel.get());
            }
            else
            {
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
                zcom::attr::Shadow shadowAttr;
                shadowAttr.color = D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f);
                shadowAttr.blurStandardDeviation = 25.0f;
                _confirmationButton->SetAttribute(shadowAttr);
                _confirmationButton->SetActivation(zcom::ButtonActivation::RELEASE);
                _confirmationButton->SetOnActivated([&]()
                {
                    _waitingForConfirmation = false;
                    _gettingUserInput = false;
                    _connection->Send(znet::Packet((int)PacketType::CONFIRMATION));
                    _board->SendUserInputResponse();

                    _canvas->RemoveComponent(_confirmationButton.get());
                });

                _canvas->AddComponent(_confirmationButton.get());
            }

            break;
        }
        case UserInputType::YES_OR_NO:
        {
            _waitingForYesOrNoChoice = true;
            _gettingUserInput = true;

            auto params = reinterpret_cast<UserInputParams_YesOrNo*>(request->inputParams.get());

            if (params->playerIndex != _playerIndex)
            {
                _opponentYesOrNoLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentYesOrNoLabel->SetBaseSize(600, 180);
                _opponentYesOrNoLabel->SetVerticalOffsetPixels(60);
                _opponentYesOrNoLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentYesOrNoLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentYesOrNoLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentYesOrNoLabel->SetFont(L"Arial");
                _opponentYesOrNoLabel->SetFontSize(36.0f);
                _opponentYesOrNoLabel->SetWordWrap(true);
                _opponentYesOrNoLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentYesOrNoLabel->Resize(_opponentYesOrNoLabel->GetBaseWidth(), 100);
                _opponentYesOrNoLabel->SetBaseHeight(_opponentYesOrNoLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentYesOrNoLabel.get());
            }
            else
            {
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
                    _connection->Send(znet::Packet((int)PacketType::YES_OR_NO).From(params->yes));
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
                    _connection->Send(znet::Packet((int)PacketType::YES_OR_NO).From(params->yes));
                    _board->SendUserInputResponse();

                    _canvas->RemoveComponent(_yesOrNoPanel.get());
                    _yesOrNoPanel->ClearItems();
                });

                _yesOrNoPanel->AddItem(_yesOrNoPromptLabel.get());
                _yesOrNoPanel->AddItem(_yesButton.get());
                _yesOrNoPanel->AddItem(_noButton.get());
                _canvas->AddComponent(_yesOrNoPanel.get());
            }

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

            if (params->playerIndex != _playerIndex)
            {
                _opponentNumberChoiceLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentNumberChoiceLabel->SetBaseSize(600, 180);
                _opponentNumberChoiceLabel->SetVerticalOffsetPixels(60);
                _opponentNumberChoiceLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentNumberChoiceLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentNumberChoiceLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentNumberChoiceLabel->SetFont(L"Arial");
                _opponentNumberChoiceLabel->SetFontSize(36.0f);
                _opponentNumberChoiceLabel->SetWordWrap(true);
                _opponentNumberChoiceLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentNumberChoiceLabel->Resize(_opponentNumberChoiceLabel->GetBaseWidth(), 100);
                _opponentNumberChoiceLabel->SetBaseHeight(_opponentNumberChoiceLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentNumberChoiceLabel.get());
            }
            else
            {
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
                    _connection->Send(znet::Packet((int)PacketType::NUMBER_CHOICE).From(params->chosenNumber));
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
            }

            break;
        }
        case UserInputType::CHOOSE_OPTION:
        {
            _waitingForChoice = true;
            _gettingUserInput = true;

            auto params = reinterpret_cast<UserInputParams_ChooseOption*>(request->inputParams.get());
            int optionCount = params->options.size();

            if (params->playerIndex != _playerIndex)
            {
                _opponentChoiceLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentChoiceLabel->SetBaseSize(600, 180);
                _opponentChoiceLabel->SetVerticalOffsetPixels(60);
                _opponentChoiceLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentChoiceLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentChoiceLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentChoiceLabel->SetFont(L"Arial");
                _opponentChoiceLabel->SetFontSize(36.0f);
                _opponentChoiceLabel->SetWordWrap(true);
                _opponentChoiceLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentChoiceLabel->Resize(_opponentChoiceLabel->GetBaseWidth(), 100);
                _opponentChoiceLabel->SetBaseHeight(_opponentChoiceLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentChoiceLabel.get());
            }
            else
            {
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
                        _connection->Send(znet::Packet((int)PacketType::OPTION_CHOICE).From(params->chosenOptionIndex));
                        _board->SendUserInputResponse();
                        _board->DisableNoClickMode();

                        _CleanUpAfterChoice();
                    });

                    _choicePanel->AddItem(choiceButton.release(), true);
                }

                _choicePanel->AddItem(_choicePromptLabel.get());
                _canvas->AddComponent(_choicePanel.get());

                _board->EnableNoClickMode();
            }
            
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

            if (params->choosingPlayerIndex != _playerIndex)
            {
                _opponentChooseCardsFromHandLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentChooseCardsFromHandLabel->SetBaseSize(600, 180);
                _opponentChooseCardsFromHandLabel->SetVerticalOffsetPixels(60);
                _opponentChooseCardsFromHandLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentChooseCardsFromHandLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentChooseCardsFromHandLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentChooseCardsFromHandLabel->SetFont(L"Arial");
                _opponentChooseCardsFromHandLabel->SetFontSize(36.0f);
                _opponentChooseCardsFromHandLabel->SetWordWrap(true);
                _opponentChooseCardsFromHandLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentChooseCardsFromHandLabel->Resize(_opponentChooseCardsFromHandLabel->GetBaseWidth(), 100);
                _opponentChooseCardsFromHandLabel->SetBaseHeight(_opponentChooseCardsFromHandLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentChooseCardsFromHandLabel.get());
            }
            else
            {
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

                    size_t packetSize = params->chosenCards.size() * sizeof(uint32_t);
                    auto data = std::make_unique<int8_t[]>(packetSize);
                    for (int i = 0; i < params->chosenCards.size(); i++)
                        ((uint32_t*)data.get())[i] = params->chosenCards[i]->GetCardSessionId();
                    _connection->Send(znet::Packet(std::move(data), packetSize, (int)PacketType::CHOOSE_CARD_FROM_HAND));

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
            }

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

            if (params->choosingPlayerIndex != _playerIndex)
            {
                _opponentChooseCardsFromActiveCardsLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentChooseCardsFromActiveCardsLabel->SetBaseSize(600, 180);
                _opponentChooseCardsFromActiveCardsLabel->SetVerticalOffsetPixels(60);
                _opponentChooseCardsFromActiveCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentChooseCardsFromActiveCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentChooseCardsFromActiveCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentChooseCardsFromActiveCardsLabel->SetFont(L"Arial");
                _opponentChooseCardsFromActiveCardsLabel->SetFontSize(36.0f);
                _opponentChooseCardsFromActiveCardsLabel->SetWordWrap(true);
                _opponentChooseCardsFromActiveCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentChooseCardsFromActiveCardsLabel->Resize(_opponentChooseCardsFromActiveCardsLabel->GetBaseWidth(), 100);
                _opponentChooseCardsFromActiveCardsLabel->SetBaseHeight(_opponentChooseCardsFromActiveCardsLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentChooseCardsFromActiveCardsLabel.get());
            }
            else
            {
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

                    size_t packetSize = params->chosenCards.size() * sizeof(uint32_t);
                    auto data = std::make_unique<int8_t[]>(packetSize);
                    for (int i = 0; i < params->chosenCards.size(); i++)
                        ((uint32_t*)data.get())[i] = params->chosenCards[i]->GetCardSessionId();
                    _connection->Send(znet::Packet(std::move(data), packetSize, (int)PacketType::CHOOSE_CARD_FROM_ACTIVE_CARDS));

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
            }

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

            if (params->playerIndex != _playerIndex)
            {
                _opponentChooseCardsFromDisplayedCardsLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentChooseCardsFromDisplayedCardsLabel->SetBaseSize(600, 180);
                _opponentChooseCardsFromDisplayedCardsLabel->SetVerticalOffsetPixels(60);
                _opponentChooseCardsFromDisplayedCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentChooseCardsFromDisplayedCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentChooseCardsFromDisplayedCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentChooseCardsFromDisplayedCardsLabel->SetFont(L"Arial");
                _opponentChooseCardsFromDisplayedCardsLabel->SetFontSize(36.0f);
                _opponentChooseCardsFromDisplayedCardsLabel->SetWordWrap(true);
                _opponentChooseCardsFromDisplayedCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentChooseCardsFromDisplayedCardsLabel->Resize(_opponentChooseCardsFromDisplayedCardsLabel->GetBaseWidth(), 100);
                _opponentChooseCardsFromDisplayedCardsLabel->SetBaseHeight(_opponentChooseCardsFromDisplayedCardsLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentChooseCardsFromDisplayedCardsLabel.get());
            }
            else
            {
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
                _chooseCardsFromDisplayedCardsDoneButton->SetBaseSize(150.0f, 60.0f);
                _chooseCardsFromDisplayedCardsDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
                _chooseCardsFromDisplayedCardsDoneButton->SetVerticalOffsetPixels(-100);
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

                    size_t packetSize = params->chosenCards.size() * sizeof(uint32_t);
                    auto data = std::make_unique<int8_t[]>(packetSize);
                    for (int i = 0; i < params->chosenCards.size(); i++)
                        ((uint32_t*)data.get())[i] = params->chosenCards[i]->GetCardSessionId();
                    _connection->Send(znet::Packet(std::move(data), packetSize, (int)PacketType::CHOOSE_CARD_FROM_DISPLAYED_CARDS));

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
            }

            break;
        }
        case UserInputType::CHOOSE_CARD_FROM_GRAVEYARD:
        {
            auto params = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(request->inputParams.get());

            if (params->maxCardCount <= 0)
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingToChooseCardsFromGraveyard = true;
            _gettingUserInput = true;

            if (params->playerIndex != _playerIndex)
            {
                _opponentChooseCardsFromGraveyardLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentChooseCardsFromGraveyardLabel->SetBaseSize(600, 180);
                _opponentChooseCardsFromGraveyardLabel->SetVerticalOffsetPixels(60);
                _opponentChooseCardsFromGraveyardLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentChooseCardsFromGraveyardLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentChooseCardsFromGraveyardLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentChooseCardsFromGraveyardLabel->SetFont(L"Arial");
                _opponentChooseCardsFromGraveyardLabel->SetFontSize(36.0f);
                _opponentChooseCardsFromGraveyardLabel->SetWordWrap(true);
                _opponentChooseCardsFromGraveyardLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentChooseCardsFromGraveyardLabel->Resize(_opponentChooseCardsFromGraveyardLabel->GetBaseWidth(), 100);
                _opponentChooseCardsFromGraveyardLabel->SetBaseHeight(_opponentChooseCardsFromGraveyardLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentChooseCardsFromGraveyardLabel.get());
            }
            else
            {
                _chooseCardsFromGraveyardLabel = Create<zcom::Label>(request->inputPrompt);
                _chooseCardsFromGraveyardLabel->SetBaseSize(600, 180);
                _chooseCardsFromGraveyardLabel->SetVerticalOffsetPixels(60);
                _chooseCardsFromGraveyardLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _chooseCardsFromGraveyardLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _chooseCardsFromGraveyardLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _chooseCardsFromGraveyardLabel->SetFont(L"Arial");
                _chooseCardsFromGraveyardLabel->SetFontSize(36.0f);
                _chooseCardsFromGraveyardLabel->SetWordWrap(true);
                _chooseCardsFromGraveyardLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _chooseCardsFromGraveyardLabel->Resize(_chooseCardsFromGraveyardLabel->GetBaseWidth(), 100);
                _chooseCardsFromGraveyardLabel->SetBaseHeight(_chooseCardsFromGraveyardLabel->GetTextHeight() + 100);

                _chooseCardsFromGraveyardDoneButton = Create<zcom::Button>(L"Done");
                _chooseCardsFromGraveyardDoneButton->Text()->SetFont(L"Arial");
                _chooseCardsFromGraveyardDoneButton->Text()->SetFontSize(36.0f);
                _chooseCardsFromGraveyardDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
                _chooseCardsFromGraveyardDoneButton->SetBaseSize(150.0f, 60.0f);
                _chooseCardsFromGraveyardDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
                _chooseCardsFromGraveyardDoneButton->SetVerticalOffsetPixels(-100);
                _chooseCardsFromGraveyardDoneButton->SetZIndex(10);
                _chooseCardsFromGraveyardDoneButton->SetBorderVisibility(true);
                _chooseCardsFromGraveyardDoneButton->SetBorderWidth(4.0f);
                _chooseCardsFromGraveyardDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
                _chooseCardsFromGraveyardDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
                _chooseCardsFromGraveyardDoneButton->SetOnActivated([&, request]()
                {
                    _waitingToChooseCardsFromGraveyard = false;
                    _gettingUserInput = false;

                    auto params = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(request->inputParams.get());
                    params->chosenCards.clear();
                    for (auto card : _chosenCardsFromGraveyard)
                        params->chosenCards.push_back(card);

                    size_t packetSize = params->chosenCards.size() * sizeof(uint32_t);
                    auto data = std::make_unique<int8_t[]>(packetSize);
                    for (int i = 0; i < params->chosenCards.size(); i++)
                        ((uint32_t*)data.get())[i] = params->chosenCards[i]->GetCardSessionId();
                    _connection->Send(znet::Packet(std::move(data), packetSize, (int)PacketType::CHOOSE_CARD_FROM_GRAVEYARD));

                    _board->SendUserInputResponse();
                    _board->DisableChooseCardFromGraveyardMode();

                    _canvas->RemoveComponent(_chooseCardsFromGraveyardLabel.get());
                    _canvas->RemoveComponent(_chooseCardsFromGraveyardDoneButton.get());
                });
                _chooseCardsFromGraveyardDoneButton->SetVisible(params->minCardCount <= 0);

                _canvas->AddComponent(_chooseCardsFromGraveyardLabel.get());
                _canvas->AddComponent(_chooseCardsFromGraveyardDoneButton.get());

                _chosenCardsFromGraveyard.clear();
                _board->EnableChooseCardFromGraveyardMode(
                    params->playerIndex,
                    params->maxCardCount,
                    params->allowedTypes,
                    [&, request](cards::Card* selectedCard)
                    {
                        auto params = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(request->inputParams.get());

                        _chosenCardsFromGraveyard.insert(selectedCard);
                        if (_chosenCardsFromGraveyard.size() >= params->minCardCount)
                            _chooseCardsFromGraveyardDoneButton->SetVisible(true);
                    },
                    [&, request](cards::Card* deselectedCard)
                    {
                        auto params = reinterpret_cast<UserInputParams_ChooseCardFromGraveyard*>(request->inputParams.get());

                        _chosenCardsFromGraveyard.erase(deselectedCard);
                        if (_chosenCardsFromGraveyard.size() < params->minCardCount)
                            _chooseCardsFromGraveyardDoneButton->SetVisible(false);
                    }
                );
            }

            break;
        }
        case UserInputType::CHOOSE_DECK:
        {
            auto params = reinterpret_cast<UserInputParams_ChooseDeck*>(request->inputParams.get());

            if (params->maxDeckCount <= 0)
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingToChooseDecks = true;
            _gettingUserInput = true;

            if (params->playerIndex != _playerIndex)
            {
                _opponentChooseDecksLabel = Create<zcom::Label>(L"Waiting for opponent");
                _opponentChooseDecksLabel->SetBaseSize(600, 180);
                _opponentChooseDecksLabel->SetVerticalOffsetPixels(60);
                _opponentChooseDecksLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _opponentChooseDecksLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _opponentChooseDecksLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _opponentChooseDecksLabel->SetFont(L"Arial");
                _opponentChooseDecksLabel->SetFontSize(36.0f);
                _opponentChooseDecksLabel->SetWordWrap(true);
                _opponentChooseDecksLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _opponentChooseDecksLabel->Resize(_opponentChooseDecksLabel->GetBaseWidth(), 100);
                _opponentChooseDecksLabel->SetBaseHeight(_opponentChooseDecksLabel->GetTextHeight() + 100);

                _canvas->AddComponent(_opponentChooseDecksLabel.get());
            }
            else
            {
                _chooseDecksLabel = Create<zcom::Label>(request->inputPrompt);
                _chooseDecksLabel->SetBaseSize(600, 180);
                _chooseDecksLabel->SetVerticalOffsetPixels(60);
                _chooseDecksLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
                _chooseDecksLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
                _chooseDecksLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
                _chooseDecksLabel->SetFont(L"Arial");
                _chooseDecksLabel->SetFontSize(36.0f);
                _chooseDecksLabel->SetWordWrap(true);
                _chooseDecksLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
                _chooseDecksLabel->Resize(_chooseDecksLabel->GetBaseWidth(), 100);
                _chooseDecksLabel->SetBaseHeight(_chooseDecksLabel->GetTextHeight() + 100);

                _chooseDecksDoneButton = Create<zcom::Button>(L"Done");
                _chooseDecksDoneButton->Text()->SetFont(L"Arial");
                _chooseDecksDoneButton->Text()->SetFontSize(36.0f);
                _chooseDecksDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
                _chooseDecksDoneButton->SetBaseSize(150.0f, 60.0f);
                _chooseDecksDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
                _chooseDecksDoneButton->SetVerticalOffsetPixels(-100);
                _chooseDecksDoneButton->SetZIndex(10);
                _chooseDecksDoneButton->SetBorderVisibility(true);
                _chooseDecksDoneButton->SetBorderWidth(4.0f);
                _chooseDecksDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
                _chooseDecksDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
                _chooseDecksDoneButton->SetOnActivated([&, request]()
                {
                    _waitingToChooseDecks = false;
                    _gettingUserInput = false;

                    auto params = reinterpret_cast<UserInputParams_ChooseDeck*>(request->inputParams.get());
                    params->chosenDecks.clear();
                    for (auto card : _chosenDecks)
                        params->chosenDecks.push_back(card);

                    size_t packetSize = params->chosenDecks.size() * sizeof(cards::CardType);
                    auto data = std::make_unique<int8_t[]>(packetSize);
                    for (int i = 0; i < params->chosenDecks.size(); i++)
                        ((cards::CardType*)data.get())[i] = params->chosenDecks[i];
                    _connection->Send(znet::Packet(std::move(data), packetSize, (int)PacketType::CHOOSE_DECK));

                    _board->SendUserInputResponse();
                    _board->DisableChooseDeckMode();

                    _canvas->RemoveComponent(_chooseDecksLabel.get());
                    _canvas->RemoveComponent(_chooseDecksDoneButton.get());
                });
                _chooseDecksDoneButton->SetVisible(params->minDeckCount <= 0);

                _canvas->AddComponent(_chooseDecksLabel.get());
                _canvas->AddComponent(_chooseDecksDoneButton.get());

                _chosenDecks.clear();
                _board->EnableChooseDeckMode(
                    params->playerIndex,
                    params->maxDeckCount,
                    params->allowedDecks,
                    [&, request](cards::CardType selectedCard)
                    {
                        auto params = reinterpret_cast<UserInputParams_ChooseDeck*>(request->inputParams.get());

                        _chosenDecks.insert(selectedCard);
                        if (_chosenDecks.size() >= params->minDeckCount)
                            _chooseDecksDoneButton->SetVisible(true);
                    },
                    [&, request](cards::CardType deselectedCard)
                    {
                        auto params = reinterpret_cast<UserInputParams_ChooseDeck*>(request->inputParams.get());

                        _chosenDecks.erase(deselectedCard);
                        if (_chosenDecks.size() < params->minDeckCount)
                            _chooseDecksDoneButton->SetVisible(false);
                    }
                );
            }

            break;
        }
        case UserInputType::PLAY_CARD:
        {
            auto params = reinterpret_cast<UserInputParams_PlayCard*>(request->inputParams.get());
            params->playedCards.clear();

            if (_PlayEndStateReached(params))
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingForCardPlay = true;
            _gettingUserInput = true;

            if (params->playerIndex != _playerIndex)
                _playCardsLabel = Create<zcom::Label>(L"Waiting for opponent to play cards");
            else
                _playCardsLabel = Create<zcom::Label>(request->inputPrompt);
            _playCardsLabel->SetBaseSize(600, 180);
            _playCardsLabel->SetVerticalOffsetPixels(60);
            _playCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
            _playCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _playCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _playCardsLabel->SetFont(L"Arial");
            _playCardsLabel->SetFontSize(36.0f);
            _playCardsLabel->SetWordWrap(true);
            _playCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
            _playCardsLabel->Resize(_playCardsLabel->GetBaseWidth(), 100);
            _playCardsLabel->SetBaseHeight(_playCardsLabel->GetTextHeight() + 100);

            _canvas->AddComponent(_playCardsLabel.get());

            if (params->playerIndex == _playerIndex)
            {
                _playCardsDoneButton = Create<zcom::Button>(L"Done");
                _playCardsDoneButton->Text()->SetFont(L"Arial");
                _playCardsDoneButton->Text()->SetFontSize(36.0f);
                _playCardsDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
                _playCardsDoneButton->SetBaseSize(150.0f, 60.0f);
                _playCardsDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
                _playCardsDoneButton->SetZIndex(10);
                _playCardsDoneButton->SetVerticalOffsetPixels(-20.0f);
                _playCardsDoneButton->SetBorderVisibility(true);
                _playCardsDoneButton->SetBorderWidth(4.0f);
                _playCardsDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
                _playCardsDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
                _playCardsDoneButton->SetOnActivated([&, request]()
                {
                    _waitingForCardPlay = false;
                    _gettingUserInput = false;

                    _connection->Send(znet::Packet((int)PacketType::PLAY_CARDS_DONE));
                    _board->SendUserInputResponse();
                    _board->DisablePlayCardMode();

                    _canvas->RemoveComponent(_playCardsLabel.get());
                    _canvas->RemoveComponent(_playCardsDoneButton.get());
                });
                _playCardsDoneButton->SetVisible(params->playedCards.size() >= params->minCardCount);

                _canvas->AddComponent(_playCardsDoneButton.get());
            }

            _board->EnablePlayCardMode(params->playerIndex, params->opponentIndex, params->allowedTypes, [&, request](cards::Card* playedCard)
            {
                auto params = reinterpret_cast<UserInputParams_PlayCard*>(request->inputParams.get());
                params->playedCards.push_back(playedCard);

                if (params->playedCards.size() >= params->minCardCount && params->playerIndex == _playerIndex)
                    _playCardsDoneButton->SetVisible(true);

                if (_PlayEndStateReached(params))
                {
                    _waitingForCardPlay = false;
                    _gettingUserInput = false;

                    _board->SendUserInputResponse();
                    _board->DisablePlayCardMode();

                    _canvas->RemoveComponent(_playCardsLabel.get());
                    _canvas->RemoveComponent(_playCardsDoneButton.get());
                }
            });

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

            if (params->playerIndex != _playerIndex)
                _drawCardsLabel = Create<zcom::Label>(L"Waiting for opponent to draw cards");
            else
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

            _canvas->AddComponent(_drawCardsLabel.get());

            if (params->playerIndex == _playerIndex)
            {
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

                    _connection->Send(znet::Packet((int)PacketType::DRAW_CARDS_DONE));
                    _board->SendUserInputResponse();
                    _board->DisableDrawCardMode();

                    _canvas->RemoveComponent(_drawCardsLabel.get());
                    _canvas->RemoveComponent(_drawCardsDoneButton.get());
                });
                _drawCardsDoneButton->SetVisible(params->minCardCount <= 0);

                _canvas->AddComponent(_drawCardsDoneButton.get());
            }

            _board->EnableDrawCardMode(params->playerIndex, params->allowedTypes, [&, request](cards::Card* drawnCard)
            {
                auto params = reinterpret_cast<UserInputParams_DrawCard*>(request->inputParams.get());
                params->drawnCards.push_back(drawnCard);

                if (params->drawnCards.size() >= params->minCardCount && params->playerIndex == _playerIndex)
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
        case UserInputType::DISCARD_CARD:
        {
            auto params = reinterpret_cast<UserInputParams_DiscardCard*>(request->inputParams.get());
            params->discardedCards.clear();

            if (_DiscardEndStateReached(params))
            {
                _board->SendUserInputResponse();
                break;
            }

            _waitingForCardDiscard = true;
            _gettingUserInput = true;

            if (params->playerIndex != _playerIndex)
                _discardCardsLabel = Create<zcom::Label>(L"Waiting for opponent to discard cards");
            else
                _discardCardsLabel = Create<zcom::Label>(request->inputPrompt);
            _discardCardsLabel->SetBaseSize(600, 180);
            _discardCardsLabel->SetVerticalOffsetPixels(60);
            _discardCardsLabel->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::START);
            _discardCardsLabel->SetVerticalTextAlignment(zcom::Alignment::CENTER);
            _discardCardsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::CENTER);
            _discardCardsLabel->SetFont(L"Arial");
            _discardCardsLabel->SetFontSize(36.0f);
            _discardCardsLabel->SetWordWrap(true);
            _discardCardsLabel->SetBackgroundColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.5f));
            _discardCardsLabel->Resize(_discardCardsLabel->GetBaseWidth(), 100);
            _discardCardsLabel->SetBaseHeight(_discardCardsLabel->GetTextHeight() + 100);

            _canvas->AddComponent(_discardCardsLabel.get());

            if (params->playerIndex == _playerIndex)
            {
                _discardCardsDoneButton = Create<zcom::Button>(L"Done");
                _discardCardsDoneButton->Text()->SetFont(L"Arial");
                _discardCardsDoneButton->Text()->SetFontSize(36.0f);
                _discardCardsDoneButton->Text()->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
                _discardCardsDoneButton->SetBaseSize(150.0f, 60.0f);
                _discardCardsDoneButton->SetAlignment(zcom::Alignment::CENTER, zcom::Alignment::END);
                _discardCardsDoneButton->SetZIndex(10);
                _discardCardsDoneButton->SetVerticalOffsetPixels(-20.0f);
                _discardCardsDoneButton->SetBorderVisibility(true);
                _discardCardsDoneButton->SetBorderWidth(4.0f);
                _discardCardsDoneButton->SetBorderColor(D2D1::ColorF(0.8f, 0.8f, 0.8f));
                _discardCardsDoneButton->SetBackgroundColor(D2D1::ColorF(0.2f, 0.2f, 0.2f));
                _discardCardsDoneButton->SetOnActivated([&, request]()
                {
                    _waitingForCardDiscard = false;
                    _gettingUserInput = false;

                    _connection->Send(znet::Packet((int)PacketType::DISCARD_CARDS_DONE));
                    _board->SendUserInputResponse();
                    _board->DisableDiscardCardMode();

                    _canvas->RemoveComponent(_discardCardsLabel.get());
                    _canvas->RemoveComponent(_discardCardsDoneButton.get());
                });
                _discardCardsDoneButton->SetVisible(params->discardedCards.size() >= params->minCardCount);

                _canvas->AddComponent(_discardCardsDoneButton.get());
            }

            _board->EnableDiscardCardMode(params->playerIndex, params->allowedTypes, [&, request](cards::Card* discardedCard)
            {
                auto params = reinterpret_cast<UserInputParams_DiscardCard*>(request->inputParams.get());
                params->discardedCards.push_back(discardedCard);

                if (params->discardedCards.size() >= params->minCardCount && params->playerIndex == _playerIndex)
                    _discardCardsDoneButton->SetVisible(true);

                if (_DiscardEndStateReached(params))
                {
                    _waitingForCardDiscard = false;
                    _gettingUserInput = false;

                    _board->SendUserInputResponse();
                    _board->DisableDiscardCardMode();

                    _canvas->RemoveComponent(_discardCardsLabel.get());
                    _canvas->RemoveComponent(_discardCardsDoneButton.get());
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
        if (params->playerIndex == _playerIndex)
        {
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
}

void EntryScene::_CleanUpAfterChoice()
{
    _canvas->RemoveComponent(_choicePanel.get());
    _choicePanel->ClearItems();
}

bool EntryScene::_PlayEndStateReached(UserInputParams_PlayCard* params)
{
    if (params->playedCards.size() >= params->maxCardCount)
        return true;
    auto& hand = core.GetState().players[params->playerIndex].hand;
    if (hand.empty())
        return true;
    if (!params->allowedTypes.empty() &&
        std::find_if(hand.begin(), hand.end(), [&](auto& card) {
            return zutil::Contains(params->allowedTypes, card->GetCardType());
        }) == hand.end())
        return true;
    return false;
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

bool EntryScene::_DiscardEndStateReached(UserInputParams_DiscardCard* params)
{
    if (params->discardedCards.size() >= params->maxCardCount)
        return true;
    auto& hand = core.GetState().players[params->playerIndex].hand;
    if (hand.empty())
        return true;
    if (!params->allowedTypes.empty() &&
        std::find_if(hand.begin(), hand.end(), [&](auto& card) {
            return zutil::Contains(params->allowedTypes, card->GetCardType());
        }) == hand.end())
        return true;
    return false;
}