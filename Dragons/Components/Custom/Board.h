#pragma once

#include "Components/Base/ComponentBase.h"
#include "Components/Base/Label.h"
#include "Components/Base/Panel.h"
#include "Components/Property.h"

#include "Helper/Event.h"

#include "Core/Core.h"
#include "Core/Cards/CardPosition.h"
#include "Core/Events/TurnEvents.h"
#include "Core/Events/ActionEvents.h"
#include "Core/Events/CardStateEvents.h"
#include "Core/Events/StatsEvents.h"

#include <set>

enum UIElement
{
    OFFENSE_DECK = 0x0000001,
    DEFENSE_DECK = 0x0000001,
    UTILITY_DECK = 0x0000001,
    COMBO_DECK = 0x0000001,
};

struct UIEvent
{
    enum Type
    {
        USER_INPUT_REQUEST,
        TURN_BEGIN,
        TURN_END,
        ACTION_COUNT_CHANGED,
        POST_DAMAGE,
        POST_HEAL,
        POST_ADD_ARMOR,
        POST_DESTROY_ARMOR,
        POST_HEALTH_CHANGE,
        POST_MAX_HEALTH_CHANGE,
        POST_ARMOR_CHANGE,
        CARD_ENTER_HAND,
        CARD_LEAVE_HAND,
        CARD_ENTER_ACTIVES,
        CARD_LEAVE_ACTIVES,
        CARD_ENTER_DECK,
        CARD_LEAVE_DECK,
        CARD_ENTER_GRAVEYARD,
        CARD_LEAVE_GRAVEYARD,
        CARD_ENTER_DESTROYED_CARDS,
        CARD_LEAVE_DESTROYED_CARDS,
        CARD_ENTER_IN_PLAY_CARDS,
        CARD_LEAVE_IN_PLAY_CARDS,
        CARD_ENTER_DISPLAYED_CARDS,
        CARD_LEAVE_DISPLAYED_CARDS,
        HAND_REVEAL_STATE_CHANGED,
        DECK_SHUFFLED
    };

    Type type;
    std::any event;
};

struct EventTimings
{
    Duration preEvent;
    Duration postEvent;
};

EventTimings GetEventTimings(UIEvent::Type event);

struct UIPlayer
{
    int health;
    int maxHealth;
    int armor;
    int actionsLeft;
    std::vector<ExtraAction> extraActions;
    std::vector<cards::Card*> hand;
    std::vector<cards::Card*> activeCards;

    bool handRevealed;
    std::vector<std::string> revealSources;
};

struct UIState
{
    std::vector<UIPlayer> players;
    int currentPlayer = -1;
    int opposingPlayer = -1;

    std::vector<cards::Card*> offenseDeck;
    std::vector<cards::Card*> defenseDeck;
    std::vector<cards::Card*> utilityDeck;
    std::vector<cards::Card*> comboDeck;
    std::vector<cards::Card*> graveyard;
    std::vector<cards::Card*> inPlayCards;
    std::vector<cards::Card*> destroyedCards;

    std::vector<DisplayInfo> displayedCards;

    std::vector<cards::Card*>& GetDeck(cards::CardType type)
    {
        switch (type)
        {
        case cards::CardType::OFFENSE:
            return offenseDeck;
        case cards::CardType::DEFENSE:
            return defenseDeck;
        case cards::CardType::UTILITY:
            return utilityDeck;
        case cards::CardType::COMBO:
            return comboDeck;
        default:
            return offenseDeck;
        }
    }
    DisplayInfo GetDisplayInfo(cards::Card* card)
    {
        for (auto& displayedCard : displayedCards)
        {
            if (displayedCard.card == card)
            {
                return displayedCard;
            }
        }
        return { nullptr, { false, false } };
    }
    void AddCardToHand(cards::Card* card, int playerIndex)
    {
        players[playerIndex].hand.push_back(card);
    }
    void AddCardToActiveCards(cards::Card* card, int playerIndex)
    {
        players[playerIndex].activeCards.push_back(card);
    }
    void AddCardToDeck(cards::Card* card)
    {
        GetDeck(card->GetCardType()).push_back(card);
    }
    void AddCardToGraveyard(cards::Card* card)
    {
        graveyard.push_back(card);
    }
    void AddCardToInPlayCards(cards::Card* card)
    {
        inPlayCards.push_back(card);
    }
    void AddCardToDestroyedCards(cards::Card* card)
    {
        destroyedCards.push_back(card);
    }
    void RemoveCardFromHand(cards::Card* card, int playerIndex)
    {
        RemoveCardFromSet(players[playerIndex].hand, card);
    }
    void RemoveCardFromActiveCards(cards::Card* card, int playerIndex)
    {
        RemoveCardFromSet(players[playerIndex].activeCards, card);
    }
    void RemoveCardFromDeck(cards::Card* card)
    {
        RemoveCardFromSet(GetDeck(card->GetCardType()), card);
    }
    void RemoveCardFromGraveyard(cards::Card* card)
    {
        RemoveCardFromSet(graveyard, card);
    }
    void RemoveCardFromInPlayCards(cards::Card* card)
    {
        RemoveCardFromSet(inPlayCards, card);
    }
    void RemoveCardFromDestroyedCards(cards::Card* card)
    {
        RemoveCardFromSet(destroyedCards, card);
    }

