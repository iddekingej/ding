#ifndef __RAID_H_
#define __RAID_H_

#include <QStringList>
#include <QString>
#include "base/linklist.h"
#include "devicebase.h"

class TRaidDevice
{
private:
	QList<TDeviceBase*>  raidDevices;
	QString              raidLevel;
	TDeviceBase          *device;
	QString              type;
public:
	inline QList<TDeviceBase*> *getRaidDevices()
	{
		return &raidDevices;
	}
	
	inline const QString &getRaidLevel()
	{
		return raidLevel;
	}
	
	inline TDeviceBase *getDevice()
	{
		return device;
	}
	
	inline const QString &getType()
	{
		return type;
	}
	
	inline void addRaidDevice(TDeviceBase *p_device)
	{
		raidDevices << p_device;
	}
	
	QString getRaidDeviceString()
	{
		QListIterator<TDeviceBase *>l_iter(raidDevices);
		QString l_return;
		while(l_iter.hasNext()){
			if(l_return.length()>0) l_return += QStringLiteral(",");
			l_return += l_iter.next()->getName();
		}
		return l_return;
	}
	
	TRaidDevice(TDeviceBase *p_device,const QString &p_type,const QString &p_level);
}
;
class TRaidInfo
{
private:
	TLinkList<TRaidDevice>devices;
public:
	inline TLinkList<TRaidDevice> *getDevices(){
		return &devices;
	}
	
	int getLength()
	{
		return devices.getLength();
	}	
	
	TRaidDevice *AddRaidDevice(TDeviceBase *p_device,const QString &p_type,const QString &p_raidLevel);
	void addRaid(TDeviceBase *p_device,const QString &p_type,const QString &p_level,QList<TDeviceBase *>p_members);
}
;
#endif
