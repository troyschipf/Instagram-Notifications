#include <iostream>
#include "Notifications.h"


// declaration of all of the print and consecutive prints
void Likes::print(std::ofstream& output){
	output << user << " liked your post." << std::endl;
}

void Likes::aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis){
	output << consecutiveNotis.front()->getUser() << ", ";
	consecutiveNotis.pop_front();
	output << consecutiveNotis.front()->getUser() << " and " << consecutiveNotis.size()-1 << " others liked your post." << std::endl;
}

void Tags::print(std::ofstream& output){
	output << user << " tagged you in a post." << std::endl;
}

void Tags::aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis){
	output << consecutiveNotis.front()->getUser() << ", ";
	consecutiveNotis.pop_front();
	output << consecutiveNotis.front()->getUser() << " and " << consecutiveNotis.size()-1 << " others tagged you in a post." << std::endl;
}

void Comments::print(std::ofstream& output){
	output << user << " commented on your post." << std::endl;
}

void Comments::aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis){
	output << consecutiveNotis.front()->getUser() << ", ";
	consecutiveNotis.pop_front();
	output << consecutiveNotis.front()->getUser() << " and " << consecutiveNotis.size()-1 << " others commented on your post." << std::endl;
}

void NewFollowers::print(std::ofstream& output){
	output << user << " started following you." << std::endl;
}

void NewFollowers::aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis){
	output << consecutiveNotis.front()->getUser() << ", ";
	consecutiveNotis.pop_front();
	output << consecutiveNotis.front()->getUser() << " and " << consecutiveNotis.size()-1 << " others started following you." << std::endl;
}

void MessageRequests::print(std::ofstream& output){
	output << user << " wants to send you a message." << std::endl;
}

void MessageRequests::aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis){
	output << consecutiveNotis.front()->getUser() << ", ";
	consecutiveNotis.pop_front();
	output << consecutiveNotis.front()->getUser() << " and " << consecutiveNotis.size()-1 << " others wants to send you a message." << std::endl;
}