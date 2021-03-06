#ifndef __LVM_H__
#define __LVM_H__
#include <QString>
#include "base/stringlineiterator.h"
#include "base/linklist.h"
#include "devicelist.h"
using namespace std;

/***
 * Class for reading a storing information about lvm devices
 *  
 */

enum class TSectionType{
	st_top
,	st_data
,	st_pv
,	st_vg
,	st_da0
,	st_pvsection
,       st_lvsection
,       st_lv
};

class TVolumeGroup;
class TVolumeGroupList;

class TPhysicalVolume {
private:
	QString key;
	int device=0;
	long long dev_size=0;
	QString format;
	int label_sector=0;
	QString id;
	int mdao_ignore=0;
	int mdao_start=0;
	long long mdao_size=0;
	long long mdao_free_sectors=0;
	long long dao_offset=0;
	long long dao_size=0;
	QString vgid;
	QString vgName;
	TDeviceBase *realDevice=nullptr;
	TVolumeGroup *volumeGroup=nullptr;
public:
	inline TVolumeGroup *getVolumeGroup(){ return volumeGroup;}
	inline void setVolumeGroup(TVolumeGroup *p_volumeGroup){ volumeGroup=p_volumeGroup;}
	inline TDeviceBase *getRealDevice(){ return realDevice;}
	inline void setRealDevice(TDeviceBase *p_realDevice){ realDevice=p_realDevice;}
	inline QString &getVgId(){ return vgid;}
	inline void setVgId(QString &p_vgid){ vgid=p_vgid;}
	inline QString getVgName(){ return vgName;}
	inline void setVgName(QString &p_vgName){ vgName=p_vgName;}
	inline long long getDaoSize(){ return dao_size;}
	inline void setDaoSize(long long p_daoSize){ dao_size=p_daoSize;}
	inline long long getDaoOffset(){ return dao_offset;}
	inline void setDaoOffset(long long p_daoOffset){ dao_offset=p_daoOffset;}
	inline long long getMdaoFreeSectors(){ return mdao_free_sectors;}
	inline void setMdaoFreeSectors(long long p_mdaoFreeSectors){ mdao_free_sectors=p_mdaoFreeSectors;}
	inline long long getMdaoSize(){ return mdao_start;}
	inline void setMdaoSize(long long p_mdaoSize){ mdao_size=p_mdaoSize;}
	inline int getMdaoStart(){ return mdao_start;}
	inline void setMdaoStart(int p_mdaoStart){ mdao_start=p_mdaoStart;}
	inline int getMdaoIgnore(){ return mdao_ignore;}
	inline void setMdaoIgnore(int p_mdaoIgnore){ mdao_ignore=p_mdaoIgnore;}
	inline QString getId(){ return id;}
	inline void setId(QString &p_id){ id=p_id;}
	inline int getLabelSector(){ return label_sector;}
	inline void setLabelSector(int p_labelSector){ label_sector=p_labelSector;}
	inline QString getFormat(){ return format;}
	inline void setFormat(QString &p_format){ format=p_format;}
	inline long getDevSize(){ return dev_size;}
	inline void setDevSize(long long p_devSize){ dev_size=p_devSize;}
	inline int getDevice(){ return device;}
	inline void setDevice(int p_device){ device=p_device;}
	inline QString getKey(){ return key;}
	inline void setKey(QString &p_key){ key=p_key;}
	TPhysicalVolume();
};

class TPhysicalVolumeList:public TLinkList<TPhysicalVolume>{
public:
	void processList(TVolumeGroupList *p_lie);
};



class TVolumeGroup;

class TLogicalVolume{
private:
	/**
	 * Logical volume id
	 */
	QString id;
	/**
	 * Logical volume name
	 */
	QString name;
	/**
	 * This logical volume belongs this volume group
	 */
	TVolumeGroup *volumeGroup;
	/**
	 * Device that is this logical volume
	 */
	TDevice *realDevice;
	/**
	 * readFlag - true can read lv
	 *            false can't read lv
	 */
	bool readFlag;
	/**
	 * writeFlag - true can write to lv
	 *             false can't write to lv
	 */
	bool writeFlag;
	/**
	 * visibility flag - true lv is visible
	 *                   false lv isn't visible
	 */
	bool visibleFlag;
public:
	inline bool getReadFlag(){ return readFlag;}
	inline bool getWriteFlag(){ return writeFlag;}
	inline bool getVisibleFlag(){ return visibleFlag;}
	void setReadFlag(bool p_flag){ readFlag=p_flag;}
	void setWriteFlag(bool p_flag){ writeFlag=p_flag;}
	void setVisibleFlag(bool p_flag){ visibleFlag=p_flag;}
	inline TDevice *getRealDevice(){ return realDevice;}
	inline void setRealDevice(TDevice *p_device)
	{
		realDevice=p_device;
	}
	inline QString getId(){return id;}
	inline void setId(QString &p_id){id=p_id;}
	inline QString getName(){return name;}
	inline TVolumeGroup *getVolumeGroup(){ return volumeGroup;}
	TLogicalVolume(QString &p_name,TVolumeGroup *p_volumeGroup);
};

