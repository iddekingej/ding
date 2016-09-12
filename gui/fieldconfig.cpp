/*
 * fieldconfig.cpp
 * configure which fields to display and the desired order
 */
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVariantList>
#include <QListView>
#include "fieldconfig.h"
#include "base/globals.h"
#include "base/config.h"
#include <klocalizedstring.h>
// Save configuration after pressing "Ok" button

void TFieldsConfig::save()
{
		QVariantList  l_list;
		QStandardItem l_item;
		for(int l_cnt=0;l_cnt<modelSelected->rowCount();l_cnt++){
			l_list << modelSelected->item(l_cnt)->data().toInt();
		}
		
		g_config.setDeviceFields(l_list);
		g_config.setDeviceAsTree(ui.displayTree->isChecked());
		g_config.sync();
		close();
		
}

// Move current selected item up (p_diff=-1) or down (p_diff=+1)

void TFieldsConfig::moveItem(int p_diff)
{
	QModelIndexList l_list=ui.fieldsSelected->selectionModel()->selectedIndexes();
	QListIterator<QModelIndex> l_iter=QListIterator<QModelIndex>(l_list);
	QModelIndex     l_item;	
	QModelIndex     l_newSelection;
	QList<QStandardItem *> l_itemsRow;
	int l_newPos;

	if(l_iter.hasNext()){
		l_item=l_iter.next();
		l_newPos=l_item.row()+p_diff;
		if(l_newPos >=0 && l_newPos<modelSelected->rowCount() ){
			l_itemsRow=modelSelected->takeRow(l_item.row());
			modelSelected->insertRow(l_newPos,l_itemsRow);			
			l_newSelection=modelSelected->index(l_newPos,0);
			if(l_newSelection.isValid()){
				ui.fieldsSelected->selectionModel()->select(l_newSelection,QItemSelectionModel::Clear|QItemSelectionModel::Select);
			}
		}
	}
}


void TFieldsConfig::moveUp()
{
	moveItem(-1);
}

void TFieldsConfig::moveDown()
{
	moveItem(+1);
}


void TFieldsConfig::enableRemoveButton()
{
	ui.removeField->setEnabled(modelSelected->rowCount()>0);	
}
void TFieldsConfig::removeLabel()
{
	QModelIndexList l_list=ui.fieldsSelected->selectionModel()->selectedIndexes();
	QModelIndex     l_item;
	QListIterator<QModelIndex> l_iter=QListIterator<QModelIndex>(l_list);
	int l_code;
	QStandardItem *l_selectedItem;
	l_iter.toBack();
	while(l_iter.hasPrevious()){
		l_item=l_iter.previous();
		l_selectedItem=modelSelected->item(l_item.row());
		l_code=l_selectedItem->data().toInt();
		hideByCode(ui.fieldsAvailable,l_code,false);
		modelSelected->removeRow(l_item.row());
	}
	enableRemoveButton();	
}

/*
 * When field is selected in ui.fieldsAvailable and moved to the visible field
 * The field is hidden in ui.fieldsAvailable and the 'next' field in fieldsAvailable must be selected
 * Find first nearest visible row in ui.fieldsAvailable relative to index p_index
 * first look downward and thant up
 
*/

int TFieldsConfig::getFAVisibleRow(int p_index)
{
	int l_cnt=p_index+1;
	while(l_cnt<ui.fieldsAvailable->model()->rowCount()){
		if(!ui.fieldsAvailable->isRowHidden(l_cnt)) return l_cnt;
		l_cnt++;
	}
	l_cnt=p_index-1;
	while(l_cnt>=0){
		if(!ui.fieldsAvailable->isRowHidden(l_cnt)) return l_cnt;
		l_cnt--;
	}
	return -1;
}

