#include <emscripten.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

struct ProfileEntry {
    char *key;
    char *value;
    ProfileEntry *next;
};

ProfileEntry *g_profile_values = nullptr;

char *monopoly_web_strdup(const char *value)
{
    if (!value) {
        value = "";
    }
    const size_t length = std::strlen(value) + 1;
    char *copy = static_cast<char *>(std::malloc(length));
    if (copy) {
        std::memcpy(copy, value, length);
    }
    return copy;
}

char *monopoly_web_profile_key(const char *file, const char *section, const char *key)
{
    if (!file) file = "";
    if (!section) section = "";
    if (!key) key = "";

    const size_t length = std::strlen(file) + std::strlen(section) + std::strlen(key) + 3;
    char *result = static_cast<char *>(std::malloc(length));
    if (!result) {
        return nullptr;
    }
    std::snprintf(result, length, "%s\n%s\n%s", file, section, key);
    return result;
}

ProfileEntry *monopoly_web_find_profile_entry(const char *map_key)
{
    for (ProfileEntry *entry = g_profile_values; entry; entry = entry->next) {
        if (std::strcmp(entry->key, map_key) == 0) {
            return entry;
        }
    }
    return nullptr;
}

}

extern "C" {

void monopoly_web_log(const char *message)
{
    if (!message) message = "";
    EM_ASM({
        let end = $0;
        const limit = $0 + 8192;
        while (end < limit && HEAPU8[end] !== 0) end++;
        const bytes = HEAPU8.subarray($0, end);
        let text;
        try {
            text = new TextDecoder("utf-8", { fatal: true }).decode(bytes);
        } catch (e) {
            text = new TextDecoder("windows-1252").decode(bytes);
        }
        if (typeof window !== "undefined" && window.monopolyLog) window.monopolyLog(text);
        else console.log(text);
    }, message);
}

int monopoly_web_profile_get(const char *file, const char *section, const char *key, char *buffer, size_t size)
{
    char *map_key = monopoly_web_profile_key(file, section, key);
    if (!map_key) {
        return 0;
    }

    ProfileEntry *entry = monopoly_web_find_profile_entry(map_key);
    std::free(map_key);
    if (!entry) {
        return 0;
    }

    if (buffer && size) {
        std::snprintf(buffer, size, "%s", entry->value);
    }
    return 1;
}

int monopoly_web_profile_set(const char *file, const char *section, const char *key, const char *value)
{
    if (!section || !key) {
        return 0;
    }

    char *map_key = monopoly_web_profile_key(file, section, key);
    if (!map_key) {
        return 0;
    }

    ProfileEntry *entry = monopoly_web_find_profile_entry(map_key);
    if (!value) {
        ProfileEntry **cursor = &g_profile_values;
        while (*cursor) {
            if (*cursor == entry) {
                *cursor = entry->next;
                std::free(entry->key);
                std::free(entry->value);
                std::free(entry);
                break;
            }
            cursor = &((*cursor)->next);
        }
        std::free(map_key);
    } else {
        char *value_copy = monopoly_web_strdup(value);
        if (!value_copy) {
            std::free(map_key);
            return 0;
        }

        if (entry) {
            std::free(entry->value);
            entry->value = value_copy;
            std::free(map_key);
        } else {
            entry = static_cast<ProfileEntry *>(std::calloc(1, sizeof(ProfileEntry)));
            if (!entry) {
                std::free(value_copy);
                std::free(map_key);
                return 0;
            }
            entry->key = map_key;
            entry->value = value_copy;
            entry->next = g_profile_values;
            g_profile_values = entry;
        }
    }
    return 1;
}

void monopoly_web_profile_clear(const char *file)
{
    if (!file) file = "";
    const size_t prefix_length = std::strlen(file) + 1;
    char *prefix = static_cast<char *>(std::malloc(prefix_length + 1));
    if (!prefix) {
        return;
    }
    std::snprintf(prefix, prefix_length + 1, "%s\n", file);

    ProfileEntry **cursor = &g_profile_values;
    while (*cursor) {
        ProfileEntry *entry = *cursor;
        if (std::strncmp(entry->key, prefix, prefix_length) == 0) {
            *cursor = entry->next;
            std::free(entry->key);
            std::free(entry->value);
            std::free(entry);
        } else {
            cursor = &entry->next;
        }
    }

    std::free(prefix);
}

int monopoly_web_mount_idbfs(void)
{
    EM_ASM({
        FS.mkdirTree('/monopoly-user');
        try { FS.mount(IDBFS, {}, '/monopoly-user'); } catch (e) {}
    });
    return 1;
}

int monopoly_web_sync_idbfs(int populate)
{
    EM_ASM({
        FS.syncfs(!!$0, function(err) {
            if (err) console.warn('IDBFS sync failed', err);
        });
    }, populate);
    return 1;
}

void monopoly_web_present_rgba(const unsigned char *pixels, int width, int height, int pitch)
{
    EM_ASM({
        if (typeof window !== "undefined" && window.monopolyPresentRGBA) {
            window.monopolyPresentRGBA($0, $1, $2, $3);
        }
    }, pixels, width, height, pitch);
}

void monopoly_web_present_rgb565(const unsigned char *pixels, int width, int height, int pitch)
{
    static int present_count = 0;
    static bool logged_nonzero = false;
    const int call_number = ++present_count;
    unsigned int sample = 0;
    if (pixels && width > 0 && height > 0 && pitch >= width * 2) {
        for (int y = 0; y < height; y += 47) {
            const unsigned char *row = pixels + (size_t)y * (size_t)pitch;
            for (int x = 0; x < width; x += 53) {
                const unsigned int value = row[x * 2] | ((unsigned int)row[x * 2 + 1] << 8);
                sample = (sample * 33u) ^ value;
            }
        }
    }
    if (call_number <= 8 || (sample != 0 && !logged_nonzero)) {
        char message[128];
        std::snprintf(message, sizeof(message), "Primary surface present #%d: sample=0x%08X.", call_number, sample);
        monopoly_web_log(message);
        if (sample != 0) {
            logged_nonzero = true;
        }
    }
    EM_ASM({
        if (typeof window !== "undefined" && window.monopolyPresentRGBAArray) {
            const ptr = $0;
            const width = $1;
            const height = $2;
            const pitch = $3;
            const rgba = new Uint8Array(width * height * 4);
            let out = 0;
            for (let y = 0; y < height; y++) {
                let row = ptr + y * pitch;
                for (let x = 0; x < width; x++) {
                    const value = HEAPU8[row] | (HEAPU8[row + 1] << 8);
                    row += 2;
                    const r = (value >> 11) & 31;
                    const g = (value >> 5) & 63;
                    const b = value & 31;
                    rgba[out++] = (r << 3) | (r >> 2);
                    rgba[out++] = (g << 2) | (g >> 4);
                    rgba[out++] = (b << 3) | (b >> 2);
                    rgba[out++] = 255;
                }
            }
            window.monopolyPresentRGBAArray(rgba, width, height);
        }
    }, pixels, width, height, pitch);
}

}
