#ifndef __MOUNTDIALOG_H__
#define __MOUNTDIALOG_H__

#include <QDialog>
#include "ui_mount.h"
#include "data/devicebase.h"

class TMountDialog:public QDialog
{
	Q_OBJECT
private slots:
	void handleAction();
	void actionClicked();
private:
	void unmountDevice();
	void mountDevice();
	Ui::MountDialog ui;
	TDeviceBase *device;
public:
	TMountDialog(TDeviceBase *p_device);
};

#endif