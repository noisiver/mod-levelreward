#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

uint32 rewardLevel[8];

class LevelRewardPlayer : public PlayerScript
{
public:
    LevelRewardPlayer() : PlayerScript("LevelRewardPlayer") {}

    void OnLevelChanged(Player* player, uint8 oldlevel) override
    {
        uint32 level, reward = 0;

        if (oldlevel < 10 && player->getLevel() == 10)
        {
            level = 10;
            reward = rewardLevel[0];
        }
        else if (oldlevel < 20 && player->getLevel() == 20)
        {
            level = 20;
            reward = rewardLevel[1];
        }
        if (oldlevel < 30 && player->getLevel() == 30)
        {
            level = 30;
            reward = rewardLevel[2];
        }
        else if (oldlevel < 40 && player->getLevel() == 40)
        {
            level = 40;
            reward = rewardLevel[3];
        }
        else if (oldlevel < 50 && player->getLevel() == 50)
        {
            level = 50;
            reward = rewardLevel[4];
        }
        else if (oldlevel < 60 && player->getLevel() == 60)
        {
            level = 60;
            reward = rewardLevel[5];
        }
        else if (oldlevel < 70 && player->getLevel() == 70)
        {
            level = 70;
            reward = rewardLevel[6];
        }
        else if (oldlevel < 80 && player->getLevel() == 80)
        {
            level = 80;
            reward = rewardLevel[7];
        }

        if (level > 0 && reward > 0)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Congratulations on reaching level %i! Take this reward of %i gold, let it aid you in your travels.", level, reward);
            player->ModifyMoney(reward * GOLD);
        }
    }
};

class LevelRewardWorld : WorldScript
{
public:
    LevelRewardWorld() : WorldScript("LevelRewardWorld") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        rewardLevel[0] = sConfigMgr->GetOption<int32>("LevelReward.Level.10", 5);
        rewardLevel[1] = sConfigMgr->GetOption<int32>("LevelReward.Level.20", 15);
        rewardLevel[2] = sConfigMgr->GetOption<int32>("LevelReward.Level.30", 30);
        rewardLevel[3] = sConfigMgr->GetOption<int32>("LevelReward.Level.40", 45);
        rewardLevel[4] = sConfigMgr->GetOption<int32>("LevelReward.Level.50", 60);
        rewardLevel[5] = sConfigMgr->GetOption<int32>("LevelReward.Level.60", 80);
        rewardLevel[6] = sConfigMgr->GetOption<int32>("LevelReward.Level.70", 125);
        rewardLevel[7] = sConfigMgr->GetOption<int32>("LevelReward.Level.80", 250);
    }
};

void AddLevelRewardScripts()
{
    new LevelRewardPlayer();
    new LevelRewardWorld();
}
