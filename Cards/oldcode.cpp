#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <ctime>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

bool statement = true;

void P2play(string P2ChooseCard);
void P1play(string P1ChooseCard);
void P1draw(string P1ChooseDeck);
void P2draw(string P1ChooseDeck);

time_t miliseconds;

int P1ActionsLeft = 2;
int P2ActionsLeft = 2;

int Player1HP = 20;
int Player2HP = 20;
int Player1Armor = 0;
int Player2Armor = 0;
int Player1MaxHP = 20;
int Player2MaxHP = 20;

int CardsLeftInOffDeck = 19;
int CardsLeftInDefDeck = 19;
int CardsLeftInUtiDeck = 19;
int CardsLeftInComDeck = 19;
int CardsInGraveyard = 0;

int cardsInP1Hand = 0;
int cardsInP2Hand = 0;

string Player1Hand[20];
string Player2Hand[20];

string OffDeck[20];
string DefDeck[20];
string UtiDeck[20];
string ComDeck[20];
string Graveyard[80];

string P1ActiveCards[20];
int P1NumberOfActiveCards = 0;
string P2ActiveCards[20];
int P2NumberOfActiveCards = 0;

int Random_Number;

string RandomOffCard;
string OFFCARDS[20];
string RandomDefCard;
string DEFCARDS[20];
string RandomUtiCard;
string UTICARDS[20];
string RandomComCard;
string COMCARDS[20];

string UsedOff[20];
string UsedDef[20];
int RandomizedOffCards = 0;
int RandomizedDefCards = 0;

bool P1ActiveSpiritShelter = false;
bool P1ActiveSpiritShelterD = false;
bool P2ActiveSpiritShelter = false;
bool P2ActiveSpiritShelterD = false;

bool P1ActiveBarrier = false;
bool P1ActiveBarrierD = false;
bool P2ActiveBarrier = false;
bool P2ActiveBarrierD = false;

bool P1ActiveSoothingSpell = false;
bool P1ActiveSoothingSpellD = false;
bool P2ActiveSoothingSpell = false;
bool P2ActiveSoothingSpellD = false;

bool P1ActiveVitalSpot = false;
bool P1ActiveVitalSpotD = false;
bool P2ActiveVitalSpot = false;
bool P2ActiveVitalSpotD = false;

bool P1ActiveFireMoon = false;
bool P1ActiveFireMoonD = false;
bool P2ActiveFireMoon = false;
bool P2ActiveFireMoonD = false;

bool P1ActiveAbsoluteZero = false;
bool P1ActiveAbsoluteZeroD = false;
bool P2ActiveAbsoluteZero = false;
bool P2ActiveAbsoluteZeroD = false;

bool P1UsedRapidAttack = false;
bool P1UsedRapidAttackD = false;
bool P2UsedRapidAttack = false;
bool P2UsedRapidAttackD = false;

bool NoCardsInDecks = false;

void P1damageSelf(int P1dmgSelf)
{
    if (P1ActiveVitalSpot == true || P1ActiveVitalSpotD == true) {
        Player1HP -= P1dmgSelf;
        if (P1ActiveVitalSpot == true) {
            P1ActiveVitalSpot = false;
            Graveyard[CardsInGraveyard] = "P1ActiveVitalSpot";
            CardsInGraveyard++;
        }
        else {
            P1ActiveVitalSpotD = false;
            Graveyard[CardsInGraveyard] = "P1ActiveVitalSpotD";
            CardsInGraveyard++;
        }
        cout << "Dealt " << P1dmgSelf << " damage. Player 1 has " << Player1HP << " HP and " << Player1Armor << " armor" << endl;
    }
    else {
        if (P1ActiveBarrier == false && P1ActiveBarrierD == false) {
            for (int P1SelfDmgTick = 0; P1SelfDmgTick < P1dmgSelf; P1SelfDmgTick++) {
                if (Player1Armor > 0) {
                    Player1Armor--;
                }
                else Player1HP--;
            }
            cout << "Dealt " << P1dmgSelf << " damage. Player 1 has " << Player1HP << " HP and " << Player1Armor << " armor" << endl;
        }
        else if (P1ActiveBarrier == true) {
            P1ActiveBarrier = false;
            Graveyard[CardsInGraveyard] = "P1ActiveBarrier";
            CardsInGraveyard++;
        }
        else {
            P1ActiveBarrierD = false;
            Graveyard[CardsInGraveyard] = "P1ActiveBarrierD";
            CardsInGraveyard++;
        }
    }
}

void P2damageSelf(int P2dmgSelf)
{
    if (P2ActiveVitalSpot == true || P2ActiveVitalSpotD == true) {
        Player2HP -= P2dmgSelf;
        if (P2ActiveVitalSpot == true) {
            P2ActiveVitalSpot = false;
            Graveyard[CardsInGraveyard] = "P2ActiveVitalSpot";
            CardsInGraveyard++;
        }
        else {
            P2ActiveVitalSpotD = false;
            Graveyard[CardsInGraveyard] = "P2ActiveVitalSpotD";
            CardsInGraveyard++;
        }
        cout << "Dealt " << P2dmgSelf << " damage. Player 2 has " << Player2HP << " HP and " << Player2Armor << " armor" << endl;
    }
    else {
        if (P2ActiveBarrier == false && P2ActiveBarrierD == false) {
            for (int P2SelfDmgTick = 0; P2SelfDmgTick < P2dmgSelf; P2SelfDmgTick++) {
                if (Player2Armor > 0) {
                    Player2Armor--;
                }
                else Player2HP--;
            }
            cout << "Dealt " << P2dmgSelf << " damage. Player 2 has " << Player2HP << " HP and " << Player2Armor << " armor" << endl;
        }
        else if (P2ActiveBarrier == true) {
            P2ActiveBarrier = false;
            Graveyard[CardsInGraveyard] = "P2ActiveBarrier";
            CardsInGraveyard++;
        }
        else {
            P2ActiveBarrierD = false;
            Graveyard[CardsInGraveyard] = "P2ActiveBarrierD";
            CardsInGraveyard++;
        }
    }
}

void GameEndDraw()
{
    NoCardsInDecks = true;
    cout << "No cards in any decks. Draw." << endl;
}

void P1AddCardInHand(string P1Add)
{
    if (P1Add == OFFCARDS[0] || P1Add == OFFCARDS[1] || P1Add == OFFCARDS[2] || P1Add == OFFCARDS[3] ||
        P1Add == OFFCARDS[4] || P1Add == OFFCARDS[5] || P1Add == OFFCARDS[6] || P1Add == OFFCARDS[7] ||
        P1Add == OFFCARDS[8] || P1Add == OFFCARDS[9] || P1Add == OFFCARDS[10] || P1Add == OFFCARDS[11] ||
        P1Add == OFFCARDS[12] || P1Add == OFFCARDS[13] || P1Add == OFFCARDS[14] || P1Add == OFFCARDS[15] ||
        P1Add == OFFCARDS[16] || P1Add == OFFCARDS[17] || P1Add == OFFCARDS[18] || P1Add == OFFCARDS[19] ||
        P1Add == DEFCARDS[0] || P1Add == DEFCARDS[1] || P1Add == DEFCARDS[2] || P1Add == DEFCARDS[3] ||
        P1Add == DEFCARDS[4] || P1Add == DEFCARDS[5] || P1Add == DEFCARDS[6] || P1Add == DEFCARDS[7] ||
        P1Add == DEFCARDS[8] || P1Add == DEFCARDS[9] || P1Add == DEFCARDS[10] || P1Add == DEFCARDS[11] ||
        P1Add == DEFCARDS[12] || P1Add == DEFCARDS[13] || P1Add == DEFCARDS[14] || P1Add == DEFCARDS[15] ||
        P1Add == DEFCARDS[16] || P1Add == DEFCARDS[17] || P1Add == DEFCARDS[18] || P1Add == DEFCARDS[19] ||
        P1Add == UTICARDS[0] || P1Add == UTICARDS[1] || P1Add == UTICARDS[2] || P1Add == UTICARDS[3] ||
        P1Add == UTICARDS[4] || P1Add == UTICARDS[5] || P1Add == UTICARDS[6] || P1Add == UTICARDS[7] ||
        P1Add == UTICARDS[8] || P1Add == UTICARDS[9] || P1Add == UTICARDS[10] || P1Add == UTICARDS[11] ||
        P1Add == UTICARDS[12] || P1Add == UTICARDS[13] || P1Add == UTICARDS[14] || P1Add == UTICARDS[15] ||
        P1Add == UTICARDS[16] || P1Add == UTICARDS[17] || P1Add == UTICARDS[18] || P1Add == UTICARDS[19] ||
        P1Add == COMCARDS[0] || P1Add == COMCARDS[1] || P1Add == COMCARDS[2] || P1Add == COMCARDS[3] ||
        P1Add == COMCARDS[4] || P1Add == COMCARDS[5] || P1Add == COMCARDS[6] || P1Add == COMCARDS[7] ||
        P1Add == COMCARDS[8] || P1Add == COMCARDS[9] || P1Add == COMCARDS[10] || P1Add == COMCARDS[11] ||
        P1Add == COMCARDS[12] || P1Add == COMCARDS[13] || P1Add == COMCARDS[14] || P1Add == COMCARDS[15] ||
        P1Add == COMCARDS[16] || P1Add == COMCARDS[17] || P1Add == COMCARDS[18] || P1Add == COMCARDS[19]) {

        Player1Hand[cardsInP1Hand] = P1Add;
        cardsInP1Hand++;
    }
    else cout << "This card does not exist" << endl;
}

void P2AddCardInHand(string P2Add)
{
    if (P2Add == OFFCARDS[0] || P2Add == OFFCARDS[1] || P2Add == OFFCARDS[2] || P2Add == OFFCARDS[3] ||
        P2Add == OFFCARDS[4] || P2Add == OFFCARDS[5] || P2Add == OFFCARDS[6] || P2Add == OFFCARDS[7] ||
        P2Add == OFFCARDS[8] || P2Add == OFFCARDS[9] || P2Add == OFFCARDS[10] || P2Add == OFFCARDS[11] ||
        P2Add == OFFCARDS[12] || P2Add == OFFCARDS[13] || P2Add == OFFCARDS[14] || P2Add == OFFCARDS[15] ||
        P2Add == OFFCARDS[16] || P2Add == OFFCARDS[17] || P2Add == OFFCARDS[18] || P2Add == OFFCARDS[19] ||
        P2Add == DEFCARDS[0] || P2Add == DEFCARDS[1] || P2Add == DEFCARDS[2] || P2Add == DEFCARDS[3] ||
        P2Add == DEFCARDS[4] || P2Add == DEFCARDS[5] || P2Add == DEFCARDS[6] || P2Add == DEFCARDS[7] ||
        P2Add == DEFCARDS[8] || P2Add == DEFCARDS[9] || P2Add == DEFCARDS[10] || P2Add == DEFCARDS[11] ||
        P2Add == DEFCARDS[12] || P2Add == DEFCARDS[13] || P2Add == DEFCARDS[14] || P2Add == DEFCARDS[15] ||
        P2Add == DEFCARDS[16] || P2Add == DEFCARDS[17] || P2Add == DEFCARDS[18] || P2Add == DEFCARDS[19] ||
        P2Add == UTICARDS[0] || P2Add == UTICARDS[1] || P2Add == UTICARDS[2] || P2Add == UTICARDS[3] ||
        P2Add == UTICARDS[4] || P2Add == UTICARDS[5] || P2Add == UTICARDS[6] || P2Add == UTICARDS[7] ||
        P2Add == UTICARDS[8] || P2Add == UTICARDS[9] || P2Add == UTICARDS[10] || P2Add == UTICARDS[11] ||
        P2Add == UTICARDS[12] || P2Add == UTICARDS[13] || P2Add == UTICARDS[14] || P2Add == UTICARDS[15] ||
        P2Add == UTICARDS[16] || P2Add == UTICARDS[17] || P2Add == UTICARDS[18] || P2Add == UTICARDS[19] ||
        P2Add == COMCARDS[0] || P2Add == COMCARDS[1] || P2Add == COMCARDS[2] || P2Add == COMCARDS[3] ||
        P2Add == COMCARDS[4] || P2Add == COMCARDS[5] || P2Add == COMCARDS[6] || P2Add == COMCARDS[7] ||
        P2Add == COMCARDS[8] || P2Add == COMCARDS[9] || P2Add == COMCARDS[10] || P2Add == COMCARDS[11] ||
        P2Add == COMCARDS[12] || P2Add == COMCARDS[13] || P2Add == COMCARDS[14] || P2Add == COMCARDS[15] ||
        P2Add == COMCARDS[16] || P2Add == COMCARDS[17] || P2Add == COMCARDS[18] || P2Add == COMCARDS[19]) {

        Player2Hand[cardsInP2Hand] = P2Add;
        cardsInP2Hand++;
    }
    else cout << "This card does not exist" << endl;
}

void P1TakeCardOutOfHand(string P1ChooseCard)
{
    for (int P1TestForPlay = 0; P1TestForPlay < 10; P1TestForPlay++) {
        if (P1ChooseCard == Player1Hand[P1TestForPlay]) {
            Graveyard[CardsInGraveyard] = Player1Hand[P1TestForPlay];
            Player1Hand[P1TestForPlay] = Player1Hand[cardsInP1Hand - 1];
            Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
            CardsInGraveyard++;
            cardsInP1Hand--;
        }
    }
}

void P2TakeCardOutOfHand(string P2ChooseCard)
{
    for (int P2TestForPlay = 0; P2TestForPlay < 10; P2TestForPlay++) {
        if (P2ChooseCard == Player2Hand[P2TestForPlay]) {
            Graveyard[CardsInGraveyard] = Player2Hand[P2TestForPlay];
            Player2Hand[P2TestForPlay] = Player2Hand[cardsInP2Hand - 1];
            Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
            CardsInGraveyard++;
            cardsInP2Hand--;
        }
    }
}

void P1damage(int P1Dmg)
{
    if (P1ActiveVitalSpot == true || P1ActiveVitalSpotD == true) {
        Player2HP -= P1Dmg;
        if (P1ActiveVitalSpot == true) {
            P1ActiveVitalSpot = false;
            Graveyard[CardsInGraveyard] = "P1ActiveVitalSpot";
            CardsInGraveyard++;
        }
        else {
            P1ActiveVitalSpotD = false;
            Graveyard[CardsInGraveyard] = "P1ActiveVitalSpotD";
            CardsInGraveyard++;
        }
        cout << "Dealt " << P1Dmg << " damage. Player 2 has " << Player2HP << " HP and " << Player2Armor << " armor" << endl;
    }
    else {
        if (P2ActiveBarrier == false && P2ActiveBarrierD == false) {
            for (int P1DmgTick = 0; P1DmgTick < P1Dmg; P1DmgTick++) {
                if (Player2Armor > 0) {
                    Player2Armor--;
                }
                else Player2HP--;
            }
            cout << "Dealt " << P1Dmg << " damage. Player 2 has " << Player2HP << " HP and " << Player2Armor << " armor" << endl;
        }
        else if (P2ActiveBarrier == true) {
            P2ActiveBarrier = false;
            Graveyard[CardsInGraveyard] = "P2ActiveBarrier";
            CardsInGraveyard++;
        }
        else {
            P2ActiveBarrierD = false;
            Graveyard[CardsInGraveyard] = "P2ActiveBarrierD";
            CardsInGraveyard++;
        }
    }
}

