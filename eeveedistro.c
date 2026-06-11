#include <gb/gb.h>
#include <gb/hardware.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <rand.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "gen1.h"

#define PARTY_SIZE 6
#define POKE_SIZE 44
#define NAME_LEN 11
#define SELP_LEN 8
#define POKE_LEN POKE_SIZE * PARTY_SIZE
#define ORIG_LEN NAME_LEN * PARTY_SIZE
#define NICK_LEN NAME_LEN * PARTY_SIZE
#define FULL_LEN NAME_LEN + SELP_LEN + (POKE_SIZE * PARTY_SIZE) + (NAME_LEN * PARTY_SIZE) + (NAME_LEN * PARTY_SIZE)

#define SER_REG_DIR (*(uint8_t *)0xFF01)
#define SER_OPT_REG_DIR (*(uint8_t *)0xFF02)

// Eevee image.
const unsigned char mew_tiles[] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xe3, 0xe3, 0xe5, 0xed, 0xee, 0xe6, 0xe2, 0xe7, 0xe4, 0xe3, 0xf0, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0xbf, 0x5f, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xfc, 0xff, 0xfb, 0xff, 0xf7, 0xff, 0xef, 0xef, 0xef, 0xff,
        0xdf, 0xfa, 0xf1, 0xb1, 0x18, 0xb8, 0xb8, 0xf9, 0x78, 0x79, 0x78, 0x78, 0x78, 0x7c, 0x7a, 0x7d, 0x76, 0xdf, 0x00,
        0x2c, 0x1f, 0x90, 0x7f, 0xc0, 0x7f, 0xc1, 0xbf, 0x03, 0x3e, 0x01, 0x3e, 0x41, 0x3e, 0xf2, 0x61, 0x40, 0x9f, 0x10,
        0xef, 0xe8, 0xf7, 0xe0, 0x1f, 0x95, 0x8a, 0x14, 0x0a, 0x16, 0x08, 0xf9, 0xf9, 0x63, 0xf9, 0x13, 0xe3, 0x47, 0x83,
        0x8f, 0x07, 0x1f, 0x0f, 0x3f, 0x1f, 0xff, 0x7f, 0xdf, 0xdf, 0xfe, 0xbf, 0xb5, 0xbf, 0xaa, 0xbf, 0x94, 0xbf, 0x80,
        0xbf, 0x80, 0xbf, 0x95, 0xaa, 0x76, 0x77, 0xf6, 0x77, 0x37, 0xa3, 0x23, 0x81, 0x44, 0x82, 0x68, 0x87, 0xe6, 0x0f,
        0xff, 0x07, 0xe8, 0x27, 0x00, 0xff, 0xa2, 0xc1, 0xf8, 0xe3, 0xf0, 0x77, 0x40, 0x80, 0x00, 0xff, 0x7c, 0xff, 0xc5,
        0x19, 0x0d, 0xf1, 0x0b, 0xf3, 0x13, 0xe1, 0x41, 0x8d, 0xc1, 0x3e, 0x38, 0x7e, 0x7c, 0xfe, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xea, 0x95, 0xd5, 0xca, 0xeb, 0xd4, 0xe7,
        0xe8, 0xff, 0xe0, 0xf7, 0xf0, 0xfd, 0xf8, 0xff, 0xfe, 0xf2, 0x07, 0xec, 0x03, 0xde, 0x00, 0xee, 0x10, 0xc2, 0x3c,
        0xa0, 0x3e, 0xe1, 0x3e, 0x10, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xbf, 0x7f, 0x5f, 0x3f, 0x40, 0x80, 0x00, 0xfc, 0x60,
        0xfc, 0x72, 0x7c, 0x5d, 0xbe, 0xab, 0xdd, 0xc1, 0xcd, 0xb7, 0xc3, 0x37, 0x07, 0x6f, 0x0f, 0x6f, 0x0f, 0xef, 0x0f,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xdf, 0xf4, 0xe7, 0xfc, 0xf8,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7e, 0x38, 0xbe, 0x18, 0x1f, 0xc8, 0x8f, 0xf8,
        0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0x1f, 0xdf, 0x1f, 0xbf, 0x3f, 0x7f, 0x7f, 0xff, 0x7f, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff
};

