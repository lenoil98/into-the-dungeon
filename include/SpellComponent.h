/*
 * Into the Dungeon++
 *
 * Copyright (C) 2014 Teon Banek
 *
 * This file is part of Into the Dungeon++.
 * Into the Dungeon++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Into the Dungeon++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Into the Dungeon++. If not, see <http://www.gnu.org/licenses/>
 *
 * Author(s): Teon Banek <intothedungeon@gmail.com>
 */
#ifndef SPELLCOMPONENT_H
#define SPELLCOMPONENT_H

#include "IComponent.h"

#include <string>
#include <vector>

#include "GameObject.h"
#include "StatsComponent.h"
#include "Sfx.h"

/**
 * Describes an effect of a spell.
 */
class Effect {
  public:
    Effect();
    virtual ~Effect() {}
    virtual void loadIML(const IMLNode &node);

    // Source of the effect.
    GameObject source;
    // Target of the effect.
    GameObject target;
    // Effect type. Possible types are described in spells.xml.
    std::string type;
    // Damage value.
    int damage;
    // Duration if the effect type is 'dot' (damage over time).
    int duration;
    // Name of the status ailment.
    std::string ailment;
    // Chance of triggering if the type is 'trigger'.
    int chance;
    // Spell name which is triggered.
    std::string spell;
    // Stat changes.
    StatsComponent stats;
    // Entity name which is spawned if type is 'spawn'.
    std::string entity;
};

/**
 * Spell component of an object.
 * Contains a list of effects which are applied.
 * Spell objects are usually created by name applied and
 * immediately destroyed.
 */
class SpellComponent: public IComponent {
  public:
    // Used by ComponentFactory to instantiate this class.
    static IComponent *creator();
    static int hreg;

    SpellComponent();
    virtual ~SpellComponent() {}
    virtual void loadIML(const IMLNode &node);

    // Targeting type. Types are listed in spells.xml.
    std::string targeting;
    // Name of the spell.
    std::string name;
    // Description of the spell.
    std::string description;
    // Collection of effects.
    std::vector<Effect> effects;
    // Source of the spell.
    GameObject source;
    // Target of the spell.
    GameObject target;
    // Special effect which is played before applying effects.
    Sfx pre_sfx;
    // Special effect which is played after applying effects.
    Sfx post_sfx;
    // Area of effect type. Types are listed in spells.xml.
    std::string aoe;
    // AOE radius.
    int radius;
    // Number how much this spell bounces on nearby entities.
    int bounces;
    // Range in which bounce works.
    int bounce_range;
};

namespace serialization {

template<class Archive>
inline void save(Archive &archive, Effect &effect, unsigned int version)
{
  archive << effect.source;
  archive << effect.target;
  archive << MakeNameValuePair("type", effect.type);
  archive << MakeNameValuePair("damage", effect.damage);
  archive << MakeNameValuePair("duration", effect.duration);
  archive << MakeNameValuePair("ailment", effect.ailment);
  archive << MakeNameValuePair("chance", effect.chance);
  archive << MakeNameValuePair("spell", effect.spell);
  archive << effect.stats;
  archive << MakeNameValuePair("entity", effect.entity);
}

template<class Archive>
inline void load(Archive &archive, Effect &effect, unsigned int version)
{
  archive >> effect.source;
  archive >> effect.target;
  archive >> effect.type;
  archive >> effect.damage;
  archive >> effect.duration;
  archive >> effect.ailment;
  archive >> effect.chance;
  archive >> effect.spell;
  archive >> effect.stats;
  archive >> effect.entity;
}

template<class Archive>
inline void save(Archive &archive, SpellComponent &comp, unsigned int version)
{
  archive << *static_cast<IComponent*>(&comp);
  archive << MakeNameValuePair("targeting", comp.targeting);
  archive << MakeNameValuePair("name", comp.name);
  archive << MakeNameValuePair("description", comp.description);
  typedef std::vector<Effect> Effects;
  Effects::size_type effects = comp.effects.size();
  archive << MakeNameValuePair("effects", effects);
  for (Effects::iterator it = comp.effects.begin();
      it != comp.effects.end(); ++it) {
    archive << *it;
  }
  archive << comp.source;
  archive << comp.target;
  archive << comp.pre_sfx;
  archive << comp.post_sfx;
  archive << MakeNameValuePair("aoe", comp.aoe);
  archive << MakeNameValuePair("radius", comp.radius);
  archive << MakeNameValuePair("bounces", comp.bounces);
  archive << MakeNameValuePair("bounceRange", comp.bounce_range);
}

template<class Archive>
inline void load(Archive &archive, SpellComponent &comp, unsigned int version)
{
  archive >> *static_cast<IComponent*>(&comp);
  archive >> comp.targeting;
  archive >> comp.name;
  archive >> comp.description;
  typedef std::vector<Effect> Effects;
  Effects::size_type effects = 0;
  archive >> effects;
  comp.effects.clear();
  for (Effects::size_type i = 0; i < effects; ++i) {
    Effect effect;
    archive >> effect;
    comp.effects.push_back(effect);
  }
  archive >> comp.source;
  archive >> comp.target;
  archive >> comp.pre_sfx;
  archive >> comp.post_sfx;
  archive >> comp.aoe;
  archive >> comp.radius;
  archive >> comp.bounces;
  archive >> comp.bounce_range;
}

} // namespace serialization

#endif