    void RemoveCardFromSet(std::vector<cards::Card*>& cardSet, cards::Card* card)
    {
        auto it = std::find(cardSet.begin(), cardSet.end(), card);
        if (it != cardSet.end())
            cardSet.erase(it);
    }
};

namespace zcom
{
    class Board : public Base
    {
#pragma region base_class
    protected:
        bool _Redraw()
        {
            return true;
        }

        void _OnUpdate();
        void _OnDraw(Graphics g);
        void _OnResize(int width, int height);

        EventTargets _OnMouseMove(int deltaX, int deltaY);
        void _OnMouseEnter();
        void _OnMouseLeave();
        EventTargets _OnLeftPressed(int x, int y);
        EventTargets _OnLeftReleased(int x, int y);
        EventTargets _OnRightPressed(int x, int y);
        EventTargets _OnWheelUp(int x, int y);
        EventTargets _OnWheelDown(int x, int y);

        void _DetectHoveredCard();
        void _HandleInputRequest(cards::PlayResult& result);

    public:
        const char* GetName() const { return Name(); }
        static const char* Name() { return "board"; }
#pragma endregion

    private:
        Core* _core = nullptr;
        UIState _uiState;

        struct _Card
        {
            cards::Card* card = nullptr;
            ID2D1Bitmap* faceBitmap = nullptr;
            ID2D1Bitmap* backBitmap = nullptr;

            float xPos = 0.0f;
            float yPos = 0.0f;
            float rotation = 0.0f;
            float opacity = 1.0f;
            float baseScale = 1.0f;
            float scale = 1.0f;
            cards::CardSet set = { cards::CardSets::NONE, -1 };
            int zIndex = 0;

            bool displayed = false;
            bool displayEnding = false;

            float targetXPos = 0.0f;
            float targetYPos = 0.0f;
            float targetRotation = 0.0f;
            float targetOpacity = 1.0f;
            float targetBaseScale = 1.0f;
            cards::CardSet targetSet = { cards::CardSets::NONE, -1 };
            float startXPos = 0.0f;
            float startYPos = 0.0f;
            float startRotation = 0.0f;
            float startOpacity = 1.0f;
            float startBaseScale = 1.0f;
            cards::CardSet startSet = { cards::CardSets::NONE, -1 };

            bool moving = false;
            TimePoint moveStartTime = 0;
            Duration moveDuration = 0;
        };
        std::vector<std::unique_ptr<_Card>> _cards;
        _Card* _hoveredCard = nullptr;
        _Card* _heldCard = nullptr;

        static constexpr float CARD_WIDTH = 180;
        static constexpr float CARD_HEIGHT = 240;
        static constexpr float RENDER_CARD_WIDTH = 320;
        static constexpr float RENDER_CARD_HEIGHT = 500;
        float DISPLAYED_CARD_SCALE = 1.3f;

        static constexpr float PI = 3.141592f;
        static constexpr float RADIAN = 57.2958f;

        ID2D1Bitmap* _offenseCardBitmap = nullptr;
        ID2D1Bitmap* _defenseCardBitmap = nullptr;
        ID2D1Bitmap* _utilityCardBitmap = nullptr;
        ID2D1Bitmap* _comboCardBitmap = nullptr;
        float shadowRadius = 2.0f;

