vector<void*> players;
int MAX_PLAYERS = 8;

void clearPlayers() {
    vector<void*> pls;
    for (int i = 0; i < players.size(); i++) {
        if (players[i] != NULL) {
            pls.push_back(players[i]);
        }
    }
    players = pls;
}

bool playerFind(void *pl) {
    if (pl != NULL) {
        for (int i = 0; i < players.size(); i++) {
            if (pl == players[i]) return true;
        }
    }
    return false;
}

void *myPlayer = NULL;
void (*old_Player_update)(...);
void Player_update(void *player) {
    if (player != NULL) {
        bool isMine = *(bool *) ((uint64_t) player + 0x5D);
        if (isMine) {
            myPlayer = player;
        }
		
        if (!playerFind(player)) players.push_back(player);
        if (players.size() > MAX_PLAYERS) {
            players.clear();
        }
    }
    clearPlayers();
    old_Player_update(player);
}
