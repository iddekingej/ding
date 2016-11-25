#include "lvm.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <QString>
#include <iostream>
#include "base/stringlineiterator.h"
#include "base/utils.h"
using namespace std;

TLVMResponseParser::TLVMResponseParser(QString& p_text)
{
	text=p_text;
	iter=new TStringLineIterator(p_text);
}

TLVMResponseParser::~TLVMResponseParser()
{
	delete iter;
}


void TLVMResponseParser::chapter(QString p_item)
{
}

void TLVMResponseParser::section(QString& p_section)
{
}

void TLVMResponseParser::main(QString& p_code)
{
}


void TLVMResponseParser::setVar(QString &p_name, QString &p_value)
{
}

TPVParser::TPVParser(TDeviceList* p_devList, QString& p_text):TLVMResponseParser(p_text)
{
	items=new TLinkList<TPVInfo>();
	current=nullptr;
	devList=p_devList;
}

void TPVParser::chapter(QString p_item)
{
	current=new TPVInfo();
	current->setKey(p_item);
	items->append(current);
}

void TPVParser::setVar(QString& p_name, QString& p_value)
{	
	if(p_name=="id") current->setId(p_value);
	else if(p_name=="device"){
		TDeviceBase *l_db=devList->getDeviceByDeviceNo(p_value.toLong());
		current->setDevice(p_value.toLong());
		current->setRealDevice(l_db);
	}
	else if(p_name=="dev_size") current->setDevSize(p_value.toLongLong());
	else if(p_name=="vgname") current->setVgName(p_value);
	else if(p_name=="vgid") current->setVgId(p_value);
	else if(p_name=="dao_size") current->setDaoSize(p_value.toLongLong());
	else if(p_name=="dao_offset") current->setDaoOffset(p_value.toLongLong());
	else if(p_name=="mdao_freesectors") current->setMdaoFreeSectors(p_value.toLongLong());
	else if(p_name=="mdao_size") current->setMdaoSize(p_value.toLongLong());
	else if(p_name=="mdao_start") current->setMdaoStart(p_value.toLongLong());
	else if(p_name=="mdao_ignore") current->setMdaoIgnore(p_value.toLongLong());
	else if(p_name=="id") current->setId(p_value);
	else if(p_name=="label_sector") current->setLabelSector(p_value.toLongLong());
	else if(p_name=="format")  current->setFormat(p_value);
	
}


void TLVMResponseParser::parseChapter()
{
	QString l_oldPrefix;
	while(iter->hasNext()){
		QString l_str=iter->next();
		
		if(l_str.endsWith("{")){
			l_oldPrefix=prefix;
			if(prefix.length()>0) prefix += "_";
			prefix += l_str.mid(0,l_str.length()-1).trimmed();
			section(prefix);
			parseChapter();
			prefix=l_oldPrefix;
		} else 	if(l_str.trimmed()=="}"){
			return;
		} else {
			int l_split=l_str.indexOf("=");
			if(l_split>=0){
				QString l_pre=l_str.mid(0,l_split).trimmed();
				if(prefix.length()>0) l_pre=prefix+"_"+l_pre;
				QString l_after=l_str.mid(l_split+1).trimmed();
				if(l_after.startsWith('"') && l_after.endsWith('"')){
					l_after=l_after.mid(1,l_after.length()-2);
				}
				setVar(l_pre,l_after);
			}
				
		}
	}
}


void TLVMResponseParser::parse()
{
	QString l_token;
	while(iter->hasNext()){
		QString l_str=iter->next();
		if(l_str.endsWith("{")){
			while(iter->hasNext()){
				l_token=iter->next();
				if(l_token.endsWith('{')){
					chapter(l_token.mid(0,l_token.length()-1).trimmed());
					prefix="";
					parseChapter();
				}
			}
		}
	}
}



bool TLVMHandler::writeSocket(const char* p_str)
{
	int l_return=write(readSocket,p_str,strlen(p_str));
	return l_return >=0;
}

bool TLVMHandler::openLVMSocket()
{
	struct sockaddr_un l_addr;
	readSocket=socket(PF_LOCAL,SOCK_STREAM,0);
	if(readSocket<0) return false;
	l_addr.sun_family=AF_LOCAL;
	strcpy(l_addr.sun_path,"/run/lvm/lvmetad.socket");
	int l_return=connect(readSocket,(const struct sockaddr *)&l_addr,sizeof(l_addr));
	if(l_return<0){
		close(readSocket);
		readSocket=-1;
		return false;
	}
	return true;
}

bool TLVMHandler::sendMessage(const char *p_message,QString &p_return)
{
	if(!writeSocket(p_message)) return false;
	char l_buffer[1024];
	ssize_t l_num;
	p_return="";
	while(true){
		l_num=read(readSocket,l_buffer,sizeof(l_buffer)-1);
		l_buffer[l_num]=0;
		p_return += l_buffer;
		cout << l_buffer <<endl;
		if(l_num+1<sizeof(l_buffer)) break;
	}
	return true;
}
 
TLinkList< TPVInfo >* TLVMHandler::pvList(TDeviceList* p_devList)
{
	QString l_return;
	bool l_ok=sendMessage("request = \"pv_list\"\ntoken = \"filter:0\"\n\n##\n",l_return);
	if(l_ok){
		TPVParser l_parser(p_devList,l_return);
		l_parser.parse();
		TLinkList<TPVInfo> *l_return=l_parser.getItems();
		return l_return;
	}
	return nullptr;
}

TLVMHandler::TLVMHandler()
{
	readSocket=-1;
}

TLVMHandler::~TLVMHandler()
{
	closeLVMSocket();
}


void TLVMHandler::closeLVMSocket()
{
	if(readSocket !=-1) close(readSocket);
	readSocket=-1;
}


void TLVM::processInfo(TDeviceList* p_devList)
{
	TPVInfo *l_info;
	pvList=nullptr;
	TLVMHandler l_handler;
	if(l_handler.openLVMSocket()){
		pvList=l_handler.pvList(p_devList);
		TLinkListIterator<TPVInfo> l_iter(pvList);
		while(l_iter.hasNext()){
			l_info=l_iter.next();
			if(l_info->getRealDevice() !=nullptr){
				pvIndexByDevice.insert(l_info->getRealDevice()->getName(),l_info);
				l_info->getRealDevice()->setVGName(l_info->getVgName());
			}
		}
		l_handler.closeLVMSocket();
	}
	
}