        int _displayedCardScroll = 0;
        int _maxDisplayedCardScroll = 0;
        zcom::Property<float> _displayedCardOffset{ 0.0f };

        // Input modes

        bool _playCardMode = false;
        int _playCardPlayerIndex = -1;
        int _playCardOpponentIndex = -1;
        std::vector<cards::CardType> _allowedCardTypesToPlay;
        std::function<void(cards::Card*)> _playCardHandler;

        bool _drawCardMode = false;
        int _drawCardPlayerIndex = -1;
        std::vector<cards::CardType> _allowedCardTypesToDraw;
        std::function<void(cards::Card*)> _drawCardHandler;

        bool _noClickMode = false;

        bool _chooseCardsFromHandMode = false;
        int _chooseCardsFromHandChoosingPlayerIndex = -1;
        int _chooseCardsFromHandTargetPlayerIndex = -1;
        int _chooseCardsFromHandMaxCardCount = -1;
        std::vector<cards::CardType> _chooseCardsFromHandAllowedTypes;
        std::set<cards::Card*> _chosenCardsFromHand;
        std::function<void(cards::Card*)> _selectedCardInHandHandler;
        std::function<void(cards::Card*)> _deselectedCardInHandHandler;

        bool _chooseCardsFromActiveCardsMode = false;
        int _chooseCardsFromActiveCardsChoosingPlayerIndex = -1;
        int _chooseCardsFromActiveCardsTargetPlayerIndex = -1;
        int _chooseCardsFromActiveCardsMaxCardCount = -1;
        std::vector<cards::CardType> _chooseCardsFromActiveCardsAllowedTypes;
        std::set<cards::Card*> _chosenCardsFromActiveCards;
        std::function<void(cards::Card*)> _selectedCardInActiveCardsHandler;
        std::function<void(cards::Card*)> _deselectedCardInActiveCardsHandler;

        bool _chooseCardsFromDisplayedCardsMode = false;
        int _chooseCardsFromDisplayedCardsPlayerIndex = -1;
        int _chooseCardsFromDisplayedCardsMaxCardCount = -1;
        std::vector<cards::CardType> _chooseCardsFromDisplayedCardsAllowedTypes;
        std::set<cards::Card*> _chosenCardsFromDisplayedCards;
        std::function<void(cards::Card*)> _selectedCardInDisplayedCardsHandler;
        std::function<void(cards::Card*)> _deselectedCardInDisplayedCardsHandler;

        bool _chooseCardsFromGraveyardMode = false;
        int _chooseCardsFromGraveyardPlayerIndex = -1;
        int _chooseCardsFromGraveyardMaxCardCount = -1;
        std::vector<cards::CardType> _chooseCardsFromGraveyardAllowedTypes;
        std::set<cards::Card*> _chosenCardsFromGraveyard;
        std::function<void(cards::Card*)> _selectedCardInGraveyardHandler;
        std::function<void(cards::Card*)> _deselectedCardInGraveyardHandler;
        std::vector<DisplayInfo> _displayedCardStash;

        bool _chooseDecksMode = false;
        int _chooseDecksPlayerIndex = -1;
        int _chooseDecksMaxDeckCount = -1;
        std::vector<cards::CardType> _chooseDecksAllowedTypes;
        std::set<cards::CardType> _chosenDecks;
        std::function<void(cards::CardType)> _selectedDeckHandler;
        std::function<void(cards::CardType)> _deselectedDeckHandler;

        // Stat labels
        std::unique_ptr<zcom::Panel> _p1StatsPanel = nullptr;
        std::unique_ptr<zcom::Label> _p1HealthLabel = nullptr;
        std::unique_ptr<zcom::Label> _p1MaxHealthLabel = nullptr;
        std::unique_ptr<zcom::Label> _p1ArmorLabel = nullptr;
        std::unique_ptr<zcom::Label> _p1HealthAmountLabel = nullptr;
        std::unique_ptr<zcom::Label> _p1MaxHealthAmountLabel = nullptr;
        std::unique_ptr<zcom::Label> _p1ArmorAmountLabel = nullptr;

