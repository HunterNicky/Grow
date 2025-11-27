#pragma once

#include <random>

namespace chroma::shared::utils {

    class Random {
    public:
        static int Int(int min, int max) {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(GetRng());
        }

    private:
        // Um gerador por thread, inicializado apenas uma vez
        static std::mt19937& GetRng() {
            static thread_local std::mt19937 rng{ std::random_device{}() };
            return rng;
        }
    };
}
