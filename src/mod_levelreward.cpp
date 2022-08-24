#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Tokenize.h"

uint32 levelRewardCreature;
std::string levelRewardMailSubject[8];
std::string levelRewardMailBody[8];
uint32 levelRewardGold[8];

struct LevelRewardItems
{
    uint32 level;
    uint32 entry;
    uint32 count;
};

std::vector<LevelRewardItems> levelRewardItems;

class LevelReward : public PlayerScript
{
public:
    LevelReward() : PlayerScript("LevelReward") {}

    void OnLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (player->getLevel() % 10 != 0 || player->getLevel() > 80)
            return;

        SendMailTo(player);
    }

private:
    void SendMailTo(Player* receiver)
    {
        uint32 guid = receiver->GetGUID().GetCounter();
        uint32 level = receiver->getLevel();
        uint8 index = (level / 10) - 1;

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        MailDraft* mail = new MailDraft(levelRewardMailSubject[index], levelRewardMailBody[index]);

        if (levelRewardGold[index] > 0)
            mail->AddMoney(levelRewardGold[index]);

        for (auto& item : levelRewardItems)
        {
            if (item.level == level && item.count > 0)
            {
                if (ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item.entry))
                {
                    if (Item* mailItem = Item::CreateItem(item.entry, item.count))
                    {
                        mailItem->SaveToDB(trans);
                        mail->AddItem(mailItem);
                    }
                }
            }
        }

        MailSender mailSender = MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);

        if (levelRewardCreature > 0)
            if (sObjectMgr->GetCreatureTemplate(levelRewardCreature))
                mailSender = MailSender(MAIL_CREATURE, levelRewardCreature, MAIL_STATIONERY_GM);

        mail->SendMailTo(trans, receiver ? receiver : MailReceiver(guid), mailSender, MAIL_CHECK_MASK_RETURNED);
        delete mail;
        CharacterDatabase.CommitTransaction(trans);
    }
};

class LevelRewardConfig : WorldScript
{
public:
    LevelRewardConfig() : WorldScript("LevelRewardConfig") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        levelRewardCreature = sConfigMgr->GetOption<uint32>("LevelReward.Mail.Creature", 0);

        levelRewardItems.clear();
        for (int i = 0; i < 8; i++)
            LoadLevelOptions(i);
    }

private:
    void LoadLevelOptions(int index)
    {
        uint32 level = (index + 1) * 10;
        levelRewardGold[index] = sConfigMgr->GetOption<uint32>(Acore::StringFormat("LevelReward.Level.%i.Money", level), 0);
        levelRewardMailSubject[index] = sConfigMgr->GetOption<std::string>(Acore::StringFormat("LevelReward.Level.%i.Mail.Subject", level), Acore::StringFormat("Level %i", level));
        levelRewardMailBody[index] = sConfigMgr->GetOption<std::string>(Acore::StringFormat("LevelReward.Level.%i.Mail.Body", level), Acore::StringFormat("Congratulations on reaching level %i! Take this reward, let it aid you in your travels.", level));

        int i = levelRewardItems.size();
        std::string rewardItems(sConfigMgr->GetOption<std::string>(Acore::StringFormat("LevelReward.Level.%i.Items", level), ""));
        for (std::string_view item : Acore::Tokenize(rewardItems, ';', false))
        {
            std::vector<std::string_view> list = Acore::Tokenize(item, ',', false);

            if (list.size() != 2)
                continue;

            uint32 entry = Acore::StringTo<uint32>(list[0]).value_or(0);
            uint32 count = Acore::StringTo<uint32>(list[1]).value_or(0);

            levelRewardItems.push_back(LevelRewardItems());
            levelRewardItems[i].level = level;
            levelRewardItems[i].entry = entry;
            levelRewardItems[i].count = count;

            i++;
        }
    }
};

void AddLevelRewardScripts()
{
    new LevelReward();
    new LevelRewardConfig();
}