        std::unique_ptr<zcom::Panel> _p2StatsPanel = nullptr;
        std::unique_ptr<zcom::Label> _p2HealthLabel = nullptr;
        std::unique_ptr<zcom::Label> _p2MaxHealthLabel = nullptr;
        std::unique_ptr<zcom::Label> _p2ArmorLabel = nullptr;
        std::unique_ptr<zcom::Label> _p2HealthAmountLabel = nullptr;
        std::unique_ptr<zcom::Label> _p2MaxHealthAmountLabel = nullptr;
        std::unique_ptr<zcom::Label> _p2ArmorAmountLabel = nullptr;

        std::unique_ptr<zcom::Label> _p1ExtraActionsLabel = nullptr;
        std::unique_ptr<zcom::Label> _p2ExtraActionsLabel = nullptr;

        // Game event handlers
        UserInputRequest _pendingUserInputRequest;
        std::vector<UIEvent> _uiEventQueue;
        TimePoint _prevEventTime;
        Duration _minTimeUntilNextEvent;

        std::unique_ptr<EventHandler<TurnBeginEvent>> _turnBeginHandler = nullptr;
        std::unique_ptr<EventHandler<TurnEndEvent>> _turnEndHandler = nullptr;
        std::unique_ptr<EventHandler<ActionCountChangedEvent>> _actionCountChangedHandler = nullptr;
        std::unique_ptr<EventHandler<PostDamageEvent>> _postDamageHandler = nullptr;
        std::unique_ptr<EventHandler<PostHealEvent>> _postHealHandler = nullptr;
        std::unique_ptr<EventHandler<PostAddArmorEvent>> _postAddArmorHandler = nullptr;
        std::unique_ptr<EventHandler<PostDestroyArmorEvent>> _postDestroyArmorHandler = nullptr;
        std::unique_ptr<EventHandler<PostHealthChangeEvent>> _postHealthChangeHandler = nullptr;
        std::unique_ptr<EventHandler<PostMaxHealthChangeEvent>> _postMaxHealthChangeHandler = nullptr;
        std::unique_ptr<EventHandler<PostArmorChangeEvent>> _postArmorChangeHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterHandEvent>> _cardEnterHandHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveHandEvent>> _cardLeaveHandHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterActivesEvent>> _cardEnterActivesHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveActivesEvent>> _cardLeaveActivesHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterDeckEvent>> _cardEnterDeckHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveDeckEvent>> _cardLeaveDeckHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterGraveyardEvent>> _cardEnterGraveyardHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveGraveyardEvent>> _cardLeaveGraveyardHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterDestroyedCardsEvent>> _cardEnterDestroyedCardsHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveDestroyedCardsEvent>> _cardLeaveDestroyedCardsHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterInPlayCardsEvent>> _cardEnterInPlayCardsHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveInPlayCardsEvent>> _cardLeaveInPlayCardsHandler = nullptr;
        std::unique_ptr<EventHandler<CardEnterDisplayedCardsEvent>> _cardEnterDisplayedCardsHandler = nullptr;
        std::unique_ptr<EventHandler<CardLeaveDisplayedCardsEvent>> _cardLeaveDisplayedCardsHandler = nullptr;
        std::unique_ptr<EventHandler<HandRevealStateChangedEvent>> _handRevealStateChangedHandler = nullptr;
        std::unique_ptr<EventHandler<DeckShuffledEvent>> _deckShuffledHandler = nullptr;

        // User input interface
        //bool _userInputRequestAvailable = false;
        //UserInputRequest _userInputRequest;
        std::vector<UserInputRequest> _userInputRequests;
        int _requestCounter = 0;
    public:
        UserInputRequest* GetUserInputRequest();
        int GetRequestCounter();
        void SendUserInputResponse();
    private:

