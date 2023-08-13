#pragma once

#include "Scene.h"

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"
#include "Components/Base/Button.h"

#include "Components/Custom/Board.h"

#include "Core/Core.h"
#include "Core/Events/TurnEvents.h"

#include <set>

struct EntrySceneOptions : public SceneOptionsBase
{

};

class EntryScene : public Scene
{
    Core core;

    std::unique_ptr<zcom::Board> _board = nullptr;

    // User input

    void _ProcessNewInputRequests();
    void _UpdateInputRequests();
    bool _gettingUserInput = false;
    int _currentInputRequestId = -1;

    bool _waitingForConfirmation = false;
    std::unique_ptr<zcom::Button> _confirmationButton = nullptr;

    bool _waitingForYesOrNoChoice = false;
    std::unique_ptr<zcom::Panel> _yesOrNoPanel = nullptr;
    std::unique_ptr<zcom::Label> _yesOrNoPromptLabel = nullptr;
    std::unique_ptr<zcom::Button> _yesButton = nullptr;
    std::unique_ptr<zcom::Button> _noButton = nullptr;

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

    bool _waitingForChoice = false;
    std::unique_ptr<zcom::Panel> _choicePanel = nullptr;
    std::unique_ptr<zcom::Label> _choicePromptLabel = nullptr;
    void _CleanUpAfterChoice();

    bool _waitingToChooseCardsFromHand = false;
    std::set<cards::Card*> _chosenCardsFromHand;
    std::unique_ptr<zcom::Label> _chooseCardsFromHandLabel;
    std::unique_ptr<zcom::Button> _chooseCardsFromHandDoneButton;

    bool _waitingToChooseCardsFromActiveCards = false;
    std::set<cards::Card*> _chosenCardsFromActiveCards;
    std::unique_ptr<zcom::Label> _chooseCardsFromActiveCardsLabel;
    std::unique_ptr<zcom::Button> _chooseCardsFromActiveCardsDoneButton;

    bool _waitingToChooseCardsFromDisplayedCards = false;
    std::set<cards::Card*> _chosenCardsFromDisplayedCards;
    std::unique_ptr<zcom::Label> _chooseCardsFromDisplayedCardsLabel;
    std::unique_ptr<zcom::Button> _chooseCardsFromDisplayedCardsDoneButton;

    bool _waitingToChooseCardsFromGraveyard = false;
    std::set<cards::Card*> _chosenCardsFromGraveyard;
    std::unique_ptr<zcom::Label> _chooseCardsFromGraveyardLabel;
    std::unique_ptr<zcom::Button> _chooseCardsFromGraveyardDoneButton;

    bool _waitingToChooseDecks = false;
    std::set<cards::CardType> _chosenDecks;
    std::unique_ptr<zcom::Label> _chooseDecksLabel;
    std::unique_ptr<zcom::Button> _chooseDecksDoneButton;

    bool _waitingForCardPlay = false;
    std::unique_ptr<zcom::Label> _playCardsLabel;
    std::unique_ptr<zcom::Button> _playCardsDoneButton;
    bool _PlayEndStateReached(UserInputParams_PlayCard* params);

    bool _waitingForCardDraw = false;
    std::unique_ptr<zcom::Label> _drawCardsLabel;
    std::unique_ptr<zcom::Button> _drawCardsDoneButton;
    bool _DrawEndStateReached(UserInputParams_DrawCard* params);

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
