#include "tralics/Buffer.h"
#include "tralics/LabelInfo.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <unordered_map>

auto Istring::labinfo() const -> LabelInfo * {
    static std::unordered_map<std::string, LabelInfo> LM;
    return &LM[*this];
}