    protected:
        friend class Scene;
        friend class Base;
        Board(Scene* scene) : Base(scene) {}
        void Init(Core* core)
        {
            _displayedCardOffset.EnableAnimation();
            _displayedCardOffset.SetAnimationDuration(Duration(150, MILLISECONDS));
            _displayedCardOffset.SetInterpolationFunction([](float delta) { return 1.0f - std::powf(delta - 1.0f, 2.0f); });

            _core = core;

            _uiState.players.push_back(UIPlayer());
            _uiState.players.push_back(UIPlayer());
            _uiState.players[0].health = _core->GetState().players[0].health;
            _uiState.players[0].maxHealth = _core->GetState().players[0].maxHealth;
            _uiState.players[0].armor = _core->GetState().players[0].armor;
            _uiState.players[0].actionsLeft = _core->GetState().players[0].actionsLeft;
            _uiState.players[0].extraActions = _core->GetState().players[0].extraActions;
            _uiState.players[0].handRevealed = _core->GetState().players[0].handRevealed;
            _uiState.players[1].health = _core->GetState().players[1].health;
            _uiState.players[1].maxHealth = _core->GetState().players[1].maxHealth;
            _uiState.players[1].armor = _core->GetState().players[1].armor;
            _uiState.players[1].actionsLeft = _core->GetState().players[1].actionsLeft;
            _uiState.players[1].extraActions = _core->GetState().players[1].extraActions;
            _uiState.players[1].handRevealed = _core->GetState().players[1].handRevealed;
            _uiState.currentPlayer = _core->GetState().currentPlayer;
            _uiState.opposingPlayer = _core->GetState().opposingPlayer;

            _CreateMissingCards();

            _p1HealthLabel = Create<zcom::Label>(L"Health:");
            _p1HealthLabel->SetVerticalOffsetPixels(0);
            _p1HealthLabel->SetBaseSize(250, 40);
            _p1HealthLabel->SetFont(L"Arial");
            _p1HealthLabel->SetFontSize(36.0f);
            _p1HealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p1MaxHealthLabel = Create<zcom::Label>(L"Max health:");
            _p1MaxHealthLabel->SetVerticalOffsetPixels(40);
            _p1MaxHealthLabel->SetBaseSize(250, 40);
            _p1MaxHealthLabel->SetFont(L"Arial");
            _p1MaxHealthLabel->SetFontSize(36.0f);
            _p1MaxHealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p1ArmorLabel = Create<zcom::Label>(L"Armor:");
            _p1ArmorLabel->SetVerticalOffsetPixels(80);
            _p1ArmorLabel->SetBaseSize(250, 40);
            _p1ArmorLabel->SetFont(L"Arial");
            _p1ArmorLabel->SetFontSize(36.0f);
            _p1ArmorLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p1HealthAmountLabel = Create<zcom::Label>(L"10");
            _p1HealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
            _p1HealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p1HealthAmountLabel->SetVerticalOffsetPixels(0);
            _p1HealthAmountLabel->SetBaseSize(50, 40);
            _p1HealthAmountLabel->SetFont(L"Arial");
            _p1HealthAmountLabel->SetFontSize(36.0f);
            _p1HealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p1MaxHealthAmountLabel = Create<zcom::Label>(L"10");
            _p1MaxHealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
            _p1MaxHealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p1MaxHealthAmountLabel->SetVerticalOffsetPixels(40);
            _p1MaxHealthAmountLabel->SetBaseSize(50, 40);
            _p1MaxHealthAmountLabel->SetFont(L"Arial");
            _p1MaxHealthAmountLabel->SetFontSize(36.0f);
            _p1MaxHealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p1ArmorAmountLabel = Create<zcom::Label>(L"0");
            _p1ArmorAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
            _p1ArmorAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p1ArmorAmountLabel->SetVerticalOffsetPixels(80);
            _p1ArmorAmountLabel->SetBaseSize(50, 40);
            _p1ArmorAmountLabel->SetFont(L"Arial");
            _p1ArmorAmountLabel->SetFontSize(36.0f);
            _p1ArmorAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p1StatsPanel = Create<zcom::Panel>();
            _p1StatsPanel->AddItem(_p1HealthLabel.get());
            _p1StatsPanel->AddItem(_p1MaxHealthLabel.get());
            _p1StatsPanel->AddItem(_p1ArmorLabel.get());
            _p1StatsPanel->AddItem(_p1HealthAmountLabel.get());
            _p1StatsPanel->AddItem(_p1MaxHealthAmountLabel.get());
            _p1StatsPanel->AddItem(_p1ArmorAmountLabel.get());
            _p1StatsPanel->Resize(300, 120);

            _p2HealthLabel = Create<zcom::Label>(L"Health:");
            _p2HealthLabel->SetVerticalOffsetPixels(0);
            _p2HealthLabel->SetBaseSize(250, 40);
            _p2HealthLabel->SetFont(L"Arial");
            _p2HealthLabel->SetFontSize(36.0f);
            _p2HealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p2MaxHealthLabel = Create<zcom::Label>(L"Max health:");
            _p2MaxHealthLabel->SetVerticalOffsetPixels(40);
            _p2MaxHealthLabel->SetBaseSize(250, 40);
            _p2MaxHealthLabel->SetFont(L"Arial");
            _p2MaxHealthLabel->SetFontSize(36.0f);
            _p2MaxHealthLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p2ArmorLabel = Create<zcom::Label>(L"Armor:");
            _p2ArmorLabel->SetVerticalOffsetPixels(80);
            _p2ArmorLabel->SetBaseSize(250, 40);
            _p2ArmorLabel->SetFont(L"Arial");
            _p2ArmorLabel->SetFontSize(36.0f);
            _p2ArmorLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p2HealthAmountLabel = Create<zcom::Label>(L"10");
            _p2HealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
            _p2HealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p2HealthAmountLabel->SetVerticalOffsetPixels(0);
            _p2HealthAmountLabel->SetBaseSize(50, 40);
            _p2HealthAmountLabel->SetFont(L"Arial");
            _p2HealthAmountLabel->SetFontSize(36.0f);
            _p2HealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p2MaxHealthAmountLabel = Create<zcom::Label>(L"10");
            _p2MaxHealthAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
            _p2MaxHealthAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p2MaxHealthAmountLabel->SetVerticalOffsetPixels(40);
            _p2MaxHealthAmountLabel->SetBaseSize(50, 40);
            _p2MaxHealthAmountLabel->SetFont(L"Arial");
            _p2MaxHealthAmountLabel->SetFontSize(36.0f);
            _p2MaxHealthAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p2ArmorAmountLabel = Create<zcom::Label>(L"0");
            _p2ArmorAmountLabel->SetHorizontalAlignment(zcom::Alignment::END);
            _p2ArmorAmountLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p2ArmorAmountLabel->SetVerticalOffsetPixels(80);
            _p2ArmorAmountLabel->SetBaseSize(50, 40);
            _p2ArmorAmountLabel->SetFont(L"Arial");
            _p2ArmorAmountLabel->SetFontSize(36.0f);
            _p2ArmorAmountLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);