void P2damage(int P2Dmg)
{
    if (P2ActiveVitalSpot == true || P2ActiveVitalSpotD == true) {
        Player1HP -= P2Dmg;
        if (P2ActiveVitalSpot == true) {
            P2ActiveVitalSpot = false;
            Graveyard[CardsInGraveyard] = "P1ActiveVitalSpot";
            CardsInGraveyard++;
        }
        else {
            P2ActiveVitalSpotD = false;
            Graveyard[CardsInGraveyard] = "P1ActiveVitalSpotD";
            CardsInGraveyard++;
        }
        cout << "Dealt " << P2Dmg << " damage. Player 1 has " << Player1HP << " HP and " << Player1Armor << " armor" << endl;
    }
    if (P1ActiveBarrier == false && P1ActiveBarrierD == false) {
        for (int P2DmgTick = 0; P2DmgTick < P2Dmg; P2DmgTick++) {
            if (Player1Armor > 0) {
                Player1Armor--;
            }
            else Player1HP--;
        }
        cout << "Dealt " << P2Dmg << " damage. Player 1 has " << Player1HP << " HP and " << Player1Armor << " armor" << endl;
    }
    else if (P1ActiveBarrier == true) {
        P1ActiveBarrier = false;
        Graveyard[CardsInGraveyard] = "P2ActiveBarrier";
        CardsInGraveyard++;
    }
    else {
        P1ActiveBarrierD = false;
        Graveyard[CardsInGraveyard] = "P2ActiveBarrierD";
        CardsInGraveyard++;
    }
}

void displayInfoP1()
{
    cout << " " << endl;
    cout << "Player 1" << endl;
    cout << " " << endl;
    cout << "Cards:" << endl;
    cout << " " << endl;
    for (int DisplayP1Hand = 0; DisplayP1Hand < 10; DisplayP1Hand++) {
        cout << Player1Hand[DisplayP1Hand] << endl;
    }
    cout << " " << endl;
    cout << "Stats:" << endl;
    cout << " " << endl;
    cout << "Health - " << Player1HP << endl;
    cout << "Armor - " << Player1Armor << endl;
    cout << "Max HP - " << Player1MaxHP << endl;
    cout << " " << endl;
    cout << "Actions Left:" << endl;
    cout << P1ActionsLeft << endl;
    cout << " " << endl;
    if (P1ActiveFireMoon == true) cout << "Fire Moon active" << endl;
    if (P1ActiveFireMoonD == true) cout << "Fire Moon active" << endl;
    if (P1ActiveSoothingSpell == true) cout << "Soothing Spell active" << endl;
    if (P1ActiveSoothingSpellD == true) cout << "Soothing Spell active" << endl;
    if (P1ActiveAbsoluteZero == true) cout << "Absolute Zero active" << endl;
    if (P1ActiveAbsoluteZeroD == true) cout << "Absolute Zero active" << endl;
    if (P1ActiveVitalSpot == true && P1ActiveVitalSpotD == true) cout << "Two Vital Spots active" << endl;
    else if (P1ActiveVitalSpot == true) cout << "Vital Spot active" << endl;
    else if (P1ActiveVitalSpotD == true) cout << "Vital Spot active" << endl;
    if (P1ActiveSpiritShelter == true) cout << "Spirit Shelter active" << endl;
    if (P1ActiveSpiritShelterD == true) cout << "Spirit Shelter active" << endl;
}

void displayInfoP2()
{
    cout << " " << endl;
    cout << "Player 2" << endl;
    cout << " " << endl;
    cout << "Cards:" << endl;
    cout << " " << endl;
    for (int DisplayP2Hand = 0; DisplayP2Hand < 10; DisplayP2Hand++) {
        cout << Player2Hand[DisplayP2Hand] << endl;
    }
    cout << " " << endl;
    cout << "Stats:" << endl;
    cout << " " << endl;
    cout << "Health - " << Player2HP << endl;
    cout << "Armor - " << Player2Armor << endl;
    cout << "Max HP - " << Player2MaxHP << endl;
    cout << " " << endl;
    cout << "Actions Left:" << endl;
    cout << P2ActionsLeft << endl;
    cout << " " << endl;
    if (P2ActiveFireMoon == true) cout << "Fire Moon active" << endl;
    if (P2ActiveFireMoonD == true) cout << "Fire Moon active" << endl;
    if (P2ActiveSoothingSpell == true) cout << "Soothing Spell active" << endl;
    if (P2ActiveSoothingSpellD == true) cout << "Soothing Spell active" << endl;
    if (P2ActiveAbsoluteZero == true) cout << "Absolute Zero active" << endl;
    if (P2ActiveAbsoluteZeroD == true) cout << "Absolute Zero active" << endl;
    if (P2ActiveVitalSpot == true) cout << "Vital Spot active" << endl;
    if (P2ActiveVitalSpotD == true) cout << "Vital Spot active" << endl;
    if (P2ActiveBarrier == true) cout << "Barrier active" << endl;
    if (P2ActiveBarrierD == true) cout << "Barrier active" << endl;
    if (P2ActiveSpiritShelter == true) cout << "Spirit Shelter active" << endl;
    if (P2ActiveSpiritShelterD == true) cout << "Spirit Shelter active" << endl;
}

void Peace_Pact()
{
    for (int P2TestEnemyHand = 0; P2TestEnemyHand < cardsInP1Hand; P2TestEnemyHand++){
        if (Player1Hand[P2TestEnemyHand] == OFFCARDS[0] || Player1Hand[P2TestEnemyHand] == OFFCARDS[1] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[2] || Player1Hand[P2TestEnemyHand] == OFFCARDS[3] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[4] || Player1Hand[P2TestEnemyHand] == OFFCARDS[5] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[6] || Player1Hand[P2TestEnemyHand] == OFFCARDS[7] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[8] || Player1Hand[P2TestEnemyHand] == OFFCARDS[9] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[10] || Player1Hand[P2TestEnemyHand] == OFFCARDS[11] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[12] || Player1Hand[P2TestEnemyHand] == OFFCARDS[13] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[14] || Player1Hand[P2TestEnemyHand] == OFFCARDS[15] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[16] || Player1Hand[P2TestEnemyHand] == OFFCARDS[17] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[18] || Player1Hand[P2TestEnemyHand] == OFFCARDS[19]){
            OffDeck[CardsLeftInOffDeck] = Player1Hand[P2TestEnemyHand];
            Player1Hand[P2TestEnemyHand] = "xEmptyx";
        }
    }

    sort(Player1Hand, Player1Hand + 10);

    for (int P1TestEnemyHand = 0; P1TestEnemyHand < cardsInP2Hand; P1TestEnemyHand++){
        if (Player2Hand[P1TestEnemyHand] == OFFCARDS[0] || Player2Hand[P1TestEnemyHand] == OFFCARDS[1] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[2] || Player2Hand[P1TestEnemyHand] == OFFCARDS[3] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[4] || Player2Hand[P1TestEnemyHand] == OFFCARDS[5] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[6] || Player2Hand[P1TestEnemyHand] == OFFCARDS[7] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[8] || Player2Hand[P1TestEnemyHand] == OFFCARDS[9] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[10] || Player2Hand[P1TestEnemyHand] == OFFCARDS[11] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[12] || Player2Hand[P1TestEnemyHand] == OFFCARDS[13] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[14] || Player2Hand[P1TestEnemyHand] == OFFCARDS[15] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[16] || Player2Hand[P1TestEnemyHand] == OFFCARDS[17] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[18] || Player2Hand[P1TestEnemyHand] == OFFCARDS[19]){
            OffDeck[CardsLeftInOffDeck] = Player2Hand[P1TestEnemyHand];
            Player2Hand[P1TestEnemyHand] = "xEmptyx";
        }
    }

    sort(Player2Hand, Player2Hand + 10);

    /////

    time(&miliseconds);
    srand((unsigned int)miliseconds);

    while (RandomizedOffCards < 20) {
        Random_Number = (rand() % 20);
        RandomOffCard = OFFCARDS[Random_Number];
        if (RandomOffCard != UsedOff[0] && RandomOffCard != UsedOff[1] && RandomOffCard != UsedOff[2] && RandomOffCard != UsedOff[3] &&
            RandomOffCard != UsedOff[4] && RandomOffCard != UsedOff[5] && RandomOffCard != UsedOff[6] && RandomOffCard != UsedOff[7] &&
            RandomOffCard != UsedOff[8] && RandomOffCard != UsedOff[9] && RandomOffCard != UsedOff[10] && RandomOffCard != UsedOff[11] &&
            RandomOffCard != UsedOff[12] && RandomOffCard != UsedOff[13] && RandomOffCard != UsedOff[14] && RandomOffCard != UsedOff[15] &&
            RandomOffCard != UsedOff[16] && RandomOffCard != UsedOff[17] && RandomOffCard != UsedOff[18] && RandomOffCard != UsedOff[19]) {
            OffDeck[RandomizedOffCards] = OFFCARDS[Random_Number];
            UsedOff[RandomizedOffCards] = OFFCARDS[Random_Number];
            cout << OffDeck[RandomizedOffCards] << endl;
            RandomizedOffCards++;
        }
    }
}

void P2Heal(int HealAmount)
{
    if (Player2HP < Player2MaxHP) {
        if (Player2MaxHP - Player2HP < HealAmount) {
            HealAmount = Player2MaxHP - Player2HP;
            Player2HP += HealAmount;
            cout << "You restored " << HealAmount << " health!" << " Your current HP is " << Player2HP << endl;
        }
        else {
            Player2HP += HealAmount;
            cout << "You restored " << HealAmount << " health!" << " Your current HP is " << Player2HP << endl;
        }
        P2ActionsLeft--;
    }
    else cout << "You can't heal, you have full health" << endl;
    P2ActionsLeft++;
}

void P1Heal(int HealAmount)
{
    if (Player1HP < Player1MaxHP) {
        if (Player1MaxHP - Player1HP < HealAmount) {
            HealAmount = Player1MaxHP - Player1HP;
            Player1HP += HealAmount;
            cout << "You restored " << HealAmount << " health!" << " Your current HP is " << Player1HP << endl;
        }
        else {
            Player1HP += HealAmount;
            cout << "You restored " << HealAmount << " health!" << " Your current HP is " << Player1HP << endl;
        }
        P1ActionsLeft--;
    }
    else cout << "You can't heal, you have full health" << endl;
    P1ActionsLeft++;
}

void P2discard(string P2DiscardCard)
{
    if (P2DiscardCard == Player2Hand[0] || P2DiscardCard == Player2Hand[1] || P2DiscardCard == Player2Hand[2]
         || P2DiscardCard == Player2Hand[3] || P2DiscardCard == Player2Hand[4] || P2DiscardCard == Player2Hand[5]
          || P2DiscardCard == Player2Hand[6] || P2DiscardCard == Player2Hand[7] || P2DiscardCard == Player2Hand[8]
           || P2DiscardCard == Player2Hand[9]) {
        for (int P2TestForDiscard = 0; P2TestForDiscard < 10; P2TestForDiscard++) {
            if (P2DiscardCard == Player2Hand[P2TestForDiscard]) {
                Graveyard[CardsInGraveyard] = Player2Hand[P2TestForDiscard];
                Player2Hand[P2TestForDiscard] = Player2Hand[cardsInP2Hand - 1];
                Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
                CardsInGraveyard++;
                cardsInP2Hand--;
                cout << "you discarded a card" << endl;
                cout << " " << endl;
                P2ActionsLeft--;
            }
        }
        sort(Player2Hand, Player2Hand + 20);
    }
    else cout << "You don't have that card" << endl;
}