// It has +128 since we are loading Mew sprites starting at position 128
// to prevent collisions with the font sprites.
const unsigned char mew_map[] = {
        0x94, 0x94, 0x94, 0x8b, 0x90, 0x94, 0x83, 0x87, 0x8c, 0x91, 0x80, 0x84, 0x88, 0x8d, 0x92, 0x81, 0x85, 0x89, 0x8e,
        0x94, 0x82, 0x86, 0x8a, 0x8f, 0x93
};

enum connection_state_t connection_state = NOT_CONNECTED;
enum trade_state_t trade_state = INIT;
//uint8_t INPUT_BLOCK[PARTY_DATA_SIZE];
uint8_t DATA_BLOCK[PARTY_DATA_SIZE];
//uint8_t PATCH_INPUT_BLOCK[PATCH_SIZE];
uint8_t PATCH_BLOCK[PATCH_SIZE];
uint8_t scheduled_refill = TRUE;
unsigned char name[11] = {
        // OT Name
        pokechar_V,
        pokechar_I,
        pokechar_W,
        pokechar_A,
        pokechar_L,
        pokechar_L,
        pokechar_S,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE
};
unsigned char nicknames[11] = {
        // Pokemon Nickname
        pokechar_E,
        pokechar_E,
        pokechar_V,
        pokechar_E,
        pokechar_E,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE,
        pokechar_STOP_BYTE
};

typedef struct TraderPacket {
    // Name must not exceed 10 characters + 1 STOP_BYTE
    // Any leftover space must be filled with STOP_BYTE
    unsigned char name[11];
    struct SelectedPokemon selected_pokemon;
    struct PartyMember pokemon[6];
    unsigned char original_trainer_names[6][11];
    unsigned char pokemon_nicknames[6][11];
} TraderPacket;

void party_member_to_bytes(struct PartyMember *pPartyMember, uint8_t *out) {
    uint8_t res[POKE_SIZE] = {
            pPartyMember->pokemon,
            (uint8_t)(pPartyMember->current_hp >> 8),
            (uint8_t)(pPartyMember->current_hp & 0x00FF),
            pPartyMember->level,
            pPartyMember->status,
            pPartyMember->type1,
            pPartyMember->type2,
            pPartyMember->catch_rate_or_held_item,
            pPartyMember->move1,
            pPartyMember->move2,
            pPartyMember->move3,
            pPartyMember->move4,
            (uint8_t)(pPartyMember->original_trainer_id >> 8),
            (uint8_t)(pPartyMember->original_trainer_id & 0x00FF),
            (uint8_t)((pPartyMember->experience & 0x00FF0000) >> 16),
            (uint8_t)((pPartyMember->experience & 0x0000FF00) >> 8),
            (uint8_t)(pPartyMember->experience & 0x000000FF),
            (uint8_t)(pPartyMember->HP_exp >> 8),
            (uint8_t)(pPartyMember->HP_exp & 0x00FF),
            (uint8_t)(pPartyMember->attack_exp >> 8),
            (uint8_t)(pPartyMember->attack_exp & 0x00FF),
            (uint8_t)(pPartyMember->defense_exp >> 8),
            (uint8_t)(pPartyMember->defense_exp & 0x00FF),
            (uint8_t)(pPartyMember->speed_exp >> 8),
            (uint8_t)(pPartyMember->speed_exp & 0x00FF),
            (uint8_t)(pPartyMember->special_exp >> 8),
            (uint8_t)(pPartyMember->special_exp & 0x00FF),
            (uint8_t)(((pPartyMember->attack_dv & 0xF) << 4) | (pPartyMember->defense_dv & 0xF)),
            (uint8_t)(((pPartyMember->speed_dv & 0xF) << 4) | (pPartyMember->special_dv & 0xF)),
            pPartyMember->move1_pp,
            pPartyMember->move2_pp,
            pPartyMember->move3_pp,
            pPartyMember->move4_pp,
            pPartyMember->level,
            (uint8_t)(pPartyMember->max_hp >> 8),
            (uint8_t)(pPartyMember->max_hp & 0x00FF),
            (uint8_t)(pPartyMember->attack >> 8),
            (uint8_t)(pPartyMember->attack & 0x00FF),
            (uint8_t)(pPartyMember->defense >> 8),
            (uint8_t)(pPartyMember->defense & 0x00FF),
            (uint8_t)(pPartyMember->speed >> 8),
            (uint8_t)(pPartyMember->speed & 0x00FF),
            (uint8_t)(pPartyMember->special >> 8),
            (uint8_t)(pPartyMember->special & 0x00FF),
    };
    for (size_t i = 0; i < POKE_SIZE; i++) {
        out[i] = res[i];
    }
}

