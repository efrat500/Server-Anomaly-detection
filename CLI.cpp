
#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->data = new DataCommands();
}
void CLI::start(){
    //this->data = new DataCommands();
    int option = 0;
    UploadCommend commend1 = UploadCommend(dio,data);
    this->v_commands.push_back(&commend1);
    algorithmsSettings commend2 = algorithmsSettings(dio,data);
    this->v_commands.push_back(&commend2);
    detectAnomalies commend3 = detectAnomalies(dio,data);
    this->v_commands.push_back(&commend3);
    displayResults commend4 = displayResults(dio,data) ;
    this->v_commands.push_back(&commend4);
    UplodAnomalies commend5 =  UplodAnomalies(dio,data);
    this->v_commands.push_back(&commend5);
    Exit commend6 = Exit(dio,data);
    this->v_commands.push_back(&commend6);
    while(option != 6){
       dio->write("Welcome to the Anomaly Detection Server.\n");
       dio->write("Please choose an option:\n");
       for(int i = 0; i < v_commands.size(); i++){
           v_commands[i]->getDescript();
        }
        string str = dio->read();
        option = std::stoi(str);  
        v_commands[option-1]->execute();
    }
}


CLI::~CLI() {
}

