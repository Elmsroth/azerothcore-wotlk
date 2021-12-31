/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "LFGMgr.h"
#include "PassiveAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

///////////////////////////////////////
////// GOS
///////////////////////////////////////

///////////////////////////////////////
////// NPCS
///////////////////////////////////////

enum Spells
{
    SPELL_GOBLIN_DISGUISE           = 71450,
    SPELL_GOBLIN_CARRY_CRATE        = 71459,

    NPC_SOMETHING_STINKS_CREDIT     = 37558,
};

enum Quests
{
    QUEST_PILGRIM_HORDE             = 24541,
    QUEST_PILGRIM_ALLIANCE          = 24656
};

enum SupplySentrySay
{
    SAY_SUPPLY_SENTRY_0 = 0
};

struct npc_love_in_air_supply_sentry : public ScriptedAI
{
    npc_love_in_air_supply_sentry(Creature* creature) : ScriptedAI(creature), lock(0)
    {
    }

    uint32 lock;

    void MoveInLineOfSight(Unit* who) override
    {
        if (lock > 1000 && me->GetDistance(who) < 10.0f && who->GetTypeId() == TYPEID_PLAYER && who->HasAura(SPELL_GOBLIN_DISGUISE) && !who->HasAura(SPELL_GOBLIN_CARRY_CRATE))
        {
            lock = 0;
            if (urand(0, 1))
            {
                me->AI()->Talk(SAY_SUPPLY_SENTRY_0, who->ToPlayer());
            }
            else
            {
                me->AI()->Talk(SAY_SUPPLY_SENTRY_0, who->ToPlayer());
            }

            if (who->ToPlayer()->GetTeamId() == TEAM_ALLIANCE)
            {
                who->ToPlayer()->CompleteQuest(QUEST_PILGRIM_ALLIANCE);
            }
            else
            {
                who->ToPlayer()->CompleteQuest(QUEST_PILGRIM_HORDE);
            }

            me->CastSpell(who, SPELL_GOBLIN_CARRY_CRATE, true);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        lock += diff;
    }
};

enum hotOnTrail
{
    QUEST_HOT_ON_TRAIL_ALLY         = 24849,
    QUEST_HOT_ON_TRAIL_HORDE        = 24851,

    NPC_SNIVEL_ALLY                 = 38334,
    NPC_SNIVEL_COUNTER              = 38340,

    NPC_SNIVEL_HORDE                = 38337,

    SPELL_SNIVEL_GUN                = 71715,
};

const uint32 spellTable[6] = {71713, 71745, 71752, 71759, 71760, 71758};

struct npc_love_in_air_snivel : public NullCreatureAI
{
    npc_love_in_air_snivel(Creature* creature) : NullCreatureAI(creature) { }

    int32 delay;

    void Reset() override
    {
        delay = 0;
        me->SetReactState(REACT_AGGRESSIVE);
    }

    bool AllowAction(Player* player)
    {
        uint16 slot = player->FindQuestSlot(player->GetTeamId() == TEAM_ALLIANCE ? QUEST_HOT_ON_TRAIL_ALLY : QUEST_HOT_ON_TRAIL_HORDE);
        if (slot >= MAX_QUEST_LOG_SIZE)
            return false;

        QuestStatusData& qData = player->getQuestStatusMap()[(player->GetTeamId() == TEAM_ALLIANCE ? QUEST_HOT_ON_TRAIL_ALLY : QUEST_HOT_ON_TRAIL_HORDE)];
        if (qData.CreatureOrGOCount[me->GetEntry() - NPC_SNIVEL_COUNTER] == 0)
            return true;

        return false;
    }

