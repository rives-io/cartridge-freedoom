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
    int maxlevels;
    int frames;

    int punchhits;
    int punchmisses;
    int pistolhits;
    int pistolmisses;
} sum_stats_t;

/* Display statistics for a single player. */

static void PrintPlayerStats(FILE *stream, sum_stats_t *sum, wbstartstruct_t *stats, int player_num)
{
    wbplayerstruct_t *player = &stats->plyr[0];
    fprintf(stream, "\t\"kills\":%d,\n\t\"maxkills\":%d,\n",
                    player->skills, stats->maxkills);
    sum->numkills += player->skills;
    sum->maxkills += stats->maxkills;
    fprintf(stream, "\t\"items\":%d,\n\t\"maxitems\":%d,\n",
                    player->sitems, stats->maxitems);
    sum->numitems += player->sitems;
    sum->maxitems += stats->maxitems;
    fprintf(stream, "\t\"secrets\":%d,\n\t\"maxsecrets\":%d,\n",
                    player->ssecret, stats->maxsecret);
    sum->numsecrets += player->ssecret;
    sum->maxsecrets += stats->maxsecret;
    fprintf(stream, "\t\"punchhits\":%d,\n\t\"punchmisses\":%d,\n",
                    player->spunchhits, player->spunchmisses);
    sum->punchhits += player->spunchhits;
    sum->punchmisses += player->spunchmisses;
    fprintf(stream, "\t\"pistolhits\":%d,\n\t\"pistolmisses\":%d,\n",
                    player->spistolhits, player->spistolmisses);
    sum->pistolhits += player->spistolhits;
    sum->pistolmisses += player->spistolmisses;
}

/* Print details of a statistics buffer to the given file. */
static void PrintStats(FILE *stream, sum_stats_t *sum, wbstartstruct_t *stats)
{
    fprintf(stream, "{\n");
    int episode = stats->epsd;
    int level = stats->last;
    fprintf(stream, "\t\"name\":\"E%iM%i\",\n", episode + 1, level + 1);

    int leveltime = stats->plyr[0].stime;

    if (stats->plyr[0].in) {
        PrintPlayerStats(stream, sum, stats, 0);
    }
    fprintf(stream, "\t\"frames\":%d\n", leveltime);
    sum->frames += leveltime;
    if (stats->completed) {
        sum->numlevels += 1;
    }
    sum->maxlevels += 1;

    fprintf(stream, "}");
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
    StatDump();
}

void StatDump()
{
    FILE *stream = fmemopen(riv->outcard, RIV_SIZE_OUTCARD, "wb");
    if (!stream) {
        return;
    }
    sum_stats_t sum = {0};
    fprintf(stream, "JSON{\n");
    fprintf(stream, "\"levelstats\":[");
    for (int i = 0; i < num_captured_stats; ++i) {
        if (i != 0) {
            fprintf(stream, ", ");
        }
        PrintStats(stream, &sum, &captured_stats[i]);
    }
    fprintf(stream, "],\n");
    int64_t score =
                100000
                + sum.numlevels * 100000
                + sum.numsecrets * 20000
                + sum.numkills * 2000
                + sum.numitems * 500
                - sum.frames;
    fprintf(stream, "\"score\":%ld,\n", score);
    fprintf(stream, "\"levels\":%d,\n", sum.numlevels);
    fprintf(stream, "\"maxlevels\":%d,\n", sum.maxlevels);
    fprintf(stream, "\"kills\":%d,\n", sum.numkills);
    fprintf(stream, "\"maxkills\":%d,\n", sum.maxkills);
    fprintf(stream, "\"items\":%d,\n", sum.numitems);
    fprintf(stream, "\"maxitems\":%d,\n", sum.maxitems);
    fprintf(stream, "\"secrets\":%d,\n", sum.numsecrets);
    fprintf(stream, "\"maxsecrets\":%d,\n", sum.maxsecrets);
    fprintf(stream, "\"punchhits\":%d,\n\"punchmisses\":%d,\n",
                    sum.punchhits, sum.punchmisses);
    fprintf(stream, "\"pistolhits\":%d,\n\"pistolmisses\":%d,\n",
                    sum.pistolhits, sum.pistolmisses);
    fprintf(stream, "\"frames\":%d\n", sum.frames);
    fprintf(stream, "}");
    fflush(stream);

    riv->outcard_len = ftell(stream);
    fclose(stream);
}
