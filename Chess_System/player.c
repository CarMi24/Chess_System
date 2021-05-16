#include "player.h"
#include <stdlib.h>

struct Player_t
{
    int player_id;
    int total_wins;
    int total_losses;
    int total_draws;
    double level;
    int total_play_time;
    Map tournament_rates; // KeyElement = tournament_id, DataElement = TournamentStats (struct)
};

static MapKeyElement copyKeyInt(MapKeyElement n)
{
    if (n == NULL)
    {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(int *)n;
    return copy;
}

static MapDataElement coptDataTournamentStats(MapDataElement tournament_stats)
{
    if (tournament_stats == NULL)
    {
        return NULL;
    }
    TournamentStats copy = malloc(sizeof(*copy));
    if (copy == NULL)
    {
        return NULL;
    }
    *copy = *(TournamentStats)tournament_stats;
    return copy;
}

static void freeKeyInt(MapKeyElement n)
{
    free(n);
}

static void freeDataTournamentStats(MapDataElement tournament_stats)
{
    free(tournament_stats);
}

static int compareInts(MapKeyElement n1, MapKeyElement n2)
{
    return (*(int *)n1 - *(int *)n2);
}

Player createPlayer(int player_id)
{
    Player player = malloc(sizeof(*player));
    player->player_id = player_id;
    player->total_wins = 0;
    player->total_losses = 0;
    player->total_draws = 0;
    player->total_play_time = 0;
    player->total_draws = 0;
    Map tournament_rates = mapCreate(copyKeyInt, coptDataTournamentStats, freeKeyInt, freeDataTournamentStats, compareInts);
    if (tournament_rates == NULL) // if map allocation failed or recieved null values
    {
        free(player);
        return NULL;
    }
    player->tournament_rates = tournament_rates;
}

void destroyPlayer(Player player)
{
    if (player == NULL)
    {
        return NULL;
    }
    mapDestroy(player->tournament_rates);
    free(player);
}

double calculateAveragePlayTime(Player player)
{
    return ((player->total_play_time).0 / (player->total_wins + player->total_losses + player->total_draws).0);
}
double editTournamentRate(Player player, int tournament_id, int tournament_rate);

PlayerResult removeTournament(Player player, int tournament_id)
{
    mapRemove(player->tournament_rates, tournament_id);
}

Map getPlayerTournamentsMap(Player player)
{
    return player->tournament_rates;
}

double calculatePlayersLevel(Player player, int num_of_games)
{
    return ((((6 * player->total_wins) - (10 * player->total_losses) + (2 * player->total_draws)).0) / num_of_games .0)
}