    void MoveInLineOfSight(Unit* who) override
    {
        if (delay == 0 && me->GetDistance(who) < 7.0f && who->GetTypeId() == TYPEID_PLAYER)
        {
            Player* plr = who->ToPlayer();
            if (AllowAction(plr))
            {
                delay = 25000;
                uint8 index = plr->GetTeamId() * 3 + (me->GetEntry() - NPC_SNIVEL_COUNTER);
                plr->CastSpell(plr, spellTable[index], true);
            }
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (delay > 0)
            delay -= std::min<int32>(delay, diff);
    }
};

enum SnivelRealSay
{
    SAY_SNIVEL_REAL_0 = 0,
    SAY_SNIVEL_REAL_1 = 1,
    SAY_SNIVEL_REAL_2 = 2,
    SAY_SNIVEL_REAL_3 = 3
};

struct npc_love_in_air_snivel_real : public ScriptedAI
{
    npc_love_in_air_snivel_real(Creature* creature) : ScriptedAI(creature)
    {
        actionTimer = 7000;
        actionCounter = 0;
    }

    uint32 actionTimer;
    uint32 actionCounter;

    bool Talk(uint32 time)
    {
        switch (me->GetEntry())
        {
            case NPC_SNIVEL_ALLY:
            case NPC_SNIVEL_HORDE:
                {
                    switch (time)
                    {
                        case 1:
                            me->AI()->Talk(SAY_SNIVEL_REAL_0);
                            return false;
                        case 2:
                            me->AI()->Talk(SAY_SNIVEL_REAL_1);
                            return true;
                    }
                    break;
                }
            case NPC_SNIVEL_ALLY+1:
            case NPC_SNIVEL_HORDE+1:
                {
                    switch (time)
                    {
                        case 1:
                            me->AI()->Talk(SAY_SNIVEL_REAL_0);
                            return false;
                        case 2:
                            me->AI()->Talk(SAY_SNIVEL_REAL_1);
                            return false;
                        case 3:
                            me->AI()->Talk(SAY_SNIVEL_REAL_2);
                            return true;
                    }
                    break;
                }

            case NPC_SNIVEL_ALLY+2:
            case NPC_SNIVEL_HORDE+2:
                {
                    switch (time)
                    {
                        case 1:
                            me->AI()->Talk(SAY_SNIVEL_REAL_0);
                            return false;
                        case 2:
                            me->AI()->Talk(SAY_SNIVEL_REAL_1);
                            return false;
                        case 3:
                            me->AI()->Talk(SAY_SNIVEL_REAL_2);
                            return false;
                        case 4:
                            me->AI()->Talk(SAY_SNIVEL_REAL_3);
                            return true;
                    }
                    break;
                }
        }

        return true;
    }

    void UpdateAI(uint32 diff) override
    {
        actionTimer += diff;
        if (actionTimer >= 7000)
        {
            actionTimer = 0;
            actionCounter++;
            if (Talk(actionCounter))
            {
                if (me->ToTempSummon())
                    if (Unit* owner = me->ToTempSummon()->GetSummonerUnit())
                        me->CastSpell(owner, SPELL_SNIVEL_GUN, true);

                me->DespawnOrUnsummon(1000);
            }
        }
    }
};

///////////////////////////////////////
////// BOSS
///////////////////////////////////////

enum hummel
{
    QUEST_YOUVE_BEEN_SERVED         = 14488,
    NPC_APOTHECARY_HUMMEL           = 36296,
    NPC_APOTHECARY_FRYE             = 36272,
    NPC_APOTHECARY_BAXTER           = 36565,

    SPELL_COLOGNE_IMMUNE            = 68530,
    SPELL_COLOGNE_PASSIVE_DAMAGE    = 68947,
    SPELL_PERFUME_PASSIVE_DAMAGE    = 68641,

    SPELL_THROW_COLOGNE             = 68614,
    SPELL_THROW_PERFUME             = 68798,

    // Real fight
    SPELL_COLOGNE_SPRAY             = 68948,
    SPELL_ALLURING_PERFUME_SPRAY    = 68607,
    SPELL_CHAIN_REACTION            = 68821,

    ACTION_START_EVENT              = 1,
    ACTION_RELEASE_HELPER           = 2,

