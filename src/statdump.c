 /*

 Copyright(C) 2005-2014 Simon Howard

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 --

 Functions for presenting the information captured from the statistics
 buffer to a file.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "d_player.h"
#include "d_mode.h"
#include "m_argv.h"

#include "statdump.h"

#include <riv.h>

// Array of end-of-level statistics that have been captured.

#define MAX_CAPTURES 32
static wbstartstruct_t captured_stats[MAX_CAPTURES];
static int num_captured_stats = 0;

typedef struct sum_stats {
    int numkills;
    int maxkills;
    int numitems;
    int maxitems;
    int numsecrets;
    int maxsecrets;
    int numlevels;
    int frames;
} sum_stats_t;

/* Display statistics for a single player. */

static void PrintPlayerStats(FILE *stream, sum_stats_t *sum, wbstartstruct_t *stats, int player_num)
{
    wbplayerstruct_t *player = &stats->plyr[0];

    /* Kills percentage */
    double kills = stats->maxkills > 0 ? (player->skills * 100.0) / stats->maxkills : 100.0;
    fprintf(stream, "\t\t\"pctkills\": %.2f,\n"
                    "\t\t\"numkills\": %d,\n",
                    kills, player->skills);
    sum->numkills += player->skills;
    sum->maxkills += stats->maxkills;

    /* Items percentage */
    double items = stats->maxitems > 0 ? (player->sitems * 100.0) / stats->maxitems : 100.0;
    fprintf(stream, "\t\t\"pctitems\": %.2f,\n"
                    "\t\t\"numitems\": %d,\n",
                    items, player->sitems);
    sum->numitems += player->sitems;
    sum->maxitems += stats->maxitems;

    /* Secrets percentage */
    double secret = stats->maxsecret > 0 ? (player->ssecret * 100.0) / stats->maxsecret : 100.0;
    fprintf(stream, "\t\t\"pctsecrets\": %.2f,\n"
                    "\t\t\"numsecrets\": %d,\n",
                    secret, player->ssecret);
    sum->numsecrets += player->ssecret;
    sum->maxsecrets += stats->maxsecret;

}

/* Print details of a statistics buffer to the given file. */
static void PrintStats(FILE *stream, sum_stats_t *sum, wbstartstruct_t *stats)
{
    fprintf(stream, "{\n");
    int episode = stats->epsd;
    int level = stats->last;
    fprintf(stream, "\t\t\"name\": \"E%iM%i\",\n", episode + 1, level + 1);

    int leveltime = stats->plyr[0].stime;

    if (stats->plyr[0].in) {
        PrintPlayerStats(stream, sum, stats, 0);
    }
    fprintf(stream, "\t\t\"frames\": %d\n", leveltime);
    sum->frames += leveltime;
    sum->numlevels += 1;

    fprintf(stream, "\t}");
}

void StatCopy(wbstartstruct_t *stats)
{
    if (num_captured_stats < MAX_CAPTURES) {
        /* Replace stats for existing level */
        int to_save_index = -1;
        for (int i = 0; i < num_captured_stats; ++i) {
            if (captured_stats[i].epsd == stats->epsd && captured_stats[i].last == stats->last) {
                to_save_index = i;
                break;
            }
        }
        if (to_save_index == -1) {
            to_save_index = num_captured_stats;
            ++num_captured_stats;
        }
        /* Copy stats */
        memcpy(&captured_stats[to_save_index], stats, sizeof(wbstartstruct_t));
    }
    /* Dump stats */
    StatDump();
}

void StatPartialDump(int kills, int items, int secrets, int frames, bool died)
{
    FILE *stream = fmemopen(riv->outcard, RIV_SIZE_OUTCARD, "wb");
    if (!stream) {
        return;
    }
    sum_stats_t sum = {0};
    sum.numkills = kills;
    sum.numitems = items;
    sum.numsecrets = secrets;
    sum.frames = frames;
    fprintf(stream, "JSON{\n");
    fprintf(stream, "\t\"level_stats\": [");
    for (int i = 0; i < num_captured_stats; ++i) {
        if (i != 0) {
            fprintf(stream, ", ");
        }
        PrintStats(stream, &sum, &captured_stats[i]);
    }
    fprintf(stream, "],\n");
    int score = sum.numitems*5 +
                sum.numkills*15 +
                sum.numsecrets*60 +
                sum.numlevels*120 - sum.frames / TICRATE;
    fprintf(stream, "\t\"score\": %d,\n", score);
    fprintf(stream, "\t\"levels\": %d,\n", sum.numlevels);
    fprintf(stream, "\t\"kills\": %d,\n", sum.numkills);
    fprintf(stream, "\t\"items\": %d,\n", sum.numitems);
    fprintf(stream, "\t\"secrets\": %d,\n", sum.numsecrets);
    fprintf(stream, "\t\"frames\": %d,\n", sum.frames);
    if (died) {
        fprintf(stream, "\t\"died\": true\n");
    } else {
        fprintf(stream, "\t\"died\": false\n");
    }
    fprintf(stream, "}");
    fflush(stream);

    riv->outcard_len = ftell(stream);
    fclose(stream);
}

void StatDump(void)
{
    StatPartialDump(0, 0, 0, 0, false);
}
