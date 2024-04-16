#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "doomtype.h"

#include "deh_str.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_swap.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "w_wad.h"
#include "z_zone.h"

#include <riv.h>

//#define FEATURE_MAIN_MUSIC
#ifdef FEATURE_MAIN_MUSIC
uint64_t main_music_buffer_id = 0;
uint64_t main_music_id = 0;
int music_volume = 0;
#endif

static void I_RIV_ShutdownMusic(void)
{
    // ignore
}

static boolean I_RIV_InitMusic(void)
{
#ifdef FEATURE_MAIN_MUSIC
    FILE* fp = fopen("e1m1.mp3", "rb");
    if (!fp) {
        return false;
    }
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    void *buf = malloc(len);
    fread(buf, 1, len, fp);
    main_music_buffer_id = riv_make_soundbuffer(&(riv_soundbuffer_desc){
        .format = RIV_SOUNDFORMAT_MP3,
        .data = (riv_span_uint8){.data=(uint8_t*)(buf), .size=len},
    });
    free(buf);
    fclose(fp);
#endif
    return true;
}

static void I_RIV_SetMusicVolume(int volume)
{
#ifdef FEATURE_MAIN_MUSIC
    music_volume = volume;
    if (main_music_id == 0) return;
    riv_sound(&(riv_sound_desc){
        .id = main_music_id,
        .volume = volume / 127.0f,
    });
#endif
}

static void I_RIV_PlaySong(void *handle, boolean looping)
{
#ifdef FEATURE_MAIN_MUSIC
    if (main_music_id != 0) return;
    main_music_id = riv_sound(&(riv_sound_desc){
        .buffer_id = main_music_buffer_id,
        .volume = music_volume / 127.0f,
        .duration = -1.0f,
    });
#endif
}

static void I_RIV_PauseSong(void)
{
    // ignore
}

static void I_RIV_ResumeSong(void)
{
    // ignore
}

static void I_RIV_StopSong(void)
{
    // ignore
}

static void I_RIV_UnRegisterSong(void *handle)
{
    // ignore
}

static void *I_RIV_RegisterSong(void *data, int len)
{
#ifdef FEATURE_MAIN_MUSIC
    return (void*)(main_music_buffer_id);
#else
    return 0;
#endif
}

static boolean I_RIV_MusicIsPlaying(void)
{
#ifdef FEATURE_MAIN_MUSIC
    return main_music_id != 0;
#else
    return 0;
#endif
}

static void I_RIV_PollMusic(void)
{
    // ignore
}

static snddevice_t music_riv_devices[] =
{
    SNDDEVICE_SB,
};

music_module_t music_riv_module =
{
    music_riv_devices,
    arrlen(music_riv_devices),
    I_RIV_InitMusic,
    I_RIV_ShutdownMusic,
    I_RIV_SetMusicVolume,
    I_RIV_PauseSong,
    I_RIV_ResumeSong,
    I_RIV_RegisterSong,
    I_RIV_UnRegisterSong,
    I_RIV_PlaySong,
    I_RIV_StopSong,
    I_RIV_MusicIsPlaying,
    I_RIV_PollMusic,
};
