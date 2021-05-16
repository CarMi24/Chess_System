#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "map.h";
#include "game.h";

#define NULL_ID -1

/** Type for defining the Tournament */
typedef enum TournamentResult_t
{
    TOURNAMENT_OUT_OF_MEMORY,
    TOURNAMENT_NULL_ARGUMENT,
    TOURNAMENT_ERROR
    
}TournamentResult;

typedef struct Tournament_t
{
    int tournament_id;
    char* tournament_location;
    Map tournament_games;
    int max_games_per_player;
    int tournament_winner_id;
    int winner_id;
    int longest_game_time;
    bool is_closed;

} *Tournament;


/** Functions for the games map**/

/**
 * GameKey is type of integer pointer - the number of the game in the tournament.
 * GameData is type of Game - games that are taken place in the tournament.
 * The key will be the tournament_id where the game takes place.
*/
static MapKeyElement copyGameKey(MapKeyElement game_key);
static MapDataElement copyGameData(MapDataElement game);
static void freeGameKey(MapKeyElement game_key);
static void freeGameData(MapDataElement game);
static int compareKeyGame(MapKeyElement game1, MapKeyElement game2);


/**
 * iterates the games map and calculate each player score in the tournament.
 * at the end the winner's id will be updated in the tournament.
 */
static void calculateTournamentWinner(Tournament tournament);


/**
 * updates the longest game time if new_time is bigger.
 * otherwise does nothing. 
 */
static void updateLongestGameTime(int new_time);



/**
* createTournament: Allocates a new empty tournament.
* @return
* 	NULL - if allocations failed.
* 	A new Tournament in case of success.
*/
Tournament createTournament(int tournament_id, char* location, int max_games);


void destroyTournament(Tournament tournament);

/**
 * Check if the game already taken place in the tournament
 */
bool checkIfGameExists(Tournament tournament, Game game);

/**
 * Adds a game to the games map.
 */
TournamentResult addGameToTournament(Tournament tournament, Game game);

/**
 * Returns the Tournament id
 * If recieve NULL parameter return -1.
 */
int getTournamentID(Tournament tournament);

/**
 * Iterates the games map and remove the player from each game he plays in.
 * @return -
 * TOURNAMENT_NULL_ARGUMENT
 * TOURNAMENT_ERROR
 */
TournamentResult removePlayerFromTournament(Tournament tournament, int player_id);

/**
 * Closes the tournament - Set the tournament is_closed value to true.
 * @return -
 * TOURNAMENT_NULL_ARGUMENT
 */
TournamentResult endTournament(Tournament tournament);


int getTournamentLongestGameTime(Tournament tournament);

double calculateAverageTournamentGameTime(Tournament tournament);

bool getTournamentStatus(Tournament Tournament);

/**
 * prints the tournaments statistics to the a file located in the given path_file.
 * @return -
 * TOURNAMENT_NULL_ARGUMENT
 * TOURNAMENT_ERROR - if tournaments closed
 */
TournamentResult printTournamentStats(Tournament tournament,char* path_file);

/**
 *
 */
void endTournament(Tournament tournament);

#endif