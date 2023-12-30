#include <iostream>
#include <fstream>
#include <list>
#ifndef NOTIFICATIONS_H_
#define NOTIFICATIONS_H_

class Notifications{
public:
	Notifications(std::string user) : user(user){}
	const std::string& getUser() const {return user;}
	virtual const std::string getType() {return "notification";}
	virtual ~Notifications(){}
	// prints single line notifications
	virtual void print(std::ofstream& output){}
	// prints aggregated notifications
	virtual void aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis){}
protected:
	std::string user;
};
class Likes : public Notifications{
public:
	Likes(std::string user) : Notifications(user) {}
	~Likes(){}
	void print(std::ofstream& output);
	const std::string getType() {return "liked";}
	void aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis);
};
class Tags : public Notifications{
public:
	Tags(std::string user) : Notifications(user) {}
	~Tags(){}
	void print(std::ofstream& output);
	const std::string getType() {return "tagged";}
	void aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis);
};
class Comments : public Notifications{
public:
	Comments(std::string user) : Notifications(user) {}
	~Comments(){}
	void print(std::ofstream& output);
	const std::string getType() {return "commented";}
	void aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis);
};
class NewFollowers : public Notifications{
public:
	NewFollowers(std::string user) : Notifications(user) {}
	~NewFollowers(){}
	void print(std::ofstream& output);
	const std::string getType() {return "followed";}
	void aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis);
};
class MessageRequests : public Notifications{
public:
	MessageRequests(std::string user) : Notifications(user) {}
	~MessageRequests(){}
	void print(std::ofstream& output);
	const std::string getType() {return "message";}
	void aggregatePrint(std::ofstream& output, std::list<Notifications*> consecutiveNotis);
};
#endif