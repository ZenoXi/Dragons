#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"
#include "Components/Base/Button.h"

#include "Components/Custom/Board.h"

#include "Core/Core.h"
#include "Core/Events/TurnEvents.h"

#include "Helper/NetBase2.h"

#include <set>

struct EntrySceneOptions : public SceneOptionsBase
{
    znet::TCPClientRef connection;
    uint32_t gameSeed;
    int playerIndex;
    int opponentIndex;
};

class EntryScene : public Scene
{
    Core core;

    std::unique_ptr<zcom::Board> _board = nullptr;

    // Network

    znet::TCPClientRef _connection;
    uint32_t _gameSeed;
    int _playerIndex;
    int _opponentIndex;

    // User input

    void _ProcessOpponentActions();
    void _ProcessNewInputRequests();
    void _UpdateInputRequests();
    bool _gettingUserInput = false;
    int _currentInputRequestId = -1;

    bool _waitingForConfirmation = false;
    std::unique_ptr<zcom::Button> _confirmationButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentConfirmationLabel = nullptr;

    bool _waitingForYesOrNoChoice = false;
    std::unique_ptr<zcom::Panel> _yesOrNoPanel = nullptr;
    std::unique_ptr<zcom::Label> _yesOrNoPromptLabel = nullptr;
    std::unique_ptr<zcom::Button> _yesButton = nullptr;
    std::unique_ptr<zcom::Button> _noButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentYesOrNoLabel = nullptr;

    bool _waitingForNumberChoice = false;
    int _chosenNumber = 0;
    std::unique_ptr<zcom::Panel> _numberChoicePanel = nullptr;
    std::unique_ptr<zcom::Label> _numberChoicePromptLabel = nullptr;
    std::unique_ptr<zcom::Label> _chosenNumberLabel = nullptr;
    std::unique_ptr<zcom::Button> _plus1Button = nullptr;
    std::unique_ptr<zcom::Button> _plus2Button = nullptr;
    std::unique_ptr<zcom::Button> _plus5Button = nullptr;
    std::unique_ptr<zcom::Button> _minus1Button = nullptr;
    std::unique_ptr<zcom::Button> _minus2Button = nullptr;
    std::unique_ptr<zcom::Button> _minus5Button = nullptr;
    std::unique_ptr<zcom::Button> _numberChoiceConfirmButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentNumberChoiceLabel = nullptr;

    bool _waitingForChoice = false;
    std::unique_ptr<zcom::Panel> _choicePanel = nullptr;
    std::unique_ptr<zcom::Label> _choicePromptLabel = nullptr;
    std::unique_ptr<zcom::Label> _opponentChoiceLabel = nullptr;
    void _CleanUpAfterChoice();

    bool _waitingToChooseCardsFromHand = false;
    std::set<cards::Card*> _chosenCardsFromHand;
    std::unique_ptr<zcom::Label> _chooseCardsFromHandLabel = nullptr;
    std::unique_ptr<zcom::Button> _chooseCardsFromHandDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentChooseCardsFromHandLabel = nullptr;

    bool _waitingToChooseCardsFromActiveCards = false;
    std::set<cards::Card*> _chosenCardsFromActiveCards;
    std::unique_ptr<zcom::Label> _chooseCardsFromActiveCardsLabel = nullptr;
    std::unique_ptr<zcom::Button> _chooseCardsFromActiveCardsDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentChooseCardsFromActiveCardsLabel = nullptr;

    bool _waitingToChooseCardsFromDisplayedCards = false;
    std::set<cards::Card*> _chosenCardsFromDisplayedCards;
    std::unique_ptr<zcom::Label> _chooseCardsFromDisplayedCardsLabel = nullptr;
    std::unique_ptr<zcom::Button> _chooseCardsFromDisplayedCardsDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentChooseCardsFromDisplayedCardsLabel = nullptr;

    bool _waitingToChooseCardsFromGraveyard = false;
    std::set<cards::Card*> _chosenCardsFromGraveyard;
    std::unique_ptr<zcom::Label> _chooseCardsFromGraveyardLabel = nullptr;
    std::unique_ptr<zcom::Button> _chooseCardsFromGraveyardDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentChooseCardsFromGraveyardLabel = nullptr;

    bool _waitingToChooseDecks = false;
    std::set<cards::CardType> _chosenDecks;
    std::unique_ptr<zcom::Label> _chooseDecksLabel = nullptr;
    std::unique_ptr<zcom::Button> _chooseDecksDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentChooseDecksLabel = nullptr;

    bool _waitingForCardPlay = false;
    std::unique_ptr<zcom::Label> _playCardsLabel = nullptr;
    std::unique_ptr<zcom::Button> _playCardsDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentPlayCardsLabel = nullptr;
    bool _PlayEndStateReached(UserInputParams_PlayCard* params);

    bool _waitingForCardDraw = false;
    std::unique_ptr<zcom::Label> _drawCardsLabel = nullptr;
    std::unique_ptr<zcom::Button> _drawCardsDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentDrawCardsLabel = nullptr;
    bool _DrawEndStateReached(UserInputParams_DrawCard* params);

    bool _waitingForCardDiscard = false;
    std::unique_ptr<zcom::Label> _discardCardsLabel = nullptr;
    std::unique_ptr<zcom::Button> _discardCardsDoneButton = nullptr;
    std::unique_ptr<zcom::Label> _opponentDiscardCardsLabel = nullptr;
    bool _DiscardEndStateReached(UserInputParams_DiscardCard* params);

public:
    EntryScene(App* app);

    const char* GetName() const { return "entry"; }
    static const char* StaticName() { return "entry"; }

private:
    void _Init(const SceneOptionsBase* options);
    void _Uninit();
    void _Focus();
    void _Unfocus();
    void _Update();
    void _Resize(int width, int height);
};