void TFieldsConfig::addLabel()
{
	QModelIndexList l_list=ui.fieldsAvailable->selectionModel()->selectedIndexes();
	QModelIndex l_item;
	QStandardItem *l_itemSource;
	QModelIndex     l_newSelection;

	if(l_list.size()>0){
		l_item=l_list[0];
		
		ui.fieldsAvailable->setRowHidden(l_item.row(),true);
		int l_row=l_item.row();
		ui.fieldsAvailable->clearSelection();
		int l_newIndex=getFAVisibleRow(l_row);
		if(l_newIndex>=0){
			l_newSelection=ui.fieldsAvailable->model()->index(l_newIndex,0);
			if(l_newSelection.isValid()){
				ui.fieldsAvailable->selectionModel()->select(l_newSelection,QItemSelectionModel::Select);
			}
		}
		l_itemSource=modelAvailable->item(l_item.row());			
		modelSelected->appendRow(l_itemSource->clone());
		l_newSelection=modelSelected->index(modelSelected->rowCount()-1,0);
		if(l_newSelection.isValid()){
			ui.fieldsSelected->selectionModel()->select(l_newSelection,QItemSelectionModel::Clear|QItemSelectionModel::Select);
		}		
		
	}
	enableRemoveButton();
}

void TFieldsConfig::hideByCode(QListView *p_view,int p_code,bool p_flag)
{
	QStandardItem *l_item;
	QStandardItemModel *l_model=(QStandardItemModel *)p_view->model();
	for(int l_cnt=0;l_cnt<g_numDeviceFields;l_cnt++){
		l_item=l_model->item(l_cnt);
		if(l_item->data().toInt()==p_code){
			p_view->setRowHidden(l_cnt,p_flag);
		}
	}
}

void TFieldsConfig::fillAvailableList()
{
	QStandardItem *l_item;
	for(int l_cnt=0;l_cnt<g_numDeviceFields;l_cnt++){
		l_item=new QStandardItem(i18n(g_deviceFields[l_cnt]));
		l_item->setData(l_cnt);
		l_item->setEditable(false);
		modelAvailable->setItem(l_cnt,0,l_item);
	}
	ui.fieldsAvailable->setModel(modelAvailable);
}

void TFieldsConfig::fillSelectedList()
{
	QStandardItem *l_item;
	QVariantList l_enabledFields=g_config.getDeviceFields();
	for(int l_cnt=0;l_cnt<l_enabledFields.count();l_cnt++){
		int l_selectedId=l_enabledFields[l_cnt].toInt();
		if(l_selectedId<g_numDeviceFields){
			l_item=new QStandardItem(i18n(g_deviceFields[l_selectedId]));
			l_item->setData(l_selectedId);
			l_item->setEditable(false);
			modelSelected->setItem(l_cnt,0,l_item);
			hideByCode(ui.fieldsAvailable,l_selectedId,true);
		}
	}
	enableRemoveButton();
	ui.fieldsSelected->setModel(modelSelected);
}
TFieldsConfig::TFieldsConfig():QDialog()
{

	modelAvailable=new QStandardItemModel(g_numDeviceFields,1,this);
	modelSelected=new QStandardItemModel(0,1,this);

	ui.setupUi(this);
	connect(ui.buttonBox,SIGNAL(accepted()),SLOT(save()));
	connect(ui.buttonBox,SIGNAL(rejected()),SLOT(close()));
	connect(ui.addLabel,SIGNAL(pressed()),SLOT(addLabel()));
	connect(ui.removeField,SIGNAL(pressed()),SLOT(removeLabel()));
	connect(ui.moveUp,SIGNAL(pressed()),SLOT(moveUp()));
	connect(ui.moveDown,SIGNAL(pressed()),SLOT(moveDown()));
	bool l_tree=g_config.getDeviceAsTree();
	ui.displayTree->setChecked(l_tree);
	ui.displayGrid->setChecked(!l_tree);
	
	fillAvailableList();
	fillSelectedList();
}

TFieldsConfig::~TFieldsConfig(){	
	delete modelAvailable;
	delete modelSelected;
}
