#pragma once

#include "MXmlParser.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include "StringView.h"

enum MCHANNEL_RULE {
	MCHANNEL_RULE_NOVICE=0,
	MCHANNEL_RULE_NEWBIE,
	MCHANNEL_RULE_ROOKIE,
	MCHANNEL_RULE_MASTERY,
	MCHANNEL_RULE_ELITE,
	MCHANNEL_RULE_CHAMPION,
	MCHANNEL_RULE_QUEST,
	MCHANNEL_RULE_DUELTOURNAMENT,
	MCHANNEL_RULE_SET1,									
	MCHANNEL_RULE_SET2,							
	MCHANNEL_RULE_SET3,								

	MCHANNEL_RULE_MAX
};

#define MCHANNEL_RULE_NOVICE_STR		"novice"
#define MCHANNEL_RULE_NEWBIE_STR		"newbie"
#define MCHANNEL_RULE_ROOKIE_STR		"rookie"
#define MCHANNEL_RULE_MASTERY_STR		"mastery"
#define MCHANNEL_RULE_ELITE_STR			"elite"
#define MCHANNEL_RULE_CHAMPION_STR		"champion"
#define MCHANNEL_RULE_QUEST_STR			"quest"
#define MCHANNEL_RULE_DUELTOURNAMENT_STR "dueltournament"
#define MCHANNEL_RULE_SET1_STR			"set1"		
#define MCHANNEL_RULE_SET2_STR			"set2"		
#define MCHANNEL_RULE_SET3_STR			"set3"		



class MChannelRuleMapList : public std::list<int>
{
private:
	std::set<int> m_Set;
public:
	void Add(int nMapID)
	{
		m_Set.insert(nMapID);
		push_back(nMapID);
	}
	void Add(const StringView& strMapName);
	
	bool Exist(int nMapID, bool bOnlyDuel);
	bool Exist(const StringView& pszMapName, bool bOnlyDuel);
	
	bool ExistAsCTF(int nMapID);
	bool ExistAsCTF(const StringView& pszMapName);
};

class MChannelRuleGameTypeList : public std::list<int>
{
private:
	std::set<int>	m_Set;
public:
	void Add(int nGameTypeID)
	{
		m_Set.insert(nGameTypeID);
		push_back(nGameTypeID);
	}
	bool Exist(int nGameTypeID)
	{
		if (m_Set.find(nGameTypeID) != m_Set.end()) return true;
		return false;
	}

	const int GetFirstGameType()
	{
		if( empty() )
		{
			return -1;
		}

		return *(begin());
	}
};

class MChannelRule {
protected:
	int							m_nID;
	std::string					m_Name;
	MChannelRuleMapList			m_MapList;
	MChannelRuleGameTypeList	m_GameTypeList;
	int							m_nDefault;

public:
	MChannelRule()						{}
	virtual ~MChannelRule()				{}
	void Init(int nID, const char* pszName)
	{
		m_nDefault = 0;
		m_nID = nID;
		m_Name = pszName;
	}

     int GetID()	const					{ return m_nID; }
	const char* GetName() const			{ return m_Name.c_str(); }

	void AddMap(const StringView& strMapName)		{ m_MapList.Add(strMapName); }
	void AddGameType(int nGameTypeID)	{ m_GameTypeList.Add(nGameTypeID); }
	bool CheckMap(int nMapID, bool bOnlyDuel)
	{
		return m_MapList.Exist(nMapID, bOnlyDuel);
	}
	bool CheckMap(const StringView& pszMapName, bool bOnlyDuel)
	{
		return m_MapList.Exist(pszMapName, bOnlyDuel);
	}
	bool CheckCTFMap(const StringView& pszMapName)
	{
		return m_MapList.ExistAsCTF(pszMapName);
	}
	bool CheckCTFMap(int nMapID)
	{
		return m_MapList.ExistAsCTF(nMapID);
	}
	bool CheckGameType(int nGameTypeID)
	{
		return m_GameTypeList.Exist(nGameTypeID);
	}
	MChannelRuleMapList* GetMapList()					{ return &m_MapList; }
	MChannelRuleGameTypeList* GetGameTypeList()			{ return &m_GameTypeList; }
	void SetDefault( int nDefault)						{ m_nDefault = nDefault; }
	int GetDefault()									{ return m_nDefault; }
};

class MChannelRuleMgr : public std::map<std::string, MChannelRule*>, public MXmlParser
{
private:
	std::map<MCHANNEL_RULE, MChannelRule*>		m_RuleTypeMap;
	void AddRule(MChannelRule* pRule);
public:
	MChannelRuleMgr();
	virtual ~MChannelRuleMgr();
	void Clear();	
	MChannelRule* GetRule(const StringView& strName);
	MChannelRule* GetRule(MCHANNEL_RULE nChannelRule);

protected:
	void ParseRule(MXmlElement* element);
	virtual void ParseRoot(const char* szTagName, MXmlElement* pElement);
};
