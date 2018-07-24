#include<QCoreApplication>
#include"storedata.h"
#include<string>
#include<iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    StoreData* pstoredata=new StoreData;

    std::string flag;
    std::cout<<"Input b to begin recording."<<std::endl;
    std::cout<<"Input s to stop recording."<<std::endl;
    while(std::cin>>flag)
    {
        if(flag=="b")
        {
            pstoredata->startRecording();
        }else if(flag=="s")
        {
            pstoredata->stopRecording();
            break;
        }else{
            std::cout<<"Wrong input! Please input b or s."<<std::endl;
        }
    }
    return a.exec();
}
