#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <list>
#include "Notifications.h"

// function that allows extraction of substrings from a given string between two key substrings
std::string getSubString(std::string input, std::string first, std::string last){
    std::size_t startPos = input.find(first);
    if(startPos == std::string::npos){
        return "";
    }
    startPos += first.length();

    std::size_t endPos = input.find(last);
    if(endPos == std::string::npos){
        return "";
    }
    return input.substr(startPos, endPos-startPos);
}

int main(int argc, char* argv[]){
	std::ifstream posts(argv[1]);
	std::ifstream users(argv[2]);
	std::ifstream events(argv[3]);
	std::ofstream output(argv[4]);
	std::string username = argv[5];
	// checks if there are the proper amount of command line arguments
	try{
		if(argc > 6){
			throw std::string("Too many input arguments");
		}
	}
	catch(std::string& error){
		std::cerr << error << std::endl;
		exit(1);
	}
	// check if all the input files and the output file are opening
	try{
		if(!posts.is_open()) {
            throw std::string("Failed to open the posts input file.");
	    }
	}
	catch(std::string& error){
		std::cerr << argv[1] << error << std::endl;
		exit(1);
	}
	try{
		if (!users.is_open()) {
            throw std::string("Failed to open the users input file.");
	    }
	}
	catch(std::string& error){
		std::cerr << argv[2] << error << std::endl;
		exit(1);
	}
	try{
		if (!events.is_open()) {
            throw std::string("Failed to open the events input file.");
	    }
	}
	catch(std::string& error){
		std::cerr << argv[3] << error << std::endl;
		exit(1);
	}
    // checks if output file is open
    try{
		if(!output.is_open()){
			throw std::string("Failed to open the output file.");
		}
	}
	catch(std::string& error){
		std::cerr << argv[4] << error << std::endl;
		exit(1);
	}
	std::string line;
	std::string userMatch;
	std::string postId;
	std::vector<std::string> postIdMatches;
	std::string postUsername;
	// reads in all needed information from the posts file
	while(std::getline(posts, line)){
		postId = getSubString(line, "{\"id\":\"", "\",\"type\":");
		postUsername = getSubString(line,"\"ownerUsername\":\"", "\",\"ownerId\":");
		if(postUsername == username){
			userMatch = postUsername;
			postIdMatches.push_back(postId);
		}
	}
	std::string user;
	std::string pauseAll; 
	std::string likes;
	std::string tags;
	std::string comments;
	std::string follower;
	std::string message;
	// reads in all needed information of the users file
	while(std::getline(users, line)){
		user = getSubString(line, "\"username\": \"", "\", \"url\": ");
		if(user == username){
			pauseAll = getSubString(line,"{\"pauseAll\": \"", "\", \"likes\": \"");
			likes = getSubString(line, "\", \"likes\": \"", "\", \"tags\": \"");
			tags = getSubString(line, "\", \"tags\": \"", "\", \"comments\": \"");
			comments = getSubString(line, "\", \"comments\": \"", "\", \"newFollowers\": \"");
			follower = getSubString(line, "\", \"newFollowers\": \"", "\", \"messageRequests\": \"");
			message = getSubString(line, "\", \"messageRequests\": \"", "\"}]}");
		}
	}
	// set to store all notifications
	std::stack<Notifications*> notifications;
	// vector to keep track of consecutive notifications
	// set up conditions so you will not read in unnessesary events
	if(pauseAll == "true"){
		return 0;
	}
	// checks if pause all if off but everything else if off too
	else if(pauseAll == "false" && likes == "false" && tags == "false" && comments == "false" && follower == "false" && message == "false"){
		return 0;
	}
	else{
	// create a list of notifications that stores the previous occurences of a given type if the current type matches the previous type add it to
	// the list. If the list is greater than 3 print out the aggregated version else just print out the notification.
		std::string subUser;
		std::string action;
		std::string mainUser;
		while(events >> subUser >> action >> mainUser){
			Notifications* noti = NULL;
			// if second name given matches the given username
			if(mainUser == username){
				// add new object of type matching the action if the action is turned on 
				if(action == "likes" && likes == "true"){
					noti = new Likes(subUser);
					notifications.push(noti);
				}
				if(action == "tags" && tags == "true"){
					noti = new Tags(subUser);
					notifications.push(noti);
				}
				if(action == "comments_on" && comments == "true"){
					noti = new Comments(subUser);
					notifications.push(noti);
				}
				if(action == "follows" && follower == "true"){
					noti = new NewFollowers(subUser);
					notifications.push(noti);
				}
				if(action == "messageRequests" && message == "true"){
					noti = new MessageRequests(subUser);
					notifications.push(noti);
				}
			}
			else{
				for(unsigned int i=0;i<postIdMatches.size(); i++){
					// if the second name given matchs one of the post ids from your user
					if(mainUser == postIdMatches[i]){
						if(action == "likes" && likes == "true"){
							noti = new Likes(subUser);
							notifications.push(noti);
						}
						if(action == "comments_on" && comments == "true"){
							noti = new Comments(subUser);
							notifications.push(noti);
						}
					}
				}
			}
		}
	}
	int count = 0;
	// stores the current consecutive notifications by type
	std::list<Notifications*> consecutiveNotis;
	// prints the first 100 notifications if there are more than 100 notifications
	if(notifications.size() > 100){
		while(count < 100){
			if(count == 0 ){
				// initialize the first position in the list of consecutive notifications
				consecutiveNotis.push_back(notifications.top());
				notifications.top() = NULL;
				delete notifications.top();
				notifications.pop();
			}
			if(notifications.top()->getType() == consecutiveNotis.front()->getType()){
				consecutiveNotis.push_back(notifications.top());
				notifications.top() = NULL;
				delete notifications.top();
				notifications.pop();
			}
			// if the type changes check the size of the consecutive vector if above 3 aggregate print else print each one
			if(notifications.top()->getType() != consecutiveNotis.front()->getType()){
				// check for more than 3 consecutive of the same type
				if(consecutiveNotis.size() > 3){
					consecutiveNotis.front()->aggregatePrint(output, consecutiveNotis);
					for(std::list<Notifications*>::iterator itr = consecutiveNotis.begin(); itr != consecutiveNotis.end(); itr++){
						delete (*itr);
					}
					count++;
				}
				else{
					for(std::list<Notifications*>::iterator itr = consecutiveNotis.begin(); itr != consecutiveNotis.end(); itr++){
						// makes sure count doesn't go out of bounds in the for loop resulting in too many lines in output
						if(count < 100 ){
							(*itr)->print(output);
						}
						delete (*itr);
						count++;
					}
				}
				// clear vector and reinsert the next top
				consecutiveNotis.clear();
				consecutiveNotis.push_back(notifications.top());
				notifications.top() = NULL;
				delete notifications.top();
				notifications.pop();
			}
		}
	}
	else{
		while(notifications.size() > 0){
			// checks if there is enough notifications to properly construct a consecutive list
			if(count == 0 && notifications.size() > 3){
				// initialize the first position in the list of consecutive notifications
				consecutiveNotis.push_back(notifications.top());
				notifications.top() = NULL;
				delete notifications.top();
				notifications.pop();
				count++;
			}
			if(count==0 && notifications.size() <= 3){
				// if there are 3 or less notifications print them out one by one
				while(notifications.size() > 0){
					notifications.top()->print(output);
					notifications.top() = NULL;
					delete notifications.top();
					notifications.pop();
				}
			}
			// checks whether you should add to the consevutive and if not print out the previous consecutive depending on its size
			if(consecutiveNotis.size() > 0 && notifications.size() > 0){
				if(notifications.top()->getType() == consecutiveNotis.front()->getType()){
					consecutiveNotis.push_back(notifications.top());
					notifications.top() = NULL;
					delete notifications.top();
					notifications.pop();
				}
				// if the type changes check the size of the consecutive vector if above 3 aggregate print else print each one
				if(notifications.top()->getType() != consecutiveNotis.front()->getType()){
					// check for more than 3 consecutive of the same type
					if(consecutiveNotis.size() > 3){
						consecutiveNotis.front()->aggregatePrint(output, consecutiveNotis);
						for(std::list<Notifications*>::iterator itr = consecutiveNotis.begin(); itr != consecutiveNotis.end(); itr++){
							delete (*itr);
						}
						count++;
					}
					else{
						for(std::list<Notifications*>::iterator itr = consecutiveNotis.begin(); itr != consecutiveNotis.end(); itr++){
							(*itr)->print(output);
							delete (*itr);
							count++;
						}
					}
					// clear vector and reinsert the next top
					consecutiveNotis.clear();
					consecutiveNotis.push_back(notifications.top());
					notifications.top() = NULL;
					delete notifications.top();
					notifications.pop();
					if(notifications.size() == 0){
						for(std::list<Notifications*>::iterator itr = consecutiveNotis.begin(); itr != consecutiveNotis.end(); itr++){
							(*itr)->print(output);
							delete (*itr);
						}
						consecutiveNotis.clear();
					}
				}
			}
		}
	}
	// delete last left in consecutive notifications
	while(consecutiveNotis.size() != 0){
		delete consecutiveNotis.front();
		consecutiveNotis.pop_front();
	}
	// cleans up left over memory
	while(notifications.size() > 0){
		delete notifications.top();
		notifications.pop();
	}
	return 0;
}
