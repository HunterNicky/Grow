#include "chroma/shared/core/components/Nivel.h"
#include "chroma/shared/core/components/Component.h"

namespace chroma::shared::core::component {

Nivel::Nivel(int nivel)
    : nivel_(nivel), experience_(0.0F), experience_to_next_nivel_(100.0F)
{
    type_ = ComponentType::CHARACTER_NIVEL;
}

int Nivel::GetNivel() const { return nivel_; }

float Nivel::GetExperience() const { return experience_; }

float Nivel::GetExperienceToNextNivel() const { return experience_to_next_nivel_; }

void Nivel::SetNivel(int nivel) { nivel_ = nivel; }

void Nivel::SetExperience(float experience) { experience_ = experience; }

void Nivel::SetExperienceToNextNivel(float experience_to_next_nivel) { experience_to_next_nivel_ = experience_to_next_nivel; }

} // namespace chroma::shared::core::component