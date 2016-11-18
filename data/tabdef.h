#ifndef __TABDEF_H_
#define __TABDEF_H_
#include <QString>
#include <QVector>
#include "qjsonwrapper/Json.h"

#include "../base/doublelinkedlist.h"

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
	QString        name;
	int            conditionField;
	TObjectType    conditionObject;
	TConditionType conditionType;
	QString        conditionValue;
	QVector<int>   selectedList;
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
	inline QVector<int> *getSelectedList(){ return &selectedList;};
	void toJson(QList<QVariant> &p_variant);
	TTabDef(QVariant &p_json);
	void addSelectedList(int p_field);
	inline bool hasFieldInSelected(int p_field){ return selectedList.indexOf(p_field) != -1;}
	
	TTabDef(const QString &p_name);
	
};

class TTabDefList:public TDoubleLinkedList<TTabDef>{
public:
	TTabDef *createTabDef(const QString &p_name);
	TTabDef *getByName(QString &p_name);
	TDoubleLinkedListItem<TTabDef> *getByPosition(int p_pos);
	void save();
	void read();
};

#endif