            _p2StatsPanel = Create<zcom::Panel>();
            _p2StatsPanel->AddItem(_p2HealthLabel.get());
            _p2StatsPanel->AddItem(_p2MaxHealthLabel.get());
            _p2StatsPanel->AddItem(_p2ArmorLabel.get());
            _p2StatsPanel->AddItem(_p2HealthAmountLabel.get());
            _p2StatsPanel->AddItem(_p2MaxHealthAmountLabel.get());
            _p2StatsPanel->AddItem(_p2ArmorAmountLabel.get());
            _p2StatsPanel->Resize(300, 120);

            _p1ExtraActionsLabel = Create<zcom::Label>(L"Actions left: 2\nExtra actions: 3\n[Play] -\n[Play] -\n[Play or Draw] -");
            _p1ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::END);
            _p1ExtraActionsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p1ExtraActionsLabel->SetOffsetPixels(-40, -160);
            _p1ExtraActionsLabel->SetFont(L"Arial");
            _p1ExtraActionsLabel->SetFontSize(24.0f);
            _p1ExtraActionsLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _p1ExtraActionsLabel->Resize(300, 250);

            _p2ExtraActionsLabel = Create<zcom::Label>(L"Actions left: 2\nExtra actions: 3\n[Play] -\n[Play] -\n[Play or Draw] -");
            _p2ExtraActionsLabel->SetVerticalTextAlignment(zcom::Alignment::START);
            _p2ExtraActionsLabel->SetHorizontalTextAlignment(zcom::TextAlignment::TRAILING);
            _p2ExtraActionsLabel->SetOffsetPixels(-40, 160);
            _p2ExtraActionsLabel->SetFont(L"Arial");
            _p2ExtraActionsLabel->SetFontSize(24.0f);
            _p2ExtraActionsLabel->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
            _p2ExtraActionsLabel->Resize(300, 250);