void P2ArmorUp(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    Player2Armor += 2;
    cout << "You now have " << Player2Armor << " armor!" << endl;
    cout << " " << endl;
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2Barrier(string P2ChooseCard)
{
    for (int P2TestForPlay = 0; P2TestForPlay < 10; P2TestForPlay++) {
        if (P2ChooseCard == Player2Hand[P2TestForPlay]) {
            Player2Hand[P2TestForPlay] = Player2Hand[cardsInP2Hand - 1];
            Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
            cardsInP2Hand--;
        }
    }
    if (P2ChooseCard == "Barrier") {
    P2ActiveBarrier = true;
    }
    else P2ActiveBarrierD = true;
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2WarBlessing(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    Player2MaxHP += 5;
    P2Heal(1);
    P2ActionsLeft--;
    cout << "You played a card" << endl;
    cout << "You now have " << Player2HP << " health. Your max health is " << Player2MaxHP << endl;
}

void P2Recreation(string P2ChooseCard)
{
    if (CardsLeftInDefDeck == 0) {
        P2TakeCardOutOfHand(P2ChooseCard);
        int DefCardsInGraveyard = 0;
        for (int drawDefFromGraveyard = 0; drawDefFromGraveyard < CardsInGraveyard; drawDefFromGraveyard++) {
            if (Graveyard[drawDefFromGraveyard] == "Armor_Up" || Graveyard[drawDefFromGraveyard] == "Barrier" ||
                Graveyard[drawDefFromGraveyard] == "War_Blessing" || Graveyard[drawDefFromGraveyard] == "Cure" ||
                Graveyard[drawDefFromGraveyard] == "Preparations" || Graveyard[drawDefFromGraveyard] == "Spirit_Shelter" ||
                Graveyard[drawDefFromGraveyard] == "Healer" || Graveyard[drawDefFromGraveyard] == "Peace_Pact" ||
                Graveyard[drawDefFromGraveyard] == "Soothing_Spell" || Graveyard[drawDefFromGraveyard] == "d_Armor_Up" ||
                Graveyard[drawDefFromGraveyard] == "d_Barrier" || Graveyard[drawDefFromGraveyard] == "d_War_Blessing" ||
                Graveyard[drawDefFromGraveyard] == "d_Cure" || Graveyard[drawDefFromGraveyard] == "d_Preparations" ||
                Graveyard[drawDefFromGraveyard] == "d_Spirit_Shelter" || Graveyard[drawDefFromGraveyard] == "d_Healer" ||
                Graveyard[drawDefFromGraveyard] == "d_Peace_Pact" || Graveyard[drawDefFromGraveyard] == "d_Soothing_Spell") {
                DefCardsInGraveyard++;
                DefDeck[CardsLeftInDefDeck] = Graveyard[drawDefFromGraveyard];
                CardsLeftInDefDeck++;
                Graveyard[drawDefFromGraveyard] = "xEmptyx";
                CardsInGraveyard--;
            }
        }
        sort(Graveyard, Graveyard + 80);
        P2ActionsLeft--;
        cout << "You played a card" << endl;

        time(&miliseconds);
        srand((unsigned int)miliseconds);

        while (RandomizedDefCards < CardsLeftInDefDeck) {
            Random_Number = (rand() % CardsLeftInDefDeck);
            RandomDefCard = DEFCARDS[Random_Number];
            if (RandomDefCard != UsedDef[0] && RandomDefCard != UsedDef[1] && RandomDefCard != UsedDef[2] && RandomDefCard != UsedDef[3] &&
                RandomDefCard != UsedDef[4] && RandomDefCard != UsedDef[5] && RandomDefCard != UsedDef[6] && RandomDefCard != UsedDef[7] &&
                RandomDefCard != UsedDef[8] && RandomDefCard != UsedDef[9] && RandomDefCard != UsedDef[10] && RandomDefCard != UsedDef[11] &&
                RandomDefCard != UsedDef[12] && RandomDefCard != UsedDef[13] && RandomDefCard != UsedDef[14] && RandomDefCard != UsedDef[15] &&
                RandomDefCard != UsedDef[16] && RandomDefCard != UsedDef[17] && RandomDefCard != UsedDef[18] && RandomDefCard != UsedDef[19]) {
                DefDeck[RandomizedDefCards] = DEFCARDS[Random_Number];
                UsedDef[RandomizedDefCards] = DEFCARDS[Random_Number];
                cout << DefDeck[RandomizedDefCards] << endl;
                RandomizedDefCards++;
            }
        }
    }
    else cout << "There's still cards left in defensive deck" << endl;
}

void P2Cure(string P2ChooseCard)
{
    if (Player2HP < Player2MaxHP) {
        P2TakeCardOutOfHand(P2ChooseCard);
        P2Heal((Player2MaxHP - Player2HP) / 2);
        P2ActionsLeft--;
        cout << "You played a card" << endl;
    }
    else cout << "You have full health" << endl;
}

void P2Preparations(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    for (int P2TestEnemyHand = 0; P2TestEnemyHand < 20; P2TestEnemyHand++){
        if (Player1Hand[P2TestEnemyHand] == OFFCARDS[0] || Player1Hand[P2TestEnemyHand] == OFFCARDS[1] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[2] || Player1Hand[P2TestEnemyHand] == OFFCARDS[3] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[4] || Player1Hand[P2TestEnemyHand] == OFFCARDS[5] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[6] || Player1Hand[P2TestEnemyHand] == OFFCARDS[7] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[8] || Player1Hand[P2TestEnemyHand] == OFFCARDS[9] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[10] || Player1Hand[P2TestEnemyHand] == OFFCARDS[11] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[12] || Player1Hand[P2TestEnemyHand] == OFFCARDS[13] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[14] || Player1Hand[P2TestEnemyHand] == OFFCARDS[15] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[16] || Player1Hand[P2TestEnemyHand] == OFFCARDS[17] ||
            Player1Hand[P2TestEnemyHand] == OFFCARDS[18] || Player1Hand[P2TestEnemyHand] == OFFCARDS[19]){
            Player2Armor++;
        }
    }
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2SpiritShelter(string P2ChooseCard)
{
    for (int P2TestForPlay = 0; P2TestForPlay < 10; P2TestForPlay++) {
        if (P2ChooseCard == Player2Hand[P2TestForPlay]) {
            Player2Hand[P2TestForPlay] = Player2Hand[cardsInP2Hand - 1];
            Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
            cardsInP2Hand--;
        }
    }
    if (P2ChooseCard == "Spirit_Shelter") {
        P2ActiveSpiritShelter = true;
    }
    else {
        P2ActiveSpiritShelterD = true;
    }
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2Healer(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    P2Heal(2);
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2PeacePact(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    Peace_Pact();
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2SoothingSpell(string P2ChooseCard)
{
    for (int P2TestForPlay = 0; P2TestForPlay < 10; P2TestForPlay++) {
        if (P2ChooseCard == Player2Hand[P2TestForPlay]) {
            Player2Hand[P2TestForPlay] = Player2Hand[cardsInP2Hand - 1];
            Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
            cardsInP2Hand--;
        }
    }
    if (P2ChooseCard == "Soothing_Spell") {
        P2ActiveSoothingSpell = true;
    }
    else P2ActiveSoothingSpellD = true;
    P2ActionsLeft--;
    if (cardsInP2Hand > 0) {
        int NumberOfOffCards = 0;
        for (int SSCheckForOffCards = 0; SSCheckForOffCards < 20; SSCheckForOffCards++){
            if (Player2Hand[SSCheckForOffCards] == OFFCARDS[0] || Player2Hand[SSCheckForOffCards] == OFFCARDS[1] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[2] || Player2Hand[SSCheckForOffCards] == OFFCARDS[3] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[4] || Player2Hand[SSCheckForOffCards] == OFFCARDS[5] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[6] || Player2Hand[SSCheckForOffCards] == OFFCARDS[7] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[8] || Player2Hand[SSCheckForOffCards] == OFFCARDS[9] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[10] || Player2Hand[SSCheckForOffCards] == OFFCARDS[11] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[12] || Player2Hand[SSCheckForOffCards] == OFFCARDS[13] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[14] || Player2Hand[SSCheckForOffCards] == OFFCARDS[15] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[16] || Player2Hand[SSCheckForOffCards] == OFFCARDS[17] ||
                Player2Hand[SSCheckForOffCards] == OFFCARDS[18] || Player2Hand[SSCheckForOffCards] == OFFCARDS[19]) {
                NumberOfOffCards++;
            }
        }

        bool SSCardPlayed = false;
        string SSPlayCard;
        cout << "You can play one more card: ";
        while (SSCardPlayed == false) {
            getline(cin, SSPlayCard);
            if (SSPlayCard == Player2Hand[0] || SSPlayCard == Player2Hand[1] || SSPlayCard == Player2Hand[2] || SSPlayCard == Player2Hand[3] ||
                SSPlayCard == Player2Hand[4] || SSPlayCard == Player2Hand[5] || SSPlayCard == Player2Hand[6] || SSPlayCard == Player2Hand[7] ||
                SSPlayCard == Player2Hand[8] || SSPlayCard == Player2Hand[9] || SSPlayCard == Player2Hand[10] || SSPlayCard == Player2Hand[11] ||
                SSPlayCard == Player2Hand[12] || SSPlayCard == Player2Hand[13] || SSPlayCard == Player2Hand[14] || SSPlayCard == Player2Hand[15] ||
                SSPlayCard == Player2Hand[16] || SSPlayCard == Player2Hand[17] || SSPlayCard == Player2Hand[18] || SSPlayCard == Player2Hand[19]) {
                if (P1ActiveSoothingSpell == true || P1ActiveSoothingSpellD == true) {
                    if (SSPlayCard == OFFCARDS[0] || SSPlayCard == OFFCARDS[1] || SSPlayCard == OFFCARDS[2] || SSPlayCard == OFFCARDS[3] ||
                        SSPlayCard == OFFCARDS[4] || SSPlayCard == OFFCARDS[5] || SSPlayCard == OFFCARDS[6] || SSPlayCard == OFFCARDS[7] ||
                        SSPlayCard == OFFCARDS[8] || SSPlayCard == OFFCARDS[9] || SSPlayCard == OFFCARDS[10] || SSPlayCard == OFFCARDS[11] ||
                        SSPlayCard == OFFCARDS[12] || SSPlayCard == OFFCARDS[13] || SSPlayCard == OFFCARDS[14] || SSPlayCard == OFFCARDS[15] ||
                        SSPlayCard == OFFCARDS[16] || SSPlayCard == OFFCARDS[17] || SSPlayCard == OFFCARDS[18] || SSPlayCard == OFFCARDS[19]) {
                        if (cardsInP2Hand > NumberOfOffCards) {
                            cout << "Enemy has Soothing Spell active, you cant play offensive cards, choose another card: ";
                        }
                        else {
                            cout << "Enemy has Soothing Spell, but you don't have non-offensive cards, and won't play anything" << endl;
                            SSCardPlayed = true;
                        }
                    }
                    else {
                        P2play(SSPlayCard);
                        SSCardPlayed = true;
                        P2ActionsLeft++;
                    }
                }
                else {
                    P2play(SSPlayCard);
                    SSCardPlayed = true;
                    P2ActionsLeft++;
                }
            }
            else cout << "You don't have this card, choose another: ";
        }
    }
    else cout << "You have no cards in your hand so you won't play anything" << endl;
}

void P2Stab(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    P2damage(2);
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2HellfireSword(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    Player1Armor = 0;
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2FireMoon(string P2ChooseCard)
{
    for (int P2TestForPlay = 0; P2TestForPlay < 10; P2TestForPlay++) {
        if (P2ChooseCard == Player2Hand[P2TestForPlay]) {
            Player2Hand[P2TestForPlay] = Player2Hand[cardsInP2Hand - 1];
            Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
            cardsInP2Hand--;
        }
    }
    if (P2ChooseCard == "Fire_Moon") {
        P2ActiveFireMoon = true;
    }
    else P2ActiveFireMoonD = true;
    P2ActionsLeft--;
    cout << "You played a card" << endl;
    string P2FireMoonDraw;
    bool CardDrawn = false;
    cout << "From which deck do you want to draw? ";
    while (CardDrawn == false) {
        getline(cin, P2FireMoonDraw);
        if (P2FireMoonDraw == "Off" || P2FireMoonDraw == "Def" ||
            P2FireMoonDraw == "Uti" || P2FireMoonDraw == "Com") {
            if (P2FireMoonDraw == "Off") {
                if (CardsLeftInOffDeck > 0) {
                    P2draw(P2FireMoonDraw);
                    CardDrawn = true;
                    P2ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
            if (P2FireMoonDraw == "Def") {
                if (CardsLeftInDefDeck > 0) {
                    P2draw(P2FireMoonDraw);
                    CardDrawn = true;
                    P2ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
            if (P2FireMoonDraw == "Uti") {
                if (CardsLeftInUtiDeck > 0) {
                    P2draw(P2FireMoonDraw);
                    CardDrawn = true;
                    P2ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
            if (P2FireMoonDraw == "Com") {
                if (CardsLeftInComDeck > 0) {
                    P2draw(P2FireMoonDraw);
                    CardDrawn = true;
                    P2ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
        }
        else cout << "Invalid deck, choose another: ";
    }
}

void P2VitalSpot(string P2ChooseCard)
{
    for (int P2TestForPlay = 0; P2TestForPlay < 10; P2TestForPlay++) {
        if (P2ChooseCard == Player2Hand[P2TestForPlay]) {
            Player2Hand[P2TestForPlay] = Player2Hand[cardsInP2Hand - 1];
            Player2Hand[cardsInP2Hand - 1] = "xEmptyx";
            cardsInP2Hand--;
        }
    }
    if (P2ChooseCard == "Vital_Spot") {
        P2ActiveVitalSpot = true;
    }
    else P2ActiveVitalSpotD = true;
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2StarCurse(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    Player1MaxHP = Player1HP;
    P2ActionsLeft--;cout << "You played a card" << endl;
}

void P2RapidAttack(string P2ChooseCard)
{
    if (P2UsedRapidAttack == false) {
        P2damage(1);
        P2ActionsLeft--;
        cout << "You played a card" << endl;
    }
    else cout << "You already played that card this turn" << endl;
}

void P2RapidAttackD(string P2ChooseCard)
{
    if (P2UsedRapidAttackD == false) {
        P2damage(1);
        P2ActionsLeft--;
        cout << "You played a card" << endl;
    }
    else cout << "You already played that card this turn" << endl;
}

void P2DesperateStrike(string P2ChooseCard)
{
    if (cardsInP2Hand < cardsInP2Hand) {
        P2damage(3);
        cout << "You played a card" << endl;
    }
    else cout << "You don't have less cards than your enemy, you did no damage" << endl;
    P2ActionsLeft--;
    P2TakeCardOutOfHand(P2ChooseCard);
}

void P2SacrificialAttack(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    P2damage(3);
    P2damageSelf(3);
    P2ActionsLeft--;
    cout << "You played a card" << endl;
}

void P2LastBreath(string P2ChooseCard)
{
    if (Player2HP < Player1HP) {
        P2TakeCardOutOfHand(P2ChooseCard);
        string P2ListOfYourOffCards[20];
        int OffCardsInYourHand = 0;

        for (int P2TestForOffCards = 0; P2TestForOffCards < 10; P2TestForOffCards++) {
            if (Player2Hand[P2TestForOffCards] == OFFCARDS[0] || Player2Hand[P2TestForOffCards] == OFFCARDS[1] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[2] || Player2Hand[P2TestForOffCards] == OFFCARDS[3] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[4] || Player2Hand[P2TestForOffCards] == OFFCARDS[5] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[6] || Player2Hand[P2TestForOffCards] == OFFCARDS[7] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[8] || Player2Hand[P2TestForOffCards] == OFFCARDS[9] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[10] || Player2Hand[P2TestForOffCards] == OFFCARDS[11] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[12] || Player2Hand[P2TestForOffCards] == OFFCARDS[13] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[14] || Player2Hand[P2TestForOffCards] == OFFCARDS[15] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[16] || Player2Hand[P2TestForOffCards] == OFFCARDS[17] ||
                Player2Hand[P2TestForOffCards] == OFFCARDS[18] || Player2Hand[P2TestForOffCards] == OFFCARDS[19]) {

                P2ListOfYourOffCards[OffCardsInYourHand] = Player2Hand[P2TestForOffCards];
                OffCardsInYourHand++;
            }
        }

        cout << "Your offensive cards: " << endl;
        for (int DisplayYourOffCards = 0; DisplayYourOffCards < OffCardsInYourHand; DisplayYourOffCards++) {
            cout << DisplayYourOffCards + 1 << "." << P2ListOfYourOffCards[DisplayYourOffCards] << endl;
        }

        string PlayOffCard;

        while (OffCardsInYourHand > 0) {
            cout << "Which card do you want to play? ";
            getline(cin, PlayOffCard);

            if (PlayOffCard == P2ListOfYourOffCards[0] || PlayOffCard == P2ListOfYourOffCards[1] ||
                PlayOffCard == P2ListOfYourOffCards[2] || PlayOffCard == P2ListOfYourOffCards[3] ||
                PlayOffCard == P2ListOfYourOffCards[4] || PlayOffCard == P2ListOfYourOffCards[5] ||
                PlayOffCard == P2ListOfYourOffCards[6] || PlayOffCard == P2ListOfYourOffCards[7] ||
                PlayOffCard == P2ListOfYourOffCards[8] || PlayOffCard == P2ListOfYourOffCards[9]) {

                if (PlayOffCard == "Stab" || PlayOffCard == "d_Stab") {
                    P2Stab(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Hellfire_Sword" || PlayOffCard == "d_Hellfire_Sword") {
                    P2HellfireSword(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Fire_Moon" || PlayOffCard == "d_Fire_Moon") {
                    P2FireMoon(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Vital_Spot" || PlayOffCard == "d_Vital_Spot") {
                    P2VitalSpot(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Star_Curse" || PlayOffCard == "d_Star_Curse") {
                    P2StarCurse(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Rapid_Attack") {
                    P2RapidAttack(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "d_Rapid_Attack") {
                    P2RapidAttackD(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Desperate_Strike" || PlayOffCard == "d_Desperate_Strike") {
                    int DesperateStrikesInP2Hand = 0;
                    for (int P2CheckForDesperateStrikes = 0; P2CheckForDesperateStrikes < 20; P2CheckForDesperateStrikes++) {
                        if (P2ListOfYourOffCards[P2CheckForDesperateStrikes] == "Desperate_Strike" || P2ListOfYourOffCards[P2CheckForDesperateStrikes] == "d_Desperate_Strike") {
                            DesperateStrikesInP2Hand++;
                        }
                    }
                    if (DesperateStrikesInP2Hand == 1) {
                        if (OffCardsInYourHand > 1) {
                            P2DesperateStrike(PlayOffCard);
                        }
                        else if (cardsInP2Hand < cardsInP1Hand) {
                            P2DesperateStrike(PlayOffCard);
                        }
                        else cout << "You have nothing else to play, and this card is unavailable so it will be discarded" << endl;
                    }
                    if (DesperateStrikesInP2Hand == 2) {
                        if (OffCardsInYourHand > 2) {
                            P2DesperateStrike(PlayOffCard);
                        }
                        else if (cardsInP2Hand < cardsInP1Hand) {
                            P2DesperateStrike(PlayOffCard);
                        }
                        else cout << "You have nothing else to play, and this card is unavailable so it will be discarded" << endl;
                    }
                }

                if (PlayOffCard == "Sacrificial_Attack" || PlayOffCard == "d_Sacrificial_Attack") {
                    P2SacrificialAttack(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Devils_Deal" || PlayOffCard == "d_Devils_Deal") {
                    void P2DevilsDeal(string P2ChooseCard);
                    P2DevilsDeal(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Last_Breath" || PlayOffCard == "d_Last_Breath") {
                    cout << "No effect" << endl;
                    OffCardsInYourHand--;
                    for (int P2RemoveOffCardsFromList = 0; P2RemoveOffCardsFromList < 10; P2RemoveOffCardsFromList++) {
                        if (P2ListOfYourOffCards[P2RemoveOffCardsFromList] == PlayOffCard) {
                            P2ListOfYourOffCards[P2RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }
            }
            else cout << "You don't have that card" << endl;
        }
        P2ActionsLeft = 0;
    }
    else cout << "You can't play this card" << endl;
}

void P2DevilsDeal(string P2ChooseCard)
{
    P2TakeCardOutOfHand(P2ChooseCard);
    string P2ListOfEnemyOffCards[10];
    int OffCardsInEnemyHand = 0;

    for (int P2TestForEnemyOffCards = 0; P2TestForEnemyOffCards < 20; P2TestForEnemyOffCards++) {
        if (Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[0] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[1] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[2] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[3] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[4] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[5] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[6] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[7] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[8] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[9] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[10] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[11] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[12] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[13] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[14] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[15] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[16] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[17] ||
            Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[18] || Player1Hand[P2TestForEnemyOffCards] == OFFCARDS[19]) {
            P2ListOfEnemyOffCards[OffCardsInEnemyHand] = Player1Hand[P2TestForEnemyOffCards];
            OffCardsInEnemyHand++;
        }
    }

    if (CardsLeftInOffDeck > 0) {
        if (OffCardsInEnemyHand == 0) {
            P2ListOfEnemyOffCards[OffCardsInEnemyHand] = OffDeck[CardsLeftInOffDeck - 1];
            OffCardsInEnemyHand++;
            P1draw("Off");
        }

        cout << "Player 1 Offensive cards:" << endl;
        for (int P2DisplayEnemyOffCards = 0; P2DisplayEnemyOffCards < OffCardsInEnemyHand; P2DisplayEnemyOffCards++) {
            cout << P2DisplayEnemyOffCards + 1 << "." << P2ListOfEnemyOffCards[P2DisplayEnemyOffCards] << endl;
        }

        bool CardPlayed = false;
        string CardPlayedOnYourself;

        while (CardPlayed == false) {
            cout << "Choose which card you want to play on yourself: "; getline(cin, CardPlayedOnYourself);
            if (CardPlayedOnYourself == P2ListOfEnemyOffCards[0] || CardPlayedOnYourself == P2ListOfEnemyOffCards[5] ||
                CardPlayedOnYourself == P2ListOfEnemyOffCards[1] || CardPlayedOnYourself == P2ListOfEnemyOffCards[6] ||
                CardPlayedOnYourself == P2ListOfEnemyOffCards[2] || CardPlayedOnYourself == P2ListOfEnemyOffCards[7] ||
                CardPlayedOnYourself == P2ListOfEnemyOffCards[3] || CardPlayedOnYourself == P2ListOfEnemyOffCards[8] ||
                CardPlayedOnYourself == P2ListOfEnemyOffCards[4] || CardPlayedOnYourself == P2ListOfEnemyOffCards[9]) {

                if (CardPlayedOnYourself == "Stab" || CardPlayedOnYourself == "d_Stab") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2Stab("None");
                }

                if (CardPlayedOnYourself == "Hellfire_Sword" || CardPlayedOnYourself == "d_HellFire_Sword") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2HellfireSword("None");
                }

                if (CardPlayedOnYourself == "Vital_Spot" || CardPlayedOnYourself == "d_Vital_Spot") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2VitalSpot("None");
                }

                if (CardPlayedOnYourself == "Star_Curse" || CardPlayedOnYourself == "d_Star_Curse") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2StarCurse("None");
                }

                if (CardPlayedOnYourself == "Rapid_Attack") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2RapidAttack("None");
                }

                if (CardPlayedOnYourself == "d_Rapid_Attack") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2RapidAttackD("None");
                }

                if (CardPlayedOnYourself == "Desperate_Strike" || CardPlayedOnYourself == "d_Desperate_Strike") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2DesperateStrike("None");
                }

                if (CardPlayedOnYourself == "Sacrificial_Attack" || CardPlayedOnYourself == "d_Sacrificial_Strike") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2SacrificialAttack("None");
                }

                if (CardPlayedOnYourself == "Devils_Deal" || CardPlayedOnYourself == "d_Devils_Deal") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2DevilsDeal("None");
                }

                if (CardPlayedOnYourself == "Last_Breath" || CardPlayedOnYourself == "d_Last_Breath") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    cout << "No effect" << endl;
                }

                if (CardPlayedOnYourself == "Fire_Moon" || CardPlayedOnYourself == "d_Fire_Moon") {
                    P1TakeCardOutOfHand(CardPlayedOnYourself);
                    P2FireMoon("None");
                    P2ActionsLeft++;
                }

                CardPlayed = true;
            }
            else cout << "You don't have that card" << endl;
        }
    }
    else {
        if (OffCardsInEnemyHand > 0) {
            cout << "Player 1 Offensive cards:" << endl;
            for (int P2DisplayEnemyOffCards = 0; P2DisplayEnemyOffCards < OffCardsInEnemyHand; P2DisplayEnemyOffCards++) {
                cout << P2DisplayEnemyOffCards + 1 << "." << P2ListOfEnemyOffCards[P2DisplayEnemyOffCards] << endl;
            }

            bool CardPlayed = false;
            string CardPlayedOnYourself;

            while (CardPlayed == false) {
                cout << "Choose which card you want to play on yourself: "; getline(cin, CardPlayedOnYourself);
                if (CardPlayedOnYourself == P2ListOfEnemyOffCards[0] || CardPlayedOnYourself == P2ListOfEnemyOffCards[5] ||
                    CardPlayedOnYourself == P2ListOfEnemyOffCards[1] || CardPlayedOnYourself == P2ListOfEnemyOffCards[6] ||
                    CardPlayedOnYourself == P2ListOfEnemyOffCards[2] || CardPlayedOnYourself == P2ListOfEnemyOffCards[7] ||
                    CardPlayedOnYourself == P2ListOfEnemyOffCards[3] || CardPlayedOnYourself == P2ListOfEnemyOffCards[8] ||
                    CardPlayedOnYourself == P2ListOfEnemyOffCards[4] || CardPlayedOnYourself == P2ListOfEnemyOffCards[9]) {

                    if (CardPlayedOnYourself == "Stab" || CardPlayedOnYourself == "d_Stab") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2Stab("None");
                    }

                    if (CardPlayedOnYourself == "Hellfire_Sword" || CardPlayedOnYourself == "d_HellFire_Sword") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2HellfireSword("None");
                    }

                    if (CardPlayedOnYourself == "Vital_Spot" || CardPlayedOnYourself == "d_Vital_Spot") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2VitalSpot("None");
                    }

                    if (CardPlayedOnYourself == "Star_Curse" || CardPlayedOnYourself == "d_Star_Curse") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2StarCurse("None");
                    }

                    if (CardPlayedOnYourself == "Rapid_Attack") {
                        P2RapidAttack("None");
                    }

                    if (CardPlayedOnYourself == "d_Rapid_Attack") {
                        P2RapidAttackD("None");
                    }

                    if (CardPlayedOnYourself == "Desperate_Strike" || CardPlayedOnYourself == "d_Desperate_Strike") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2DesperateStrike("None");
                    }

                    if (CardPlayedOnYourself == "Sacrificial_Attack" || CardPlayedOnYourself == "d_Sacrificial_Strike") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2SacrificialAttack("None");
                    }

                    if (CardPlayedOnYourself == "Devils_Deal" || CardPlayedOnYourself == "d_Devils_Deal") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2DevilsDeal("None");
                    }

                    if (CardPlayedOnYourself == "Last_Breath" || CardPlayedOnYourself == "d_Last_Breath") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        cout << "No effect" << endl;
                    }

                    if (CardPlayedOnYourself == "Fire_Moon" || CardPlayedOnYourself == "d_Fire_Moon") {
                        P1TakeCardOutOfHand(CardPlayedOnYourself);
                        P2FireMoon("None");
                        P2ActionsLeft++;
                    }

                    CardPlayed = true;
                }
                else cout << "You don't have that card" << endl;
            }
        }
        else cout << "Enemy has no offensive cards and the offensive deck is empty, the card won't have any effect" << endl;
    }
}

void P2play(string P2ChooseCard)
{
    if (P2ChooseCard == Player2Hand[0] || P2ChooseCard == Player2Hand[1] || P2ChooseCard == Player2Hand[2]
         || P2ChooseCard == Player2Hand[3] || P2ChooseCard == Player2Hand[4] || P2ChooseCard == Player2Hand[5]
          || P2ChooseCard == Player2Hand[6] || P2ChooseCard == Player2Hand[7] || P2ChooseCard == Player2Hand[8]
           || P2ChooseCard == Player2Hand[9]) {
///Defensive Cards/////////////////////////////////////////////////////////////////////////////////////
        if (P2ChooseCard == "Armor_Up" || P2ChooseCard == "d_Armor_Up" || P2ChooseCard == "Barrier" || P2ChooseCard == "d_Barrier" ||
            P2ChooseCard == "War_Blessing" || P2ChooseCard == "d_War_Blessing" || P2ChooseCard == "Recreation" || P2ChooseCard == "d_Recreation" ||
            P2ChooseCard == "Cure" || P2ChooseCard == "d_Cure" || P2ChooseCard == "Preparations" || P2ChooseCard == "d_Preparations" ||
            P2ChooseCard == "Spirit_Shelter" || P2ChooseCard == "d_Spirit_Shelter" || P2ChooseCard == "Healer" || P2ChooseCard == "d_Healer" ||
            P2ChooseCard == "Peace_Pact" || P2ChooseCard == "d_Peace_Pact" || P2ChooseCard == "Soothing_Spell" || P2ChooseCard == "d_Soothing_Spell") {
            if (P1ActiveFireMoon == false && P1ActiveFireMoonD == false) {

                if (P2ChooseCard == "Armor_Up" || P2ChooseCard == "d_Armor_Up") {
                    P2ArmorUp(P2ChooseCard);
                }

                if (P2ChooseCard == "Barrier" || P2ChooseCard == "d_Barrier") {
                    P2Barrier(P2ChooseCard);
                }

                if (P2ChooseCard == "War_Blessing" || P2ChooseCard == "d_War_Blessing") {
                    P2WarBlessing(P2ChooseCard);
                }

                if (P2ChooseCard == "Recreation" || P2ChooseCard == "d_Recreation") {
                    P2Recreation(P2ChooseCard);
                }

                if (P2ChooseCard == "Cure" || P2ChooseCard == "d_Cure") {
                    P2Cure(P2ChooseCard);
                }

                if (P2ChooseCard == "Preparations" || P2ChooseCard == "d_Preparations") {
                    P2Preparations(P2ChooseCard);
                }

                if (P2ChooseCard == "Spirit_Shelter" || P2ChooseCard == "d_Spirit_Shelter") {
                    P2SpiritShelter(P2ChooseCard);
                }

                if (P2ChooseCard == "Healer" || P2ChooseCard == "d_Healer") {
                    P2Healer(P2ChooseCard);
                }

                if (P2ChooseCard == "Peace_Pact" || P2ChooseCard == "d_Peace_Pact") {
                    P2PeacePact(P2ChooseCard);
                }

                if (P2ChooseCard == "Soothing_Spell" || P2ChooseCard == "d_Soothing_Spell") {
                    P2SoothingSpell(P2ChooseCard);
                }
            }
            else cout << "You can't play defensive cards, because Fire Moon is active" << endl;
        }
///Offensive Cards///////////////////////////////////////////////////////////////////////////
        if (P2ChooseCard == "Stab" || P2ChooseCard == "d_Stab" || P2ChooseCard == "Fire_Moon" || P2ChooseCard == "d_Fire_Moon" ||
            P2ChooseCard == "Hellfire_Sword" || P2ChooseCard == "d_Hellfire_Sword" || P2ChooseCard == "Vital_Spot" || P2ChooseCard == "d_Vital_Spot" ||
            P2ChooseCard == "Star_Curse" || P2ChooseCard == "d_Star_Curse" || P2ChooseCard == "Rapid_Attack" || P2ChooseCard == "d_Rapid_Attack" ||
            P2ChooseCard == "Desperate_Strike" || P2ChooseCard == "d_Desperate_Strike" || P2ChooseCard == "Sacrificial_Attack" || P2ChooseCard == "d_Sacrificial_Attack" ||
            P2ChooseCard == "Devils_Deal" || P2ChooseCard == "d_Devils_Deal" || P2ChooseCard == "Last_Breath" || P2ChooseCard == "d_Last_Breath") {
            if (P1ActiveSoothingSpell == false && P1ActiveSoothingSpellD == false) {

                if (P2ChooseCard == "Stab" || P2ChooseCard == "d_Stab") {
                    P2Stab(P2ChooseCard);
                }

                if (P2ChooseCard == "Hellfire_Sword" || P2ChooseCard == "d_Hellfire_Sword") {
                    P2HellfireSword(P2ChooseCard);
                }

                if (P2ChooseCard == "Fire_Moon" || P2ChooseCard == "d_Fire_Moon") {
                    P2FireMoon(P2ChooseCard);
                }

                if (P2ChooseCard == "Vital_Spot" || P2ChooseCard == "d_Vital_Spot") {
                    P2VitalSpot(P2ChooseCard);
                }

                if (P2ChooseCard == "Star_Curse" || P2ChooseCard == "d_Star_Curse") {
                    P2StarCurse(P2ChooseCard);
                }

                if (P2ChooseCard == "Rapid_Attack")  {
                    P2RapidAttack(P2ChooseCard);
                }

                if (P2ChooseCard == "d_Rapid_Attack") {
                    P2RapidAttackD(P2ChooseCard);
                }

                if (P2ChooseCard == "Desperate_Strike" || P2ChooseCard == "d_Desperate_Strike") {
                    P2DesperateStrike(P2ChooseCard);
                }

                if (P2ChooseCard == "Sacrificial_Attack" || P2ChooseCard == "d_Sacrificial_Attack") {
                    P2SacrificialAttack(P2ChooseCard);
                }

                if (P2ChooseCard == "Last_Breath" || P2ChooseCard == "d_Last_Breath") {
                    P2LastBreath(P2ChooseCard);
                }

                if (P2ChooseCard == "Devils_Deal" || P2ChooseCard == "d_Devils_Deal") {
                    P2DevilsDeal(P2ChooseCard);
                }
            }
            else cout << "You can't play offensive cards, because Soothing Spell is active" << endl;
        }
    }
    else cout << "You don't have that card" << endl;
}

void P2draw(string P2ChooseDeck)
{
    if (P2ChooseDeck == "Off") {
        if (CardsLeftInOffDeck > 0) {
            Player2Hand[cardsInP2Hand] = OffDeck[CardsLeftInOffDeck];
            cout << " " << endl;
            cardsInP2Hand++;
            OffDeck[CardsLeftInOffDeck] = "xEmptyx";
            CardsLeftInOffDeck--;
            cout << "You drew a card" << endl;
            P2ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;

    }
    else if (P2ChooseDeck == "Def") {
        if (CardsLeftInDefDeck > 0) {
            Player2Hand[cardsInP2Hand] = DefDeck[CardsLeftInDefDeck];
            cout << " " << endl;
            cardsInP2Hand++;
            DefDeck[CardsLeftInDefDeck] = "xEmptyx";
            CardsLeftInDefDeck--;
            cout << "You drew a card" << endl;
            P2ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;
    }
    else if (P2ChooseDeck == "Uti") {
        if (CardsLeftInUtiDeck > 0) {
            Player2Hand[cardsInP2Hand] = UtiDeck[CardsLeftInUtiDeck];
            cout << " " << endl;
            cardsInP2Hand++;
            UtiDeck[CardsLeftInUtiDeck] = "xEmptyx";
            CardsLeftInUtiDeck--;
            cout << "You drew a card" << endl;
            P2ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;
    }
    else if (P2ChooseDeck == "Com") {
        if (CardsLeftInComDeck > 0) {
            Player2Hand[cardsInP2Hand] = ComDeck[CardsLeftInComDeck];
            cout << " " << endl;
            cardsInP2Hand++;
            ComDeck[CardsLeftInComDeck] = "xEmptyx";
            CardsLeftInComDeck--;
            cout << "You drew a card" << endl;
            P2ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;
    }
    else cout << "There is no deck called " << P2ChooseDeck << endl;
}

void P2choose(string P2Action)
{
    if (P2Action == "Draw") {
        if (CardsLeftInOffDeck == 0 && CardsLeftInDefDeck == 0 && CardsLeftInUtiDeck == 0 && CardsLeftInComDeck == 0) {
            GameEndDraw();
        }
        else {
            if (cardsInP2Hand == 10) {
                cout << "You can't draw cards, your hand is full" << endl;
            }
            else {
                string P2ChooseDeck;
                getline(cin, P2ChooseDeck);
                P2draw(P2ChooseDeck);
            }
        }
    }
    if (P2Action == "Play") {
        string P2ChooseCard;
        getline(cin, P2ChooseCard);
        P2play(P2ChooseCard);
    }
    if (P2Action == "Discard") {
        if (cardsInP2Hand > 0) {
            string P2DiscardCard;
            getline(cin, P2DiscardCard);
            P2discard(P2DiscardCard);
        }
        else cout << "You have no cards to discard" << endl;
    }
    if (P2Action == "InfoP1") {
        displayInfoP1();
    }
    if (P2Action == "InfoP2") {
        displayInfoP2();
    }
    if (P2Action == "AddP1") {
        string P1Add;
        getline(cin, P1Add);
        P1AddCardInHand(P1Add);
    }
    if (P2Action == "AddP2") {
        string P2Add;
        getline(cin, P2Add);
        P2AddCardInHand(P2Add);
    }
    if (P2Action == "EndturnP2") {
        P2ActionsLeft = 0;
    }
    if (P2Action != "Draw" && P2Action != "Play" && P2Action != "Discard" && P2Action != "InfoP1" &&
        P2Action != "InfoP2" && P2Action != "AddP1" && P2Action != "AddP2" && P2Action != "EndturnP2") {
        cout << "This isn't a viable option" << endl;
    }
}

void P1discard(string P1DiscardCard)
{
    if (P1DiscardCard == Player1Hand[0] || P1DiscardCard == Player1Hand[1] || P1DiscardCard == Player1Hand[2]
         || P1DiscardCard == Player1Hand[3] || P1DiscardCard == Player1Hand[4] || P1DiscardCard == Player1Hand[5]
          || P1DiscardCard == Player1Hand[6] || P1DiscardCard == Player1Hand[7] || P1DiscardCard == Player1Hand[8]
           || P1DiscardCard == Player1Hand[9]) {
        for (int P1TestForDiscard = 0; P1TestForDiscard < 10; P1TestForDiscard++) {
            if (P1DiscardCard == Player1Hand[P1TestForDiscard]) {
                Graveyard[CardsInGraveyard] = Player1Hand[P1TestForDiscard];
                Player1Hand[P1TestForDiscard] = Player1Hand[cardsInP1Hand - 1];
                Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
                CardsInGraveyard++;
                cardsInP1Hand--;
                cout << "you discarded a card" << endl;
                cout << " " << endl;
                P1ActionsLeft--;
            }
        }
        sort(Player1Hand, Player1Hand + 20);
    }
    else cout << "You don't have that card" << endl;
}

void P1ArmorUp(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    Player1Armor += 2;
    cout << "You now have " << Player1Armor << " armor!" << endl;
    cout << " " << endl;
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1Barrier(string P1ChooseCard)
{
    for (int P1TestForPlay = 0; P1TestForPlay < 10; P1TestForPlay++) {
        if (P1ChooseCard == Player1Hand[P1TestForPlay]) {
            Player1Hand[P1TestForPlay] = Player1Hand[cardsInP1Hand - 1];
            Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
            cardsInP1Hand--;
        }
    }
    if (P1ChooseCard == "Barrier") {
    P1ActiveBarrier = true;
    }
    else P1ActiveBarrierD = true;
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1WarBlessing(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    Player1MaxHP += 5;
    P1Heal(1);
    P1ActionsLeft--;
    cout << "You played a card" << endl;
    cout << "You now have " << Player1HP << " health. Your max health is " << Player1MaxHP << endl;
}

void P1Recreation(string P1ChooseCard)
{
    if (CardsLeftInDefDeck == 0) {
        P1TakeCardOutOfHand(P1ChooseCard);
        int DefCardsInGraveyard = 0;
        for (int drawDefFromGraveyard = 0; drawDefFromGraveyard < CardsInGraveyard; drawDefFromGraveyard++) {
            if (Graveyard[drawDefFromGraveyard] == "Armor_Up" || Graveyard[drawDefFromGraveyard] == "Barrier" ||
                Graveyard[drawDefFromGraveyard] == "War_Blessing" || Graveyard[drawDefFromGraveyard] == "Cure" ||
                Graveyard[drawDefFromGraveyard] == "Preparations" || Graveyard[drawDefFromGraveyard] == "Spirit_Shelter" ||
                Graveyard[drawDefFromGraveyard] == "Healer" || Graveyard[drawDefFromGraveyard] == "Peace_Pact" ||
                Graveyard[drawDefFromGraveyard] == "Soothing_Spell" || Graveyard[drawDefFromGraveyard] == "d_Armor_Up" ||
                Graveyard[drawDefFromGraveyard] == "d_Barrier" || Graveyard[drawDefFromGraveyard] == "d_War_Blessing" ||
                Graveyard[drawDefFromGraveyard] == "d_Cure" || Graveyard[drawDefFromGraveyard] == "d_Preparations" ||
                Graveyard[drawDefFromGraveyard] == "d_Spirit_Shelter" || Graveyard[drawDefFromGraveyard] == "d_Healer" ||
                Graveyard[drawDefFromGraveyard] == "d_Peace_Pact" || Graveyard[drawDefFromGraveyard] == "d_Soothing_Spell") {
                DefCardsInGraveyard++;
                DefDeck[CardsLeftInDefDeck] = Graveyard[drawDefFromGraveyard];
                CardsLeftInDefDeck++;
                Graveyard[drawDefFromGraveyard] = "xEmptyx";
                CardsInGraveyard--;
            }
        }
        sort(Graveyard, Graveyard + 80);
        P1ActionsLeft--;
        cout << "You played a card" << endl;

        time(&miliseconds);
        srand((unsigned int)miliseconds);

        while (RandomizedDefCards < CardsLeftInDefDeck) {
            Random_Number = (rand() % CardsLeftInDefDeck);
            RandomDefCard = DEFCARDS[Random_Number];
            if (RandomDefCard != UsedDef[0] && RandomDefCard != UsedDef[1] && RandomDefCard != UsedDef[2] && RandomDefCard != UsedDef[3] &&
                RandomDefCard != UsedDef[4] && RandomDefCard != UsedDef[5] && RandomDefCard != UsedDef[6] && RandomDefCard != UsedDef[7] &&
                RandomDefCard != UsedDef[8] && RandomDefCard != UsedDef[9] && RandomDefCard != UsedDef[10] && RandomDefCard != UsedDef[11] &&
                RandomDefCard != UsedDef[12] && RandomDefCard != UsedDef[13] && RandomDefCard != UsedDef[14] && RandomDefCard != UsedDef[15] &&
                RandomDefCard != UsedDef[16] && RandomDefCard != UsedDef[17] && RandomDefCard != UsedDef[18] && RandomDefCard != UsedDef[19]) {
                DefDeck[RandomizedDefCards] = DEFCARDS[Random_Number];
                UsedDef[RandomizedDefCards] = DEFCARDS[Random_Number];
                cout << DefDeck[RandomizedDefCards] << endl;
                RandomizedDefCards++;
            }
        }
    }
    else cout << "There's still cards left in defensive deck" << endl;
}

void P1Cure(string P1ChooseCard)
{
    if (Player1HP < Player1MaxHP) {
        P1TakeCardOutOfHand(P1ChooseCard);
        P1Heal((Player1MaxHP - Player1HP + 0.1) / 2);
        P1ActionsLeft--;
        cout << "You played a card" << endl;
    }
    else cout << "You have full health" << endl;
}

void P1Preparations(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    for (int P1TestEnemyHand = 0; P1TestEnemyHand < 20; P1TestEnemyHand++){
        if (Player2Hand[P1TestEnemyHand] == OFFCARDS[0] || Player2Hand[P1TestEnemyHand] == OFFCARDS[1] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[2] || Player2Hand[P1TestEnemyHand] == OFFCARDS[3] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[4] || Player2Hand[P1TestEnemyHand] == OFFCARDS[5] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[6] || Player2Hand[P1TestEnemyHand] == OFFCARDS[7] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[8] || Player2Hand[P1TestEnemyHand] == OFFCARDS[9] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[10] || Player2Hand[P1TestEnemyHand] == OFFCARDS[11] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[12] || Player2Hand[P1TestEnemyHand] == OFFCARDS[13] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[14] || Player2Hand[P1TestEnemyHand] == OFFCARDS[15] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[16] || Player2Hand[P1TestEnemyHand] == OFFCARDS[17] ||
            Player2Hand[P1TestEnemyHand] == OFFCARDS[18] || Player2Hand[P1TestEnemyHand] == OFFCARDS[19]){
            Player1Armor++;
        }
    }
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1SpiritShelter(string P1ChooseCard)
{
    for (int P1TestForPlay = 0; P1TestForPlay < 10; P1TestForPlay++) {
        if (P1ChooseCard == Player1Hand[P1TestForPlay]) {
            Player1Hand[P1TestForPlay] = Player1Hand[cardsInP1Hand - 1];
            Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
            cardsInP1Hand--;
        }
    }
    if (P1ChooseCard == "Spirit_Shelter") {
        P1ActiveSpiritShelter = true;
    }
    else {
        P1ActiveSpiritShelterD = true;
    }
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1Healer(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    P1Heal(2);
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1PeacePact(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    Peace_Pact();
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1SoothingSpell(string P1ChooseCard)
{
    for (int P1TestForPlay = 0; P1TestForPlay < 10; P1TestForPlay++) {
        if (P1ChooseCard == Player1Hand[P1TestForPlay]) {
            Player1Hand[P1TestForPlay] = Player1Hand[cardsInP1Hand - 1];
            Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
            cardsInP1Hand--;
        }
    }
    if (P1ChooseCard == "Soothing_Spell") {
        P1ActiveSoothingSpell = true;
    }
    else P1ActiveSoothingSpellD = true;
    P1ActionsLeft--;
    if (cardsInP1Hand > 0) {
        int NumberOfOffCards = 0;
        for (int SSCheckForOffCards = 0; SSCheckForOffCards < 20; SSCheckForOffCards++){
            if (Player1Hand[SSCheckForOffCards] == OFFCARDS[0] || Player1Hand[SSCheckForOffCards] == OFFCARDS[1] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[2] || Player1Hand[SSCheckForOffCards] == OFFCARDS[3] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[4] || Player1Hand[SSCheckForOffCards] == OFFCARDS[5] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[6] || Player1Hand[SSCheckForOffCards] == OFFCARDS[7] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[8] || Player1Hand[SSCheckForOffCards] == OFFCARDS[9] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[10] || Player1Hand[SSCheckForOffCards] == OFFCARDS[11] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[12] || Player1Hand[SSCheckForOffCards] == OFFCARDS[13] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[14] || Player1Hand[SSCheckForOffCards] == OFFCARDS[15] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[16] || Player1Hand[SSCheckForOffCards] == OFFCARDS[17] ||
                Player1Hand[SSCheckForOffCards] == OFFCARDS[18] || Player1Hand[SSCheckForOffCards] == OFFCARDS[19]) {
                NumberOfOffCards++;
            }
        }

        bool SSCardPlayed = false;
        string SSPlayCard;
        cout << "You can play one more card: ";
        while (SSCardPlayed == false) {
            getline(cin, SSPlayCard);
            if (SSPlayCard == Player1Hand[0] || SSPlayCard == Player1Hand[1] || SSPlayCard == Player1Hand[2] || SSPlayCard == Player1Hand[3] ||
                SSPlayCard == Player1Hand[4] || SSPlayCard == Player1Hand[5] || SSPlayCard == Player1Hand[6] || SSPlayCard == Player1Hand[7] ||
                SSPlayCard == Player1Hand[8] || SSPlayCard == Player1Hand[9] || SSPlayCard == Player1Hand[10] || SSPlayCard == Player1Hand[11] ||
                SSPlayCard == Player1Hand[12] || SSPlayCard == Player1Hand[13] || SSPlayCard == Player1Hand[14] || SSPlayCard == Player1Hand[15] ||
                SSPlayCard == Player1Hand[16] || SSPlayCard == Player1Hand[17] || SSPlayCard == Player1Hand[18] || SSPlayCard == Player1Hand[19]) {
                if (P2ActiveSoothingSpell == true || P2ActiveSoothingSpellD == true) {
                    if (SSPlayCard == OFFCARDS[0] || SSPlayCard == OFFCARDS[1] || SSPlayCard == OFFCARDS[2] || SSPlayCard == OFFCARDS[3] ||
                        SSPlayCard == OFFCARDS[4] || SSPlayCard == OFFCARDS[5] || SSPlayCard == OFFCARDS[6] || SSPlayCard == OFFCARDS[7] ||
                        SSPlayCard == OFFCARDS[8] || SSPlayCard == OFFCARDS[9] || SSPlayCard == OFFCARDS[10] || SSPlayCard == OFFCARDS[11] ||
                        SSPlayCard == OFFCARDS[12] || SSPlayCard == OFFCARDS[13] || SSPlayCard == OFFCARDS[14] || SSPlayCard == OFFCARDS[15] ||
                        SSPlayCard == OFFCARDS[16] || SSPlayCard == OFFCARDS[17] || SSPlayCard == OFFCARDS[18] || SSPlayCard == OFFCARDS[19]) {
                        if (cardsInP1Hand > NumberOfOffCards) {
                            cout << "Enemy has Soothing Spell active, you cant play offensive cards, choose another card: ";
                        }
                        else {
                            cout << "Enemy has Soothing Spell, but you don't have non-offensive cards, and won't play anything" << endl;
                            SSCardPlayed = true;
                        }
                    }
                    else {
                        P1play(SSPlayCard);
                        SSCardPlayed = true;
                        P1ActionsLeft++;
                    }
                }
                else {
                    P1play(SSPlayCard);
                    SSCardPlayed = true;
                    P1ActionsLeft++;
                }
            }
            else cout << "You don't have this card, choose another: ";
        }
    }
    else cout << "You have no cards in your hand so you won't play anything" << endl;
}

void P1Stab(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    P1damage(2);
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1HellfireSword(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    Player2Armor = 0;
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1FireMoon(string P1ChooseCard)
{
    for (int P1TestForPlay = 0; P1TestForPlay < 10; P1TestForPlay++) {
        if (P1ChooseCard == Player1Hand[P1TestForPlay]) {
            Player1Hand[P1TestForPlay] = Player1Hand[cardsInP1Hand - 1];
            Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
            cardsInP1Hand--;
        }
    }
    if (P1ChooseCard == "Fire_Moon") {
        P1ActiveFireMoon = true;
    }
    else P1ActiveFireMoonD = true;
    P1ActionsLeft--;
    string P1FireMoonDraw;
    bool CardDrawn = false;
    cout << "From which deck do you want to draw? ";
    while (CardDrawn == false) {
        getline(cin, P1FireMoonDraw);
        if (P1FireMoonDraw == "Off" || P1FireMoonDraw == "Def" ||
            P1FireMoonDraw == "Uti" || P1FireMoonDraw == "Com") {
            if (P1FireMoonDraw == "Off") {
                if (CardsLeftInOffDeck > 0) {
                    P1draw(P1FireMoonDraw);
                    CardDrawn = true;
                    P1ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
            if (P1FireMoonDraw == "Def") {
                if (CardsLeftInDefDeck > 0) {
                    P1draw(P1FireMoonDraw);
                    CardDrawn = true;
                    P1ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
            if (P1FireMoonDraw == "Uti") {
                if (CardsLeftInUtiDeck > 0) {
                    P1draw(P1FireMoonDraw);
                    CardDrawn = true;
                    P1ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
            if (P1FireMoonDraw == "Com") {
                if (CardsLeftInComDeck > 0) {
                    P1draw(P1FireMoonDraw);
                    CardDrawn = true;
                    P1ActionsLeft++;
                }
                else cout << "No cards in this deck, choose another deck: ";
            }
        }
    else cout << "Invalid deck, choose another: ";
    }
}

void P1VitalSpot(string P1ChooseCard)
{
    for (int P1TestForPlay = 0; P1TestForPlay < 10; P1TestForPlay++) {
        if (P1ChooseCard == Player1Hand[P1TestForPlay]) {
            Player1Hand[P1TestForPlay] = Player1Hand[cardsInP1Hand - 1];
            Player1Hand[cardsInP1Hand - 1] = "xEmptyx";
            cardsInP1Hand--;
        }
    }
    if (P1ChooseCard == "Vital_Spot") {
        P1ActiveVitalSpot = true;
    }
    else P1ActiveVitalSpotD = true;
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1StarCurse(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    Player2MaxHP = Player2HP;
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1RapidAttack(string P1ChooseCard)
{
    if (P1UsedRapidAttack == false) {
        P1damage(1);
        P1ActionsLeft--;
        P1UsedRapidAttack = true;
        cout << "You played a card" << endl;
    }
    else cout << "You already played that card this turn" << endl;
}

void P1RapidAttackD(string P1ChooseCard)
{
    if (P1UsedRapidAttackD == false) {
        P1damage(1);
        P1ActionsLeft--;
        P1UsedRapidAttackD = true;
        cout << "You played a card" << endl;
    }
    else cout << "You already played that card this turn" << endl;
}

void P1DesperateStrike(string P1ChooseCard)
{
    if (cardsInP1Hand < cardsInP2Hand) {
        P1damage(3);
        cout << "You played a card" << endl;
    }
    else cout << "You don't have less cards than your enemy, you did no damage" << endl;
    P1ActionsLeft--;
    P1TakeCardOutOfHand(P1ChooseCard);
}

void P1SacrificialAttack(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    P1damage(3);
    P1damageSelf(3);
    P1ActionsLeft--;
    cout << "You played a card" << endl;
}

void P1LastBreath(string P1ChooseCard)
{
    if (Player1HP < Player2HP) {
        P1TakeCardOutOfHand(P1ChooseCard);
        string P1ListOfYourOffCards[20];
        int OffCardsInYourHand = 0;

        for (int P1TestForOffCards = 0; P1TestForOffCards < 10; P1TestForOffCards++) {
            if (Player1Hand[P1TestForOffCards] == OFFCARDS[0] || Player1Hand[P1TestForOffCards] == OFFCARDS[1] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[2] || Player1Hand[P1TestForOffCards] == OFFCARDS[3] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[4] || Player1Hand[P1TestForOffCards] == OFFCARDS[5] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[6] || Player1Hand[P1TestForOffCards] == OFFCARDS[7] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[8] || Player1Hand[P1TestForOffCards] == OFFCARDS[9] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[10] || Player1Hand[P1TestForOffCards] == OFFCARDS[11] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[12] || Player1Hand[P1TestForOffCards] == OFFCARDS[13] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[14] || Player1Hand[P1TestForOffCards] == OFFCARDS[15] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[16] || Player1Hand[P1TestForOffCards] == OFFCARDS[17] ||
                Player1Hand[P1TestForOffCards] == OFFCARDS[18] || Player1Hand[P1TestForOffCards] == OFFCARDS[19]) {

                P1ListOfYourOffCards[OffCardsInYourHand] = Player1Hand[P1TestForOffCards];
                OffCardsInYourHand++;
            }
        }

        cout << "Your offensive cards: " << endl;
        for (int DisplayYourOffCards = 0; DisplayYourOffCards < OffCardsInYourHand; DisplayYourOffCards++) {
            cout << DisplayYourOffCards + 1 << "." << P1ListOfYourOffCards[DisplayYourOffCards] << endl;
        }

        string PlayOffCard;

        while (OffCardsInYourHand > 0) {
            cout << "Which card do you want to play? ";
            getline(cin, PlayOffCard);

            if (PlayOffCard == P1ListOfYourOffCards[0] || PlayOffCard == P1ListOfYourOffCards[1] ||
                PlayOffCard == P1ListOfYourOffCards[2] || PlayOffCard == P1ListOfYourOffCards[3] ||
                PlayOffCard == P1ListOfYourOffCards[4] || PlayOffCard == P1ListOfYourOffCards[5] ||
                PlayOffCard == P1ListOfYourOffCards[6] || PlayOffCard == P1ListOfYourOffCards[7] ||
                PlayOffCard == P1ListOfYourOffCards[8] || PlayOffCard == P1ListOfYourOffCards[9]) {

                if (PlayOffCard == "Stab" || PlayOffCard == "d_Stab") {
                    P1Stab(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Hellfire_Sword" || PlayOffCard == "d_Hellfire_Sword") {
                    P1HellfireSword(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Fire_Moon" || PlayOffCard == "d_Fire_Moon") {
                    P1FireMoon(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Vital_Spot" || PlayOffCard == "d_Vital_Spot") {
                    P1VitalSpot(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Star_Curse" || PlayOffCard == "d_Star_Curse") {
                    P1StarCurse(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Rapid_Attack") {
                    P1RapidAttack(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "d_Rapid_Attack") {
                    P1RapidAttackD(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Desperate_Strike" || PlayOffCard == "d_Desperate_Strike") {
                    int DesperateStrikesInP1Hand = 0;
                    for (int P1CheckForDesperateStrikes = 0; P1CheckForDesperateStrikes < 20; P1CheckForDesperateStrikes++) {
                        if (P1ListOfYourOffCards[P1CheckForDesperateStrikes] == "Desperate_Strike" || P1ListOfYourOffCards[P1CheckForDesperateStrikes] == "d_Desperate_Strike") {
                            DesperateStrikesInP1Hand++;
                        }
                    }
                    if (DesperateStrikesInP1Hand == 1) {
                        if (OffCardsInYourHand > 1) {
                            P1DesperateStrike(PlayOffCard);
                        }
                        else if (cardsInP1Hand < cardsInP2Hand) {
                            P1DesperateStrike(PlayOffCard);
                        }
                        else cout << "You have nothing else to play, and this card is unavailable so it will be discarded" << endl;
                    }
                    if (DesperateStrikesInP1Hand == 2) {
                        if (OffCardsInYourHand > 2) {
                            P1DesperateStrike(PlayOffCard);
                        }
                        else if (cardsInP1Hand < cardsInP2Hand) {
                            P1DesperateStrike(PlayOffCard);
                        }
                        else cout << "You have nothing else to play, and this card is unavailable so it will be discarded" << endl;
                    }
                }

                if (PlayOffCard == "Sacrificial_Attack" || PlayOffCard == "d_Sacrificial_Attack") {
                    P1SacrificialAttack(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Devils_Deal" || PlayOffCard == "d_Devils_Deal") {
                    void P1DevilsDeal(string P1ChooseCard);
                    P1DevilsDeal(PlayOffCard);
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }

                if (PlayOffCard == "Last_Breath" || PlayOffCard == "d_Last_Breath") {
                    cout << "No effect" << endl;
                    OffCardsInYourHand--;
                    for (int P1RemoveOffCardsFromList = 0; P1RemoveOffCardsFromList < 10; P1RemoveOffCardsFromList++) {
                        if (P1ListOfYourOffCards[P1RemoveOffCardsFromList] == PlayOffCard) {
                            P1ListOfYourOffCards[P1RemoveOffCardsFromList] = "xEmptyx";
                        }
                    }
                }
            }
            else cout << "You don't have that card" << endl;
        }
        P1ActionsLeft = 0;
    }
    else cout << "You can't play this card" << endl;
}

void P1DevilsDeal(string P1ChooseCard)
{
    P1TakeCardOutOfHand(P1ChooseCard);
    string P1ListOfEnemyOffCards[10];
    int OffCardsInEnemyHand = 0;

    for (int P1TestForEnemyOffCards = 0; P1TestForEnemyOffCards < 20; P1TestForEnemyOffCards++) {
        if (Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[0] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[1] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[2] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[3] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[4] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[5] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[6] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[7] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[8] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[9] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[10] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[11] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[12] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[13] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[14] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[15] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[16] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[17] ||
            Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[18] || Player2Hand[P1TestForEnemyOffCards] == OFFCARDS[19]) {
            P1ListOfEnemyOffCards[OffCardsInEnemyHand] = Player2Hand[P1TestForEnemyOffCards];
            OffCardsInEnemyHand++;
        }
    }

    if (CardsLeftInOffDeck > 0) {
        if (OffCardsInEnemyHand == 0) {
            P1ListOfEnemyOffCards[OffCardsInEnemyHand] = OffDeck[CardsLeftInOffDeck - 1];
            OffCardsInEnemyHand++;
            P2draw("Off");
        }

        cout << "Player 2 Offensive cards:" << endl;
        for (int P1DisplayEnemyOffCards = 0; P1DisplayEnemyOffCards <= OffCardsInEnemyHand; P1DisplayEnemyOffCards++) {
            cout << P1ListOfEnemyOffCards[P1DisplayEnemyOffCards] << endl;
        }

        bool CardPlayed = false;
        string CardPlayedOnYourself;

        while (CardPlayed == false) {
            cout << "Choose which card you want to play on yourself: "; getline(cin, CardPlayedOnYourself);
            if (CardPlayedOnYourself == P1ListOfEnemyOffCards[0] || CardPlayedOnYourself == P1ListOfEnemyOffCards[5] ||
                CardPlayedOnYourself == P1ListOfEnemyOffCards[1] || CardPlayedOnYourself == P1ListOfEnemyOffCards[6] ||
                CardPlayedOnYourself == P1ListOfEnemyOffCards[2] || CardPlayedOnYourself == P1ListOfEnemyOffCards[7] ||
                CardPlayedOnYourself == P1ListOfEnemyOffCards[3] || CardPlayedOnYourself == P1ListOfEnemyOffCards[8] ||
                CardPlayedOnYourself == P1ListOfEnemyOffCards[4] || CardPlayedOnYourself == P1ListOfEnemyOffCards[9]) {

                CardPlayed = true;

                if (CardPlayedOnYourself == "Stab" || CardPlayedOnYourself == "d_Stab") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1Stab("None");
                }

                if (CardPlayedOnYourself == "Hellfire_Sword" || CardPlayedOnYourself == "d_HellFire_Sword") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1HellfireSword("None");
                }

                if (CardPlayedOnYourself == "Vital_Spot" || CardPlayedOnYourself == "d_Vital_Spot") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1VitalSpot("None");
                }

                if (CardPlayedOnYourself == "Star_Curse" || CardPlayedOnYourself == "d_Star_Curse") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1StarCurse("None");
                }

                if (CardPlayedOnYourself == "Rapid_Attack") {
                    P1RapidAttack("None");
                }

                if (CardPlayedOnYourself == "d_Rapid_Attack") {
                    P1RapidAttackD("None");
                }

                if (CardPlayedOnYourself == "Desperate_Strike" || CardPlayedOnYourself == "d_Desperate_Strike") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1DesperateStrike("None");
                }

                if (CardPlayedOnYourself == "Sacrificial_Attack" || CardPlayedOnYourself == "d_Sacrificial_Strike") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P2SacrificialAttack("None");
                }

                if (CardPlayedOnYourself == "Devils_Deal" || CardPlayedOnYourself == "d_Devils_Deal") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1DevilsDeal("None");
                }

                if (CardPlayedOnYourself == "Last_Breath" || CardPlayedOnYourself == "d_Last_Breath") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    cout << "No effect" << endl;
                }

                if (CardPlayedOnYourself == "Fire_Moon" || CardPlayedOnYourself == "d_Fire_Moon") {
                    P2TakeCardOutOfHand(CardPlayedOnYourself);
                    P1FireMoon("None");
                    P1ActionsLeft++;
                }
            }
            else cout << "You don't have that card" << endl;
        }
    }
    else {
        if (OffCardsInEnemyHand > 0) {
            cout << "Player 2 Offensive cards:" << endl;
            for (int P1DisplayEnemyOffCards = 0; P1DisplayEnemyOffCards < OffCardsInEnemyHand; P1DisplayEnemyOffCards++) {
                cout << P1DisplayEnemyOffCards + 1 << "." << P1ListOfEnemyOffCards[P1DisplayEnemyOffCards] << endl;
            }

            bool CardPlayed = false;
            string CardPlayedOnYourself;

            while (CardPlayed == false) {
                cout << "Choose which card you want to play on yourself: "; getline(cin, CardPlayedOnYourself);
                if (CardPlayedOnYourself == P1ListOfEnemyOffCards[0] || CardPlayedOnYourself == P1ListOfEnemyOffCards[5] ||
                    CardPlayedOnYourself == P1ListOfEnemyOffCards[1] || CardPlayedOnYourself == P1ListOfEnemyOffCards[6] ||
                    CardPlayedOnYourself == P1ListOfEnemyOffCards[2] || CardPlayedOnYourself == P1ListOfEnemyOffCards[7] ||
                    CardPlayedOnYourself == P1ListOfEnemyOffCards[3] || CardPlayedOnYourself == P1ListOfEnemyOffCards[8] ||
                    CardPlayedOnYourself == P1ListOfEnemyOffCards[4] || CardPlayedOnYourself == P1ListOfEnemyOffCards[9]) {

                    CardPlayed = true;

                    if (CardPlayedOnYourself == "Stab" || CardPlayedOnYourself == "d_Stab") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1Stab("None");
                    }

                    if (CardPlayedOnYourself == "Hellfire_Sword" || CardPlayedOnYourself == "d_HellFire_Sword") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1HellfireSword("None");
                    }

                    if (CardPlayedOnYourself == "Vital_Spot" || CardPlayedOnYourself == "d_Vital_Spot") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1VitalSpot("None");
                    }

                    if (CardPlayedOnYourself == "Star_Curse" || CardPlayedOnYourself == "d_Star_Curse") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1StarCurse("None");
                    }

                    if (CardPlayedOnYourself == "Rapid_Attack") {
                        P1RapidAttack("None");
                    }

                    if (CardPlayedOnYourself == "d_Rapid_Attack") {
                        P1RapidAttackD("None");
                    }

                    if (CardPlayedOnYourself == "Desperate_Strike" || CardPlayedOnYourself == "d_Desperate_Strike") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1DesperateStrike("None");
                    }

                    if (CardPlayedOnYourself == "Sacrificial_Attack" || CardPlayedOnYourself == "d_Sacrificial_Strike") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P2SacrificialAttack("None");
                    }

                    if (CardPlayedOnYourself == "Devils_Deal" || CardPlayedOnYourself == "d_Devils_Deal") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1DevilsDeal("None");
                    }

                    if (CardPlayedOnYourself == "Last_Breath" || CardPlayedOnYourself == "d_Last_Breath") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        cout << "No effect" << endl;
                    }

                    if (CardPlayedOnYourself == "Fire_Moon" || CardPlayedOnYourself == "d_Fire_Moon") {
                        P2TakeCardOutOfHand(CardPlayedOnYourself);
                        P1FireMoon("None");
                        P1ActionsLeft++;
                    }
                }
                else cout << "You don't have that card 2" << endl;
            }
        }
        else cout << "Enemy has no offensive cards and the offensive deck is empty, the card won't have any effect" << endl;
    }
}

void P1play(string P1ChooseCard)
{
    if (P1ChooseCard == Player1Hand[0] || P1ChooseCard == Player1Hand[1] || P1ChooseCard == Player1Hand[2]
         || P1ChooseCard == Player1Hand[3] || P1ChooseCard == Player1Hand[4] || P1ChooseCard == Player1Hand[5]
          || P1ChooseCard == Player1Hand[6] || P1ChooseCard == Player1Hand[7] || P1ChooseCard == Player1Hand[8]
           || P1ChooseCard == Player1Hand[9]) {
///Defensive cards////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (P1ChooseCard == "Armor_Up" || P1ChooseCard == "d_Armor_Up" || P1ChooseCard == "Barrier" || P1ChooseCard == "d_Barrier" ||
            P1ChooseCard == "War_Blessing" || P1ChooseCard == "d_War_Blessing" || P1ChooseCard == "Recreation" || P1ChooseCard == "d_Recreation" ||
            P1ChooseCard == "Cure" || P1ChooseCard == "d_Cure" || P1ChooseCard == "Preparations" || P1ChooseCard == "d_Preparations" ||
            P1ChooseCard == "Spirit_Shelter" || P1ChooseCard == "d_Spirit_Shelter" || P1ChooseCard == "Healer" || P1ChooseCard == "d_Healer" ||
            P1ChooseCard == "Peace_Pact" || P1ChooseCard == "d_Peace_Pact" || P1ChooseCard == "Soothing_Spell" || P1ChooseCard == "d_Soothing_Spell") {
            if (P2ActiveFireMoon == false && P2ActiveFireMoonD == false) {

                if (P1ChooseCard == "Armor_Up" || P1ChooseCard == "d_Armor_Up") {
                    P1ArmorUp(P1ChooseCard);
                }

                if (P1ChooseCard == "Barrier" || P1ChooseCard == "d_Barrier") {
                    P1Barrier(P1ChooseCard);
                }

                if (P1ChooseCard == "War_Blessing" || P1ChooseCard == "d_War_Blessing") {
                    P1WarBlessing(P1ChooseCard);
                }

                if (P1ChooseCard == "Recreation" || P1ChooseCard == "d_Recreation") {
                    P1Recreation(P1ChooseCard);
                }

                if (P1ChooseCard == "Cure" || P1ChooseCard == "d_Cure") {
                    P1Cure(P1ChooseCard);
                }

                if (P1ChooseCard == "Preparations" || P1ChooseCard == "d_Preparations") {
                    P1Preparations(P1ChooseCard);
                }

                if (P1ChooseCard == "Spirit_Shelter" || P1ChooseCard == "d_Spirit_Shelter") {
                    P1SpiritShelter(P1ChooseCard);
                }

                if (P1ChooseCard == "Healer" || P1ChooseCard == "d_Healer") {
                    P1Healer(P1ChooseCard);
                }

                if (P1ChooseCard == "Peace_Pact" || P1ChooseCard == "d_Peace_Pact") {
                    P1PeacePact(P1ChooseCard);
                }

                if (P1ChooseCard == "Soothing_Spell" || P1ChooseCard == "d_Soothing_Spell") {
                    P1SoothingSpell(P1ChooseCard);
                }
            }
            else cout << "You cant't play defensive cards, because Fire Moon is active" << endl;
        }
///Offensive cards////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (P1ChooseCard == "Stab" || P1ChooseCard == "d_Stab" || P1ChooseCard == "Fire_Moon" || P1ChooseCard == "d_Fire_Moon" ||
            P1ChooseCard == "Hellfire_Sword" || P1ChooseCard == "d_Hellfire_Sword" || P1ChooseCard == "Vital_Spot" || P1ChooseCard == "d_Vital_Spot" ||
            P1ChooseCard == "Star_Curse" || P1ChooseCard == "d_Star_Curse" || P1ChooseCard == "Rapid_Attack" || P1ChooseCard == "d_Rapid_Attack" ||
            P1ChooseCard == "Desperate_Strike" || P1ChooseCard == "d_Desperate_Strike" || P1ChooseCard == "Sacrificial_Attack" || P1ChooseCard == "d_Sacrificial_Attack" ||
            P1ChooseCard == "Devils_Deal" || P1ChooseCard == "d_Devils_Deal" || P1ChooseCard == "Last_Breath" || P1ChooseCard == "d_Last_Breath") {
            if (P2ActiveSoothingSpell == false && P2ActiveSoothingSpellD == false) {

                if (P1ChooseCard == "Stab" || P1ChooseCard == "d_Stab") {
                    P1Stab(P1ChooseCard);
                }

                if (P1ChooseCard == "Hellfire_Sword" || P1ChooseCard == "d_Hellfire_Sword") {
                    P1HellfireSword(P1ChooseCard);
                }

                if (P1ChooseCard == "Fire_Moon" || P1ChooseCard == "d_Fire_Moon") {
                    P1FireMoon(P1ChooseCard);
                }

                if (P1ChooseCard == "Vital_Spot" || P1ChooseCard == "d_Vital_Spot") {
                    P1VitalSpot(P1ChooseCard);
                }

                if (P1ChooseCard == "Star_Curse" || P1ChooseCard == "d_Star_Curse") {
                    P1StarCurse(P1ChooseCard);
                }

                if (P1ChooseCard == "Rapid_Attack")  {
                    P1RapidAttack(P1ChooseCard);
                }

                if (P1ChooseCard == "d_Rapid_Attack") {
                    P1RapidAttackD(P1ChooseCard);
                }

                if (P1ChooseCard == "Desperate_Strike" || P1ChooseCard == "d_Desperate_Strike") {
                    P1DesperateStrike(P1ChooseCard);
                }

                if (P1ChooseCard == "Sacrificial_Attack" || P1ChooseCard == "d_Sacrificial_Attack") {
                    P1SacrificialAttack(P1ChooseCard);
                }

                if (P1ChooseCard == "Last_Breath" || P1ChooseCard == "d_Last_Breath") {
                    P1LastBreath(P1ChooseCard);
                }

                if (P1ChooseCard == "Devils_Deal" || P1ChooseCard == "d_Devils_Deal") {
                    P1DevilsDeal(P1ChooseCard);
                }
            }
            else cout << "You can't play offensive cards, because Soothing Spell is active" << endl;
        }
    }
    else cout << "You don't have that card" << endl;
}

void P1draw(string P1ChooseDeck)
{
    if (P1ChooseDeck == "Off") {
        if (CardsLeftInOffDeck > 0) {
            Player1Hand[cardsInP1Hand] = OffDeck[CardsLeftInOffDeck];
            cout << " " << endl;
            cardsInP1Hand++;
            OffDeck[CardsLeftInOffDeck] = "xEmptyx";
            CardsLeftInOffDeck--;
            cout << "You drew a card" << endl;
            P1ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;

    }
    else if (P1ChooseDeck == "Def") {
        if (CardsLeftInDefDeck > 0) {
            Player1Hand[cardsInP1Hand] = DefDeck[CardsLeftInDefDeck];
            cout << " " << endl;
            cardsInP1Hand++;
            DefDeck[CardsLeftInDefDeck] = "xEmptyx";
            CardsLeftInDefDeck--;
            cout << "You drew a card" << endl;
            P1ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;
    }
    else if (P1ChooseDeck == "Uti") {
        if (CardsLeftInUtiDeck > 0) {
            Player1Hand[cardsInP1Hand] = UtiDeck[CardsLeftInUtiDeck];
            cout << " " << endl;
            cardsInP1Hand++;
            UtiDeck[CardsLeftInUtiDeck] = "xEmptyx";
            CardsLeftInUtiDeck--;
            cout << "You drew a card" << endl;
            P1ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;
    }
    else if (P1ChooseDeck == "Com") {
        if (CardsLeftInComDeck > 0) {
            Player1Hand[cardsInP1Hand] = ComDeck[CardsLeftInComDeck];
            cout << " " << endl;
            cardsInP1Hand++;
            ComDeck[CardsLeftInComDeck] = "xEmptyx";
            CardsLeftInComDeck--;
            cout << "You drew a card" << endl;
            P1ActionsLeft--;
        }
        else cout << "This deck has no cards in it" << endl;
    }
    else cout << "There is no deck called " << P1ChooseDeck << endl;
}

void P1choose(string P1Action)
{
    if (P1Action == "Draw") {
        if (CardsLeftInOffDeck == 0 && CardsLeftInDefDeck == 0 && CardsLeftInUtiDeck == 0 && CardsLeftInComDeck == 0) {
            GameEndDraw();
        }
        else {
            if (cardsInP1Hand == 10) {
                cout << "You can't draw cards, your hand is full" << endl;
            }
            else {
                string P1ChooseDeck;
                getline(cin, P1ChooseDeck);
                P1draw(P1ChooseDeck);
            }
        }
    }
    if (P1Action == "Play") {
        if (cardsInP1Hand > 0) {
            string P1ChooseCard;
            getline(cin, P1ChooseCard);
            P1play(P1ChooseCard);
        }
        else cout << "You have no cards in your hand" << endl;
    }
    if (P1Action == "Discard") {
        if (cardsInP1Hand > 0) {
            string P1DiscardCard;
            getline(cin, P1DiscardCard);
            P1discard(P1DiscardCard);
        }
        else cout << "You have no cards to discard" << endl;
    }
    if (P1Action == "InfoP1") {
        displayInfoP1();
    }
    if (P1Action == "InfoP2") {
        displayInfoP2();
    }
    if (P1Action == "AddP1") {
        string P1Add;
        getline(cin, P1Add);
        P1AddCardInHand(P1Add);
    }
    if (P1Action == "AddP2") {
        string P2Add;
        getline(cin, P2Add);
        P2AddCardInHand(P2Add);
    }
    if (P1Action == "EndturnP1") {
        P1ActionsLeft = 0;
    }
    if (P1Action != "Draw" && P1Action != "Play" && P1Action != "Discard" && P1Action != "InfoP1" &&
        P1Action != "InfoP2" && P1Action != "AddP1" && P1Action != "AddP2" && P1Action != "EndturnP1") {
        cout << "This isn't a viable option" << endl;
    }
}///////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    OFFCARDS[0]  = "Hellfire_Sword";
    OFFCARDS[1]  = "Sacrificial_Attack";
    OFFCARDS[2]  = "Desperate_Strike";
    OFFCARDS[3]  = "Rapid_Attack";
    OFFCARDS[4]  = "Devils_Deal";
    OFFCARDS[5]  = "Star_Curse";
    OFFCARDS[6]  = "Last_Breath";
    OFFCARDS[7]  = "Vital_Spot";
    OFFCARDS[8]  = "Stab";
    OFFCARDS[9]  = "Fire_Moon";
    OFFCARDS[10] = "d_Hellfire_Sword";
    OFFCARDS[11] = "d_Sacrificial_Attack";
    OFFCARDS[12] = "d_Desperate_Strike";
    OFFCARDS[13] = "d_Rapid_Attack";
    OFFCARDS[14] = "d_Devils_Deal";
    OFFCARDS[15] = "d_Star_Curse";
    OFFCARDS[16] = "d_Last_Breath";
    OFFCARDS[17] = "d_Vital_Spot";
    OFFCARDS[18] = "d_Stab";
    OFFCARDS[19] = "d_Fire_Moon";

    time(&miliseconds);
    srand((unsigned int)miliseconds);

    for (int FillUsedOff = 0; FillUsedOff < 19; FillUsedOff++){
        UsedOff[FillUsedOff] = "Nothing";
    }
    while (RandomizedOffCards < 20) {
        Random_Number = (rand() % 20);
        RandomOffCard = OFFCARDS[Random_Number];
        if (RandomOffCard != UsedOff[0] && RandomOffCard != UsedOff[1] && RandomOffCard != UsedOff[2] && RandomOffCard != UsedOff[3] &&
            RandomOffCard != UsedOff[4] && RandomOffCard != UsedOff[5] && RandomOffCard != UsedOff[6] && RandomOffCard != UsedOff[7] &&
            RandomOffCard != UsedOff[8] && RandomOffCard != UsedOff[9] && RandomOffCard != UsedOff[10] && RandomOffCard != UsedOff[11] &&
            RandomOffCard != UsedOff[12] && RandomOffCard != UsedOff[13] && RandomOffCard != UsedOff[14] && RandomOffCard != UsedOff[15] &&
            RandomOffCard != UsedOff[16] && RandomOffCard != UsedOff[17] && RandomOffCard != UsedOff[18] && RandomOffCard != UsedOff[19]) {
            OffDeck[RandomizedOffCards] = OFFCARDS[Random_Number];
            UsedOff[RandomizedOffCards] = OFFCARDS[Random_Number];
            cout << OffDeck[RandomizedOffCards] << endl;
            RandomizedOffCards++;
        }
    }

    cout << " " << endl;

    DEFCARDS[0]  = "Armor_Up";
    DEFCARDS[1]  = "Barrier";
    DEFCARDS[2]  = "War_Blessing";
    DEFCARDS[3]  = "Recreation";
    DEFCARDS[4]  = "Cure";
    DEFCARDS[5]  = "Preparations";
    DEFCARDS[6]  = "Spirit_Shelter";
    DEFCARDS[7]  = "Healer";
    DEFCARDS[8]  = "Peace_Pact";
    DEFCARDS[9]  = "Soothing_Spell";
    DEFCARDS[10] = "d_Armor_Up";
    DEFCARDS[11] = "d_Barrier";
    DEFCARDS[12] = "d_War_Blessing";
    DEFCARDS[13] = "d_Recreation";
    DEFCARDS[14] = "d_Cure";
    DEFCARDS[15] = "d_Preparations";
    DEFCARDS[16] = "d_Spirit_Shelter";
    DEFCARDS[17] = "d_Healer";
    DEFCARDS[18] = "d_Peace_Pact";
    DEFCARDS[19] = "d_Soothing_Spell";

    time(&miliseconds);
    srand((unsigned int)miliseconds);

    for (int FillUsedDef = 0; FillUsedDef < 19; FillUsedDef++){
        UsedDef[FillUsedDef] = "Nothing";
    }
    while (RandomizedDefCards < 20) {
        Random_Number = (rand() % 20);
        RandomDefCard = DEFCARDS[Random_Number];
        if (RandomDefCard != UsedDef[0] && RandomDefCard != UsedDef[1] && RandomDefCard != UsedDef[2] && RandomDefCard != UsedDef[3] &&
            RandomDefCard != UsedDef[4] && RandomDefCard != UsedDef[5] && RandomDefCard != UsedDef[6] && RandomDefCard != UsedDef[7] &&
            RandomDefCard != UsedDef[8] && RandomDefCard != UsedDef[9] && RandomDefCard != UsedDef[10] && RandomDefCard != UsedDef[11] &&
            RandomDefCard != UsedDef[12] && RandomDefCard != UsedDef[13] && RandomDefCard != UsedDef[14] && RandomDefCard != UsedDef[15] &&
            RandomDefCard != UsedDef[16] && RandomDefCard != UsedDef[17] && RandomDefCard != UsedDef[18] && RandomDefCard != UsedDef[19]) {
            DefDeck[RandomizedDefCards] = DEFCARDS[Random_Number];
            UsedDef[RandomizedDefCards] = DEFCARDS[Random_Number];
            cout << DefDeck[RandomizedDefCards] << endl;
            RandomizedDefCards++;
        }
    }

    cout << " " << endl;

    UTICARDS[0]  = "Summon_Dead";
    UTICARDS[1]  = "Dragon_Flame";
    UTICARDS[2]  = "Dragon_Power";
    UTICARDS[3]  = "Cheating";
    UTICARDS[4]  = "Hidden_Trasures";
    UTICARDS[5]  = "Set_Up";
    UTICARDS[6]  = "The_Thief";
    UTICARDS[7]  = "Mind_Steal";
    UTICARDS[8]  = "Dragon_Sight";
    UTICARDS[9]  = "Absolute_Zero";
    UTICARDS[10] = "d_Summon_Dead";
    UTICARDS[11] = "d_Dragon_Flame";
    UTICARDS[12] = "d_Dragon_Power";
    UTICARDS[13] = "d_Cheating";
    UTICARDS[14] = "d_Hidden_Treasures";
    UTICARDS[15] = "d_Set_Up";
    UTICARDS[16] = "d_The_Thief";
    UTICARDS[17] = "d_Mind_Steal";
    UTICARDS[18] = "d_Dragon_Sight";
    UTICARDS[19] = "d_Absolute_Zero";

    time(&miliseconds);
    srand((unsigned int)miliseconds);

    string UsedUti[20];
    for (int FillUsedUti = 0; FillUsedUti < 20; FillUsedUti++){
        UsedUti[FillUsedUti] = "Nothing";
    }
    int RandomizedUtiCards = 0;
    while (RandomizedUtiCards < 20) {
        Random_Number = (rand() % 20);
        RandomUtiCard = UTICARDS[Random_Number];
        if (RandomUtiCard != UsedUti[0] && RandomUtiCard != UsedUti[1] && RandomUtiCard != UsedUti[2] && RandomUtiCard != UsedUti[3] &&
            RandomUtiCard != UsedUti[4] && RandomUtiCard != UsedUti[5] && RandomUtiCard != UsedUti[6] && RandomUtiCard != UsedUti[7] &&
            RandomUtiCard != UsedUti[8] && RandomUtiCard != UsedUti[9] && RandomUtiCard != UsedUti[10] && RandomUtiCard != UsedUti[11] &&
            RandomUtiCard != UsedUti[12] && RandomUtiCard != UsedUti[13] && RandomUtiCard != UsedUti[14] && RandomUtiCard != UsedUti[15] &&
            RandomUtiCard != UsedUti[16] && RandomUtiCard != UsedUti[17] && RandomUtiCard != UsedUti[18] && RandomUtiCard != UsedUti[19]) {
            UtiDeck[RandomizedUtiCards] = UTICARDS[Random_Number];
            UsedUti[RandomizedUtiCards] = UTICARDS[Random_Number];
            cout << UtiDeck[RandomizedUtiCards] << endl;
            RandomizedUtiCards++;
        }
    }

    cout << " " << endl;

    COMCARDS[0]  = "Dream_Land";
    COMCARDS[1]  = "Fire_Phoenix";
    COMCARDS[2]  = "Shackling";
    COMCARDS[3]  = "Superiority";
    COMCARDS[4]  = "Magic_Trick";
    COMCARDS[5]  = "Ultimate_Defence";
    COMCARDS[6]  = "Power_Up";
    COMCARDS[7]  = "Final_Hour";
    COMCARDS[8]  = "Battle_Wizard";
    COMCARDS[9]  = "Double_Strike";
    COMCARDS[10] = "Underworld";
    COMCARDS[11] = "The_Search";
    COMCARDS[12] = "Three_Gods_Strike";
    COMCARDS[13] = "Counter_Scale";
    COMCARDS[14] = "Fake_Out";
    COMCARDS[15] = "Different_Sides";
    COMCARDS[16] = "Soulstealer";
    COMCARDS[17] = "Swap_Pact";
    COMCARDS[18] = "Total_Entanglement";
    COMCARDS[19] = "Nirvana";

    time(&miliseconds);
    srand((unsigned int)miliseconds);

    string UsedCom[20];
    for (int FillUsedCom = 0; FillUsedCom < 20; FillUsedCom++){
        UsedCom[FillUsedCom] = "Nothing";
    }
    int RandomizedComCards = 0;
    while (RandomizedComCards < 20) {
        Random_Number = (rand() % 20);
        RandomComCard = COMCARDS[Random_Number];
        if (RandomComCard != UsedCom[0] && RandomComCard != UsedCom[1] && RandomComCard != UsedCom[2] && RandomComCard != UsedCom[3] &&
            RandomComCard != UsedCom[4] && RandomComCard != UsedCom[5] && RandomComCard != UsedCom[6] && RandomComCard != UsedCom[7] &&
            RandomComCard != UsedCom[8] && RandomComCard != UsedCom[9] && RandomComCard != UsedCom[10] && RandomComCard != UsedCom[11] &&
            RandomComCard != UsedCom[12] && RandomComCard != UsedCom[13] && RandomComCard != UsedCom[14] && RandomComCard != UsedCom[15] &&
            RandomComCard != UsedCom[16] && RandomComCard != UsedCom[17] && RandomComCard != UsedCom[18] && RandomComCard != UsedCom[19]) {
            ComDeck[RandomizedComCards] = COMCARDS[Random_Number];
            UsedCom[RandomizedComCards] = COMCARDS[Random_Number];
            cout << ComDeck[RandomizedComCards] << endl;
            RandomizedComCards++;
        }
    }

    cout << " " << endl;

    for (int FillGraveyard = 0; FillGraveyard < 80; FillGraveyard++){
        Graveyard[FillGraveyard] = "xEmptyx";
    }

    for (int FillHandP1 = 0; FillHandP1 < 20; FillHandP1++) {
        Player1Hand[FillHandP1] = "xEmptyx";
    }
    for (int FillHandP2 = 0; FillHandP2 < 20; FillHandP2++) {
        Player2Hand[FillHandP2] = "xEmptyx";
    }

    Player1Hand[0] = OffDeck[CardsLeftInOffDeck];
    CardsLeftInOffDeck--;
    cardsInP1Hand++;
    Player1Hand[1] = DefDeck[CardsLeftInDefDeck];
    CardsLeftInDefDeck--;
    cardsInP1Hand++;
    Player1Hand[2] = UtiDeck[CardsLeftInUtiDeck];
    CardsLeftInUtiDeck--;
    cardsInP1Hand++;
    Player1Hand[3] = ComDeck[CardsLeftInComDeck];
    CardsLeftInComDeck--;
    cardsInP1Hand++;
    cout << Player1Hand[0] << endl;
    cout << Player1Hand[1] << endl;
    cout << Player1Hand[2] << endl;
    cout << Player1Hand[3] << endl;
    cout << Player1Hand[4] << endl;
    cout << Player1Hand[5] << endl;
    cout << Player1Hand[6] << endl;
    cout << Player1Hand[7] << endl;
    cout << Player1Hand[8] << endl;
    cout << Player1Hand[9] << endl;

    cout << " " << endl;

    Player2Hand[0] = OffDeck[CardsLeftInOffDeck];
    CardsLeftInOffDeck--;
    cardsInP2Hand++;
    Player2Hand[1] = DefDeck[CardsLeftInDefDeck];
    CardsLeftInDefDeck--;
    cardsInP2Hand++;
    Player2Hand[2] = UtiDeck[CardsLeftInUtiDeck];
    CardsLeftInUtiDeck--;
    cardsInP2Hand++;
    Player2Hand[3] = ComDeck[CardsLeftInComDeck];
    CardsLeftInComDeck--;
    cardsInP2Hand++;
    cout << Player2Hand[0] << endl;
    cout << Player2Hand[1] << endl;
    cout << Player2Hand[2] << endl;
    cout << Player2Hand[3] << endl;
    cout << Player2Hand[4] << endl;
    cout << Player2Hand[5] << endl;
    cout << Player2Hand[6] << endl;
    cout << Player2Hand[7] << endl;
    cout << Player2Hand[8] << endl;
    cout << Player2Hand[9] << endl;

    cout << " " << endl;

    string P1Action;
    string P2Action;
    bool PlayersAlive = true;

    while (PlayersAlive == true) {
        if (Player1HP > 0 && Player2HP > 0) {
            P2ActionsLeft = 2;

            if (P1ActiveFireMoon == true)
                P1ActiveFireMoon = false;
            if (P1ActiveFireMoonD == true)
                P1ActiveFireMoonD = false;
            if (P1ActiveSoothingSpell == true)
                P1ActiveSoothingSpell = false;
            if (P1ActiveSoothingSpellD == true)
                P1ActiveSoothingSpellD = false;
            if (P1ActiveAbsoluteZero == true)
                P1ActiveAbsoluteZero = false;
            if (P1ActiveAbsoluteZeroD == true)
                P1ActiveAbsoluteZeroD = false;
            if (P1UsedRapidAttack == true)
                P1UsedRapidAttack = false;
            if (P1UsedRapidAttackD == true)
                P1UsedRapidAttackD = false;

            if (P2ActiveAbsoluteZero == true || P2ActiveAbsoluteZeroD == true) {
                cout << "Enemy has Absolute Zero active. P1 skips a turn" << endl;
            }
            else {
                while (P1ActionsLeft > 0) {
                    getline(cin, P1Action);
                    P1choose(P1Action);
                }
                if (NoCardsInDecks == true) {
                    Player1HP = 0;
                    Player2HP = 0;
                }
                else cout << "End of P1 turn" << endl;
            }
            if (P1ActiveSpiritShelter == false && P1ActiveSpiritShelterD == false) {
                if (Player1Armor > 0) {
                    Player1Armor--;
                }
                else Player1HP--;
            }
        }
        else {
            PlayersAlive == false;
        }

        statement = true;

        if (Player1HP > 0 && Player2HP > 0) {
            P1ActionsLeft = 2;

            if (P2ActiveFireMoon == true)
                P2ActiveFireMoon = false;
            if (P2ActiveFireMoonD == true)
                P2ActiveFireMoonD = false;
            if (P2ActiveSoothingSpell == true)
                P2ActiveSoothingSpell = false;
            if (P2ActiveSoothingSpellD == true)
                P2ActiveSoothingSpellD = false;
            if (P2ActiveAbsoluteZero == true)
                P2ActiveAbsoluteZero = false;
            if (P2ActiveAbsoluteZeroD == true)
                P2ActiveAbsoluteZeroD = false;
            if (P2UsedRapidAttack == true)
                P2UsedRapidAttack = false;
            if (P2UsedRapidAttackD == true)
                P2UsedRapidAttackD = false;

            if (P1ActiveAbsoluteZero == true || P1ActiveAbsoluteZeroD == true) {
                cout << "Enemy has Absolute Zero active. P2 skips a turn" << endl;
            }
            else {
                while (P2ActionsLeft > 0) {
                    getline(cin, P2Action);
                    P2choose(P2Action);
                }
                if (NoCardsInDecks == true) {
                    Player1HP = 0;
                    Player2HP = 0;
                }
                else cout << "End of P2 turn" << endl;
            }
            if (P2ActiveSpiritShelter == false && P2ActiveSpiritShelterD == false) {
                if (Player2Armor > 0) {
                    Player2Armor--;
                }
                else Player2HP--;
            }
        }
        else {
            PlayersAlive == false;
        }
    }
    return 0;
}