void selected_pokemon_to_bytes(struct SelectedPokemon *pSelectedPokemon, uint8_t *out) {
    uint8_t res[SELP_LEN] = {
            pSelectedPokemon->number,
            pSelectedPokemon->pokemon[0],
            pSelectedPokemon->pokemon[1],
            pSelectedPokemon->pokemon[2],
            pSelectedPokemon->pokemon[3],
            pSelectedPokemon->pokemon[4],
            pSelectedPokemon->pokemon[5],
            0xFF,
    };
    for (size_t i = 0; i < SELP_LEN; i++) {
        out[i] = res[i];
    }
}

void trader_packet_to_bytes(struct TraderPacket *pTraderPacket) {
    // Reinitialize to 0.
    memset(DATA_BLOCK, 0, sizeof(DATA_BLOCK));
    memset(PATCH_BLOCK, 0, sizeof(PATCH_BLOCK));

    uint8_t name_bytes[NAME_LEN];
    uint8_t selected_pokemon_bytes[SELP_LEN];
    uint8_t pokemon_bytes[POKE_LEN];
    uint8_t original_trainer_names_bytes[ORIG_LEN];
    uint8_t pokemon_nicknames_bytes[NICK_LEN];

    // Serialize the data
    for (size_t i = 0; i < NAME_LEN; i++) {
        name_bytes[i] = (uint8_t)pTraderPacket->name[i];
    }

    // Calculate the selected_pokemon and pokemon w/ stats and such
    for (size_t i = 0; i < PARTY_SIZE; i++) {
        uint8_t poke[POKE_SIZE];
        party_member_to_bytes(&pTraderPacket->pokemon[i], poke);

        // Full Party Data (all stats and such)
        for (size_t j = 0; j < POKE_SIZE; j++) {
            pokemon_bytes[(i * POKE_SIZE) + j] = (uint8_t)poke[j];
        }
    }
    selected_pokemon_to_bytes(&pTraderPacket->selected_pokemon, selected_pokemon_bytes);

    for (size_t i = 0; i < PARTY_SIZE; i++) {
        for (size_t j = 0; j < NAME_LEN; j++) {
            original_trainer_names_bytes[(i * NAME_LEN) + j] = (uint8_t)pTraderPacket->original_trainer_names[i][j];
        }
    }

    for (size_t i = 0; i < PARTY_SIZE; i++) {
        for (size_t j = 0; j < NAME_LEN; j++) {
            pokemon_nicknames_bytes[(i * NAME_LEN) + j] = (uint8_t)pTraderPacket->pokemon_nicknames[i][j];
        }
    }

    // Flatten the data
    uint8_t res[FULL_LEN + 3];

    for (size_t i = 0; i < NAME_LEN; i++) {
        res[i] = name_bytes[i];
    }
    for (size_t i = 0; i < SELP_LEN; i++) {
        res[i + NAME_LEN] = selected_pokemon_bytes[i];
    }
    for (size_t i = 0; i < POKE_LEN; i++) {
        res[i + NAME_LEN + SELP_LEN] = pokemon_bytes[i];
    }
    for (size_t i = 0; i < ORIG_LEN; i++) {
        res[i + NAME_LEN + SELP_LEN + POKE_LEN] = original_trainer_names_bytes[i];
    }
    for (size_t i = 0; i < NICK_LEN; i++) {
        res[i + NAME_LEN + SELP_LEN + POKE_LEN + ORIG_LEN] = pokemon_nicknames_bytes[i];
    }
    res[FULL_LEN] = 0x00;
    res[FULL_LEN + 1] = 0x00;
    res[FULL_LEN + 2] = 0x00;

    // Write the bytes to *out.
    uint8_t tmp_patch_list_1[100];
    uint8_t tmp_patch_list_2[100];
    size_t tmp_list_1_counter = 0;
    size_t tmp_list_2_counter = 0;
    for (size_t i = 0; i < PARTY_DATA_SIZE; i++) {
        DATA_BLOCK[i] = res[i];
        // Replace 0xFE with 0xFF (they will later be replaced again by the receiving game using the data we send in
        // the patch section).
        if (i >= 19 && DATA_BLOCK[i] == 0xFE) {
            DATA_BLOCK[i] = 0xFF;
            if (i < 0xFC) {
                tmp_patch_list_1[tmp_list_1_counter++] = i - 19 + 1;
            } else {
                tmp_patch_list_2[tmp_list_2_counter++] = i - 0xFC - 19 + 1;
            }
        }
    }
    tmp_patch_list_1[tmp_list_1_counter++] = 0xFF;
    tmp_patch_list_2[tmp_list_2_counter++] = 0xFF;

    size_t tmp_list_1_set_counter = 0;
    size_t tmp_list_2_set_counter = 0;
    // Now fill patch data.
    for (size_t i = PATCH_DATA_START_POS; i < PATCH_SIZE; i++) {
        if (tmp_list_1_counter > 0) {
            PATCH_BLOCK[i] = tmp_patch_list_1[tmp_list_1_set_counter++];
            tmp_list_1_counter--;
        } else if (tmp_list_2_counter > 0) {
            PATCH_BLOCK[i] = tmp_patch_list_2[tmp_list_2_set_counter++];
            tmp_list_2_counter--;
        }

        if (tmp_list_1_counter == 0 && tmp_list_2_counter == 0) {
            break;
        }
    }
}

