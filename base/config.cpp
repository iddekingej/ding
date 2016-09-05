#include "config.h"
#include "globals.h"

TConfig::TConfig()
{
}

/* Initialize config =>called in main after setting up QApplication*/
void TConfig::setup()
{
	config=KSharedConfig::openConfig();
	configGui=config->group("gui");
	KConfigGroup  l_mainGroup=config->group("main");
	QString l_check=l_mainGroup.readEntry("check");
	if(l_check.length()==0){
		QVariantList l_enableDeviceFields;
		for(int l_cnt=0;l_cnt<g_numDeviceFields;l_cnt++){
			l_enableDeviceFields.append(l_cnt);
		}
		setDeviceFields(l_enableDeviceFields);
		l_mainGroup.writeEntry("check","1");
		config->sync();
	}

}
	
//Sync needs to be called after a change
void TConfig::sync()
{
	config->sync();
}

// Return QVariant list with enabled fields=>is integer
QVariantList TConfig::getDeviceFields()
{
	return configGui.readEntry("devicefields",QVariantList());;
}

//Set enabled fields in device list
void TConfig::setDeviceFields(QVariantList p_list)
{
	configGui.writeEntry("devicefields",p_list);
}

//Set mainw window size in config.  is called from mainwindow.resize

void TConfig::setMainWindowSize(int p_width, int p_height)
{
	configGui.writeEntry(QStringLiteral("mainWidth"),p_width );
	configGui.writeEntry(QStringLiteral("mainHeight"),p_height);
}
//Get Mainwindow height as defined in config. Return=-1 when not set

int TConfig::getMainWindowHeight()
{
	return configGui.readEntry(QStringLiteral("mainHeight"),-1);
}

//Get mainwindow height as set in config. Return=-1 when not set
int TConfig::getMainWindowWidth()
{
	return configGui.readEntry(QStringLiteral("mainWidth"),-1);
}

bool TConfig::getDisksTab(){
	return configGui.readEntry(QStringLiteral("diskTab"),true);
}
void TConfig::setdiskTab(bool p_flag)
{
	configGui.writeEntry(QStringLiteral("diskTab"),p_flag);
}

bool TConfig::getRaidTab()
{
	return configGui.readEntry(QStringLiteral("raidTab"),true);
}

void TConfig::setRaidTab(bool p_flag)
{
	configGui.writeEntry(QStringLiteral("raidTab"),p_flag);
}
bool TConfig::getFsTabTab(){
	return configGui.readEntry(QStringLiteral("fsTabTab"),true);
}

void TConfig::setFsTabTab(bool p_flag){
	configGui.writeEntry(QStringLiteral("fsTabTab"),p_flag);
}
bool TConfig::getIscsiTab(){
	return configGui.readEntry(QStringLiteral("iscsiTab"),true);	
}
void TConfig::setIscsiTab(bool p_flag){
	configGui.writeEntry(QStringLiteral("iscsiTab"),p_flag);
}

bool TConfig::getDeviceAsTree()
{
	return configGui.readEntry(QStringLiteral("devicetree"),true);
}

void TConfig::setDeviceAsTree(bool p_flag)
{
	configGui.writeEntry(QStringLiteral("devicetree"),p_flag);
}

TConfig g_config;