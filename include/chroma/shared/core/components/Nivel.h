#pragma once 

#include "chroma/shared/core/components/Component.h"    

namespace chroma::shared::core::component {

class Nivel : public Component {
public:
    explicit Nivel(int nivel = 1);
    
    [[nodiscard]] int GetNivel() const;
    [[nodiscard]] float GetExperience() const;
    [[nodiscard]] float GetExperienceToNextNivel() const;

    void SetNivel(int nivel);
    void SetExperience(float experience);
    void SetExperienceToNextNivel(float experience_to_next_nivel);

private:
    int nivel_;
    float experience_;
    float experience_to_next_nivel_;
};

} // namespace chroma::shared::core::component