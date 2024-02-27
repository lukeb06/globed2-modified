#pragma once
#include <defs.hpp>

#include <Geode/modify/LevelSelectLayer.hpp>

#include <data/types/gd.hpp>

class $modify(HookedLevelSelectLayer, LevelSelectLayer) {
    static inline const auto MAIN_LEVELS = std::to_array<LevelId>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22});

    std::unordered_map<int, uint16_t> levels;

    $override
    bool init(int p0);

    $override
    void destructor();

    $override
    void updatePageWithObject(cocos2d::CCObject* o1, cocos2d::CCObject* o2);

    void sendRequest(float);
    void updatePlayerCounts();
};
