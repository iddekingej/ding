#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <KSharedConfig>
#include <KConfigGroup>
#include <KConfig>
#include <QVariantList>
#include <QVector>
#include <QJsonArray>
/*
 * Singelton to manage configuration
 */


class TConfig
{
private:
	KSharedConfig::Ptr config;
	KConfigGroup configGui;
	QVector<int> *deviceFields;
	
public:
	TConfig();
	~TConfig();
	void setup();
	void sync();
	void setDeviceFields(QVariantList p_list);
	QVector<int>* getDeviceFields();
	void getTabDef(QJsonArray &p_list);
	void setTabDef(QJsonArray &p_list);
	void setMainWindowSize(int p_width,int p_height);
	int getMainWindowWidth();
	int getMainWindowHeight();
	bool getDisksTab();
	void setdiskTab(bool p_flag);
	bool getRaidTab();
	void setRaidTab(bool p_flag);
	bool getFsTabTab();
	void setFsTabTab(bool p_flag);
	bool getIscsiTab();
	void setIscsiTab(bool p_flag);
	bool getStatsTab();
	void setStatsTab(bool p_flag);
	bool getDeviceAsTree();
	void setDeviceAsTree(bool p_flag);
	bool getExpandByDefault();
	void setExpandByDefault(bool p_flag);
};

extern TConfig g_config;

#endif
