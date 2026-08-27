#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

#if !defined(_MSC_VER) && !defined(_byteswap_ulong)
inline uint32_t _byteswap_ulong(uint32_t v) {
    return ((v >> 24) & 0x000000FFu) |
           ((v >> 8)  & 0x0000FF00u) |
           ((v << 8)  & 0x00FF0000u) |
           ((v << 24) & 0xFF000000u);
}
#endif

#include "save_data.h"

class SaveManager {
public:
    static constexpr size_t kSaveSize = sizeof(SaveData);
    static constexpr size_t kLegacySaveSize = 52292;

    bool loadFile(const std::string& path);
    bool saveFile(const std::string& path);

    bool hasData() const { return !m_buffer.empty(); }
    size_t originalSize() const { return m_originalSize; }
    size_t bufferSize() const { return m_buffer.size(); }
    const std::string& currentPath() const { return m_currentPath; }
    const std::vector<uint8_t>& buffer() const { return m_buffer; }

    SaveData* data();
    const SaveData* data() const;

    static uint32_t calculateCRC32(const uint8_t* data, size_t length);
    void updateChecksum();
    bool verifyChecksum() const;

private:
    std::vector<uint8_t> m_buffer;
    size_t m_originalSize = 0;
    std::string m_currentPath;

    size_t writeSize() const { return m_originalSize > 0 ? m_originalSize : sizeof(SaveData); }
};
