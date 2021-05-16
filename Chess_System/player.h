#ifndef PLAYER_H
#define PLAYER_H


#include "map.h"
#include "game.h"
#include "tournament.h"

/** Type for defining the PlayerResult*/
typedef enum PlayerResult_t
{
    PLAYER_OUT_OF_MEMORY,
    PLAYER_NULL_ARGUMENT,
    PLAYER_ERROR,
    PLAYER_SUCCESS
}PlayerResult;

/** Type for defining the Player*/
typedef struct Player_t 
{
    int player_id;
    int wins;
    int losses;
    int draws;
    double level;
    int total_play_time;
    Map tournament_rates;

}*Player;

typedef struct TournamentStats_t
{
    int wins;
    int losses;
    int draws;
} *TournamentStats;

/** Functions for the tournaments map**/

/**
 * TournamentKey is type of integer pointer - tournament_id.
 * TournamentData is type of TournamentStats - the stats of the player in the tournament.
*/
static MapKeyElement copyTournamentKey(MapKeyElement tournament_key);
static MapDataElement copyTournamentData(MapDataElement tournament_data);
static void freeTournamentKey(MapKeyElement tournament_key);
static void freeTournamentData(MapDataElement tournament_data);



Player createPlayer(int player_id);


void destroyPlayer(Player player);

/**
 * Calculate and returns the average play time in all the player's games.
 */
double calculateAveragePlayTime(Player player);

/**
 * Adds a new tournament and the rate of the player in that tournament.
 * if the tournaments already exists in player tounament map - update the rate.
 */
double editTournamentRate(Player player, int tournament_id, int tournament_rate);

/**
 * Removes a tournament from the players tournaments maps.
 * Updates the player's stats (wins, losses, draws).
 *  @return -
 * PLAYER_NULL_ARGUMENT
 */
PlayerResult removeTournament(Player player, int tournament_id);

/**
 * returns the players tournaments map.
 */
Map getPlayerTournamentsMap(Player player);

/**
 * Calculate and returns the player general level.
 */
double calculatePlayersLevel(Player player, int num_of_games);




#endif