    EVENT_CALL_BAXTER               = 1,
    EVENT_CALL_FRYE                 = 2,
    EVENT_SPELL_PERFUME_SPRAY       = 3,
    EVENT_SPELL_CHAIN_REACTION      = 4,
    EVENT_SPELL_THROW               = 5,
};

enum HummelSay
{
    SAY_HUMMEL_0 = 0,
    SAY_HUMMEL_1 = 1,
    SAY_HUMMEL_2 = 2,
    SAY_HUMMEL_5 = 5
};

struct npc_love_in_air_hummel : public ScriptedAI
{
    npc_love_in_air_hummel(Creature* creature) : ScriptedAI(creature), summons(me) { }

    SummonList summons;
    EventMap events;
    uint32 speachTimer;

    bool CanBeSeen(Player const* player) override
    {
        if (player->IsGameMaster())
        {
            return true;
        }

        Group const* group = player->GetGroup();
        return group && sLFGMgr->GetDungeon(group->GetGUID()) == lfg::LFG_DUNGEON_CROWN_CHEMICAL_CO;
    }

    void Reset() override
    {
        speachTimer = 0;
        me->SetFaction(FACTION_FRIENDLY);
        summons.DespawnAll();
        events.Reset();
        me->SummonCreature(NPC_APOTHECARY_FRYE, -205.449f, 2219.56f, 79.7633f, 0.7f);
        me->SummonCreature(NPC_APOTHECARY_BAXTER, -209.602f, 2215.42f, 79.7633f, 0.723503f);
    }

    void DoAction(int32 param) override
    {
        if (param == ACTION_START_EVENT)
            speachTimer = 1;
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->AI()->Talk(SAY_HUMMEL_5);
        Map::PlayerList const& players = me->GetMap()->GetPlayers();
        if (!players.isEmpty() && players.begin()->GetSource() && players.begin()->GetSource()->GetGroup())
            sLFGMgr->FinishDungeon(players.begin()->GetSource()->GetGroup()->GetGUID(), lfg::LFG_DUNGEON_CROWN_CHEMICAL_CO, me->FindMap());
    }

