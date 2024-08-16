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
static int showstats = 0;
static int limitframes = 0;
static FILE *outcardstream;

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
}

void StatDump()
{
    FILE *stream = outcardstream;
    if (!stream) {
        return;
    }
    fseek(stream, 0, SEEK_SET);
    fprintf(stream, "JSON{\n\"levelstats\":[");
    sum_stats_t sum = {0};
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
    fprintf(stream, "\"levels\":%d,\n\"maxlevels\":%d,\n", sum.numlevels, sum.maxlevels);
    fprintf(stream, "\"kills\":%d,\n\"maxkills\":%d,\n", sum.numkills, sum.maxkills);
    fprintf(stream, "\"items\":%d,\n\"maxitems\":%d,\n", sum.numitems, sum.maxitems);
    fprintf(stream, "\"secrets\":%d,\n\"maxsecrets\":%d,\n", sum.numsecrets, sum.maxsecrets);
    fprintf(stream, "\"punchhits\":%d,\n\"punchmisses\":%d,\n", sum.punchhits, sum.punchmisses);
    fprintf(stream, "\"pistolhits\":%d,\n\"pistolmisses\":%d,\n", sum.pistolhits, sum.pistolmisses);
    fprintf(stream, "\"frames\":%d\n}", sum.frames);
    fflush(stream);

    riv->outcard_len = ftell(stream);

    if (num_captured_stats > 0 && showstats) {
        wbstartstruct_t *s = &captured_stats[num_captured_stats - 1];
        wbplayerstruct_t *p = &s->plyr[0];

        // draw time
        riv_draw_text("T", RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, 2, 164, 1, 80);
        int tx = 2 + 4*2;
        if (num_captured_stats > 1) { // draw global time
            int total_mins = sum.frames / (60*TICRATE);
            int total_secs = (sum.frames % (60*TICRATE)) / TICRATE;
            riv_draw_text(riv_tprintf("%02d:%02d", total_mins, total_secs), RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, tx, 164, 1, 112);
            tx += 6*4;
        }
        // draw level time
        int level_mins = p->stime / (60*TICRATE);
        int level_secs = (p->stime % (60*TICRATE)) / TICRATE;
        int level_centis = ((p->stime % TICRATE) * 100) / TICRATE;
        riv_draw_text(riv_tprintf("%02d:%02d.%02d", level_mins, level_secs, level_centis), RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMLEFT, tx, 164, 1, 231);

        // draw kills
        const char *s1 = riv_tprintf("%d/%d", p->skills, s->maxkills);
        const char *s2 = riv_tprintf("%d/%d", p->sitems, s->maxitems);
        const char *s3 = riv_tprintf("%d/%d", p->ssecret, s->maxsecret);
        riv_draw_text(riv_tprintf("  %-7s    %-7s    %-5s", s1, s2, s3),
                    RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMRIGHT, 317, 164, 1, 231);
        riv_draw_text("K          I          S      ", RIV_SPRITESHEET_FONT_3X5, RIV_BOTTOMRIGHT, 317, 164, 1, 80);
    }

    if (limitframes > 0) {
        riv_draw_rect_fill(0, 167, (sum.frames * 320) / limitframes, 1, 80);
        if (sum.frames > limitframes) {
            riv->quit = true;
        }
    }
}

void StatInit() {
    outcardstream = fmemopen(riv->outcard, RIV_SIZE_OUTCARD, "wb");
    showstats = M_CheckParm("-showstats");
    int i = M_CheckParmWithArgs("-limitframes", 1);
    if (i > 0) {
        limitframes = atoi(myargv[i+1]);
    }
}