// get a seed to be used for random generation by xoring values from ram which are pseudorandom on startup.
uint16_t get_ram_seed(void) {
    uint16_t *p = (uint16_t*)0xC000;
    uint16_t sum = 0;
    for (uint16_t i = 0; i < 0x1FFF; i++) {
        sum ^= p[i];
    }
    return sum;
}

void fill_pokemon_team(void) {
    /**
     * Trader Packet Init
     */
    struct TraderPacket traderPacket;
    for (size_t i = 0; i < 11; i++) {
        traderPacket.name[i] = name[i];
    }

    struct SelectedPokemon *pSelectedPokemon = &traderPacket.selected_pokemon;
    pSelectedPokemon->number = 6;
    for (size_t i = 0; i < 6; i++) {
        pSelectedPokemon->pokemon[i] = MEW;
    }

    for (size_t i = 0; i < 6; i++) {
        struct PartyMember *pPartyMember = &traderPacket.pokemon[i];
       
        pPartyMember->pokemon = EEVEE;
        pPartyMember->current_hp = 22;
        pPartyMember->max_hp = 22;
        pPartyMember->level = 5;
        pPartyMember->status = NONE;
        pPartyMember->type1 = NORMAL_TYPE;
        pPartyMember->type2 = NORMAL_TYPE;
        pPartyMember->catch_rate_or_held_item = 45;
        pPartyMember->move1 = TACKLE;
        pPartyMember->move2 = TAIL_WHIP;
        pPartyMember->move3 = GROWTH;
        pPartyMember->move4 = 0x0;
        pPartyMember->original_trainer_id = (randw() % 65535) + 1;

        pPartyMember->experience = 125;

        pPartyMember->HP_exp = 0;
        pPartyMember->attack_exp = 0;
        pPartyMember->defense_exp = 0;
        pPartyMember->speed_exp = 0;
        pPartyMember->special_exp = 0;

        pPartyMember->attack_dv = 15;
        pPartyMember->defense_dv = 15;
        pPartyMember->speed_dv = 15;
        pPartyMember->special_Dv = 15;

        pPartyMember->move1_pp = 35;
        pPartyMember->move2_pp = 30;
        pPartyMember->move3_pp = 40;
        pPartyMember->move4_pp = 0;

        pPartyMember->attack = 12;
        pPartyMember->defense = 11;
        pPartyMember->speed = 12;
        pPartyMember->special = 13;
    }

    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 11; j++) {
            traderPacket.original_trainer_names[i][j] = name[j];
        }
    }
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 11; j++) {
            traderPacket.pokemon_nicknames[i][j] = nicknames[j];
        }
    }

    trader_packet_to_bytes(&traderPacket);
}