    void JustSummoned(Creature* cr) override
    {
        summons.Summon(cr);
        cr->SetFaction(FACTION_FRIENDLY);
        cr->SetControlled(true, UNIT_STATE_STUNNED);
        cr->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(uint32 diff) override
    {
        if (speachTimer)
        {
            speachTimer += diff;
            if (speachTimer < 10000)
            {
                me->AI()->Talk(SAY_HUMMEL_0);
                speachTimer = 10000;
            }
            else if (speachTimer >= 16000 && speachTimer < 20000)
            {
                me->AI()->Talk(SAY_HUMMEL_1);
                speachTimer = 20000;
            }
            else if (speachTimer >= 26000 && speachTimer < 30000)
            {
                me->AI()->Talk(SAY_HUMMEL_2);
                speachTimer = 0;
                me->SetFaction(FACTION_MONSTER_2);
                me->SetInCombatWithZone();
                if (Unit* target = SelectTargetFromPlayerList(40.0f))
                {
                    AttackStart(target);
                    events.ScheduleEvent(EVENT_CALL_BAXTER, 10000);
                    events.ScheduleEvent(EVENT_CALL_FRYE, 20000);
                    events.ScheduleEvent(EVENT_SPELL_PERFUME_SPRAY, 7000);
                    events.ScheduleEvent(EVENT_SPELL_CHAIN_REACTION, 12000);
                }
                else
                    EnterEvadeMode();
            }
            return;
        }

        if (!UpdateVictim())
            return;

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        switch (events.ExecuteEvent())
        {
            case EVENT_CALL_BAXTER:
                {
                    EntryCheckPredicate pred(NPC_APOTHECARY_BAXTER);
                    summons.DoAction(ACTION_RELEASE_HELPER, pred);

                    break;
                }
            case EVENT_CALL_FRYE:
                {
                    EntryCheckPredicate pred(NPC_APOTHECARY_FRYE);
                    summons.DoAction(ACTION_RELEASE_HELPER, pred);

                    break;
                }
            case EVENT_SPELL_PERFUME_SPRAY:
                me->CastSpell(me->GetVictim(), SPELL_ALLURING_PERFUME_SPRAY, false);
                events.RepeatEvent(6000);
                break;
            case EVENT_SPELL_CHAIN_REACTION:
                me->CastSpell(me->GetVictim(), SPELL_CHAIN_REACTION, false);
                events.RepeatEvent(12000);
                break;
        }

        DoMeleeAttackIfReady();
    }
};

enum HummelHelperSay
{
    SAY_HUMMEL_HELPER_SAY_5 = 5,
};

struct npc_love_in_air_hummel_helper : public ScriptedAI
{
    npc_love_in_air_hummel_helper(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    bool CanBeSeen(Player const* player) override
    {
        if (player->IsGameMaster())
        {
            return true;
        }

        Group const* group = player->GetGroup();
        return group && sLFGMgr->GetDungeon(group->GetGUID()) == lfg::LFG_DUNGEON_CROWN_CHEMICAL_CO;
    }

    void Reset() override
    {
    }

    void DoAction(int32 param) override
    {
        if (param == ACTION_RELEASE_HELPER)
        {
            me->SetControlled(false, UNIT_STATE_STUNNED);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFaction(FACTION_MONSTER_2);
            me->SetInCombatWithZone();
            if (Unit* target = SelectTargetFromPlayerList(40.0f))
                AttackStart(target);

            if (me->GetEntry() == NPC_APOTHECARY_BAXTER)
            {
                events.ScheduleEvent(EVENT_SPELL_PERFUME_SPRAY, 7000);
                events.ScheduleEvent(EVENT_SPELL_CHAIN_REACTION, 12000);
            }
            else
                events.ScheduleEvent(EVENT_SPELL_THROW, 5000);
        }
    }

    void JustDied(Unit* /*killer*/) override
    {
        me->AI()->Talk(SAY_HUMMEL_HELPER_SAY_5);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        switch (events.ExecuteEvent())
        {
            case EVENT_SPELL_PERFUME_SPRAY:
                me->CastSpell(me->GetVictim(), SPELL_COLOGNE_SPRAY, false);
                events.RepeatEvent(6000);
                break;
            case EVENT_SPELL_CHAIN_REACTION:
                me->CastSpell(me->GetVictim(), SPELL_CHAIN_REACTION, false);
                events.RepeatEvent(12000);
                break;
            case EVENT_SPELL_THROW:
                if (Unit* target = SelectTarget(SelectTargetMethod::Random))
                {
                    Position pos(*me);
                    me->Relocate(target);
                    me->CastSpell(me, RAND(SPELL_THROW_COLOGNE, SPELL_THROW_PERFUME), true, nullptr, nullptr, me->GetGUID());
                    me->Relocate(pos);
                }
                events.RepeatEvent(10000);
                break;
        }

        DoMeleeAttackIfReady();
    }
};

///////////////////////////////////////
////// SPELLS
///////////////////////////////////////

class spell_love_in_air_perfume_immune : public AuraScript
{
    PrepareAuraScript(spell_love_in_air_perfume_immune);

    void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (GetId() == SPELL_COLOGNE_IMMUNE)
        {
            target->ApplySpellImmune(SPELL_COLOGNE_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_COLOGNE_PASSIVE_DAMAGE, true);
            target->ApplySpellImmune(SPELL_COLOGNE_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_COLOGNE_SPRAY, true);
            target->ApplySpellImmune(68934, IMMUNITY_ID, 68934, true);
        }
        else
        {
            target->ApplySpellImmune(SPELL_PERFUME_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_PERFUME_PASSIVE_DAMAGE, true);
            target->ApplySpellImmune(SPELL_ALLURING_PERFUME_SPRAY, IMMUNITY_ID, SPELL_ALLURING_PERFUME_SPRAY, true);
            target->ApplySpellImmune(68927, IMMUNITY_ID, 68927, true);
        }
    }

