


#ifndef _CLIENT_MAIN_CPP_
#define _CLIENT_MAIN_CPP_

#include "clientMain.hpp"
#include "Client.hpp"

std::string readMessage(){
    std::string inputMessage;
    std::cin >> inputMessage;
    return inputMessage;
}

bool verifyMessage(std::string message, std::regex rgx, int maxSize){

    if (!std::regex_match(message, rgx)) {
        return false;
    }

    if (message.size() > maxSize) {
        return false;
    }

    return true;
}

std::string replaceNums(std::string message){
    std::string processedMessage(message);
    int step = 3;

    for (int i = 0; i < processedMessage.size(); i += step) {
        processedMessage.replace(i, 1, "KB");
    }

    return processedMessage;
}

std::string processMessage(std::string message){
    std::string processedMessage(message);
    std::sort(processedMessage.rbegin(), processedMessage.rend()); 

    processedMessage = replaceNums(processedMessage);
    
    return processedMessage;
}

void replaceAll(std::string& s,std::string const& toReplace,std::string const& replaceWith) {
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

std::string calculateSum(std::string message){
    std::string processedMessage(message);
    int sum = 0; 

    replaceAll(processedMessage,"KB"," ");

    std::stringstream strStream(processedMessage); 
    std::string word;
    while (strStream >> word) { 
            sum += stoi(word);
    }
    processedMessage = std::to_string(sum);
    return processedMessage;
}

void readAndVerifyMessage(std::mutex& mtx, std::condition_variable& condVar,std::list<std::string>& messageList){
    std::string inputStr;
    std::cout << "Enter message: \n";
    while (true)
    {
        {
            inputStr = readMessage();   
        }
        if (verifyMessage(inputStr,std::regex("[0-9]+"),64))
        {
            inputStr = processMessage(inputStr);
            messageList.push_back(inputStr);
        }
        condVar.notify_one();
    }
}

void processAndSendMessage(std::mutex& mtx,std::condition_variable& condVar ,std::list<std::string>& messageList){
    NET::Client client(5656,"127.0.0.1");
    std::string currentMessage;
    bool isConnected = true;
    while (true)
    {   
        std::unique_lock<std::mutex> ulmtx(mtx);
        condVar.wait(ulmtx,[&](){return !messageList.empty();});
        if (!messageList.empty())
        {
            
            
            currentMessage = messageList.front();

            std::cout << "message: "<<currentMessage << "\n";
            currentMessage = calculateSum(currentMessage);
            
            if (client.tryConnection())
            {
                std::cout << "Enter message: \n";
                client.sendMessage(currentMessage);
                messageList.pop_front();
                client.closeConnection();
            }
            else 
            {
                std::cout << "Server not connected, send failed, restart server\n";
                std::cout << "Enter message: \n";
                condVar.wait(ulmtx);
            }
        }
    }    
}

int main(){
    std::mutex mtx;
    std::condition_variable condVar;
    std::list<std::string> messageList;
    std::thread produce_th(readAndVerifyMessage,ref(mtx),ref(condVar),ref(messageList));
    std::thread consume_th(processAndSendMessage,ref(mtx),ref(condVar),ref(messageList));

    produce_th.join();
    consume_th.join();
    return 0;
}

#endif //!_CLIENT_MAIN_CPP_