uint8_t handle_byte(uint8_t in, size_t *counter, clock_t *last_action) {
    // Simple protocol explanation.
    // Steps:
    //    First synchronization
    //    0xFD until the other gameboy is ready and answers 0xFD
    //    Random seed data (Size: 10)
    //    0xFD until the other gameboy is ready and answers 0xFD
    //    Party data (Size: 418 for Gen 1, 444 for Gen 2)
    //    0xFD until the other gameboy is ready and answers 0xFD
    //    Patch set (Size: 197)
    //    End
    // Note: Gen 2 also has mail data at the end (Size: 385).
    static uint8_t out;

    // If the cable is disconnected or the console is not ready for more than 5 seconds, reset trading process.
    // This method will only work if the distribution cartridge is acting as master.
    clock_t current_time = clock();
    if (connection_state != NOT_CONNECTED && (in == LINK_CABLE_EMPTY || in == LINK_CABLE_DISCONNECT)) {
        if (current_time - (*last_action) > 300) {
            connection_state = NOT_CONNECTED;
            trade_state = INIT;
            SC_REG = SIOF_CLOCK_INT;
            out = PKMN_MASTER;
            return out;
        }
    } else {
        (*last_action) = current_time;
    }

    switch (connection_state) {
        case NOT_CONNECTED:
            switch (in) {
                // Note: If master, always reply with PKMN_MASTER to 0x01 and 0x02. If slave, always reply with
                // PKMN_SLAVE.
                case PKMN_MASTER:
                    out = PKMN_MASTER;
                    break;
                case PKMN_SLAVE:
                    out = PKMN_MASTER;
                    break;
                case PKMN_BLANK:
                    out = PKMN_BLANK;
                    break;
                case PKMN_BREAK_LINK:
                    out = PKMN_BREAK_LINK;
                    break;
                case PKMN_CONNECTED_TIME_CAPSULE:
                case PKMN_CONNECTED:
                    connection_state = CONNECTED;
                    out = PKMN_CONNECTED;
                    break;

                default:
                    // By default, answer constantly with PKMN_MASTER (or PKMN_SLAVE if acting as slave).
                    out = PKMN_MASTER;
                    break;
            }
            break;

        case CONNECTED:
            switch (in) {
                case PKMN_CONNECTED_TIME_CAPSULE:
                    out = PKMN_TIME_CAPSULE_SELECT;
                    break;
                case PKMN_CONNECTED:
                    out = PKMN_CONNECTED;
                    break;
                case PKMN_TRADE_CENTRE:
                    connection_state = TRADE_CENTRE;
                    out = PKMN_TRADE_CENTRE;
                    break;
                case PKMN_COLOSSEUM:
                    // Not handled, everything will just be mirrored.
                    connection_state = COLOSSEUM;
                    out = PKMN_COLOSSEUM;
                    break;
                case PKMN_BREAK_LINK:
                    connection_state = NOT_CONNECTED;
                    out = PKMN_BREAK_LINK;
                    break;
                //case PKMN_MASTER:
                //    // Reset connection; something went wrong in the last trade (console reset, etc) and we need to
                //    // start again. (Should only happen if distribution cart is in Slave mode).
                //    connection_state = NOT_CONNECTED;
                //    trade_state = INIT;
                //    SC_REG = SIOF_CLOCK_EXT;
                //    out = PKMN_SLAVE;
                //    break;

                default:
                    out = in;
                    break;
            }
            break;

        case TRADE_CENTRE:
            if (trade_state == INIT && in == 0x00) {
                (*counter) = 0;
                trade_state = READY;
                out = 0x00;
            } else if (trade_state == READY && in == TRADE_CENTRE_READY) {
                trade_state = DETECTED;
                out = TRADE_CENTRE_READY;
            } else if (trade_state == DETECTED && in != TRADE_CENTRE_READY) {
                // Here random data seed is sent... Just ignore, we don't need it.
                out = in;
                trade_state = DATA_TX_RANDOM;
            } else if (trade_state == DATA_TX_RANDOM && in == TRADE_CENTRE_READY) {
                trade_state = DATA_TX_WAIT;
                out = TRADE_CENTRE_READY;
            } else if (trade_state == DATA_TX_WAIT && in == TRADE_CENTRE_READY) {
                out = 0x00;
            } else if (trade_state == DATA_TX_WAIT && in != TRADE_CENTRE_READY) {
                (*counter) = 0;
                // send first byte
                out = DATA_BLOCK[(*counter)];
                //INPUT_BLOCK[(*counter)] = in;
                trade_state = DATA_TX;
                (*counter)++;
            } else if (trade_state == DATA_TX) {
                out = DATA_BLOCK[(*counter)];
                //INPUT_BLOCK[(*counter)] = in;
                (*counter)++;
                if ((*counter) == PARTY_DATA_SIZE) {
                    trade_state = DATA_TX_PATCH;
                }
            } else if (trade_state == DATA_TX_PATCH && in == TRADE_CENTRE_READY) {
                (*counter) = 0;
                out = TRADE_CENTRE_READY;
            } else if (trade_state == DATA_TX_PATCH && in != TRADE_CENTRE_READY) {
                // Patch data explanation:
                // 0xFE is a value that cannot be sent, or it will be interpreted as no byte being ready by the
                // receiving console. So, if one byte should be 0xFE, it's converted to 0xFF, and its position is added
                // to the patch set lists (they are two, each is FF-terminated). The receiving end converts the
                // positions in the patch set data back to 0xFE.
                // First 6 bytes are always 0x00, then the 2 list follow (if lists are empty that means 2 0xff at
                // positions 7 and 8).
                // A patch set list can only go up to 0xFC position, so that's why 2 lists are needed, you need to
                // subtract 252 (0xFC) from the second one.
                // Both lists can cover from their starting position to that + 0xFC - 1 included, and their size isn't
                // fixed.
                // They start at index 19 (1-based) after "names_size + MON_INDEX_SIZE + 1". Example if we have a 0xFE
                // byte at position 46 and another one at position 284:
                // 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x1C 0xFF 0x0E 0xFF 0x00 ...
                // 0x1C being 46-19+1 and 0x0E being 284-252-19+1. We add +1 since the index is 1-based.
                out = PATCH_BLOCK[(*counter)];
                //PATCH_INPUT_BLOCK[(*counter)] = in;
                (*counter)++;
                if ((*counter) == PATCH_SIZE) {
                    trade_state = TRADE_WAIT;
                }
            } else if (trade_state == TRADE_WAIT && (in & 0x60) == 0x60) {
                if (in == 0x6f) {
                    trade_state = READY;
                    out = 0x6f;
                } else {
                    // Select same Pokémon slot as the other person.
                    out = in;
                    // out = 0x60;  // First Pokémon slot.
                }
            } else if (trade_state == TRADE_WAIT && in == 0x00) {
                out = 0;
                trade_state = TRADE_DONE;
            } else if (trade_state == TRADE_DONE && (in & 0x60) == 0x60) {
                out = in;
                if (in == 0x61) {
                    trade_state = TRADE_WAIT;
                } else {
                    trade_state = DONE;
                }
            } else if (trade_state == DONE && in == 0x00) {
                out = 0;
                trade_state = INIT;
                // Trade finished, no more data will be sent at this moment, it's safe to refill the Pokémon group
                // in order to regenerate TIDs. If the TID is fixed, this line can be commented out.
                scheduled_refill = TRUE;
            } else {
                out = in;
            }
            break;

        default:
            out = in;
            break;
    }

    return out;
}

