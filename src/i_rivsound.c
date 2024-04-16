#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "deh_str.h"
#include "i_sound.h"
#include "i_system.h"
#include "i_swap.h"
#include "m_argv.h"
#include "m_misc.h"
#include "w_wad.h"
#include "z_zone.h"

#include "doomtype.h"

#include <riv.h>

static boolean use_sfx_prefix;

static void I_RIV_PrecacheSounds(sfxinfo_t *sounds, int num_sounds)
{
    // no-op
}

static void GetSfxLumpName(sfxinfo_t *sfx, char *buf, size_t buf_len)
{
    // Linked sfx lumps? Get the lump number for the sound linked to.

    if (sfx->link != NULL)
    {
        sfx = sfx->link;
    }

    // Doom adds a DS* prefix to sound lumps; Heretic and Hexen don't
    // do this.

    if (use_sfx_prefix)
    {
        M_snprintf(buf, buf_len, "ds%s", DEH_String(sfx->name));
    }
    else
    {
        M_StringCopy(buf, DEH_String(sfx->name), buf_len);
    }
}

//
// Retrieve the raw data lump index
//  for a given SFX name.
//

static int I_RIV_GetSfxLumpNum(sfxinfo_t *sfx)
{
    char namebuf[9];

    GetSfxLumpName(sfx, namebuf, sizeof(namebuf));

    return W_GetNumForName(namebuf);
}

static void I_RIV_UpdateSoundParams(int handle, int vol, int sep)
{
    riv_sound(&(riv_sound_desc){
        .id = handle,
        .volume = vol / 255.0f,
        .pan = (sep - 127) / 127.0f,
    });
}

//
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback)
//  is set, but currently not used by mixing.
//

static int I_RIV_StartSound(sfxinfo_t *sfxinfo, int channel, int vol, int sep)
{
    // load sound
    if (sfxinfo->driver_data == NULL) {
        int lumpnum;
        unsigned int lumplen;
        int samplerate;
        unsigned int length;
        byte *data;

        // load the sound data
        lumpnum = sfxinfo->lumpnum;
        data = W_CacheLumpNum(lumpnum, PU_STATIC);
        lumplen = W_LumpLength(lumpnum);

        // Check the header, and ensure this is a valid sound
        if (lumplen < 8 || data[0] != 0x03 || data[1] != 0x00)
        {
            // Invalid sound
            return -1;
        }

        // 16 bit sample rate field, 32 bit length field

        samplerate = (data[3] << 8) | data[2];
        length = (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];

        // If the header specifies that the length of the sound is greater than
        // the length of the lump itself, this is an invalid sound lump

        // We also discard sound lumps that are less than 49 samples long,
        // as this is how DMX behaves - although the actual cut-off length
        // seems to vary slightly depending on the sample rate.  This needs
        // further investigation to better understand the correct
        // behavior.

        if (length > lumplen - 8 || length <= 48) {
            return -1;
        }

        // The DMX sound library seems to skip the first 16 and last 16
        // bytes of the lump - reason unknown.

        data += 16;
        length -= 32;

        sfxinfo->driver_data = (uint8_t*)(size_t)(riv_make_soundbuffer(&(riv_soundbuffer_desc){
            .format = RIV_SOUNDFORMAT_U8,
            .channels = 1,
            .sample_rate = samplerate,
            .data = (riv_span_uint8){.data=(uint8_t*)(data+8), .size=length},
        }));

        // release lump
        W_ReleaseLumpNum(lumpnum);
    }

    // play sound
    int handle = (int)riv_sound(&(riv_sound_desc){
        .buffer_id = (uint64_t)(size_t)(sfxinfo->driver_data),
        .volume = vol / 255.0f,
        .pan = (sep - 127) / 127.0f,
    });

    return handle;
}

static void I_RIV_StopSound(int handle)
{
    riv_sound(&(riv_sound_desc){
        .id = handle,
        .fade_out = 0.3f,
        .seek = -1.0f,
    });
}

static boolean I_RIV_SoundIsPlaying(int handle)
{
    return handle != 0;
}

//
// Periodically called to update the sound system
//

static void I_RIV_UpdateSound(void)
{

}

static void I_RIV_ShutdownSound(void)
{

}

static boolean I_RIV_InitSound(boolean _use_sfx_prefix)
{
    use_sfx_prefix = _use_sfx_prefix;
    return true;
}

static snddevice_t sound_riv_devices[] =
{
    SNDDEVICE_SB,
};

sound_module_t sound_riv_module =
{
    sound_riv_devices,
    arrlen(sound_riv_devices),
    I_RIV_InitSound,
    I_RIV_ShutdownSound,
    I_RIV_GetSfxLumpNum,
    I_RIV_UpdateSound,
    I_RIV_UpdateSoundParams,
    I_RIV_StartSound,
    I_RIV_StopSound,
    I_RIV_SoundIsPlaying,
    I_RIV_PrecacheSounds,
};