/***
 *  Information about a volume group
 */
class TVolumeGroup {
private:
	/**
	 * Volume group id
	 */
	QString key;
	/**
	 * volume group name
	 */
	QString name;
	/**
	 * List of logical volumes places in this volume group
	 */
	TLinkList<TLogicalVolume> logicalVolumns;
public:
	inline void setKey(QString &p_key){ key=p_key;}
	inline QString &getKey(){ return key;}
	inline void setName(QString &p_name){ name=p_name;}
	inline QString &getName(){ return name;}
	inline TLogicalVolume *getLVByName(QString &p_name);
	TLogicalVolume *addLv(QString &p_name);
	TLinkList<TLogicalVolume> *getLogicalVolumns(){ return &logicalVolumns;}
	   TVolumeGroup();
};

class TVolumeGroupList:public TLinkList<TVolumeGroup>{
public:
	TVolumeGroup *getVolumeGroupById(QString &p_name);
	void processInfo(TDeviceList *p_list);
};

/**
 *  The lvm data is retrieved through the lvm meta deamon.
 *  This class parses the response data
 */
class TLVMResponseParser{
private:
	QString text;
	QString prefix;
	TStringLineIterator *iter;
protected:
	TSectionType sectionType;
public:
	TLVMResponseParser(QString &p_text);
	virtual ~TLVMResponseParser();
	virtual bool chapter(QStringRef &p_item);
	void parseChapter();	
	virtual void setVar(QString &p_name,QString &p_value);
	void parse();
	
};

class TTokenParser:public TLVMResponseParser{
private:
	QString token;	
public:
	TTokenParser(QString &p_text);
	virtual void setVar(QString &p_name,QString &p_value) override;
	inline QString  getToken(){ return token;}
};




class TPVParser:public TLVMResponseParser{
private:
	TPhysicalVolumeList *items;
	TPhysicalVolume *current;
	TDeviceList *devList;
public:
	TPVParser(TDeviceList *p_devList,QString &p_text);
	virtual bool chapter(QStringRef &p_item) override;
	virtual void setVar(QString &p_name,QString &p_value) override;
	TPhysicalVolumeList *getItems(){ return items;}	

};


class TVGMainParser:public TLVMResponseParser{
private:
	TVolumeGroupList *items=nullptr;
	TVolumeGroup *current=nullptr;
public:
	TVGMainParser(QString &p_text);
	virtual bool chapter(QStringRef &p_item) override;
	virtual void setVar(QString &p_name,QString &p_value) override;
	TVolumeGroupList *getItems(){ return items;}	

};


class TVGParser:public TLVMResponseParser{
private:
	TVolumeGroup *current=nullptr;
	TLogicalVolume *currentLv=nullptr;
public:
	TVGParser(TVolumeGroup *p_item,QString &p_text);
	virtual bool chapter(QStringRef &p_item) override;
	virtual void setVar(QString &p_name,QString &p_value) override;
};


class TLVMHandler {
private:
	int readSocket;
	bool writeSocket(const char *p_str);
	bool sendMessage(const char *p_message,QString &p_return);
	QString token;
public:
	bool openLVMSocket();	
	void closeLVMSocket();
	void getToken();
	TPhysicalVolumeList *pvList(TDeviceList *p_devList);	
	TVolumeGroupList *vgList();
	TLVMHandler();
	 ~TLVMHandler();
};


class TLVM{
private:
	TPhysicalVolumeList *pvList;
	TVolumeGroupList    *vgList;
	QString token;
public:
	
	inline TLinkList<TPhysicalVolume> *getPvList(){ return pvList;}
	inline TLinkList<TVolumeGroup> *getVgList(){return vgList;}
	void processInfo(TDeviceList *p_devList);
	static bool deamonRunning();

	TLVM();
	~TLVM();
};

#endif