uint8_t sio_exchange_master(uint8_t b) {
    SB_REG = b;
    SC_REG = SIOF_XFER_START | SIOF_CLOCK_INT;
    while (SC_REG & SIOF_XFER_START);
    return SB_REG;
}

uint8_t sio_exchange_slave(uint8_t b) {
    SB_REG = b;
    SC_REG = SIOF_XFER_START | SIOF_CLOCK_EXT;
    while (SC_REG & SIOF_XFER_START);
    return SB_REG;
}

void main(void) {
    // Read from RAM to generate the seed (from 0xC000 to 0xDFFF) for later pseudo-random TID generation.
    initrand(get_ram_seed());

    font_init();
    font_set(font_load(font_spect));

    puts("\n EEVEE DISTRIBUTION");
    puts("    OT/ VIWALLS\n\n\n\n\n\n\n\n\n\n\n\n\n");
    puts("    2026 Grender");
	puts("     & ViWalls");
	
    // Load Mew tiles starting at position 128.
    set_bkg_data(128, 20, mew_tiles);
    // Draw Mew figure in the middle of the screen (more or less).
    set_bkg_tiles(7, 7, 5, 5, mew_map);

    disable_interrupts();

    static uint8_t in = 0xff;
    static size_t trade_counter = 0;
    static clock_t last_action = 0;

    SC_REG = SIOF_CLOCK_INT;
    while (TRUE) {
        if (scheduled_refill) {
            fill_pokemon_team();
            scheduled_refill = FALSE;
        }

        // Need to wait a bit before sending each byte. This is only needed if the distribution cartridge is acting as
        // master.
        delay(16);

        in = sio_exchange_master(handle_byte(in, &trade_counter, &last_action));
    }
}