    void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (GetId() == SPELL_COLOGNE_IMMUNE)
        {
            target->ApplySpellImmune(SPELL_COLOGNE_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_COLOGNE_PASSIVE_DAMAGE, false);
            target->ApplySpellImmune(SPELL_COLOGNE_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_COLOGNE_SPRAY, false);
            target->ApplySpellImmune(68934, IMMUNITY_ID, 68934, false);
        }
        else
        {
            target->ApplySpellImmune(SPELL_PERFUME_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_PERFUME_PASSIVE_DAMAGE, false);
            target->ApplySpellImmune(SPELL_PERFUME_PASSIVE_DAMAGE, IMMUNITY_ID, SPELL_ALLURING_PERFUME_SPRAY, false);
            target->ApplySpellImmune(68927, IMMUNITY_ID, 68927, false);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_love_in_air_perfume_immune::HandleEffectApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_love_in_air_perfume_immune::HandleEffectRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_love_in_air_periodic_perfumes : public AuraScript
{
    PrepareAuraScript(spell_love_in_air_periodic_perfumes);

    void PeriodicTick(AuraEffect const* /*aurEff*/)
    {
        ObjectGuid guid = GetCaster() ? GetCaster()->GetGUID() : ObjectGuid::Empty;
        if (Unit* target = GetTarget())
        {
            uint32 spellId = (GetId() == SPELL_THROW_COLOGNE ? 68934 : 68927);
            if (target->IsImmunedToSpell(sSpellMgr->GetSpellInfo(spellId)))
                return;

            target->CastSpell(target, spellId, true, nullptr, nullptr, guid);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_love_in_air_periodic_perfumes::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

enum CreateHeartCandy
{
    SPELL_CREATE_HEART_CANDY_1     = 26668,
    SPELL_CREATE_HEART_CANDY_2     = 26670,
    SPELL_CREATE_HEART_CANDY_3     = 26671,
    SPELL_CREATE_HEART_CANDY_4     = 26672,
    SPELL_CREATE_HEART_CANDY_5     = 26673,
    SPELL_CREATE_HEART_CANDY_6     = 26674,
    SPELL_CREATE_HEART_CANDY_7     = 26675,
    SPELL_CREATE_HEART_CANDY_8     = 26676
};

std::array<uint32, 8> constexpr CreateHeartCandySpells =
{
    SPELL_CREATE_HEART_CANDY_1, SPELL_CREATE_HEART_CANDY_2, SPELL_CREATE_HEART_CANDY_3,
    SPELL_CREATE_HEART_CANDY_4, SPELL_CREATE_HEART_CANDY_5, SPELL_CREATE_HEART_CANDY_6,
    SPELL_CREATE_HEART_CANDY_7, SPELL_CREATE_HEART_CANDY_8
};

// 26678 - Create Heart Candy
class spell_item_create_heart_candy : public SpellScript
{
    PrepareSpellScript(spell_item_create_heart_candy);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(CreateHeartCandySpells);
    }

    void HandleScript(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);
        if (Player* target = GetHitUnit()->ToPlayer())
        {
            target->CastSpell(target, Acore::Containers::SelectRandomContainerElement(CreateHeartCandySpells), true);
        }

    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_item_create_heart_candy::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 45102 Romantic Picnic
enum SpellsPicnic
{
    SPELL_BASKET_CHECK              = 45119, // Holiday - Valentine - Romantic Picnic Near Basket Check
    SPELL_MEAL_PERIODIC             = 45103, // Holiday - Valentine - Romantic Picnic Meal Periodic - effect dummy
    SPELL_MEAL_EAT_VISUAL           = 45120, // Holiday - Valentine - Romantic Picnic Meal Eat Visual
    //SPELL_MEAL_PARTICLE             = 45114, // Holiday - Valentine - Romantic Picnic Meal Particle - unused
    SPELL_DRINK_VISUAL              = 45121, // Holiday - Valentine - Romantic Picnic Drink Visual
    SPELL_ROMANTIC_PICNIC_ACHIEV    = 45123, // Romantic Picnic periodic = 5000
};

class spell_love_is_in_the_air_romantic_picnic : public AuraScript
{
    PrepareAuraScript(spell_love_is_in_the_air_romantic_picnic);

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->SetStandState(UNIT_STAND_STATE_SIT);
        target->CastSpell(target, SPELL_MEAL_PERIODIC, false);
    }

    void OnPeriodic(AuraEffect const* /*aurEff*/)
    {
        // Every 5 seconds
        Unit* target = GetTarget();
        Unit* caster = GetCaster();

        // If our player is no longer sit, remove all auras
        if (target->getStandState() != UNIT_STAND_STATE_SIT)
        {
            target->RemoveAura(SPELL_ROMANTIC_PICNIC_ACHIEV);
            target->RemoveAura(GetAura());
            return;
        }

        target->CastSpell(target, SPELL_BASKET_CHECK, false); // unknown use, it targets Romantic Basket
        target->CastSpell(target, RAND(SPELL_MEAL_EAT_VISUAL, SPELL_DRINK_VISUAL), false);

        bool foundSomeone = false;
        // For nearby players, check if they have the same aura. If so, cast Romantic Picnic (45123)
        // required by achievement and "hearts" visual
        std::list<Player*> playerList;
        Acore::AnyPlayerInObjectRangeCheck checker(target, INTERACTION_DISTANCE * 2);
        Acore::PlayerListSearcher<Acore::AnyPlayerInObjectRangeCheck> searcher(target, playerList, checker);
        Cell::VisitWorldObjects(target, searcher, INTERACTION_DISTANCE * 2);
        for (std::list<Player*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
        {
            if ((*itr) != target && (*itr)->HasAura(GetId())) // && (*itr)->getStandState() == UNIT_STAND_STATE_SIT)
            {
                if (caster)
                {
                    caster->CastSpell(*itr, SPELL_ROMANTIC_PICNIC_ACHIEV, true);
                    caster->CastSpell(target, SPELL_ROMANTIC_PICNIC_ACHIEV, true);
                }
                foundSomeone = true;
                // break;
            }
        }

        if (!foundSomeone && target->HasAura(SPELL_ROMANTIC_PICNIC_ACHIEV))
            target->RemoveAura(SPELL_ROMANTIC_PICNIC_ACHIEV);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_love_is_in_the_air_romantic_picnic::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_love_is_in_the_air_romantic_picnic::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

enum ServiceUniform
{
    SPELL_SERVICE_UNIFORM       = 71450,

    MODEL_GOBLIN_MALE           = 31002,
    MODEL_GOBLIN_FEMALE         = 31003
};

class spell_gen_aura_service_uniform : public AuraScript
{
    PrepareAuraScript(spell_gen_aura_service_uniform);

    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ SPELL_SERVICE_UNIFORM });
    }

    void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        // Apply model goblin
        Unit* target = GetTarget();
        if (target->GetTypeId() == TYPEID_PLAYER)
        {
            if (target->getGender() == GENDER_MALE)
                target->SetDisplayId(MODEL_GOBLIN_MALE);
            else
                target->SetDisplayId(MODEL_GOBLIN_FEMALE);
        }
    }

    void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        if (target->GetTypeId() == TYPEID_PLAYER)
            target->RestoreDisplayId();
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_gen_aura_service_uniform::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_gen_aura_service_uniform::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_event_love_in_the_air()
{
    // Npcs
    RegisterCreatureAI(npc_love_in_air_supply_sentry);
    RegisterCreatureAI(npc_love_in_air_snivel);
    RegisterCreatureAI(npc_love_in_air_snivel_real);

    // Boss
    RegisterCreatureAI(npc_love_in_air_hummel);
    RegisterCreatureAI(npc_love_in_air_hummel_helper);

    // Spells
    RegisterSpellScript(spell_love_in_air_perfume_immune);
    RegisterSpellScript(spell_love_in_air_periodic_perfumes);
    RegisterSpellScript(spell_item_create_heart_candy);
    RegisterSpellScript(spell_love_is_in_the_air_romantic_picnic);
    RegisterSpellScript(spell_gen_aura_service_uniform);
}
