// vampires.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Vampire declaration.

class Vampire
{
public:
    // Constructor
    Vampire(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool isPoisoned = false;
    int poisonTurns = 0;
    bool poisonedTwice = false;
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(string msg) const;
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();
    
private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;
    
    // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
int checkSurroundings (const Arena& a, int centerR, int centerC, int dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A vampire must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Vampire created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
    return m_col;
}

bool Vampire::isDead() const
{
    return poisonedTwice;
}

void Vampire::move()
{
    //   Return without moving if the vampire has drunk one vial of
    //   poisoned blood (so is supposed to move only every other turn) and
    //   this is a turn it does not move.
    
    //   Otherwise, attempt to move in a random direction; if can't
    //   move, don't move.  If it lands on a poisoned blood vial, drink all
    //   the blood in the vial and remove it from the game (so it is no
    //   longer on that grid point).
    
    int direction = randInt(0, 3); //choose random direction to move in
    
    if (!isPoisoned || (poisonTurns % 2 == 1))
    {
        attemptMove(*m_arena, direction, m_row, m_col); //attempt to move only if the vampire is not poisoned or it is moving on its every other turn status
    }
    
    if (isPoisoned) //increase number of poisoned turns if it was poisoned once
    {
        poisonTurns++;
    }
    
    if (m_arena -> getCellStatus(m_row, m_col) == HAS_POISON) //if there is poison on a space the vampire lands on
    {
        if (isPoisoned) //twice poisoned if it has been poisoned already
        {
            poisonedTwice = true;
        }
        else
        {
            isPoisoned = true; //poisoned once otherwise
        }
        m_arena -> setCellStatus(m_row, m_col, EMPTY); //remove the poison vial
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonVial()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned blood vial at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
    bool tryMove = attemptMove(*m_arena, dir, m_row, m_col); //check if the player can actually move
    if (m_arena -> numberOfVampiresAt(m_row, m_col) > 0)
    {
        setDead(); //declare death if player makes wrong movement choice
        return "Player walked into a vampire and died.";
    }
    
    if (tryMove == false) //dont move if the player tries to move off the grid
    {
        return "Player couldn't move; player stands.";
    }
    
    switch (dir){ //report direction of movement
        case NORTH:
            return "Player moved north.";
        case SOUTH:
            return "Player moved south.";
        case WEST:
            return "Player moved west.";
        case EAST:
            return "Player moved east.";
    }
    return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    for (int i = 0; i < MAXVAMPIRES; i++) //set all vampires initially as nullpointers
    {
        m_vampires[i] = nullptr;
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nVampires = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player;
    m_player = nullptr; //delete player and its pointer
    
    for (int i = 0; i < MAXVAMPIRES; i++)
    {
        if (m_vampires[i] == nullptr) //delete all vampires and their pointers
        {
            continue;
        }
        delete m_vampires[i];
        m_vampires[i] = nullptr;
    }
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::vampireCount() const
{
    return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
    int num = 0;
    for (int i = 0; i < MAXVAMPIRES; i++)
    {
        if (m_vampires[i] == nullptr) //skip over any nullptr vampires
        {
            continue;
        }
        if (m_vampires[i] -> row() == r && m_vampires[i] -> col() == c) //find the number of matching coordinates with grid point to be checked
        {
            num++;
        }
    }
    return num;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with dots (empty) and stars (poisoned blood vials)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
    
    // Indicate each vampire's position
    char charVals[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; //list of possible characters to display
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            int numOfVamps = numberOfVampiresAt(r, c);
            if (numOfVamps == 1) //set V for one vampire
            {
                displayGrid[r-1][c-1] = 'V';
            }
            else if (numOfVamps <= 8 && numOfVamps >= 2) //set char according to number of vampires at that grid point
            {
                displayGrid[r-1][c-1] = charVals[numOfVamps];
            }
            else if (numOfVamps >= 9) //set 9 for 9+ vampires
            {
                displayGrid[r-1][c-1] = '9';
            }
        }
    }
    
    // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');
    
    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
    
    // Write message, vampire, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << vampireCount() << " vampires remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addVampire(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a vampire on a spot with a poisoned blood vial
    if (getCellStatus(r, c) != EMPTY)
        return false;
    
    // Don't add a vampire on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    // If there are MAXVAMPIRES existing vampires, return false.  Otherwise,
    // dynamically allocate a new vampire at coordinates (r,c).  Save the
    // pointer to newly allocated vampire and return true.
    
    if (m_nVampires == MAXVAMPIRES) //don't add more if there are too many
    {
        return false;
    }
    
    m_vampires[m_nVampires] = new Vampire (this, r, c); //make a new vampire pointer
    m_nVampires++; //increment number of vampires
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Don't add a player on a spot with a vampire
    if (numberOfVampiresAt(r, c) > 0)
        return false;
    
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveVampires()
{
    for (int i = 0; i < MAXVAMPIRES; i++) //move all vampires that are not nullptr
    {
        if (m_vampires[i] == nullptr)
        {
            continue;
        }
        m_vampires[i] -> move();
    }
    
    if (numberOfVampiresAt(player() -> row(), player() -> col()) > 0) //if the vampire walks into player, the player is dead
    {
        player() -> setDead();
    }
    
    for (int i = 0; i < MAXVAMPIRES; i++)
    {
        if (m_vampires[i] == nullptr)
        {
            continue;
        }
        if (m_vampires[i] -> isDead()) //deallocate any dead vampires
        {
            delete m_vampires[i];
            m_vampires[i] = nullptr;
            m_nVampires--;
        }
    }
    
    // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
        << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
    if (nVampires < 0)
    {
        cout << "***** Cannot create Game with negative number of vampires!" << endl;
        exit(1);
    }
    if (nVampires > MAXVAMPIRES)
    {
        cout << "***** Trying to create Game with " << nVampires
        << " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
        << cols << " arena, which is too small too hold a player and "
        << nVampires << " vampires!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with vampires
    while (nVampires > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addVampire(r, c);
        nVampires--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);
        
        Player* player = m_arena->player();
        int dir;
        
        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonVial();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonVial();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->vampireCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveVampires();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
        default:  return false;
        case 'n': dir = NORTH; break;
        case 'e': dir = EAST;  break;
        case 's': dir = SOUTH; break;
        case 'w': dir = WEST;  break;
    }
    return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    if (dir == NORTH) //make sure that each possible direction of movement is in boundary
    {
        if ((r-1) < 1)
        {
            return false;
        }
        r--;
    }
    else if (dir == SOUTH)
    {
        if ((r+1) > a.rows())
        {
            return false;
        }
        r++;
    }
    else if (dir == WEST)
    {
        if ((c-1) < 1)
        {
            return false;
        }
        c--;
    }
    else if (dir == EAST)
    {
        if ((c+1) > a.cols())
        {
            return false;
        }
        c++;
    }
    return true;  // This implementation compiles, but is incorrect.
}

int checkSurroundings (const Arena& a, int centerR, int centerC, int dir)
{
    int rows = a.rows();
    int cols = a.cols();
    int numVamps = 0;
    if (dir == SOUTH) //check the southern gridpoint's surrounding grid for vampires
    {
        if (centerR <= rows - 1)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR + 1, centerC);
        }
        if (centerC <= cols - 1)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR, centerC + 1);
        }
        if (centerC >= 2)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR, centerC - 1);
        }
    }
    else if (dir == EAST) //check the eastern gridpoint's surrounding grid for vampires
    {
        if (centerC <= cols - 1)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR, centerC + 1);
        }
        if (centerR >= 2)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR - 1, centerC);
        }
        if (centerR <= rows - 1)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR + 1, centerC);
        }
    }
    else if (dir == WEST) //check the western gridpoint's surrounding grid for vampires
    {
        if (centerC >= 2)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR, centerC - 1);
        }
        if (centerR >= 2)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR - 1, centerC);
        }
        if (centerR <= rows - 1)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR + 1, centerC);
        }
    }
    else if (dir == NORTH) //check the northern gridpoint's surrounding grid for vampires
    {
        if (centerR >= 2)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR - 1, centerC);
        }
        if (centerC >= 2)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR, centerC - 1);
        }
        if (centerC <= cols - 1)
        {
            numVamps = numVamps + a.numberOfVampiresAt(centerR, centerC + 1);
        }
    }
    return numVamps;
}
// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poisoned blood vial and
// not move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    int miniMap[20][20];
    int safeSpots = 0;
    int noAccess = 0;
    int rows = a.rows();
    int cols = a.cols();
    for (int i = 0; i < 20; i++) //fill all grid points as "high risk" points to move to
    {
        for (int j = 0; j < 20; j++)
        {
            miniMap[i][j] = 4000;
        }
    }
    miniMap[r-1][c-1] = 0; //set player's position to lowest risk
    if (r <= rows - 1 && a.numberOfVampiresAt(r+1, c) > 0) //set any immediately adjacent gridpoints with vampires to high risk (1000)
    {
        miniMap[r][c-1] = 1000;
        miniMap[r-1][c-1] += a.numberOfVampiresAt(r+1, c); //increment risk of staying in place
    }
    if (c <= cols - 1 && a.numberOfVampiresAt(r, c+1) > 0)
    {
        miniMap[r-1][c] = 1000;
        miniMap[r-1][c-1] += a.numberOfVampiresAt(r, c+1);
    }
    if (c >= 2 && a.numberOfVampiresAt(r, c-1) > 0)
    {
        miniMap[r-1][c-2] = 1000;
        miniMap[r-1][c-1] += a.numberOfVampiresAt(r, c-1);
    }
    if (r >= 2 && a.numberOfVampiresAt(r-1, c) > 0)
    {
        miniMap[r-2][c-1] = 1000;
        miniMap[r-1][c-1] += a.numberOfVampiresAt(r-1, c);
    }
    if (r <= rows - 1 && miniMap[r][c-1] != 1000) //increment risk for each surrounding grid point to check for vampires IF there is not already a vampire there
    {
        miniMap[r][c-1] = checkSurroundings(a, r+1, c, SOUTH);
        if (miniMap[r][c-1] == 0)
        {
            safeSpots++;
        }
    }
    else if (r <= rows - 1 && miniMap[r][c-1] == 1000)
    {
        safeSpots = safeSpots - 1000; //tell the program it is unreasonable for the player to stay put
    }
    else
    {
        noAccess++; //tell the program that the player can't move in that direction in the first place
    }
    if (c <= cols - 1 && miniMap[r-1][c] != 1000)
    {
        miniMap[r-1][c] = checkSurroundings(a, r, c+1, EAST);
        if (miniMap[r-1][c] == 0)
        {
            safeSpots++; //Increment the surrounding safe spots
        }
    }
    else if (c <= cols - 1 && miniMap[r-1][c] == 1000)
    {
        safeSpots = safeSpots - 1000;
    }
    else
    {
        noAccess++;
    }
    if (c >= 2 && miniMap[r-1][c-2] != 1000)
    {
        miniMap[r-1][c-2] = checkSurroundings(a, r, c-1, WEST);
        if (miniMap[r-1][c-2] == 0)
        {
            safeSpots++;
        }
    }
    else if (c >= 2 && miniMap[r-1][c-2] == 1000)
    {
        safeSpots = safeSpots - 1000;
    }
    else
    {
        noAccess++;
    }
    if (r >= 2 && miniMap[r-2][c-1] != 1000)
    {
        miniMap[r-2][c-1] = checkSurroundings(a, r-1, c, NORTH);
        if (miniMap[r-2][c-1] == 0)
        {
            safeSpots++;
        }
    }
    else if (r >= 2 && miniMap[r-2][c-1] == 1000)
    {
        safeSpots = safeSpots - 1000;
    }
    else
    {
        noAccess++;
    }
    if (noAccess + safeSpots == 4)
    {
        return false;
    }
    
    int min = 2000;
    int minI = 0;
    int minJ = 0;
    for (int i = 0; i < rows; i++) //find minimum risk in the grid
    {
        for (int j = 0; j < cols; j++)
        {
            if (miniMap[i][j] < min)
            {
                min = miniMap[i][j];
                minI = i;
                minJ = j;
            }
        }
    }
    
    if (minI == r && minJ == c-1) //determine direction to move to get to the minimum risk square
    {
        bestDir = SOUTH;
        return true;
    }
    if (minI == r-1 && minJ == c)
    {
        bestDir = EAST;
        return true;
    }
    if (minI == r-1 && minJ == c-2)
    {
        bestDir = WEST;
        return true;
    }
    if (minI == r-2 && minJ == c-1)
    {
        bestDir = NORTH;
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);
    //Game g(3, 5, 2);
    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