            _UpdateStatLabels();

            _turnBeginHandler = std::make_unique<EventHandler<TurnBeginEvent>>(&_core->Events(), [=](TurnBeginEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::TURN_BEGIN, std::any(event) });
            });
            _turnEndHandler = std::make_unique<EventHandler<TurnEndEvent>>(&_core->Events(), [=](TurnEndEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::TURN_END, std::any(event) });
            });
            _actionCountChangedHandler = std::make_unique<EventHandler<ActionCountChangedEvent>>(&_core->Events(), [=](ActionCountChangedEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::ACTION_COUNT_CHANGED, std::any(event) });
            });
            _postDamageHandler = std::make_unique<EventHandler<PostDamageEvent>>(&_core->Events(), [=](PostDamageEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_DAMAGE, std::any(event) });
            });
            _postHealHandler = std::make_unique<EventHandler<PostHealEvent>>(&_core->Events(), [=](PostHealEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_HEAL, std::any(event) });
            });
            _postAddArmorHandler = std::make_unique<EventHandler<PostAddArmorEvent>>(&_core->Events(), [=](PostAddArmorEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_ADD_ARMOR, std::any(event) });
            });
            _postDestroyArmorHandler = std::make_unique<EventHandler<PostDestroyArmorEvent>>(&_core->Events(), [=](PostDestroyArmorEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_DESTROY_ARMOR, std::any(event) });
            });
            _postHealthChangeHandler = std::make_unique<EventHandler<PostHealthChangeEvent>>(&_core->Events(), [=](PostHealthChangeEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_HEALTH_CHANGE, std::any(event) });
            });
            _postMaxHealthChangeHandler = std::make_unique<EventHandler<PostMaxHealthChangeEvent>>(&_core->Events(), [=](PostMaxHealthChangeEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_MAX_HEALTH_CHANGE, std::any(event) });
            });
            _postArmorChangeHandler = std::make_unique<EventHandler<PostArmorChangeEvent>>(&_core->Events(), [=](PostArmorChangeEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::POST_ARMOR_CHANGE, std::any(event) });
            });
            _cardEnterHandHandler = std::make_unique<EventHandler<CardEnterHandEvent>>(&_core->Events(), [=](CardEnterHandEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_HAND, std::any(event) });
            });
            _cardLeaveHandHandler = std::make_unique<EventHandler<CardLeaveHandEvent>>(&_core->Events(), [=](CardLeaveHandEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_HAND, std::any(event) });
            });
            _cardEnterActivesHandler = std::make_unique<EventHandler<CardEnterActivesEvent>>(&_core->Events(), [=](CardEnterActivesEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_ACTIVES, std::any(event) });
            });
            _cardLeaveActivesHandler = std::make_unique<EventHandler<CardLeaveActivesEvent>>(&_core->Events(), [=](CardLeaveActivesEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_ACTIVES, std::any(event) });
            });
            _cardEnterDeckHandler = std::make_unique<EventHandler<CardEnterDeckEvent>>(&_core->Events(), [=](CardEnterDeckEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_DECK, std::any(event) });
            });
            _cardLeaveDeckHandler = std::make_unique<EventHandler<CardLeaveDeckEvent>>(&_core->Events(), [=](CardLeaveDeckEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_DECK, std::any(event) });
            });
            _cardEnterGraveyardHandler = std::make_unique<EventHandler<CardEnterGraveyardEvent>>(&_core->Events(), [=](CardEnterGraveyardEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_GRAVEYARD, std::any(event) });
            });
            _cardLeaveGraveyardHandler = std::make_unique<EventHandler<CardLeaveGraveyardEvent>>(&_core->Events(), [=](CardLeaveGraveyardEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_GRAVEYARD, std::any(event) });
            });
            _cardEnterDestroyedCardsHandler = std::make_unique<EventHandler<CardEnterDestroyedCardsEvent>>(&_core->Events(), [=](CardEnterDestroyedCardsEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_DESTROYED_CARDS, std::any(event) });
            });
            _cardLeaveDestroyedCardsHandler = std::make_unique<EventHandler<CardLeaveDestroyedCardsEvent>>(&_core->Events(), [=](CardLeaveDestroyedCardsEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_DESTROYED_CARDS , std::any(event) });
            });
            _cardEnterInPlayCardsHandler = std::make_unique<EventHandler<CardEnterInPlayCardsEvent>>(&_core->Events(), [=](CardEnterInPlayCardsEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_IN_PLAY_CARDS, std::any(event) });
            });
            _cardLeaveInPlayCardsHandler = std::make_unique<EventHandler<CardLeaveInPlayCardsEvent>>(&_core->Events(), [=](CardLeaveInPlayCardsEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_IN_PLAY_CARDS, std::any(event) });
            });
            _cardEnterDisplayedCardsHandler = std::make_unique<EventHandler<CardEnterDisplayedCardsEvent>>(&_core->Events(), [=](CardEnterDisplayedCardsEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_ENTER_DISPLAYED_CARDS, std::any(event) });
            });
            _cardLeaveDisplayedCardsHandler = std::make_unique<EventHandler<CardLeaveDisplayedCardsEvent>>(&_core->Events(), [=](CardLeaveDisplayedCardsEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::CARD_LEAVE_DISPLAYED_CARDS, std::any(event) });
            });
            _handRevealStateChangedHandler = std::make_unique<EventHandler<HandRevealStateChangedEvent>>(&_core->Events(), [=](HandRevealStateChangedEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::HAND_REVEAL_STATE_CHANGED, std::any(event) });
            });
            _deckShuffledHandler = std::make_unique<EventHandler<DeckShuffledEvent>>(&_core->Events(), [=](DeckShuffledEvent event)
            {
                _uiEventQueue.push_back({ UIEvent::DECK_SHUFFLED, std::any(event) });
            });

            _SyncCardPositions();
            _SyncUISets();
            _CalculateCardTargetPositions();
            for (auto& card : _cards)
            {
                card->xPos = card->targetXPos;
                card->yPos = card->targetYPos;
                card->rotation = card->targetRotation;
            }
        }
    public:
        ~Board()
        {
            _p1StatsPanel->ClearItems();
            _p2StatsPanel->ClearItems();
        }
        Board(Board&&) = delete;
        Board& operator=(Board&&) = delete;
        Board(const Board&) = delete;
        Board& operator=(const Board&) = delete;

    public:
        UIState& UIState() { return _uiState; }

        void EnableNoClickMode();
        void DisableNoClickMode();
        void EnableChooseCardFromHandMode(int choosingPlayerIndex, int targetPlayerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect);
        void DisableChooseCardFromHandMode();
        void EnableChooseCardFromActiveCardsMode(int choosingPlayerIndex, int targetPlayerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect);
        void DisableChooseCardFromActiveCardsMode();
        void EnableChooseCardFromDisplayedCardsMode(int playerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect);
        void DisableChooseCardFromDisplayedCardsMode();
        void EnableChooseCardFromGraveyardMode(int playerIndex, int maxCards, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardSelect, std::function<void(cards::Card*)> onCardDeselect);
        void DisableChooseCardFromGraveyardMode();
        void EnableChooseDeckMode(int playerIndex, int maxDecks, std::vector<cards::CardType> allowedTypes, std::function<void(cards::CardType)> onDeckSelect, std::function<void(cards::CardType)> onDeckDeselect);
        void DisableChooseDeckMode();
        void EnablePlayCardMode(int playerIndex, int opponentIndex, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardDraw);
        void DisablePlayCardMode();
        void EnableDrawCardMode(int playerIndex, std::vector<cards::CardType> allowedTypes, std::function<void(cards::Card*)> onCardDraw);
        void DisableDrawCardMode();

    private:
        void _GenerateCardBitmap(Graphics g, ID2D1Bitmap** bitmapRef, D2D1_COLOR_F color);
        void _UpdateStatLabels();
        void _CreateMissingCards();
        void _SyncCardPositions();
        void _SyncUISets();
        void _CalculateCardTargetPositions();
        _Card* _GetHoveredCard();
        _Card* _GetCardFromPointer(cards::Card* cardPtr, bool displayed = false);
        void _ProcessCardMove(_Card* card, UIEvent pairedEvent);
        void _MoveCardToSet(_Card* card, cards::CardSet newSet);
        void _MoveNewCardToSet(_Card* card, cards::CardSet set);

        void _HandleEvents();
    };
}