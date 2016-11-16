#ifndef __TABDEF_H_
#define __TABDEF_H_
#include <QString>
#include <QLinkedList>
#include <QJsonDocument>
#include "../base/linklist.h"

typedef enum{
	TT_Device=0
,	TT_Partition=1
} TObjectType;

typedef enum{
	CT_IsEmpty=0
,	CT_IsNotEmpty=1
,	CT_IsValue=2
} TConditionType;

class TTabDef{
private:
	QString       name;
	int           conditionField;
	TObjectType   conditionObject;
	TConditionType conditionType;
	QString       conditionValue;
	QLinkedList<int> selectedList;
public:
	inline void setName(const QString &p_name){name=p_name;}
	inline QString &getName(){ return name;}
	inline int getConditionField(){ return conditionField;}
	inline void setConditionField(int p_conditionField){ conditionField=p_conditionField;}
	inline TObjectType getConditionObject(){ return conditionObject;}
	inline void setConditionObject(TObjectType p_conditionObject){ conditionObject=p_conditionObject;}
	inline TConditionType getConditionType(){ return conditionType;}
	inline void setConditionType(TConditionType p_conditionType){ conditionType=p_conditionType;}
	inline QString &getConditionValue(){ return conditionValue;}
	inline void setConditionValue(QString p_value){ conditionValue=p_value;}
	void toJson(QJsonArray &o_p_document);
	void addSelectedList(int p_field);
	
	TTabDef(const QString &p_name);
	TTabDef(QJsonObject &p_json);
};

class TTabDefList:public TLinkList<TTabDef>{
public:
	TTabDef *createTabDef(const QString &p_name);
	TTabDef *getByName(QString &p_name);
	TTabDef *getByPosition(int p_pos);
	void save();
	void read();
};

#endif
