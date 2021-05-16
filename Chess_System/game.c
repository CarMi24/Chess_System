#include "game.h"
#include <stdbool.h>
#include <stdlib.h>

struct Game_t
{
    int first_player;
    int second_player;
    Winner winner;
    int play_time;
    int tournament_id;
};

/** Type for defining the Game */
typedef struct Game_t *Game;

static void editGameWinner(Winner winner);

Game createGame(int tournament_id, int first_player, int second_player,
                Winner winner, int play_time, GameResult game_result)
{
    Game game = malloc(sizeof(*game));
    if (game == NULL)
    {
        game_result = GAME_OUT_OF_MEMORY;
        return NULL;
    }
    game->tournament_id = tournament_id;
    game->first_player = first_player;
    game->second_player = second_player;
    game->winner = winner;
    game->play_time = play_time;
}

void destroyGame(Game game);

bool compareGames(Game game1, Game game2);

/**
 * Check and return if a player plays in a given game.
 */
bool isPlayerInGame(Game game, int player_id);

void removePlayer(Game Game, int index_player);